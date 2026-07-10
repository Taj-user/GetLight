# GetLight

A lightweight Windows desktop app that brings up a window acting as a light panel.

---

## Screenshot

![GetLight in action](screenshots/demo.png)

---

## Features

- Toggle a borderless, always-on-top light panel with a global hotkey (`Ctrl+Alt+L`)
- Mouse wheel to adjust brightness
- Right click the window to open a color picker
- Color presets - Warm Light, White Light, Soft Yellow
- Fully resizable window
- Settings persist between sessions via `config.json`
- System tray icon with context menu
- Launch on startup toggle

---

## Tech Stack

- **Language:** C++17
- **API:** Win32
- **Build System:** CMake
- **Config:** nlohmann/json

---

## Building

### Requirements

- MinGW-w64 (g++)
- CMake 3.20+

### Steps

```bash
git clone https://github.com/Taj-user/GetLight.git
cd GetLight
mkdir build
cd build
cmake .. -G "MinGW Makefiles"
cmake --build .
```

The executable will be at `build/GetLight.exe`.

---

## Usage

1. Run `GetLight.exe` - it launches silently to the system tray
2. Press `Ctrl+Alt+L` to toggle the light panel
3. Scroll the mouse wheel/trackpad to adjust brightness
4. Right click the window to change color
5. Right click the tray icon for presets and settings
