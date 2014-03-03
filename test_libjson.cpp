#include "stdafx.h"
#include "json_test.h"
#include <iostream>
#include "libjason/libjson.h"


class JsonTestWrapper_libjson : public JsonTestWrapper
{
public:
	JsonTestWrapper_libjson() {}
	~JsonTestWrapper_libjson() {}

	bool Parse() override
	{
		obj_ = libjson::parse(this->json_str_);
		if (obj_.empty()) return false;
		return true;
	}
	std::string Write() override
	{
		return obj_.write(this->json_str_.size());
	}

private:
	JSONNode obj_;
};

JsonTestWrapper* parse_libjson(const std::string& json_str)
{
	auto json = new JsonTestWrapper_libjson();
	json->SetJsonString(json_str);
	return json;
}
