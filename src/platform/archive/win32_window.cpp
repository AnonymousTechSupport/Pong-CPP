// #include "win32_window.h"
// #include "utils/input.h"

// LRESULT CALLBACK
// WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
// {
//     // forward input-related events to the input system
//     Input::Get().ProcessWin32Message(message, wParam, lParam);

//     switch (message)
//     {
//     case WM_CLOSE:
//         DestroyWindow(hWnd);
//         return 0;
//     case WM_DESTROY:
//         PostQuitMessage(0);
//         return 0;

//     case WM_CREATE:
//     {
//         if (lParam)
//         {
//             CREATESTRUCTW* cs =
//                 reinterpret_cast<CREATESTRUCTW*>(lParam);
//             if (cs->lpCreateParams)
//             {
//                 SetWindowLongPtrW(
//                     hWnd,
//                     GWLP_USERDATA,
//                     reinterpret_cast<LONG_PTR>(cs->lpCreateParams));
//             }
//         }
//         return 0;
//     }
//     }

//     return DefWindowProcW(hWnd, message, wParam, lParam);
// }

// Window::Window()
//     : m_instance(GetModuleHandle(nullptr)), m_hWindow(nullptr),
//       m_isRunning(true), m_registeredClass(false)
// {
//     const wchar_t* CLASS_NAME = L"Window Class";
//     // default to full screen dimensions (borderless fullscreen)
//     int clientWidth = GetSystemMetrics(SM_CXSCREEN);
//     int clientHeight = GetSystemMetrics(SM_CYSCREEN);

//     WNDCLASSW wClass = {};
//     wClass.lpszClassName = CLASS_NAME;
//     wClass.hInstance = m_instance;
//     wClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
//     wClass.lpfnWndProc = WindowProc;
//     wClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;

//     if (!RegisterClassW(&wClass))
//     {
//         MessageBoxW(
//             nullptr,
//             L"Failed to register window class",
//             L"Error",
//             MB_OK | MB_ICONERROR);
//         m_isRunning = false;
//         return;
//     }
//     m_registeredClass = true;

//     // borderless window (no caption, no borders) – still visible
//     // after ShowWindow
//     DWORD style = WS_POPUP;

//     RECT rect = {0, 0, clientWidth, clientHeight};
//     AdjustWindowRect(&rect, style, FALSE);
//     int winWidth = rect.right - rect.left;
//     int winHeight = rect.bottom - rect.top;

//     (void)winWidth;
//     (void)winHeight;

//     m_hWindow = CreateWindowExW(
//         0,
//         CLASS_NAME,
//         L"Window",
//         style,
//         CW_USEDEFAULT,
//         CW_USEDEFAULT,
//         winWidth,
//         winHeight,
//         nullptr,
//         nullptr,
//         m_instance,
//         this);

//     if (!m_hWindow)
//     {
//         MessageBoxW(
//             nullptr,
//             L"Failed to create window",
//             L"Error",
//             MB_OK | MB_ICONERROR);
//         m_isRunning = false;
//     }

//     ShowWindow(m_hWindow, SW_SHOW);
//     UpdateWindow(m_hWindow);
//     // ensure window has keyboard focus so we receive key messages
//     SetForegroundWindow(m_hWindow);
//     SetFocus(m_hWindow);
// }

// Window::~Window()
// {
//     const wchar_t* CLASS_NAME = L"Window Class";

//     if (m_hWindow)
//     {
//         Shutdown();
//     }

//     if (m_registeredClass)
//         UnregisterClassW(CLASS_NAME, m_instance);
// }

// bool Window::ProcessMessages()
// {
//     MSG message = {};
//     while (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
//     {
//         if (message.message == WM_QUIT)
//         {
//             m_isRunning = false;
//             return false;
//         }

//         TranslateMessage(&message);
//         DispatchMessage(&message);
//     }
//     return true;
// }

// void Window::Shutdown()
// {
//     if (m_hWindow)
//     {
//         DestroyWindow(m_hWindow);
//         m_hWindow = nullptr;
//         m_isRunning = false;
//     }
// }

// bool Window::IsRunning() const
// {
//     return m_isRunning;
// }

// HWND Window::GetHwnd() const
// {

//     return m_hWindow;
// }