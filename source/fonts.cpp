#include "pch.h"
#include "fonts.h"

#include "settings.hpp"
#include "log.hpp"

static bool getWindowsDirectory(std::filesystem::path& out) noexcept {
	try {
		const uint32_t size = GetWindowsDirectoryW(nullptr, 0) + 1;
		if (size == 1) return false;

		std::vector<wchar_t> buffer(size);
		const uint32_t len = GetWindowsDirectoryW(buffer.data(), size);
		if (len == 0 || len >= size) return false;

		std::string pathString;
		pathString.reserve(size);
		for (uint32_t i = 0; i < len; ++i) {
			const wchar_t wc = buffer[i];
			if (wc > 255) return false;
			pathString.push_back(static_cast<char>(wc));
		}

		std::filesystem::path path(pathString);
		if (!std::filesystem::exists(path)) return false;

		out.swap(path);
	}
	catch (...) {
		return false;
	}
	return true;
}

void Fonts::LoadFonts(ImGuiIO& io) noexcept {
	if (fontsLoaded.load()) return;
#ifdef IMPORT_FONTS
	try {
		std::filesystem::path windowsDirectory;
		if (!getWindowsDirectory(windowsDirectory)) {
			throw std::runtime_error("Failed to get windows directory");
		}

		const std::filesystem::path fontsDirectory = windowsDirectory / "Fonts";
		if (!std::filesystem::exists(fontsDirectory)) {
			throw std::runtime_error("Failed to find 'Fonts' inside windows directory");
		}

		for (const font_define& fontDefine : WINDOW_FONTS) {
			const std::string fontFileName = std::string(fontDefine.name) + ".ttf";
			const std::filesystem::path fontPath = fontsDirectory / fontFileName;
			const std::string fontPathString = fontPath.string();
			if (!std::filesystem::exists(fontPath)) {
				ELOG("Failed to find font at path: " + fontPathString);
				continue;
			}
			ImFont* font = io.Fonts->AddFontFromFileTTF(fontPathString.c_str(), fontDefine.size);
			if (!font) {
				ELOG("Failed to load font from path: " + fontPathString);
				continue;
			}
			if (!io.FontDefault) {
				io.FontDefault = font;
			}
			fonts[fontDefine.name] = font;
		}

		if (fonts.empty()) {
			throw std::runtime_error("No fonts loaded");
		}
	}
#ifdef WINDOW_CONSOLE
	catch (const std::exception& e) {
		ELOG(std::string("Failed to load fonts: ") + e.what());
		CLOG("Using default ImGui font instead.");
		io.FontDefault = io.Fonts->AddFontDefault();
	}
#endif // WINDOW_CONSOLE
	catch (...) {
		io.FontDefault = io.Fonts->AddFontDefault();
	}
#else
	io.FontDefault = io.Fonts->AddFontDefault();
#endif // IMPORT_FONTS
	fontsLoaded.store(true);
}

bool Fonts::FontsLoaded() noexcept {
	return fontsLoaded.load();
}

#ifdef IMPORT_FONTS
ImFont* Fonts::GetFont(ImGuiIO& io, const std::string& name) noexcept {
	if (!fontsLoaded.load()) return nullptr;
	auto it = fonts.find(name);
	if (it == fonts.end()) {
		return io.FontDefault;
	}
	return it->second;
}
#endif // IMPORT_FONTS
