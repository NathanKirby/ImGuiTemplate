#include "pch.h"

// d3d9 for window/rendering
#include <d3d9.h>

// shellapi for arguments
#include <shellapi.h>

// dwmapi for window styling
#include <dwmapi.h>

// Settings for window
#include "settings.hpp"

// Main logic for app interface
#include "app.h"

// Log helper
#include "log.hpp"

// Fonts 
#include "fonts.h"

// Resources for icons and more
#include "resource.h"

// Background color (defined in settings.hpp)
static const D3DCOLOR backgroundColorD3D = D3DCOLOR_RGBA(
	static_cast<int32_t>(WINDOW_BACKGROUND_COLOR[0] * WINDOW_BACKGROUND_COLOR[3] * 255.0f),
	static_cast<int32_t>(WINDOW_BACKGROUND_COLOR[1] * WINDOW_BACKGROUND_COLOR[3] * 255.0f),
	static_cast<int32_t>(WINDOW_BACKGROUND_COLOR[2] * WINDOW_BACKGROUND_COLOR[3] * 255.0f),
	static_cast<int32_t>(WINDOW_BACKGROUND_COLOR[3] * 255.0f)
);

// Presentation params
static D3DPRESENT_PARAMETERS presentParams = {
	/*BackBufferWidth*/ 0, /*BackBufferHeight*/ 0, /*BackBufferFormat*/ D3DFMT_UNKNOWN, /*BackBufferCount*/ 0,
	/*MultiSampleType*/ D3DMULTISAMPLE_NONE, /*MultiSampleQuality*/ 0,
	/*SwapEffect*/ D3DSWAPEFFECT_DISCARD, /*hDeviceWindow*/ NULL, /*Windowed*/ TRUE,
	/*EnableAutoDepthStencil*/ TRUE, /*AutoDepthStencilFormat*/ D3DFMT_D16, /*Flags*/ 0,
	/*FullScreen_RefreshRateInHz*/ 0, /*PresentationInterval*/ D3DPRESENT_INTERVAL_ONE
};

// ImGui static vars
static LPDIRECT3D9 direct3D = nullptr;
static LPDIRECT3DDEVICE9 direct3DDeviceInterface = nullptr;
static bool isDeviceLost = false;
static uint32_t pendingResizeWidth = 0;
static uint32_t pendingResizeHeight = 0;

// Create window
static bool CreateDeviceD3D(HWND windowHandle) noexcept {
	try {
		direct3D = Direct3DCreate9(D3D_SDK_VERSION);
		if (!direct3D) {
			return false;
		}

		const HRESULT hResult = direct3D->CreateDevice(
			D3DADAPTER_DEFAULT, // Adapter
			D3DDEVTYPE_HAL, // Device type
			windowHandle, // Focus window
			D3DCREATE_HARDWARE_VERTEXPROCESSING, // Behavior flags
			&presentParams, // Presentation params
			&direct3DDeviceInterface // Outputted device interface
		);
		if (hResult > 0 || !direct3DDeviceInterface) {
			return false;
		}
	}
	catch (...) {
		return false;
	}
	return true;
}

// Cleanup/release on exit
static void CleanupDeviceD3D() noexcept {
	if (direct3DDeviceInterface) {
		direct3DDeviceInterface->Release();
		direct3DDeviceInterface = nullptr;
	}

	if (direct3D) {
		direct3D->Release();
		direct3D = nullptr;
	}
}

static void ResetDevice() noexcept {
	try {
		ImGui_ImplDX9_InvalidateDeviceObjects();

		const HRESULT hResult = direct3DDeviceInterface->Reset(&presentParams);
		if (hResult == D3DERR_INVALIDCALL) {
			IM_ASSERT(0);
		}

		ImGui_ImplDX9_CreateDeviceObjects();
	}
#ifdef WINDOW_CONSOLE
	catch (const std::exception& e) {
		ELOG("Exception caught while restarting device: " + std::string(e.what()));
	}
#endif // WINDOW_CONSOLE
	catch (...) {}
}

// Forward declare message handler from imgui_impl_win32.cpp for WindowProcedure below
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);

