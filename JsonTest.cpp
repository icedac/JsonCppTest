// JsonTest.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "json_test.h"
#include <string>
#include <iostream>
#include <chrono>
#include <functional>
#include <vector>
#include <fstream>
#include <memory>

// tr2 filesystem
#include <filesystem>

/*
*/
class JsonTestWrapper_memcpy : public JsonTestWrapper
{
public:
	JsonTestWrapper_memcpy() {}
	~JsonTestWrapper_memcpy() {}

	bool Parse() override
	{
		std::vector<char> buffer(json_str_.begin(), json_str_.end());
		buffer.push_back('\0');
		buffer_.swap(buffer);

		return true;
	}
	std::string Write() override
	{
		std::string s;
		s.reserve(buffer_.size() + 1);
		//for (int i = 0; i < 50; ++i )
		//	s += "0123456789";
		s = buffer_.data();
		return s;
	}

private:
	std::vector< char > buffer_;
};

JsonTestWrapper* parse_memcpy(const std::string& json_str)
{
	JsonTestWrapper* j = new JsonTestWrapper_memcpy();
	j->SetJsonString(json_str);
	return j;
}

JsonTestWrapper* parse_rapidjson(const std::string& json_str);
JsonTestWrapper* parse_libjson(const std::string& json_str);
JsonTestWrapper* parse_jsoncpp(const std::string& json_str);
typedef std::function< JsonTestWrapper* (const std::string&) > parse_json_f;

using std::tr2::sys::path;
using std::tr2::sys::directory_iterator;

void json_tester(const std::string& name, parse_json_f parse_json, const std::string& json_dir, int parse_count, int write_count, int csv_mode = 1 )
{
	// iterates dir files;
	std::vector< std::string > json_files;

	auto json_path = path(json_dir);
	for (auto it = directory_iterator(json_path); it != directory_iterator(); ++it)
	{
		const auto& file = it->path();
		json_files.push_back( file.string() );
	}

	std::vector< std::string > json_strings;
	for (auto s : json_files)
	{
		std::ifstream in(s);
		std::string json_str;
		while (in)
		{
			std::string line;
			std::getline(in, line);
			json_str += line;
		}
		json_strings.push_back(json_str);
	}

	typedef std::unique_ptr< JsonTestWrapper > JTW_ptr;
	std::vector< JTW_ptr > json_objs;
	for (auto s : json_strings)
		json_objs.push_back( JTW_ptr(parse_json(s)) );

	// parse test
	if (csv_mode)
		std::cout << name.c_str();
	else
		std::cout << name.c_str() << ": parsing [" << parse_count << "]loops";
	auto start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < parse_count; ++i)
	{
		for (auto& json : json_objs)
		{
			json->Parse();
		}
	}
	auto end = std::chrono::high_resolution_clock::now();
	if (csv_mode)
		std::cout << ", " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	else
		std::cout << " [" << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "]msec\n";

	// write test
	if (csv_mode==0)
		std::cout << name.c_str() << ": writing [" << write_count << "]loops";
	start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < write_count; ++i)
	{
		for (auto& json : json_objs)
		{
			std::string result = json->Write();
		}
	}
	end = std::chrono::high_resolution_clock::now();
	if (csv_mode)
		std::cout << ", " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;
	else
		std::cout << " [" << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "]msec\n";
}

struct lib_def
{
	std::string		lib_name;
	parse_json_f	json_f;
};

struct test_def
{
	std::string		json_dir;
	float			json_size_in_kb;
	int				parse_count;
	int				write_count;
};

void test_push_back();

int _tmain(int argc, _TCHAR* argv[])
{
	test_push_back();

	int base_count = 50000;
	std::vector< test_def > test_defs =
	{
		{ "data-common", 1, base_count, base_count },
		{ "data-_5k", 0.5, base_count, base_count },
		{ "data-08k", 8, base_count, base_count },
		{ "data-32k", 32, base_count, base_count },
		{ "data-99k", 220, base_count, base_count },
	};

	std::vector< lib_def > lib_defs =
	{
		{ "___memcpy", parse_memcpy },
		{ "rapidjson", parse_rapidjson },
		{ "__libjson", parse_libjson },
		{ "__jsoncpp", parse_jsoncpp },
	};

	std::cout << "lib/size, parse, write" << std::endl;

	for (auto& test : test_defs)
	{
		for (auto& lib : lib_defs)
		{
				
			int parse_c = test.parse_count;
			int write_c = test.write_count;
#ifdef _DEBUG
			parse_c = parse_c / 1000;
			write_c = write_c / 1000;
#endif
			parse_c = parse_c / test.json_size_in_kb;
			write_c = write_c / test.json_size_in_kb;

			json_tester(lib.lib_name + std::string("_") + test.json_dir, lib.json_f, test.json_dir, parse_c, write_c);
		}
	}

	return 0;
}

