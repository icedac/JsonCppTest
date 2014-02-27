#include "stdafx.h"
#include <vector>
#include <array>
#include <iostream>
#include "json_test.h"

#include "rapidjson/document.h"		// rapidjson's DOM-style API
#include "rapidjson/prettywriter.h"	// for stringify JSON
#include "rapidjson/filestream.h"	// wrapper of C stream for prettywriter as output
using namespace rapidjson;

int test_rapidjson(const std::string& json_str)
{
	// printf("Original JSON:\n %s\n", json_str.c_str());

	Document document;	// Default template parameter uses UTF8 and MemoryPoolAllocator.

#if 0
	// "normal" parsing, decode strings to new buffers. Can use other input stream via ParseStream().
	if (document.Parse<0>(json).HasParseError())
		return 1;
#else
	// In-situ parsing, decode strings directly in the source string. Source must be string.

	// std::vector< char > buffer(json_str.c_str() );
	std::vector<char> buffer(json_str.begin(), json_str.end());
	buffer.push_back('\0');

	if (document.ParseInsitu<0>(buffer.data()).HasParseError())
		return 1;
#endif

	// printf("\nParsing to document succeeded.\n");
	// printf("\nModified JSON with reformatting:\n");
	//FileStream f(stdout);
	//Writer<FileStream> writer(f);
	//document.Accept(writer);	// Accept() traverses the DOM and generates Handler events.
	//std::cout << std::endl;

	ResultStringStream rss;
	Writer< ResultStringStream > writer(rss);
	document.Accept(writer);

	std::cout << rss.str().c_str() << std::endl;

	return 0;
}

struct NullStream {
	NullStream() : length_(0) {}
	void Put(char) { ++length_; }
	size_t length_;
};

class JsonTestWrapper_rapidjson : public JsonTestWrapper
{
public:
	JsonTestWrapper_rapidjson() {}
	~JsonTestWrapper_rapidjson() {}

	bool Parse() override
	{
		{
			std::vector<char> buffer(json_str_.begin(), json_str_.end());
			buffer.push_back('\0');
			buffer_.swap(buffer);
		}
		
		if (obj_.ParseInsitu<0>(buffer_.data()).HasParseError())
			return false;
		return true;
	}

	std::string Write() override
	{
#if 0
		ResultStringStream rss;
		Writer< ResultStringStream > writer(rss);
		obj_.Accept(writer);

		return rss.str();
#else
		NullStream ns;
		Writer< NullStream > writer(ns);
		obj_.Accept(writer);
		return std::string();
#endif

	}

private:
	std::vector<char>	buffer_;
	Document			obj_;
};

JsonTestWrapper* parse_rapidjson(const std::string& json_str)
{
	auto json = new JsonTestWrapper_rapidjson();
	json->SetJsonString(json_str);
	return json;
}
