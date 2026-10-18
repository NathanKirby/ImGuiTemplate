#pragma once

/*
* Compilation Settings
* 
* When working in visual studio, do not manually define/undefine these values.
* Use preprocessor definitions in the project settings.
* _CONSOLE -> defines WINDOW_CONSOLE and opens a log/console window along with the app.
* _DEBUG -> defines WINDOW_DEBUG and includes debug code to the build.
* 
* When working outside of visual studio, manually define/undefine WINDOW_DEBUG and WINDOW_CONSOLE.
* 
* If you are looking to change the file description or company name, do so in version.hpp.
*/

// Visual studio:
#ifdef _MSC_VER
#ifdef _CONSOLE
#define WINDOW_CONSOLE
#endif // _CONSOLE
#ifdef _DEBUG
#define WINDOW_DEBUG
#endif // _DEBUG

// Not visual studio (read above):
#else
#define WINDOW_DEBUG 
#define WINDOW_CONSOLE
#endif // _MSC_VER

/*
* Window Properties
* 
* Here you will set window name(s), resolution, and background color.
* Defining WINDOW_DARK_MODE enables darkmode. When undefined, it falls back to default style.
*/

#define WINDOW_DARK_MODE

static constexpr const wchar_t* WINDOW_NAME = L"ImGui Template";
static constexpr int WINDOW_WIDTH = 1280;
static constexpr int WINDOW_HEIGHT = 720;

static constexpr float WINDOW_BACKGROUND_COLOR[4] = {
	0.15f, // Red 
	0.15f, // Green
	0.15f, // Blue
	1.00f  // Alpha
};

#ifdef WINDOW_CONSOLE
static constexpr const wchar_t* CONSOLE_NAME = L"ImGui Template Console";
#endif // WINDOW_CONSOLE

/*
* Fonts
* 
* Here you will define the font(s) (if any) you want to include and use in the app.
* Remember to include the font define instead of the font file name (the one marked with .ttf).
* 
* If IMPORT_FONTS is undefined (or no valid fonts exist in WINDOW_FONTS), the app falls back to ImGui's default font.
*/

#define IMPORT_FONTS

#ifdef IMPORT_FONTS
struct font_define { const char* name; float size; };
static constexpr font_define WINDOW_FONTS[] = {
	{ "arial", 18.0f }, // First will be used for default
	{ "arialbd", 18.0f },
	{ "times", 16.0f },
	{ "timesbd", 16.0f }
};
#endif // IMPORT_FONTS
