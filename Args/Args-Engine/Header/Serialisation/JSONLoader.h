#pragma once
#include <rapidjson/document.h>
#include <string>
using namespace rapidjson;

namespace Args
{
	class JSONLoader
	{
	public:
		JSONLoader();
		Document LoadFile(std::string fileName);
	};
}
