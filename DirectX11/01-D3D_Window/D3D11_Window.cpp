#include <windows.h>
#include <stdio.h>
#include <math.h>
#include "myd3dheader.h"

/** 
 * *This header is used by Direct3D 11 Graphics
 */
#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "kernel32.lib")

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// Global Function Prototype
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Global Variable
FILE *gpFile = NULL;
char gszFileName[] = "Log.txt";
HWND ghwnd = NULL;
HGLRC ghrc = NULL;
DWORD dwStyle = NULL;
WINDOWPLACEMENT wpPrev = {sizeof(WINDOWPLACEMENT)};

bool gbFullscreen = false;
bool gbActiveWindow = false;

float gfClearColor[4]; // *color

IDXGISwapChain *gpIDXGISwapChain = NULL;                 // *storing rendered data before presenting it to an output.
ID3D11Device *gpID3D11Device = NULL;                     // *The device interface represents a virtual adapter; it is used to create graphics resources.
ID3D11DeviceContext *gpID3D11DeviceContext = NULL;       // *represents a device context which generates rendering commands.
ID3D11RenderTargetView *gpID3D11RenderTargetView = NULL; // *A render-target-view interface identifies the render-target subresources that can be accessed during rendering.

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{

    // *Function Prototype
    HRESULT initialize(void);
    void uninitialize(void);
    void display(void);

    WNDCLASSEX wndclass = {sizeof(WNDCLASSEX)};
    HWND hwnd = NULL;
    MSG msg = {};
    TCHAR szAppName[255] = TEXT("Direct3D11");
    bool bDone = false;

    // *CODE
    if (fopen_s(&gpFile, gszFileName, "w") != 0)
    {
        MessageBox(NULL, TEXT("FILE CREATION FAILED PROGRAM WILL EXIT..."), TEXT("ERROR"), MB_ICONSTOP | MB_OK | MB_TOPMOST);
        exit(0);
    }
    else
    {
        fprintf_s(gpFile, "\n******************************\n");
        fprintf_s(gpFile, "Log File Created Successfully!\n");
        fprintf_s(gpFile, "******************************\n");
        fclose(gpFile);
    }

    // *Initialize WNDCLASS
    wndclass.cbSize = sizeof(WNDCLASSEX);
    wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; // *Custom Window Own Device Context|  CS_OWNDC => Keep My DC Intact In Memory Don Discard Dont Move.
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));
    wndclass.hCursor = LoadCursor(NULL, IDC_IBEAM);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szAppName;
    wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));

    RegisterClassEx(&wndclass);

    // *Create Window In Memory
    hwnd = CreateWindowEx(WS_EX_APPWINDOW,
                          szAppName,
                          TEXT("DirectX3D-11 Application"),
                          WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE,
                          ((GetSystemMetrics(SM_CXSCREEN) - WIN_WIDTH) / 2),
                          ((GetSystemMetrics(SM_CYSCREEN) - WIN_HEIGHT) / 2),
                          WIN_WIDTH,
                          WIN_HEIGHT,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    // *Assign global Handle With Current Window Handle
    ghwnd = hwnd;

    // *Direct3d Initialization Starts Here
    HRESULT hr;
    hr = initialize();
    if (FAILED(hr))
    {
        fopen_s(&gpFile, gszFileName, "a+");
        fprintf_s(gpFile, "initialization Failed...EXITING NOW\n");
        fclose(gpFile);
        uninitialize();
        hwnd = NULL;
    }
    else
    {
        fopen_s(&gpFile, gszFileName, "a+");
        fprintf_s(gpFile, "Initialization Succeeded...\n");
        fclose(gpFile);
    }

    // *Show Window
    ShowWindow(hwnd, iCmdShow);

    // *Replacement of UpdateWindow
    SetForegroundWindow(hwnd);
    SetFocus(hwnd);

    // *THE GAME LOOP
    while (bDone == false)
    {

        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {

            if (msg.message == WM_QUIT)
            {

                bDone = true;
            }
            else
            {

                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {

            if (gbActiveWindow == true)
            {
                display();
            }
        }
    }

    // *Clean-Up
    uninitialize();

    if (gpFile)
    {
        fopen_s(&gpFile, gszFileName, "a+");
        fprintf_s(gpFile, "\n*******************************\n");
        fprintf_s(gpFile, "************ THE END ***********\n");
        fprintf_s(gpFile, "*******************************\n");
        fclose(gpFile);
        gpFile = NULL;
    }

    return ((int)msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{

    // Local Function Prototype
    void ToggleFullscreen(void);
    void display(void);
    HRESULT resize(int, int);
    void uninitialize(void);

    // Variable Declaration
    HRESULT hr;

    switch (iMsg)
    {

    case WM_CREATE:
        break;

    // *This Message Comes When Application Window Get Activated
    case WM_SETFOCUS:
        gbActiveWindow = true;
        break;

    // *This Message Comes When Application Window Get Unfocused
    case WM_KILLFOCUS:
        gbActiveWindow = false;
        break;

    case WM_ERASEBKGND:
        return (0);

    case WM_SIZE:
        if (gpID3D11DeviceContext)
        {
            hr = resize(LOWORD(lParam), HIWORD(lParam));
            if (FAILED(hr))
            {
                fopen_s(&gpFile, gszFileName, "a+");
                fprintf_s(gpFile, "resize() Failed!\n");
                fclose(gpFile);
                return (hr);
            }
            else
            {
                fopen_s(&gpFile, gszFileName, "a+");
                fprintf_s(gpFile, "resize() Succeeded\n");
                fclose(gpFile);
            }
        }
        break;

    case WM_KEYDOWN:

        switch (wParam)
        {

        case VK_ESCAPE:

            DestroyWindow(hwnd);

            break;

        case 0x46:
        case 0x66:
            ToggleFullscreen();
            break;

        default:
            break;
        }
        break;

    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;

    case WM_DESTROY:
        uninitialize();
        PostQuitMessage(0);
        break;

    default:
        break;
    }

    return (DefWindowProc(hwnd, iMsg, wParam, lParam));
}

void ToggleFullscreen(void)
{

    // *Local Variable
    MONITORINFO mi = {sizeof(MONITORINFO)};

    if (gbFullscreen == false)
    {

        // *Retrieves information about the specified window , Here Retrives Current Styel Of Given Window
        dwStyle = GetWindowLong(ghwnd, GWL_STYLE);

        // *Check If Window Has WS_OVERLAPPEDWINDOW (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX)
        if (dwStyle & WS_OVERLAPPEDWINDOW)
        {

            if ((GetWindowPlacement(ghwnd, &wpPrev)) && (GetMonitorInfo(MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY), &mi)))
            {

                SetWindowLong(ghwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);

                SetWindowPos(ghwnd,
                             HWND_TOP,
                             mi.rcMonitor.left,
                             mi.rcMonitor.top,
                             mi.rcMonitor.right - mi.rcMonitor.left,
                             mi.rcMonitor.bottom - mi.rcMonitor.top,
                             SWP_NOZORDER | SWP_FRAMECHANGED);
            }
        }

        ShowCursor(FALSE);
        gbFullscreen = true;
    }
    else
    {

        SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
        SetWindowPlacement(ghwnd, &wpPrev);
        SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
        ShowCursor(TRUE);
        gbFullscreen = false;
    }
}

HRESULT printD3DInfo(void)
{

    // *Local Variable Declaration
    HRESULT hr;
    IDXGIFactory *pIDXGIFactory = NULL;
    IDXGIAdapter *pIDXGIAdapter = NULL;
    DXGI_ADAPTER_DESC dxgiAdapterDesc;
    char str[255];

    // *CODE
    hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void **)&pIDXGIFactory);
    if (FAILED(hr))
    {
        fopen_s(&gpFile, gszFileName, "a+");
        fprintf_s(gpFile, "CreateDXGIFactory() Failed!\n");
        fclose(gpFile);
        return (hr);
    }
    if (pIDXGIFactory->EnumAdapters(0, &pIDXGIAdapter) == DXGI_ERROR_NOT_FOUND)
    {
        fopen_s(&gpFile, gszFileName, "a+");
        fprintf_s(gpFile, "IDXGIAdapter Cannot Be Found!\n");
        fclose(gpFile);
        return (hr);
    }

    ZeroMemory((void *)&dxgiAdapterDesc, sizeof(DXGI_ADAPTER_DESC));
    hr = pIDXGIAdapter->GetDesc(&dxgiAdapterDesc);
    WideCharToMultiByte(CP_ACP, 0, dxgiAdapterDesc.Description, 255, str, 255, NULL, NULL);

    fopen_s(&gpFile, gszFileName, "a+");
    fprintf_s(gpFile, "\n************************************************\n");
    fprintf_s(gpFile, "\nGraphics Card   : %s\n", str);
    fprintf_s(gpFile, "VRAM (BYTES)    : %I64d Bytes\n", (__int64)dxgiAdapterDesc.DedicatedVideoMemory);
    fprintf_s(gpFile, "VRAM (GB)       : %1.lf GB\n", ceil(((((__int64)dxgiAdapterDesc.DedicatedVideoMemory / 1024.0) / 1024.0) / 1024.0)));
    fprintf_s(gpFile, "\n************************************************\n");
    fclose(gpFile);
}

HRESULT initialize(void)
{
    // *Local Function Prototype
    void uninitialize(void);
    HRESULT printD3DInfo(void);
    HRESULT resize(int, int);

    // *Variable Declaration
    HRESULT hr;
    DXGI_SWAP_CHAIN_DESC dxgiSwapChainDecs;                              // *Describes a swap chain.
    D3D_DRIVER_TYPE d3dDriverType;                                       // *Driver type options.
    D3D_DRIVER_TYPE d3dDriverTypes[] = {D3D_DRIVER_TYPE_HARDWARE,        // *A hardware driver, which implements Direct3D features in hardware.
                                        D3D_DRIVER_TYPE_WARP,            // *A WARP driver, which is a high-performance software rasterizer.
                                        D3D_DRIVER_TYPE_REFERENCE};      // *A reference driver, which is a software implementation that supports every Direct3D feature
    D3D_FEATURE_LEVEL d3dFeatureLevel_required = D3D_FEATURE_LEVEL_11_0; // *Set of features targeted by a Direct3D device | Targets features supported by Direct3D 11.0, including shader model 5.
    D3D_FEATURE_LEVEL d3dFeatureLevel_acquired = D3D_FEATURE_LEVEL_10_0; // *Targets features supported by Direct3D 10.0, including shader model 4.
    UINT createDeviceFlags = 0;
    UINT numDriverTypes = 0;
    UINT numFeatureLevels = 1;

    // *code
    printD3DInfo(); // *Print graphics related information
    numDriverTypes = sizeof(d3dDriverTypes) / sizeof(d3dDriverTypes[0]);
    ZeroMemory((void *)&dxgiSwapChainDecs, sizeof(DXGI_SWAP_CHAIN_DESC));

    dxgiSwapChainDecs.BufferCount = 1;                                // *Number of buffers in the swap chain by default gives 1 so 1 + 1 = 2
    dxgiSwapChainDecs.BufferDesc.Width = WIN_WIDTH;                   // *The backbuffer display mode | resolution width
    dxgiSwapChainDecs.BufferDesc.Height = WIN_HEIGHT;                 // *The backbuffer display mode | resolution height
    dxgiSwapChainDecs.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // *The backbuffer display mode | the display format | A four-component, 32-bit unsigned-normalized-integer
                                                                      // *                                                 | Format that supports 8 bits per channel including alpha.
    dxgiSwapChainDecs.BufferDesc.RefreshRate.Numerator = 60;          // *The backbuffer display mode | refresh rate in hertz | unsigned integer value representing the top of the rational number
    dxgiSwapChainDecs.BufferDesc.RefreshRate.Denominator = 1;         // *The backbuffer display mode | refresh rate in hertz |  unsigned integer value representing the bottom of the rational number
    dxgiSwapChainDecs.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;  // *The surface usage and CPU access options for the back buffer |
                                                                      // *The back buffer can be used for shader input or render-target output |
                                                                      // *Use the surface or resource as an output render target
    dxgiSwapChainDecs.OutputWindow = ghwnd;                           // *An HWND handle to the output window. This member must not be NULL.
    dxgiSwapChainDecs.SampleDesc.Count = 1;                           // *Multi-sampling parameters for a resource | The number of multisamples per pixel.
    dxgiSwapChainDecs.SampleDesc.Quality = 0;                         // *Multi-sampling parameters for a resource | The image quality level. The higher the quality, the lower the performance
    dxgiSwapChainDecs.Windowed = TRUE;                                // *A Boolean value that specifies whether the output is in windowed mode | TRUE if the output is in windowed mode; otherwise, FALSE For Fullscreen

    for (UINT driverTypesIndex = 0; driverTypesIndex < numDriverTypes; driverTypesIndex++)
    {
        d3dDriverType = d3dDriverTypes[driverTypesIndex];
        hr = D3D11CreateDeviceAndSwapChain( // *Creates a device that represents the display adapter and a swap chain used for rendering
            NULL,                           // *A pointer to the video adapter to use when creating a device. Pass NULL to use the default adapter
            d3dDriverType,                  // *Represents the driver type to create.
            NULL,                           // *A handle to a DLL that implements a software rasterizer | NULL if dont have own software rasterizer
            createDeviceFlags,              // *The runtime layers to enable (see D3D11_CREATE_DEVICE_FLAG)
            &d3dFeatureLevel_required,      // *A pointer to an array of D3D_FEATURE_LEVELs, which determine the order of feature levels to attempt to create
            numFeatureLevels,               // *The number of elements in pFeatureLevels.
            D3D11_SDK_VERSION,              // *The SDK version; use D3D11_SDK_VERSION
            &dxgiSwapChainDecs,             // *A pointer to a swap chain description (see DXGI_SWAP_CHAIN_DESC) that contains initialization parameters for the swap chain.
            &gpIDXGISwapChain,              // *Returns the address of a pointer to the IDXGISwapChain object that represents the swap chain used for rendering
            &gpID3D11Device,                // *Returns the address of a pointer to an ID3D11Device object that represents the device created. If this parameter is NULL, no ID3D11Device will be returned
            &d3dFeatureLevel_acquired,      // *Returns a pointer to a D3D_FEATURE_LEVEL, which represents the first element in an array of feature levels supported by the device. Supply NULL as an input if you don't need to determine which feature level is supported.
            &gpID3D11DeviceContext);        // *Returns the address of a pointer to an ID3D11DeviceContext object that represents the device context. If this parameter is NULL, no ID3D11DeviceContext will be returned.

        if (SUCCEEDED(hr))
        {
            break;
        }
    }

    if (FAILED(hr))
    {
        fopen_s(&gpFile, gszFileName, "a+");
        fprintf_s(gpFile, "D3D11CreateDeviceAndSwapChain() Failed...EXITING NOW\n");
        fclose(gpFile);
        return (hr);
    }
    else
    {
        fopen_s(&gpFile, gszFileName, "a+");
        fprintf_s(gpFile, "\nD3D11CreateDeviceAndSwapChain() Succeeded\n");
        fprintf_s(gpFile, "The Chosen Driver Is Of ");
        if (d3dDriverType == D3D_DRIVER_TYPE_HARDWARE)
        {
            fprintf_s(gpFile, "Hardware Type!\n");
        }
        else if (d3dDriverType == D3D_DRIVER_TYPE_WARP)
        {
            fprintf_s(gpFile, "WARP Type!\n");
        }
        else if (d3dDriverType == D3D_DRIVER_TYPE_REFERENCE)
        {
            fprintf_s(gpFile, "Reference Type!\n");
        }
        else
        {
            fprintf_s(gpFile, "Unkown Type!\n");
        }

        fprintf_s(gpFile, "The Supported Highest Feature Level Is ");
        if (d3dFeatureLevel_acquired == D3D_FEATURE_LEVEL_11_0)
        {
            fprintf_s(gpFile, "11.0\n");
        }
        else if (d3dFeatureLevel_acquired == D3D_FEATURE_LEVEL_10_1)
        {
            fprintf_s(gpFile, "10.1\n");
        }
        else if (d3dFeatureLevel_acquired == D3D_FEATURE_LEVEL_10_0)
        {
            fprintf_s(gpFile, "10.0\n");
        }
        else
        {
            fprintf_s(gpFile, "Unkown\n");
        }
        fclose(gpFile);
    }

    // *Background Color
    gfClearColor[0] = 0.0f;
    gfClearColor[1] = 0.0f;
    gfClearColor[2] = 1.0f;
    gfClearColor[3] = 1.0f;

    /**
     *  *As Render Target View And Buffer Size Does Not Change According To Size
     *  *Thats Why We Have To Call Resize Inside Initialize To Handle Size Dependency
     */  
    hr = resize(WIN_WIDTH, WIN_HEIGHT);
    if (FAILED(hr))
    {
        fopen_s(&gpFile, gszFileName, "a+");
        fprintf_s(gpFile, "resize() Failed!\n");
        fclose(gpFile);
        return (hr);
    }
    else
    {
        fopen_s(&gpFile, gszFileName, "a+");
        fprintf_s(gpFile, "resize() Succeeded!\n");
        fclose(gpFile);
    }
    return (S_OK);
}

HRESULT resize(int width, int height)
{
    // *CODE
    HRESULT hr;

    // *Free Any Size-Dependent Resources
    if (gpID3D11RenderTargetView)
    {
        gpID3D11RenderTargetView->Release();
        gpID3D11RenderTargetView = NULL;
    }

    /**
     * Function: ResizeBuffers
     * *Resize Swap Chain Buffers Accordingly to new win width and size
     * @param BufferCount Number of buffers to resize
     * @param Width New resize width
     * @param Height New resize height
     * @param NewFormat New format
     * @param SwapChainFlags Flags to be use 0 is default
     */
    gpIDXGISwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

    /**
     * *Again Get Back Buffers From Swap Chain
     * *New buffer should capable of depth as well as color hence used TextureBuffer
     */
    ID3D11Texture2D *pID3D11Texture2D_BackBuffer;
    gpIDXGISwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *)&pID3D11Texture2D_BackBuffer);

    // *Again Get Render Target View From D3D11 Device From Above Back Buffer
    hr = gpID3D11Device->CreateRenderTargetView(pID3D11Texture2D_BackBuffer, NULL, &gpID3D11RenderTargetView);
    if (FAILED(hr))
    {
        fopen_s(&gpFile, gszFileName, "a+");
        fprintf_s(gpFile, "CreateRenderTargetView() Failed!\n");
        fclose(gpFile);
        return (hr);
    }
    else
    {
        fopen_s(&gpFile, gszFileName, "a+");
        fprintf_s(gpFile, "CreateRenderTargetView() Succeeded!\n");
        fclose(gpFile);
    }

    pID3D11Texture2D_BackBuffer->Release();
    pID3D11Texture2D_BackBuffer = NULL;

    /**
     *  Function: OMSetRenderTargets
     *  *Set Render Target View As Render Target
     *  @param NumView Number of views
     *  @param RenderTargetView Actual render target view 
     *  @param DepthStenciled Depth Value
     */
    gpID3D11DeviceContext->OMSetRenderTargets(1, &gpID3D11RenderTargetView, NULL);

    // *Set Viewport
    D3D11_VIEWPORT d3dViewport;         // *Defines the dimensions of a viewport.
    d3dViewport.TopLeftX = 0;           // *X position of the left hand side of the viewport.
    d3dViewport.TopLeftY = 0;           // *Y position of the top of the viewport.
    d3dViewport.Width = (float)width;   // *Width of the viewport.
    d3dViewport.Height = (float)height; // *Height  of the viewport.
    d3dViewport.MinDepth = 0.0f;        // *Minimum depth of the viewport. Ranges between 0 and 1.
    d3dViewport.MaxDepth = 1.0f;        // *Maximum depth of the viewport. Ranges between 0 and 1.

    // *Bind an array of viewports to the rasterizer stage of the pipeline.
    gpID3D11DeviceContext->RSSetViewports(1, &d3dViewport);

    return (hr);
}

void display(void)
{
    // *CODE

    /** 
     * *Clear Render Target View To A Chosen Color
     * *Set all the elements in a render target to one value. 
    */
    gpID3D11DeviceContext->ClearRenderTargetView(gpID3D11RenderTargetView, gfClearColor);

    /**
     * *Presents a rendered image to the user.
     * *Switch Between Front & Back Buffers
     */
    gpIDXGISwapChain->Present(0, 0);
}

void uninitialize(void)
{
    // *CODE
    if (gpID3D11RenderTargetView)
    {
        gpID3D11RenderTargetView->Release();
        gpID3D11RenderTargetView = NULL;
    }

    if (gpIDXGISwapChain)
    {
        gpIDXGISwapChain->Release();
        gpIDXGISwapChain = NULL;
    }

    if (gpID3D11DeviceContext)
    {
        gpID3D11DeviceContext->Release();
        gpID3D11DeviceContext = NULL;
    }

    if (gpID3D11Device)
    {
        gpID3D11Device->Release();
        gpID3D11Device = NULL;
    }
}
