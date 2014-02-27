#include "stdafx.h"
#include "json_test.h"

#include "json/json.h"



class JsonTestWrapper_jsoncpp : public JsonTestWrapper
{
public:
	JsonTestWrapper_jsoncpp() {}
	~JsonTestWrapper_jsoncpp() {}

	bool Parse() override
	{
		Json::Reader reader;
		if (reader.parse(json_str_, obj_))
			return true;
		return false;
	}

	std::string Write() override
	{
		Json::FastWriter writer;
		return writer.write(obj_);
	}

private:
	Json::Value			obj_;
};

JsonTestWrapper* parse_jsoncpp(const std::string& json_str)
{
	auto json = new JsonTestWrapper_jsoncpp();
	json->SetJsonString(json_str);
	return json;
}
