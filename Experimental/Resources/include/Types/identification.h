#pragma once
#include <Types/primitives.h>

namespace Args
{
	using id_type = uint32;
	id_type invalid_id = 0;
	using entity_id = id_type;
	using process_id = id_type;
	using type_id = id_type;

	template<typename T>
	constexpr string GetTypeName()
	{
		static string typeName = "";
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

	template<typename T>
	constexpr id_type GetTypeHash()
	{
		static id_type hash = 0;
		if (hash == 0)
		{
			hash = 0x811c9dc5;
			uint32 prime = 0x1000193;
			string typeName = GetTypeName<T>();

			for (int i = 0; i < typeName.length(); i++)
			{
				byte value = typeName[i];
				hash = hash ^ value;
				hash *= prime;
			}
		}
		return hash;
	}

	template<typename T>
	constexpr id_type GetTypeHash(T expr)
	{
		return GetTypeHash<T>();
	}

	template<size_type N>
	constexpr id_type GetNameHash(const char(&name)[N])
	{
		id_type hash = 0;

		hash = 0x811c9dc5;
		uint32 prime = 0x1000193;
		for (int i = 0; i < N-1; i++)
		{
			byte value = name[i];
			hash = hash ^ value;
			hash *= prime;
		}

		return hash;
	}

}