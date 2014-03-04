#include "stdafx.h"
#include <vector>
#include <array>
#include <iostream>
#include "json_test.h"

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
