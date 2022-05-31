#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <d2d1.h>

ID2D1Factory *pFactory = nullptr;
ID2D1HwndRenderTarget *pRenderTarget = nullptr;
ID2D1SolidColorBrush *pLightSlateGreyBrush = nullptr;
ID2D1SolidColorBrush *pCornflowerBlueBrush = nullptr;

template<class T>
inline void SafeRelease(T **ppInterfaceToRelease)
{
    if (*ppInterfaceToRelease != nullptr) 
    {
        (*ppInterfaceToRelease)->Release();

        (*ppInterfaceToRelease) = nullptr;
    }
}

HRESULT CreateGraphicsResources(HWND hWnd) 
{
    HRESULT hr = S_OK;
    if (pRenderTarget == nullptr) 
    {
        RECT rc;
        GetClientRect(hWnd, &rc);

        D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

        hr = pFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(hWnd, size),
            &pRenderTarget);

        if (SUCCEEDED(hr))
        {
            hr = pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::LightSlateGray), &pLightSlateGreyBrush);
        }

        if (SUCCEEDED(hr))
        {
            hr = pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::CornflowerBlue), &pCornflowerBlueBrush);
        }
    }

    return hr;
}

void DiscardGraphicResources()
{
    SafeRelease(&pRenderTarget);
    SafeRelease(&pLightSlateGreyBrush);
    SafeRelease(&pCornflowerBlueBrush);
}

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

    // initialize COM 
    if (FAILED(CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE))) return -1;

    // Clear out the window class for use
    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    // Fill in the struct with the needed informantion
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.lpszClassName = _T("windowClass1");

    // register the window class
    RegisterClassEx(&wc);

    // Create the window and use the result as handle
    hWnd = CreateWindowEx(0,
                          _T("windowClass1"),
                          _T("Hello, Engine![Direct 2D]"),
                          WS_OVERLAPPEDWINDOW,
                          100,
                          100,
                          960,
                          540,
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

    while(GetMessage(&msg, nullptr, 0, 0)) 
    {
        // Translate keystroke messages into the right format
        TranslateMessage(&msg);

        // Send the message to the WindowProc function
        DispatchMessage(&msg);
    }

    // Uninitialize COM 
    CoUninitialize();

    // Return this part of WM_QUIT message to Windows
    return msg.wParam;
}

// The main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) 
{
    LRESULT result = 0;
    bool wasHandled = false;

    switch (message)
    {
    case WM_CREATE:
        if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory)))
        {
            result = -1;
            return result;
        }
        wasHandled = true;
        result = 0;
        break;
    case WM_PAINT:
    {
        HRESULT hr = CreateGraphicsResources(hWnd);
        if (SUCCEEDED(hr))
        {
            PAINTSTRUCT ps;
            BeginPaint(hWnd, &ps);

            // Begin GPU draw
            pRenderTarget->BeginDraw();

            // Clear up the background
            pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

            // Get the size of rendering area
            D2D1_SIZE_F rtSize = pRenderTarget->GetSize();

            int width = static_cast<int>(rtSize.width);
            int height = static_cast<int>(rtSize.height);

            for (int x = 0; x < width; x += 10) 
            {
                pRenderTarget->DrawLine
                (
                    D2D1::Point2F(static_cast<FLOAT>(x), 0.0f),
                    D2D1::Point2F(static_cast<FLOAT>(x), rtSize.height),
                    pLightSlateGreyBrush,
                    0.5f
                );
            }

            for (int y = 0; y < height; y += 10)
            {
                pRenderTarget->DrawLine
                (
                    D2D1::Point2F(0.0f, static_cast<FLOAT>(y)),
                    D2D1::Point2F(rtSize.width, static_cast<FLOAT>(y)),
                    pLightSlateGreyBrush,
                    0.5f
                );
            }

            D2D1_RECT_F rectangle1 = D2D1::RectF
            (
                rtSize.width  / 2 - 50.0f,
                rtSize.height / 2 - 50.0f,
                rtSize.width  / 2 + 50.0f,
                rtSize.height / 2 + 50.0f
            );

            D2D1_RECT_F rectangle2 = D2D1::RectF
            (
                rtSize.width  / 2 - 100.0f,
                rtSize.height / 2 - 100.0f,
                rtSize.width  / 2 + 100.0f,
                rtSize.height / 2 + 100.0f
            );

            // Draw a filled rectangle
            pRenderTarget->FillRectangle(&rectangle1, pLightSlateGreyBrush);

            // Draw a lined rectangle
            pRenderTarget->DrawRectangle(&rectangle2, pCornflowerBlueBrush);

            hr = pRenderTarget->EndDraw();
            if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
            {
                DiscardGraphicResources();
            }

            EndPaint(hWnd, &ps);
        }
    }
        wasHandled = true;
        break;
    case WM_SIZE:
        if (pRenderTarget != nullptr)
        {
            RECT rc;
            GetClientRect(hWnd, &rc);

            D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

            pRenderTarget->Resize(size);
        }
        wasHandled = true;
        break;
    case WM_DESTROY:
        DiscardGraphicResources();
        if (pFactory) 
        {
            pFactory->Release();
            pFactory = nullptr;
        }
        PostQuitMessage(0);
        result = 0;
        wasHandled = true;
        break;
    case WM_DISPLAYCHANGE:
        InvalidateRect(hWnd, nullptr, false);
        wasHandled = true;
        break;
    default:
        break;
    }

    if (!wasHandled) 
    {
        result = DefWindowProc(hWnd, message, wParam, lParam);
    }
    return result;
}
