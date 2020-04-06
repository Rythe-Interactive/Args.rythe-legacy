#pragma once

#define enumStruct(name, ...)																	\
struct name																						\
{																								\
	enum HiddenEnum	: int32																		\
	{																							\
		__VA_ARGS__																				\
	};																							\
																								\
	HiddenEnum hiddenEnum;																		\
																								\
	operator int32() {return static_cast<int32>(hiddenEnum);}									\
																								\
	name& operator=(int32 value) {hiddenEnum = static_cast<HiddenEnum>(value); return *this;}	\
};