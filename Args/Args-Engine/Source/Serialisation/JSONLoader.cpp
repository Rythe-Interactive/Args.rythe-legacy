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
	filePath = "Assets/JSON/";
}

Args::JSONLoader::JSONLoader(std::string path)
{
	filePath = path;
}

std::string Args::JSONLoader::LoadSceneFile(std::string fileName)
{
	std::fstream inFile;
	std::string json;
	inFile.open(filePath + "JSONScenes/" + fileName + ".JSON");
	if (!inFile)
	{
		Debug::Error(DebugInfo, "Unable to open file: %s", fileName.c_str());
		return "";
	}
	if (inFile.is_open())
	{
		getline(inFile, json);
	}
	inFile.close();

	return json;
}


void Args::JSONLoader::LoadSetupSettings(std::string fileName)
{
}

std::string Args::JSONLoader::LoadKeyMap(std::string fileName/*,InputSystem inputSys*/)
{
	std::fstream inFile;
	std::string json;
	inFile.open(filePath + "JSONKeymap/" + fileName);
	if (!inFile)
	{
		Debug::Error(DebugInfo, "Unable to open file: %s", fileName.c_str());
		return "";
	}
	if (inFile.is_open())
	{
		getline(inFile, json);
	}
	inFile.close();
	return json;
}

