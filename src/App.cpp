#include "App.hpp"

App::App(HINSTANCE hInstance) 
        : m_hInstance   (hInstance)
        , m_hwnd        (nullptr)
        , m_visible     (false)
{}

App::~App() {
        m_config.save();
        UnregisterHotKey(m_hwnd, 1);
}

void App::toggleVisibility() {
        m_visible = !m_visible;
        if(m_visible) {
                ShowWindow(m_hwnd, SW_SHOW);
                SetForegroundWindow(m_hwnd);
        }
        else {
                ShowWindow(m_hwnd, SW_HIDE);
        }
}

void App::paintWindow(HDC hdc) {
        RECT rc;
        GetClientRect(m_hwnd, &rc);
        HBRUSH brush = CreateSolidBrush(RGB(m_config.color.r, m_config.color.g, m_config.color.b));
        FillRect(hdc, &rc, brush);
        DeleteObject(brush);
}

void App::adjustOpacity(int delta) {
        m_config.opacity += delta;
        if(m_config.opacity > 255) m_config.opacity = 255;
        if(m_config.opacity < 30) m_config.opacity = 30;
        SetLayeredWindowAttributes(m_hwnd, 0, m_config.opacity, LWA_ALPHA);
}

bool App::init() {
        if(!m_config.load()) return false;
        if(!registerWindowClass())      return false;
        if(!createWindow())             return false;
        if(!RegisterHotKey(m_hwnd, 1, MOD_CONTROL | MOD_ALT, 'L')) {
                MessageBox(nullptr, "Failed to register hotkey", "Error", MB_OK);
                return false;
        }
        return true;
}

bool App::registerWindowClass() {
        WNDCLASSEX wc           = {};
        wc.cbSize               = sizeof(WNDCLASSEX);
        wc.lpfnWndProc          = WndProc;
        wc.hInstance            = m_hInstance;
        wc.lpszClassName        = "GetLightClass";
        wc.hCursor              = LoadCursor(nullptr, IDC_ARROW);
        wc.hbrBackground        = nullptr;
        return RegisterClassEx(&wc) != 0;
}

bool App::createWindow() {
        m_hwnd = CreateWindowEx(
                        WS_EX_TOPMOST | WS_EX_LAYERED,
                        "GetLightClass",
                        "GetLight",
                        WS_POPUP,
                        m_config.x, m_config.y,
                        m_config.width, m_config.height,
                        nullptr, nullptr,
                        m_hInstance,
                        this
                        );

        if(!m_hwnd) {
                MessageBox(nullptr, "Window creation failed", "Error", MB_OK);
                return false;
        }

        SetLayeredWindowAttributes(m_hwnd, 0, m_config.opacity, LWA_ALPHA);
        return true;
}

LRESULT CALLBACK App::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        if(uMsg == WM_NCCREATE) {
                CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
                SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(cs->lpCreateParams));
        }

        App* app = reinterpret_cast<App*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

        switch(uMsg) {
                case WM_DESTROY:
                        PostQuitMessage(0);
                        return 0;

                case WM_PAINT: {
                        PAINTSTRUCT ps;
                        HDC hdc = BeginPaint(hwnd, &ps);
                        if(app) app->paintWindow(hdc);
                        EndPaint(hwnd, &ps);
                        return 0;
                }

                case WM_HOTKEY: {
                        if(wParam == 1 && app)
                                app->toggleVisibility();
                        return 0;
                }

                case WM_NCHITTEST:
                        return HTCAPTION;

                case WM_KEYDOWN:
                        if(wParam == VK_ESCAPE)
                                PostQuitMessage(0);
                        return 0;

                case WM_MOUSEWHEEL: {
                        int delta = GET_WHEEL_DELTA_WPARAM(wParam);
                        if(app) app->adjustOpacity(delta > 0 ? 15 : -15);
                        return 0;
                }
        }
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int App::run() {
        MSG msg = {};
        while(GetMessage(&msg, nullptr, 0, 0)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
        }
        return static_cast<int>(msg.wParam);
}