// Win32 message handler
static LRESULT WINAPI WindowProcedure(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam) noexcept {
	try {
		if (ImGui_ImplWin32_WndProcHandler(windowHandle, message, wParam, lParam)) {
			return 1;
		}

		switch (message) {
		case WM_CREATE:
#ifdef WINDOW_DEBUG
			try { App::Debug(); }	
#ifdef WINDOW_CONSOLE
			catch (const std::exception& e) {
				ELOG("Exception caught in debug code: " + std::string(e.what()));
			}
#endif // WINDOW_CONSOLE
			catch (...) {}
#endif // WINDOW_DEBUG
			try { App::Entrance(); }
#ifdef WINDOW_CONSOLE
			catch (const std::exception& e) {
				ELOG("Exception caught in app entrance: " + std::string(e.what()));
			}
#endif // WINDOW_CONSOLE
			catch (...) {}
			return 0;
		case WM_SIZE:
			if (wParam == SIZE_MINIMIZED) {
				return 0;
			}
			pendingResizeWidth = static_cast<uint32_t>(LOWORD(lParam));
			pendingResizeHeight = static_cast<uint32_t>(HIWORD(lParam));
			App::UpdateWindowSize(pendingResizeWidth, pendingResizeHeight);
			return 0;
		case WM_SYSCOMMAND:
			if ((wParam & 0xFFF0) == SC_KEYMENU) {
				return 0;
			}
			break;
		case WM_DESTROY:
			try {
				App::CloseWindow();
				App::Cleanup();
			}
#ifdef WINDOW_CONSOLE
			catch (const std::exception& e) {
				ELOG("Exception caught at WM_DESTROY: " + std::string(e.what()));
			}
#endif // WINDOW_CONSOLE
			catch (...) {}

			PostQuitMessage(0);
			return 0;
		}
		return DefWindowProcW(windowHandle, message, wParam, lParam);
	}
#ifdef WINDOW_CONSOLE
	catch (const std::exception& e) {
		ELOG("Unhandled exception caught in WindowProcedure: " + std::string(e.what()));
	}
#endif // WINDOW_CONSOLE
	catch (...) {
		return 0;
	}
	return 0;
}

#ifdef WINDOW_CONSOLE
static void AllocateConsole() {
	if (!AllocConsole()) return;

	FILE* fp = nullptr;

	// Redirect C streams
	freopen_s(&fp, "CONOUT$", "w", stdout);
	freopen_s(&fp, "CONOUT$", "w", stderr);
	freopen_s(&fp, "CONIN$", "r", stdin);

	// Force C++ streams to use the new underlying C streams
	std::ios::sync_with_stdio(true);

	// Clear any error state
	std::cout.clear();
	std::cerr.clear();
	std::clog.clear();
	std::wcout.clear();
	std::wcerr.clear();
	std::wclog.clear();
	std::cin.clear();

	SetConsoleTitle(CONSOLE_NAME);
}
#endif // WINDOW_CONSOLE

int32_t WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int32_t nCmdShow
) {
	try {
#ifdef WINDOW_CONSOLE
		// Create console
		AllocateConsole();
#endif // WINDOW_CONSOLE
		// Load icon
		HICON icon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));

		// Capture arguments
		try {
			int32_t argc = 0;
			LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
			if (argv && argc > 0) {
				std::vector<std::string> args;
				args.reserve(static_cast<size_t>(argc - 1));
				for (int32_t i = 0; i < argc; ++i) {
					const std::wstring arg(argv[i]);
					std::string argStr;
					argStr.reserve(arg.size());
					for (const wchar_t wc : arg) {
						if (wc > 255) continue;
						argStr.push_back(static_cast<char>(wc));
					}
					args.push_back(std::move(argStr));
				}
				LocalFree(argv);
				App::CaptureArguments(std::move(args));
			}
		}
#ifdef WINDOW_CONSOLE 
		catch (std::exception& e) {
			ELOG("Exception caught while handling arguments: " + std::string(e.what()));
		}
#endif // WINDOW_CONSOLE
		catch (...) {}

		// Get mouse DPI
		ImGui_ImplWin32_EnableDpiAwareness();
		const float mouseDPI = ImGui_ImplWin32_GetDpiScaleForMonitor(
			MonitorFromPoint(
				POINT{ 0L, 0L },
				MONITOR_DEFAULTTOPRIMARY
			)
		);

		// Create and register new class for window
		WNDCLASSEXW windowClass = {
			sizeof(WNDCLASSEXW),
			CS_CLASSDC,
			WindowProcedure,
			0L,
			0L,
			GetModuleHandleW(nullptr),
			icon,
			nullptr,
			nullptr,
			nullptr,
			WINDOW_NAME,
			nullptr
		};
		RegisterClassExW(&windowClass);

		// Create window handle 
		// Note: App::Entrance() called here
		const HWND windowHandle = CreateWindowW(
			windowClass.lpszClassName,
			WINDOW_NAME,
			WS_OVERLAPPEDWINDOW,
			100,
			100,
			static_cast<int32_t>(WINDOW_WIDTH * mouseDPI),
			static_cast<int32_t>(WINDOW_HEIGHT * mouseDPI),
			nullptr,
			nullptr,
			windowClass.hInstance,
			nullptr
		);

		// Create device
		{
			const bool createdDevice = CreateDeviceD3D(windowHandle);
			if (!createdDevice) {
				CleanupDeviceD3D();
				UnregisterClassW(windowClass.lpszClassName, windowClass.hInstance);
				ELOG("Failed to create new device.");
				return 1;
			}
		}

