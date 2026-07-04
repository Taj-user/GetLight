#pragma once
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
                bool            m_visible;

                void            adjustOpacity(int delta);
                void            paintWindow(HDC hdc);
                void            toggleVisibility();

                bool            createWindow();
                bool            registerWindowClass();

                static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};
