#pragma once

#include <atomic>
#include <cstdint>
#include <mutex>
#include <vector>
#include <string>

#include "settings.hpp"

struct ImGuiIO;

class App {
private:
	inline static std::mutex argMutex;
	inline static std::string appPath;
	inline static std::vector<std::string> arguments;
private:
	inline static std::atomic<bool> running{ false };
	inline static std::atomic<bool> clean{ false };
#ifdef WINDOW_DEBUG
public:
	static void Debug();
#endif // WINDOW_DEBUG
public:
	static void Entrance();
	static void Render(ImGuiIO& io);
	static void UpdateWindowSize(const uint32_t newX, const uint32_t newY);
	static void Cleanup();
public:
	static void CaptureArguments(std::vector<std::string>&& args);
	static void CloseWindow();
public:
	static bool IsRunning() noexcept;
};
