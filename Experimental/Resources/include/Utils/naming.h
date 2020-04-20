#pragma once
#include <Types/primitives.h>

namespace Args
{
	template<typename T>
	constexpr string GetTypeName()
	{
		static string typeName;
		if (typeName == "")
		{
			typeName = typeid(T).name();
			size_type token;
			if (token = typeName.find("struct ") != string::npos)
				typeName = typeName.substr(token + 6);
			else if (token = typeName.find("class ") != string::npos)
				typeName = typeName.substr(token + 5);

			if (token = typeName.find("Args::") != string::npos)
				typeName = typeName.substr(token + 5);
		}
		return typeName;
	}

	template<typename T>
	constexpr string GetTypeName(T expr)
	{
		return GetTypeName<T>();
	}
}