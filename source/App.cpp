#include "pch.h"
#include "App.h"

void App::Entrance() noexcept {
	running.store(true);

	/*
	* Put code here that you want to run when the app opens
	* Examples: Intialization, file verification, etc.
	*/
	
	KLOG("Entrance!");
}

void App::Render(ImGuiIO& io) noexcept {
	/*
	* Put your ImGui UI logic here
	* This will loop at your monitor's refresh rate
	*/

	// Example:
	ImGui::BeginChild("Template Window");
	if (ImGui::Button("Cool button")) {
		kir::log::msg("Cool button!!!");
	}
	ImGui::SameLine();
	ImGui::Text("Cool text!!");
	ImGui::EndChild();
}

void App::UpdateWindowSize(const uint32_t newX, const uint32_t newY) noexcept {
	/*
	* Put code here that you want to run when the user changes the window size
	*/

	KLOG("Window size updated!");
}

void App::Cleanup() noexcept {
	if (clean.load()) return;

	/*
	* Put code here that you want to run before the application closes
	* Examples: Joining threads, deleting cache, etc.
	*/

	KLOG("Cleaned up!");
}

void App::CaptureArguments(const int32_t argCount, char* args[]) noexcept {
	if (argCount == 0) return;
	
	// First argument is always the path of the exe.
	std::string environment(args[0]);
	if (argCount == 1) {
		argMutex.lock();
		appPath.swap(environment);
		argMutex.unlock();
		return;
	}

	// Capture actual arguments
	std::vector<std::string> argList;
	try {
		argList.reserve(static_cast<size_t>(argCount - 1));
		for (int32_t i = 1; i < argCount; ++i) {
			argList.push_back(std::string(args[i]));
		}
	}
	catch (const std::exception& e) {
		kir::log::err("Error capturing arguments: ", &e);
		return;
	}

	argMutex.lock();
	appPath.swap(environment);
	arguments.swap(argList);
	argMutex.unlock();
}

void App::CloseWindow() noexcept {
	running.store(false);

	/*
	* Do not use this function for cleanup or deconstruction.
	* Use App::Cleanup for closing logic.
	*/

	KLOG("Closing window!");
}

bool App::IsRunning() noexcept {
	return running.load();
}