# ImGui Template

A ready-to-use Windows desktop application template built with **Dear ImGui** (v1.93.0) and **Direct3D 9**. It provides a clean, modern starting point for GUI tools with sensible defaults, dark mode, optional console logging, automatic versioning, and a simple application lifecycle.

## Features

- Direct3D 9 + ImGui rendering with device-lost and resize handling
- DPI-aware window creation
- Optional dark mode (via DWM)
- Optional console window for logging (`CLOG` / `ELOG`)
- Font loading from the Windows Fonts directory (Arial, Arial Bold, Times New Roman, etc.)
- Structured application lifecycle:
  - `App::Entrance()` — runs once on startup
  - `App::Render()` — called every frame
  - `App::UpdateWindowSize()` — called on resize
  - `App::Cleanup()` — runs on exit
  - `App::Debug()` — optional debug-only code
- Command-line argument capture
- Automatic version resource generation and build-number incrementing
- Precompiled headers for faster builds
- Includes the **KirKode** utility library (v2.6.1)

## Requirements

- Windows 10 or later
- Visual Studio 2022 (or newer) with the **Desktop development with C++** workload
- Windows SDK
- DirectX 9 SDK libraries (normally included with the Windows SDK)

## Project Structure

```
├── include/          # Headers (app, settings, fonts, pch, logging)
├── source/           # Implementation (main, app, fonts)
├── lib/
│   ├── imgui-v1.93.0/   # Dear ImGui headers
│   └── kirkode-v2.6.1/  # KirKode utility library
├── resources/        # Icon, version info, resource script, build script
├── ImGuiTemplate.vcxproj
└── ImGuiTemplate.vcxproj.user
```

## Building

1. Open `ImGuiTemplate.vcxproj` in Visual Studio.
2. Select your desired configuration (**Debug** or **Release**) and platform (**Win32** or **x64**).
3. Build the project (Ctrl+Shift+B).

The build process automatically runs `resources/build.ps1`, which:
- Increments the build number (`VERSION_BUILD`)
- Regenerates version defines used by the resource compiler

You can also build from the command line with MSBuild if preferred.

### Configuration Notes

- **Console logging**: Enabled when `_CONSOLE` is defined (default in Debug/Release for this project). A console window appears alongside the application.
- **Debug code**: Enabled when `_DEBUG` is defined. This includes the `App::Debug()` hook.
- These are controlled via preprocessor definitions in the project settings. Do not manually edit the defines in `settings.hpp` when using Visual Studio.

## Customization

Most settings live in `include/settings.hpp`:

| Setting                  | Description                                      |
|--------------------------|--------------------------------------------------|
| `WINDOW_NAME`            | Window title                                     |
| `WINDOW_WIDTH` / `HEIGHT`| Default window size                              |
| `WINDOW_BACKGROUND_COLOR`| Clear color (RGBA floats)                        |
| `WINDOW_DARK_MODE`       | Enable/disable dark title bar                    |
| `IMPORT_FONTS`           | Enable loading system fonts                      |
| `WINDOW_FONTS`           | List of fonts to load (name + size)              |

### Application Logic

Edit `source/app.cpp`:

- **`App::Entrance()`** — Initialization, loading data, etc.
- **`App::Render(ImGuiIO& io)`** — Your ImGui UI code (called every frame).
- **`App::Cleanup()`** — Shutdown logic.
- **`App::UpdateWindowSize()`** — React to window resizes.

Example UI is already present in `App::Render()`.

### Version Information

Edit company name, product description, copyright, etc. in `resources/version.hpp`.  
**Do not** manually edit the auto-generated version defines at the bottom — they are managed by `build.ps1`.

## Logging

When the console is enabled:

```cpp
CLOG("Info message");
ELOG("Error message");
```

These expand to nothing when `WINDOW_CONSOLE` is not defined.

## Fonts

Fonts are loaded from `%WINDIR%\Fonts`. The default set includes:

- `arial` (18 pt) — used as the default font
- `arialbd` (18 pt)
- `times` (16 pt)
- `timesbd` (16 pt)

Use them in your UI like this:

```cpp
ImGui::PushFont(Fonts::GetFont(io, "arialbd"));
ImGui::Text("Bold text");
ImGui::PopFont();
```

## Dependencies

- **Dear ImGui** v1.93.0 (headers included under `lib/imgui-v1.93.0/`)
- **KirKode** v2.6.1 (utility library under `lib/kirkode-v2.6.1/`)
- Windows API (D3D9, DWM, Shell, etc.)

The corresponding `.lib` files are already included in the `lib/` folder.

## License

This template is provided as-is for use in your projects. Dear ImGui is MIT-licensed. Check the KirKode headers for its licensing terms.

---

Happy building!
