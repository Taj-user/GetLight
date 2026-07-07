#pragma once
#define WM_TRAYICON (WM_APP + 1)

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
                void            paintWindow(HDC hdc);
                void            removeTrayIcon();
                void            toggleVisibility();

                bool            createWindow();
                bool            registerWindowClass();

                static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};
