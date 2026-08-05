#pragma once

#include <atomic>
#include <cstdint>
#include <mutex>
#include <vector>
#include <string>

struct ImGuiIO;

class App {
private:
	inline static std::mutex argMutex;
	inline static std::string appPath;
	inline static std::vector<std::string> arguments;
private:
	inline static std::atomic<bool> running{ false };
	inline static std::atomic<bool> clean{ false };
public:
	static void Entrance() noexcept;
	static void Render(ImGuiIO& io) noexcept;
	static void UpdateWindowSize(const uint32_t newX, const uint32_t newY) noexcept;
	static void Cleanup() noexcept;
public:
	static void CaptureArguments(const int32_t argCount, char* args[]) noexcept;
	static void CloseWindow() noexcept;
public:
	static bool IsRunning() noexcept;
};