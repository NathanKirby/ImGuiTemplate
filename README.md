# C++ ImGui Application Template

A lightweight C++ application template for building desktop applications with **Dear ImGui**. This project provides a ready-to-use foundation with rendering, window management, and build configuration already set up.

## Features

* **C++17 or newer**

  * Uses modern C++ features while remaining compatible with C++17.
  * Can be upgraded to newer C++ standards as needed.

* **Dear ImGui integration**

  * Preconfigured ImGui setup.
  * Includes backend support for window handling and rendering.
  * Ready for building custom interfaces immediately.

* **Static dependencies**

  * All dependencies are compiled statically into the application.
  * No external runtime libraries or complicated linking setup required.
  * Simple project structure suitable for personal tools and utilities.

* **Precompiled headers**

  * Precompiled headers are configured and ready to use.
  * Reduces compilation times by caching commonly used headers.

* **Safe window management**

  * Window creation, message handling, resizing, and cleanup are handled safely.
  * Device loss and resource recovery are handled automatically.

* **Refresh-rate synchronized rendering**

  * The application runs synchronized with your monitor's refresh rate.
  * Uses vertical synchronization to avoid unnecessary CPU/GPU usage and screen tearing.

## Requirements

* Windows
* Visual Studio 2022 or newer
* C++17-compatible compiler

## Getting Started

1. Clone the repository:

```bash
git clone <repository-url>
```

2. Open the solution file in Visual Studio.

3. Build the project.

4. Start creating your ImGui application.

The template is already configured with:

* Window creation
* DirectX rendering
* ImGui initialization
* Main application loop
* Cleanup handling

## Project Structure

```
Application/
├── include/
|   ├── App.h
│   └── pch.h
│
├── source/
│   ├── App.cpp
│   ├── Main.cpp
│   └── pch.cpp
│
├── lib/
│   ├── imgui/
│   └── other dependencies/
│
└── Application.sln
```

## Customization

The main UI logic can be added inside the application render function.

Example:

```cpp
void App::Render(ImGuiIO& io)
{
    ImGui::Begin("Example Window");
    ImGui::Text("Hello, ImGui!");
    ImGui::End();
}
```

## Design Goals

This template is designed to remove the repetitive setup work involved with creating C++ ImGui applications.

Instead of spending time configuring:

* window creation,
* rendering backends,
* dependency linking,
* build settings,
* precompiled headers,

you can focus directly on building your application.
