#include <string>

class JsonTestWrapper
{
public:
	void SetJsonString(const std::string& json_str) { json_str_ = json_str; }
	virtual bool Parse() = 0;
	virtual std::string Write() = 0;

protected:
	std::string json_str_;
};
