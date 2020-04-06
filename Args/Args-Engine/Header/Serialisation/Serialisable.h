#pragma once
#include <string>

class ISerialisable
{
public:
	virtual std::string ObjectType() = 0;
	virtual bool SetData(const std::string& name, const std::string& value) = 0;
	virtual bool GetData(const std::string& name, std::string& value) = 0;
};