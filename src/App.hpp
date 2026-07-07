#pragma once

#define WM_TRAYICON             (WM_APP + 1)
#define IDM_PRESET_WARM         101
#define IDM_PRESET_WHITE        102
#define IDM_PRESET_SOFT         103
#define IDM_STARTUP             104
#define IDM_QUIT                105

#include <windows.h>
#include "Config.hpp"

class App {
        public:
                App(HINSTANCE hInstance);
                ~App();

                bool            init();
                int             run();

        private:
                Config          m_config;
                HINSTANCE       m_hInstance;
                HWND            m_hwnd;
                NOTIFYICONDATA  m_nid;
                bool            m_visible;

                void            addTrayIcon();
                void            adjustOpacity(int delta);
                void            handleMenuCommand(WORD id);
                void            paintWindow(HDC hdc);
                void            removeTrayIcon();
                void            showContextMenu();
                void            toggleVisibility();

                bool            createWindow();
                bool            registerWindowClass();

                static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};
