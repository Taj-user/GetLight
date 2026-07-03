#include <windows.h>
#include "App.hpp"

int WINAPI WinMain(HINSTANCE hInstance, [[maybe_unused]] HINSTANCE hPrevInstance, [[maybe_unused]] LPSTR lpCmdLine,[[maybe_unused]] int nCmdShow) {
        App app(hInstance);

        if(!app.init()) return 1;

        return app.run();
}
