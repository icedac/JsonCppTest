#include "stdafx.h"
#include "json_test.h"
#include <iostream>
#include "libjason/libjson.h"


void ParseJSON(const JSONNode & n){
	JSONNode::const_iterator i = n.begin();
	while (i != n.end()){
		// recursively call ourselves to dig deeper into the tree
		if (i->type() == JSON_ARRAY || i->type() == JSON_NODE){
			ParseJSON(*i);
		}

		// get the node name and value as a string
		std::string node_name = i->name();
		std::string rootA;
		std::string childA;
		int childB;

		// find out where to store the values
		if (node_name == "RootA"){
			rootA = i->as_string();
		}
		else if (node_name == "ChildA"){
			childA = i->as_string();
		}
		else if (node_name == "ChildB")
			childB = i->as_int();

		//increment the iterator
		++i;
	}
}

int test_libjson(const std::string& json_str)
{
	// std::string json = "{\"RootA\":\"Value in parent node\",\"ChildNode\":{\"ChildA\":\"String Value\",\"ChildB\":42}}";
	JSONNode n = libjson::parse(json_str);
	ParseJSON(n);

	// std::string jc = n.write_formatted();
	std::string jc = n.write();
	std::cout << jc << std::endl;
	return 0;
}

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
		return obj_.write();
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
