#pragma once

#include <cinttypes>
#include <memory>
#include <string>
#include "Config.h"
#include "Debug.h"


namespace Args
{
#define INHERITS_FROM(type, base_type) \
	typename = std::enable_if_t<std::is_base_of_v<base_type, type>>

#define CONCAT2(a, b) a ## b

#define CONCAT(a, b) CONCAT2(a, b)

#define TextureDir			std::string("Assets/Textures/")
#define MissingTextureDir	std::string("Assets/Textures/missing-texture.png")
#define FontDir				std::string("Assets/Fonts/")
#define ShaderDir			std::string("Assets/Shaders/")
#define ModelDir			std::string("Assets/Models/")
#define SceneDir			std::string("Assets/Scenes/")
#define ObjectDir			std::string("Assets/Objects/")
#define DataDir				std::string("Assets/Data/")
#define AudioDir			std::string("Assets/Audio/")
#define AssetDir			std::string("Assets/")

#define InvalidID 0

	using uint = unsigned int;

	using int8 = std::int8_t;
	using int16 = std::int16_t;
	using int32 = std::int32_t;
	using int64 = std::int64_t;
	using uint8 = std::uint8_t;
	using uint16 = std::uint16_t;
	using uint32 = std::uint32_t;
	using uint64 = std::uint64_t;

	template<class OwnerType, typename ReturnType, typename ...Arguments>
	using FuncPtr = ReturnType(OwnerType::*)(Arguments...);

	template<class OwnerType>
	using UpdateFunc = FuncPtr<OwnerType, void, float>;

	template<class OwnerType>
	using InitFunc = FuncPtr<OwnerType, void>;


	template<typename T>
	constexpr std::string GetTypeName()
	{
		std::string typeName = typeid(T).name();
		size_t token;
		if (token = typeName.find("struct ") != std::string::npos)
			typeName = typeName.substr(token + 6);
		else if (token = typeName.find("class ") != std::string::npos)
			typeName = typeName.substr(token + 5);

		if(token = typeName.find("Args::") != std::string::npos)
			typeName = typeName.substr(token + 5);

		return typeName;
	}

	template<typename T>
	constexpr std::string GetTypeName(T expr)
	{
		return GetTypeName<T>();
	}
}