#include <windows.h>

// Windows Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        switch(uMsg) {
                case WM_DESTROY:
                        PostQuitMessage(0);
                        return 0;

                case WM_PAINT: {
                        PAINTSTRUCT ps;
                        HDC hdc = BeginPaint(hwnd, &ps);
                        RECT rc;
                        GetClientRect(hwnd, &rc);
                        HBRUSH brush = CreateSolidBrush(RGB(255, 244, 214));
                        FillRect(hdc, &rc, brush);
                        DeleteObject(brush);
                        EndPaint(hwnd, &ps);
                        return 0;
                }

                case WM_HOTKEY: {
                        if(wParam == 1) {
                                if(IsWindowVisible(hwnd))
                                        ShowWindow(hwnd, SW_HIDE);
                                else
                                        ShowWindow(hwnd, SW_SHOW);
                        }
                        return 0;
                }

                case WM_NCHITTEST:
                        return HTCAPTION;

                case WM_KEYDOWN:
                        if(wParam == VK_ESCAPE)
                                PostQuitMessage(0);
                        return 0;
        }
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// Entry point
int WINAPI WinMain(HINSTANCE hInstance, [[maybe_unused]] HINSTANCE hPrevInstance, [[maybe_unused]] LPSTR lpCmdLine,[[maybe_unused]] int nCmdShow) {
        WNDCLASSEX wc           = {};
        wc.cbSize               = sizeof(WNDCLASSEX);
        wc.lpfnWndProc          = WndProc;
        wc.hInstance            = hInstance;
        wc.lpszClassName        = "GetLightClass";
        wc.hCursor              = LoadCursor(nullptr, IDC_ARROW);
        wc.hbrBackground        = nullptr;
        RegisterClassEx         (&wc);

        HWND hwnd = CreateWindowEx(
                        WS_EX_TOPMOST | WS_EX_LAYERED,
                        "GetLightClass",
                        "GetLight",
                        WS_POPUP,
                        100, 100,
                        500, 500,
                        nullptr, nullptr,
                        hInstance,
                        nullptr
                        );

        if(!hwnd) {
                MessageBox(nullptr, "Window creation failed", "Error", MB_OK);
                return 1;
        }

        SetLayeredWindowAttributes(hwnd, 0, 255, LWA_ALPHA);
        RegisterHotKey(hwnd, 1, MOD_CONTROL | MOD_ALT, 'L');
        ShowWindow(hwnd, SW_SHOW);
        UpdateWindow(hwnd);

        MSG msg = {};
        while(GetMessage(&msg, nullptr, 0, 0)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
        }

        return static_cast<int>(msg.wParam);
}
