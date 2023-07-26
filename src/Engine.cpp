#include "Engine.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"
#define WIN32_LEAN_AND_MEAN
#pragma clang diagnostic pop

#include <windows.h>

namespace engine {
    static bool is_active = true;
    static bool quited = false;
    static POINT cursor_pos{};
    static LARGE_INTEGER qpc_frequency{};
    static LARGE_INTEGER qpc_ref_time{};

    color buffer[screen_height][screen_width]{};

    static inline int to_wnd_virtual_key(virtual_key key) {
        switch (key) {
            case virtual_key::escape:
                return VK_ESCAPE;
            case virtual_key::space:
                return VK_SPACE;
            case virtual_key::left:
                return VK_LEFT;
            case virtual_key::up:
                return VK_UP;
            case virtual_key::right:
                return VK_RIGHT;
            case virtual_key::down:
                return VK_DOWN;
        }
    }

    bool is_key_pressed(virtual_key key) {
        const auto wnd_key = to_wnd_virtual_key(key);
        return is_active && (GetAsyncKeyState(wnd_key) & 0x8000);
    }

    static inline int to_wnd_mouse_button(mouse_button button) {
        switch (button) {
            case mouse_button::left:
                return VK_LBUTTON;
            case mouse_button::right:
                return VK_RBUTTON;
        }
    }

    bool is_mouse_button_pressed(mouse_button button) {
        const auto wnd_key = to_wnd_mouse_button(button);
        return is_active && (GetAsyncKeyState(wnd_key) != 0);
    }

    cursor_coordinate get_cursor_x() {
        return cursor_pos.x;
    }

    cursor_coordinate get_cursor_y() {
        return cursor_pos.y;
    }

    void schedule_quit() {
        quited = true;
    }

    static inline seconds count_delta_time(LARGE_INTEGER time) {
        const auto delta = static_cast<seconds>(time.QuadPart - qpc_ref_time.QuadPart);
        const auto frequency = static_cast<seconds>(qpc_frequency.QuadPart);
        return delta / frequency;
    }

    static void CALLBACK update_proc(HWND hwnd) {
        if (quited)
            return;

        is_active = GetActiveWindow() == hwnd;

        GetCursorPos(&cursor_pos);
        ScreenToClient(hwnd, &cursor_pos);

        LARGE_INTEGER time;
        QueryPerformanceCounter(&time);

        auto delta_time = count_delta_time(time);
        if (delta_time > 0.1f)
            delta_time = 0.1f;
        act(delta_time);

        if (!quited) {
            draw();
            RedrawWindow(hwnd, nullptr, nullptr, RDW_INVALIDATE | RDW_UPDATENOW);
        }

        qpc_ref_time = time;
    }

    static LRESULT CALLBACK wnd_proc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
        switch (message) {
            case WM_PAINT: {
                PAINTSTRUCT paint_struct;
                HDC hdc = BeginPaint(hwnd, &paint_struct);

                BITMAPINFOHEADER bitmap_info_header;
                bitmap_info_header.biSize = sizeof(bitmap_info_header);
                bitmap_info_header.biWidth = screen_width;
                bitmap_info_header.biHeight = -static_cast<LONG>(screen_height);
                bitmap_info_header.biPlanes = 1;
                bitmap_info_header.biBitCount = 32;
                bitmap_info_header.biCompression = BI_RGB;
                bitmap_info_header.biSizeImage = 0;
                bitmap_info_header.biXPelsPerMeter = 96;
                bitmap_info_header.biYPelsPerMeter = 96;
                bitmap_info_header.biClrUsed = 0;
                bitmap_info_header.biClrImportant = 0;
                SetDIBitsToDevice(
                        hdc,
                        0, 0,
                        screen_width, screen_height,
                        0, 0,
                        0, screen_height,
                        buffer,
                        reinterpret_cast<BITMAPINFO *>(&bitmap_info_header),
                        DIB_RGB_COLORS);

                EndPaint(hwnd, &paint_struct);
            }
                break;
            case WM_QUIT:
            case WM_DESTROY:
                quited = true;
                break;
            default:
                return DefWindowProc(hwnd, message, wParam, lParam);
        }
        return 0;
    }
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "LoopDoesntUseConditionVariableInspection"
#pragma ide diagnostic ignored "readability-non-const-parameter"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow) {
    SetProcessDPIAware();
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    WNDCLASSEXA wnd_class_ext;
    wnd_class_ext.cbSize = sizeof(WNDCLASSEX);
    wnd_class_ext.style = CS_HREDRAW | CS_VREDRAW;
    wnd_class_ext.lpfnWndProc = engine::wnd_proc;
    wnd_class_ext.cbClsExtra = 0;
    wnd_class_ext.cbWndExtra = 0;
    wnd_class_ext.hInstance = hInstance;
    wnd_class_ext.hIcon = nullptr;
    wnd_class_ext.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wnd_class_ext.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW);
    wnd_class_ext.lpszMenuName = nullptr;
    wnd_class_ext.lpszClassName = "GameTemplateWndClass";
    wnd_class_ext.hIconSm = nullptr;
    RegisterClassExA(&wnd_class_ext);

    RECT rect;
    rect.left = 0;
    rect.top = 0;
    rect.right = engine::screen_width;
    rect.bottom = engine::screen_height;
    AdjustWindowRectEx(&rect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE, 0);

    HWND hwnd = CreateWindowA(wnd_class_ext.lpszClassName, "Game", WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU,
                              CW_USEDEFAULT, 0, rect.right - rect.left, rect.bottom - rect.top, nullptr, nullptr,
                              hInstance, nullptr);

    if (!hwnd)
        return 0;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    QueryPerformanceFrequency(&engine::qpc_frequency);
    QueryPerformanceCounter(&engine::qpc_ref_time);

    engine::initialize();

    MSG msg;
    while (!engine::quited) {
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        engine::update_proc(hwnd);
        Sleep(0);
    }

    engine::finalize();
    return static_cast<int>(msg.wParam);
}

#pragma clang diagnostic pop