#ifdef WINDOW_DARK_MODE 
		{
			// Set dark mode if requested
			BOOL darkMode = TRUE;
			DwmSetWindowAttribute(windowHandle, DWMWA_USE_IMMERSIVE_DARK_MODE, &darkMode, sizeof(darkMode));
		}
#endif // WINDOW_DARK_MODE

		// Present/show the window
		ShowWindow(windowHandle, SW_SHOWDEFAULT);
		UpdateWindow(windowHandle);

		// Init ImGui
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		// Set ImGuiIO config flags
		ImGuiIO& imGuiIO = ImGui::GetIO();
		imGuiIO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		imGuiIO.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

		// Load font(s)
		Fonts::LoadFonts(imGuiIO);

		// Disable imgui.ini file creation
		imGuiIO.IniFilename = NULL;

		// Setup style
		{
			ImGuiStyle& style = ImGui::GetStyle();
			style.ScaleAllSizes(mouseDPI);
			style.FontScaleDpi = mouseDPI;
		}

		// Init ImGui backends
		ImGui_ImplWin32_Init(windowHandle);
		ImGui_ImplDX9_Init(direct3DDeviceInterface);

		// Main loop
		while (App::IsRunning()) {
			// Poll and handle messages (inputs, window resize, etc.)
			{
				MSG message;
				while (PeekMessageW(&message, nullptr, 0U, 0U, PM_REMOVE)) {
					TranslateMessage(&message);
					DispatchMessageW(&message);
				}
			}

			// Handle device lost
			if (isDeviceLost) {
				const HRESULT hResult = direct3DDeviceInterface->TestCooperativeLevel();
				if (hResult == D3DERR_DEVICELOST) {
					Sleep(10);
					continue;
				}
				if (hResult == D3DERR_DEVICENOTRESET) {
					ResetDevice();
				}
				isDeviceLost = false;
			}

			// Handle resize
			if (pendingResizeWidth != 0 && pendingResizeHeight != 0) {
				presentParams.BackBufferWidth = pendingResizeWidth;
				presentParams.BackBufferHeight = pendingResizeHeight;
				pendingResizeWidth = pendingResizeHeight = 0;
				ResetDevice();
			}

			// Create new ImGui frame
			ImGui_ImplDX9_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			// Define elements
			try { App::Render(imGuiIO); }
#ifdef WINDOW_CONSOLE 
			catch (std::exception& e) {
				ELOG("Exception caught while rendering app: " + std::string(e.what()));
			}
#endif // WINDOW_CONSOLE
			catch (...) {}

			// Clear current frame
			ImGui::EndFrame();
			direct3DDeviceInterface->SetRenderState(D3DRS_ZENABLE, FALSE);
			direct3DDeviceInterface->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			direct3DDeviceInterface->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
			direct3DDeviceInterface->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, backgroundColorD3D, 1.0f, 0);

			// Render ImGui frame
			{
				const HRESULT hResult = direct3DDeviceInterface->BeginScene();
				if (hResult >= 0) {
					ImGui::Render();
					ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
					direct3DDeviceInterface->EndScene();
				}
			}

			// Present window (even if device is lost)
			{
				const HRESULT hResult = direct3DDeviceInterface->Present(nullptr, nullptr, nullptr, nullptr);
				if (hResult == D3DERR_DEVICELOST) {
					isDeviceLost = true;
				}
			}
		}

		// Cleanup
		ImGui_ImplDX9_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

		CleanupDeviceD3D();
		DestroyWindow(windowHandle);
		UnregisterClassW(windowClass.lpszClassName, windowClass.hInstance);

		// Cleanup here just in case App::CloseWindow() was called without proper handling
		// Only runs if clean logic hasn't already ran
		try { App::Cleanup(); }
#ifdef WINDOW_CONSOLE 
		catch (std::exception& e) {
			ELOG("Exception caught in redundant app cleanup: " + std::string(e.what()));
		}
#endif // WINDOW_CONSOLE
		catch (...) {}
	}
#ifdef WINDOW_CONSOLE 
	catch (std::exception& e) {
		ELOG("Unhandled exception caught in main: " + std::string(e.what()));
		return 1;
	}
#endif // WINDOW_CONSOLE
	catch (...) { return 1; }
	return 0;
}
