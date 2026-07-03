#pragma once
#include <windows.h>

class App {
        public:
                App(HINSTANCE hInstance);
                ~App();

                bool            init();
                int             run();

        private:
                HINSTANCE       m_hInstance;
                HWND            m_hwnd;
                bool            m_visible;

                void            toggleVisibility();

                bool            registerWindowClass();
                bool            createWindow();

                static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};
