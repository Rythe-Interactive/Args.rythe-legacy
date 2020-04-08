#include "Serialisation/JSONLoader.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "Utils/Common.h"

#include <fstream>
#include <iostream>
#include <array>
#include <list>
#include <filesystem>

using namespace rapidjson;


Args::JSONLoader::JSONLoader()
{
}

Document Args::JSONLoader::LoadFile(std::string fileName)
{
	std::fstream file(fileName, std::ios::in);
	std::string json;
	if (file.is_open())
	{
		std::string line = "";

		while (getline(file, line))
			json += line + '\n';

		file.close();
	}
	else
	{
		Debug::Error(DebugInfo, "Error reading file %s", fileName.c_str());
		return Document();
	}

	Document dom;
	dom.Parse(json.c_str());

	if (dom.IsObject())
		return dom;

	Debug::Error(DebugInfo, "Internals of file %s are incorrect.", fileName.c_str());
	return Document();
}

