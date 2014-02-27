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

int test_rapidjson(const std::string& json_str);
int test_libjson(const std::string& json_str);

/*
*/
class JsonTestWrapper_memcpy : public JsonTestWrapper
{
public:
	JsonTestWrapper_memcpy() {}
	~JsonTestWrapper_memcpy() {}

	bool Parse() override
	{
		data_ = std::string(this->json_str_);
		return true;
	}
	std::string Write() override
	{
		return std::string(data_);
	}

private:
	std::string	data_;
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

void json_tester(const std::string& name, parse_json_f p, const std::vector< std::string >& json_files, int parse_count, int write_count)
{
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

	std::vector< JsonTestWrapper* > json_objs;
	for (auto s : json_strings)
		json_objs.push_back(p(s));

	// parse test
	std::cout << name.c_str() << ": parsing [" << parse_count << "]loops";
	auto start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < parse_count; ++i)
	{
		for (auto json : json_objs)
		{
			json->Parse();
		}
	}
	auto end = std::chrono::high_resolution_clock::now();	std::cout << " [" << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "]msec\n";

	// write test
	std::cout << name.c_str() << ": writing [" << write_count << "]loops";
	start = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < write_count; ++i)
	{
		for (auto json : json_objs)
		{
			json->Write();
		}
	}
	end = std::chrono::high_resolution_clock::now();	std::cout << " [" << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "]msec\n";
}

int _tmain(int argc, _TCHAR* argv[])
{
	//std::string json_str = " { \"hello\" : \"world\", \"t\" : true , \"f\" : false, \"n\": null, \"i\":123, \"pi\": 3.1416, \"a\":[1, 2, 3, 4] } ";
	//std::string json_str2 = "{\"RootA\":\"Value in parent node\",\"ChildNode\":{\"ChildA\":\"String Value\",\"ChildB\":42}}";

	//JsonTestWrapper* r_j = parse_rapidjson(json_str);
	//std::string r_s = r_j->Write();
	//std::cout << r_s.c_str() << std::endl;

	//JsonTestWrapper* l_j = parse_libjson(json_str);
	//std::string l_s = l_j->Write();
	//std::cout << l_s.c_str() << std::endl;

	int sm_parse_c = 100000;
	int sm_write_c = 100000;
	int bg_parse_c = 3000;
	int bg_write_c = 3000;

	std::vector< std::string > json_data_1 =
	{
		R"(data/glossary.json)",	// 0.6k
		R"(data/webapp.json)",		// 3.5k
		R"(data/menu.json)",		// 0.9k
		R"(data/webapp.json)",		// 3.5k
		R"(data/widget.json)",		// 0.6k
		R"(data/webapp.json)",		// 3.5k
	};
	std::vector< std::string > json_data_2 =
	{
		R"(data/glossary.json)",	// 0.6k
		R"(data/menu.json)",		// 0.9k
		R"(data/widget.json)",		// 0.6k
		R"(data/webapp.json)",		// 3.5k
		R"(data2/sample.json)",		// 687k
	};

	json_tester("___memcpy_small", parse_memcpy, json_data_1, sm_parse_c, sm_write_c);
	json_tester("rapidjson_small", parse_rapidjson, json_data_1, sm_parse_c, sm_write_c);
	json_tester("__libjson_small", parse_libjson, json_data_1, sm_parse_c, sm_write_c);
	json_tester("__jsoncpp_small", parse_jsoncpp, json_data_1, sm_parse_c, sm_write_c);

	json_tester("___memcpy_big", parse_memcpy, json_data_2, bg_parse_c, bg_write_c);
	json_tester("rapidjson_big", parse_rapidjson, json_data_2, bg_parse_c, bg_write_c);
	json_tester("__libjson_big", parse_libjson, json_data_2, bg_parse_c, bg_write_c);
	json_tester("__jsoncpp_big", parse_jsoncpp, json_data_2, bg_parse_c, bg_write_c);

	return 0;
}

