#pragma once

#define DEFAULT				1
#define TEMP				2

#define SUCCESS				3
#define WARNING				4
#ifndef ERROR
#define ERROR				0
#endif // !ERROR

//#define LIGHT_MODE

#ifndef LIGHT_MODE
#define DEBUG_BLUE			9
#define DEBUG_GREEN			10
#define DEBUG_RED			12
#define DEBUG_LIGHTBLUE		11
#define DEBUG_PURPLE		13
#define DEBUG_YELLOW		14
#define DEBUG_WHITE			15
#define DEBUG_GREY			8
#define DEBUG_LIGHTGREY		7
#else
#define DEBUG_BLUE			249
#define DEBUG_GREEN			250
#define DEBUG_RED			252
#define DEBUG_LIGHTBLUE		243
#define DEBUG_PURPLE		245
#define DEBUG_YELLOW		246
#define DEBUG_WHITE			240
#define DEBUG_GREY			247
#define DEBUG_LIGHTGREY		247
#endif