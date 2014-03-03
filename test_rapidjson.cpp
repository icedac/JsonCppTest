#include "stdafx.h"
#include <vector>
#include <array>
#include <iostream>
#include "json_test.h"

#include "rapidjson/document.h"		// rapidjson's DOM-style API
#include "rapidjson/prettywriter.h"	// for stringify JSON
#include "rapidjson/filestream.h"	// wrapper of C stream for prettywriter as output
using namespace rapidjson;

struct NullStream {
	NullStream() : length_(0) {}
	void Put(char) { ++length_; }
	size_t length_;
};

// std::string::push_back is toooooo slower than std::vector::push_back.
struct OutputStringStream {
	OutputStringStream(size_t size_hint = 1024) : length_(0) { str_.size();  str_.reserve(size_hint + 1); }
	void Put(char c) { str_.push_back(c); ++length_; }
	const std::string& GetString() const { return str_; }
	std::string str_;
	size_t length_;
};

struct FastOutputStringStream {
	FastOutputStringStream(size_t size_hint = 1024) { buffer_.reserve(size_hint + 1); }
	void Put(char c) { buffer_.push_back(c); }
	std::string GetString() const { return buffer_.data(); }
	
	std::vector< char > buffer_;
};

struct OutputStringStream2 {
	OutputStringStream2(size_t size_hint = 1024)
	{ 
		buffer_.resize(size_hint + 1); 
		p_ = buffer_.data();
	}
	void Put(char c) 
	{
		++length_;
#if 1
		if (length_ > buffer_.size())
		{
			buffer_.resize(length_ * 2);
			p_ = &(buffer_.data()[length_]);
		}
		*p_ = c; ++p_;
#else
		buffer_.push_back(c);
#endif
	}
	std::string GetString() { 
		buffer_.resize(length_);
		return buffer_.data();
	}

	std::vector< char > buffer_;
	size_t length_ = 0;
	char* p_ = nullptr;
};

class JsonTestWrapper_rapidjson : public JsonTestWrapper
{
public:
	JsonTestWrapper_rapidjson() 
	{
	}
	~JsonTestWrapper_rapidjson() {}

	bool Parse() override
	{
#if 0
		if (obj_.Parse<0>(json_str_.c_str()).HasParseError())
			return false;
#else
		buffer_.resize(json_str_.size() + 1); // only allocate the temp memory first time
		::memcpy(buffer_.data(), json_str_.c_str(), json_str_.size() + 1);
		if (obj_.ParseInsitu<0>(buffer_.data() ).HasParseError())
			return false;
#endif
		return true;
	}

	std::string Write() override
	{
#if 1
		FastOutputStringStream rss(json_str_.size() + 1);
		Writer< FastOutputStringStream > writer(rss);
		obj_.Accept(writer);

		return rss.GetString();
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
