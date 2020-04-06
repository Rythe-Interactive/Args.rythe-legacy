#pragma once
#include <rapidjson/document.h>
#include <string>
using namespace rapidjson;

namespace Args
{
	class JSONLoader
	{
	public:
		std::string filePath;
		JSONLoader();
		JSONLoader(std::string path);
		std::string LoadSceneFile(std::string fileName);
		void LoadSetupSettings(std::string fileName);
		std::string LoadKeyMap(std::string fileName);
	};
}
