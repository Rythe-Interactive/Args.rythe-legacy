#pragma once

#if defined(_MSC_VER)
#define VC_EXTRA_LEAN
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h> //define APIENTRY
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>