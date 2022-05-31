#include <windows.h>
#include <windowsx.h>
#include <tchar.h>

// The WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, 
                            UINT message,
                            WPARAM wParam,
                            LPARAM lParam);

// The entry point for windows program
int WINAPI WinMain(HINSTANCE hInstance, 
                   HINSTANCE hPrevInstance,
                   LPTSTR lpCmdLine,
                   int nCmdShow)
{
    // The handle of the window
    HWND hWnd;

    // The window class who holds the window information
    WNDCLASSEX wc;

    // Clear out the window class for use
    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    // Fill in the struct with the needed informantion
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpszClassName = _T("windowClass1");

    // register the window class
    RegisterClassEx(&wc);

    // Create the window and use the result as handle
    hWnd = CreateWindowEx(0,
                          _T("windowClass1"),
                          _T("Hello, Engine!"),
                          WS_OVERLAPPEDWINDOW,
                          300,
                          300,
                          500,
                          400,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);
    
    // Display the window on screen
    ShowWindow(hWnd, nCmdShow);

    /*
     * Enter the main loop
     */

    // The struct who holds the windows event messages
    MSG msg;

    while(GetMessage(&msg, NULL, 0, 0)) 
    {
        // Translate keystroke messages into the right format
        TranslateMessage(&msg);

        // Send the message to the WindowProc function
        DispatchMessage(&msg);
    }

    // Return this part of WM_QUIT message to Windows
    return msg.wParam;
}

// The main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) 
{
    switch (message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        RECT rect = {20, 20, 60, 80};
        HBRUSH brush = (HBRUSH) GetStockBrush(BLACK_BRUSH);

        FillRect(hdc, &rect, brush);

        EndPaint(hWnd, &ps);
    } break;
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        return 0;
    } break;
    default:
        break;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}
