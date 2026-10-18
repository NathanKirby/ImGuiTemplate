#include "pch.h"
#include "app.h"

#include "log.hpp"
#include "settings.hpp"
#include "fonts.h"

#ifdef WINDOW_DEBUG
void App::Debug() {
	/*
	* Put code here that you want to test
	*/
}
#endif // WINDOW_DEBUG

void App::Entrance() {
	running.store(true);

	/*
	* Put code here that you want to run when the app opens
	* Examples: Intialization, file verification, etc.
	*/
	
	CLOG("Entrance!");
}

void App::Render(ImGuiIO& io) {
	/*
	* Put your ImGui UI logic here
	* This will loop at your monitor's refresh rate
	*/

	// Example:
	ImGui::BeginChild("Template Window");
	if (ImGui::Button("Cool button")) {
		CLOG("Cool button!!!");
	}
	ImGui::SameLine();
	ImGui::Text("Cool text!!");
#ifdef IMPORT_FONTS
	ImGui::PushFont(Fonts::GetFont(io, "arialbd"));
	ImGui::Text("BOLD TEXT!!!");
	ImGui::PopFont();
#endif // IMPORT_FONTS
	ImGui::EndChild();
}

void App::UpdateWindowSize(const uint32_t newX, const uint32_t newY) {
	/*
	* Put code here that you want to run when the user changes the window size
	*/

	CLOG("Window size updated!");
}

void App::Cleanup() {
	if (clean.load()) return;

	/*
	* Put code here that you want to run before the application closes
	* Examples: Joining threads, deleting cache, etc.
	*/

	CLOG("Cleaned up!");
	clean.store(true);
}

void App::CaptureArguments(std::vector<std::string>&& a) {
	std::vector<std::string> args = std::move(a);
	if (args.empty()) return;
	try {
		std::lock_guard<std::mutex> lock(argMutex);
		appPath.swap(args[0]);
		args.erase(args.begin());
		if (!args.empty()) {
			arguments.swap(args);
		}
	}
#ifdef WINDOW_CONSOLE
	catch (const std::exception& e) {
		ELOG(std::string("Exception reached while capturing arguments: ") + e.what());
		return;
	}
#endif // WINDOW_CONSOLE
	catch (...) {
		return;
	}
}

void App::CloseWindow() {
	running.store(false);

	/*
	* Do not use this function for cleanup or deconstruction.
	* Use App::Cleanup for closing logic.
	*/

	CLOG("Closing window!");
}

bool App::IsRunning() noexcept {
	return running.load();
}
