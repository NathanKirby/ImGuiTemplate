#pragma once

#include <string>
#include <unordered_map>
#include <atomic>

struct ImFont;
struct ImGuiIO;

class Fonts {
private:
	inline static std::atomic<bool> fontsLoaded{ false };
	inline static std::unordered_map<std::string, ImFont*> fonts;
public:
	static void LoadFonts(ImGuiIO& io) noexcept;
public:
	static bool FontsLoaded() noexcept;
public:
	static ImFont* GetFont(ImGuiIO& io, const std::string& name) noexcept;
};
