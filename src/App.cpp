#include "App.hpp"

App::App(HINSTANCE hInstance) 
        : m_hInstance   (hInstance)
        , m_hwnd        (nullptr)
        , m_visible     (false)
{}

App::~App() {
        removeTrayIcon();
        m_config.save();
        UnregisterHotKey(m_hwnd, 1);
}

void App::addTrayIcon() {
        m_nid                   = {};
        m_nid.cbSize            = sizeof(NOTIFYICONDATA);
        m_nid.hWnd              = m_hwnd;
        m_nid.uID               = 1;
        m_nid.uFlags            = NIF_ICON | NIF_MESSAGE | NIF_TIP;
        m_nid.uCallbackMessage  = WM_TRAYICON;
        m_nid.hIcon             = LoadIcon(nullptr, IDI_APPLICATION);
        lstrcpy(m_nid.szTip, "GetLight");
        Shell_NotifyIcon(NIM_ADD, &m_nid);
}

void App::adjustOpacity(int delta) {
        m_config.opacity += delta;
        if(m_config.opacity > 255) m_config.opacity = 255;
        if(m_config.opacity < 30) m_config.opacity = 30;
        SetLayeredWindowAttributes(m_hwnd, 0, m_config.opacity, LWA_ALPHA);
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

void App::disableStartup() {
        HKEY key;
        RegOpenKeyEx(HKEY_CURRENT_USER,
                        "Software\\Microsoft\\Windows\\CurrentVersion\\Run",
                        0,
                        KEY_SET_VALUE, &key\
                        );
        RegDeleteValue(key, "GetLight");
        RegCloseKey(key);

        m_config.launchOnStartup = false;
}

void App::enableStartup() {
        char path[MAX_PATH];
        GetModuleFileName(nullptr, path, MAX_PATH);

        HKEY key;
        RegOpenKeyEx(HKEY_CURRENT_USER,
                        "Software\\Microsoft\\Windows\\CurrentVersion\\Run",
                        0,
                        KEY_SET_VALUE, &key
                        );
        RegSetValueEx(key, "GetLight", 0, REG_SZ, (BYTE*)path, strlen(path) + 1);
        RegCloseKey(key);

        m_config.launchOnStartup = true;
}

void App::handleMenuCommand(WORD id) {
        switch(id) {
                case IDM_PRESET_WARM:
                        m_config.color = {255, 244, 214};
                        InvalidateRect(m_hwnd, nullptr, TRUE);
                        break;

                case IDM_PRESET_WHITE:
                        m_config.color = {255, 255, 255};
                        InvalidateRect(m_hwnd, nullptr, TRUE);
                        break;

                case IDM_PRESET_SOFT:
                        m_config.color = {255, 230, 180};
                        InvalidateRect(m_hwnd, nullptr, TRUE);
                        break;

                case IDM_STARTUP:
                        if(m_config.launchOnStartup)
                                disableStartup();
                        else
                                enableStartup();
                        break;

                case IDM_QUIT:
                        DestroyWindow(m_hwnd);
                        break;
        }
}

bool App::init() {
        if(!m_config.load()) return false;
        if(!registerWindowClass())      return false;
        if(!createWindow())             return false;
        if(!RegisterHotKey(m_hwnd, 1, MOD_CONTROL | MOD_ALT, 'L')) {
                MessageBox(nullptr, "Failed to register hotkey", "Error", MB_OK);
                return false;
        }
        addTrayIcon();
        return true;
}


void App::paintWindow(HDC hdc) {
        RECT rc;
        GetClientRect(m_hwnd, &rc);
        HBRUSH brush = CreateSolidBrush(RGB(m_config.color.r, m_config.color.g, m_config.color.b));
        FillRect(hdc, &rc, brush);
        DeleteObject(brush);
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

void App::removeTrayIcon() {
        Shell_NotifyIcon(NIM_DELETE, &m_nid);
}

int App::run() {
        MSG msg = {};
        while(GetMessage(&msg, nullptr, 0, 0)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
        }
        return static_cast<int>(msg.wParam);
}

void App::showContextMenu() {
        HMENU menu = CreatePopupMenu();

        AppendMenu(menu, MF_STRING, IDM_PRESET_WARM, "Warm Light");
        AppendMenu(menu, MF_STRING, IDM_PRESET_WHITE, "White Light");
        AppendMenu(menu, MF_STRING, IDM_PRESET_SOFT, "Soft Yellow");
        AppendMenu(menu, MF_SEPARATOR, 0, nullptr);
        AppendMenu(menu, MF_STRING | (m_config.launchOnStartup ? MF_CHECKED : MF_UNCHECKED), IDM_STARTUP, "Launch on Startup");
        AppendMenu(menu, MF_SEPARATOR, 0, nullptr);
        AppendMenu(menu, MF_STRING, IDM_QUIT, "Quit");

        POINT pt;
        GetCursorPos(&pt);

        SetForegroundWindow(m_hwnd);
        TrackPopupMenu(menu, TPM_BOTTOMALIGN | TPM_LEFTALIGN, pt.x, pt.y, 0, m_hwnd, nullptr);
        DestroyMenu(menu);

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

LRESULT CALLBACK App::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        if(uMsg == WM_NCCREATE) {
                CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
                SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(cs->lpCreateParams));
        }

        App* app = reinterpret_cast<App*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

        switch(uMsg) {
                case WM_COMMAND:
                        if(app) app->handleMenuCommand(LOWORD(wParam));
                        return 0;

                case WM_DESTROY:
                        PostQuitMessage(0);
                        return 0;

                case WM_HOTKEY: {
                        if(wParam == 1 && app)
                                app->toggleVisibility();
                        return 0;
                }

                case WM_KEYDOWN:
                        if(wParam == VK_ESCAPE)
                                PostQuitMessage(0);
                        return 0;

                case WM_MOUSEWHEEL: {
                        int delta = GET_WHEEL_DELTA_WPARAM(wParam);
                        if(app) app->adjustOpacity(delta > 0 ? 15 : -15);
                        return 0;
                }

                case WM_NCHITTEST:
                        return HTCAPTION;

                case WM_PAINT: {
                        PAINTSTRUCT ps;
                        HDC hdc = BeginPaint(hwnd, &ps);
                        if(app) app->paintWindow(hdc);
                        EndPaint(hwnd, &ps);
                        return 0;
                }

                case WM_TRAYICON:
                        if(lParam == WM_RBUTTONUP && app)
                                app->showContextMenu();
                        return 0;
        }
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
