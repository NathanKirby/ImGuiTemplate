#pragma once

// std
#include <cstdint>
#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>

// IO
#include <fstream>
#include <filesystem>

// ImGui
#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_dx9.h>
#include <imgui/backends/imgui_impl_win32.h>

// KirKode
#include <kirkode.h>

// Windows (must be last)
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>