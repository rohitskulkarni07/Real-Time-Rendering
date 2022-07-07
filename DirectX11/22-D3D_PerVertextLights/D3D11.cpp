#include <windows.h>
#include <stdio.h>
#include <math.h>
#include "myd3dheader.h"
#include <d3d11.h>
#include <d3dcompiler.h>

#pragma warning(disable : 4838)
#include "XNAMath_204\xnamath.h"
#include "Sphere.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "D3dCompiler.lib")
#pragma comment(lib, "Sphere.lib")

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
bool gbLights = false;
bool gbActiveWindow = false;
bool gbAnimate = false;

float gfClearColor[4]; // *color

float gAngle = 0.0f;

// Sphere related variable
float sphere_vertices[1146];
float sphere_normals[1146];
float sphere_textures[746];

unsigned short sphere_elements[2280];

unsigned int num_of_sphere_vertices = 0;
unsigned int num_of_sphere_elements = 0;

IDXGISwapChain *gpIDXGISwapChain = NULL;
ID3D11Device *gpID3D11Device = NULL;
ID3D11DeviceContext *gpID3D11DeviceContext = NULL;

ID3D11RenderTargetView *gpID3D11RenderTargetView = NULL;

ID3D11VertexShader *gpID3D11VertexShader_PV = NULL;
ID3D11PixelShader *gpID3D11PixelShader_PV = NULL;
ID3D11InputLayout *gpID3D11InputLayout_PV = NULL;
ID3D11Buffer *gpID3D11Buffer_ConstantBuffer_PV = NULL;

ID3D11Buffer *gpID3D11Buffer_VertexBuffer_Position = NULL;
ID3D11Buffer *gpID3D11Buffer_VertexBuffer_Normals = NULL;
ID3D11Buffer *gpID3D11Buffer_IndexBuffer_Elements = NULL;

ID3D11RasterizerState *gpID3D11RasterizerState = NULL;

// *For Depth
ID3D11DepthStencilView *gpID3D11DepthStencilView;

// Lights Arrays
float light_ambient[3] = {0.1f, 0.1f, 0.1f};
float light_diffuse[3] = {1.0f, 1.0f, 1.0f};
float light_specular[3] = {1.0f, 1.0f, 1.0f};
float light_position[4] = {100.0f, 100.0f, -100.0f, 1.0f}; // light position

// Material Arrays
float material_ambient[3] = {0.1f, 0.1f, 0.1f};
float material_diffuse[3] = {0.5f, 0.2f, 0.7f};
float material_specular[3] = {0.7f, 0.7f, 0.7f};
float material_shininess = 128.0f;

struct CBUFFER
{
    XMMATRIX WorldMatrix;
    XMMATRIX ViewMatrix;
    XMMATRIX ProjectionMatrix;

    XMVECTOR LA; // ambient  component of light
    XMVECTOR LD; // diffuse  component of light
    XMVECTOR LS; // specular component of light

    XMVECTOR KA;            // ambient  component of material
    XMVECTOR KD;            // diffuse  component of material
    XMVECTOR KS;            // specular component of material
    XMVECTOR LightPosition; // lights position

    float MaterialShininess; // material shininess component

    unsigned int KeyPressed;
};

XMMATRIX gPerspectiveProjectionMatrix;

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

    case WM_CHAR:
        switch (LOWORD(wParam))
        {
        case 'l':
        case 'L':
            if (gbLights == true)
            {
                gbLights = false;
            }
            else
            {
                gbLights = true;
            }
            break;

        case 'a':
        case 'A':
            if (gbAnimate == true)
            {
                gbAnimate = false;
            }
            else
            {
                gbAnimate = true;
            }
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

        // *Retrieves information about the specified window , Here Retrives Current Style Of Given Window
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

    return hr;
}

HRESULT initialize(void)
{
    // *Local Function Prototype
    void uninitialize(void);
    HRESULT printD3DInfo(void);
    HRESULT resize(int, int);

    // *Variable Declaration
    HRESULT hr;
    DXGI_SWAP_CHAIN_DESC dxgiSwapChainDecs;
    D3D_DRIVER_TYPE d3dDriverType;
    D3D_DRIVER_TYPE d3dDriverTypes[] = {D3D_DRIVER_TYPE_HARDWARE,
                                        D3D_DRIVER_TYPE_WARP,
                                        D3D_DRIVER_TYPE_REFERENCE};
    D3D_FEATURE_LEVEL d3dFeatureLevel_required = D3D_FEATURE_LEVEL_11_0;
    D3D_FEATURE_LEVEL d3dFeatureLevel_acquired = D3D_FEATURE_LEVEL_10_0;
    UINT createDeviceFlags = 0;
    UINT numDriverTypes = 0;
    UINT numFeatureLevels = 1;

    // *code
    printD3DInfo();
    numDriverTypes = sizeof(d3dDriverTypes) / sizeof(d3dDriverTypes[0]);
    ZeroMemory((void *)&dxgiSwapChainDecs, sizeof(DXGI_SWAP_CHAIN_DESC));

    dxgiSwapChainDecs.BufferCount = 1;
    dxgiSwapChainDecs.BufferDesc.Width = WIN_WIDTH;
    dxgiSwapChainDecs.BufferDesc.Height = WIN_HEIGHT;
    dxgiSwapChainDecs.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

    dxgiSwapChainDecs.BufferDesc.RefreshRate.Numerator = 60;
    dxgiSwapChainDecs.BufferDesc.RefreshRate.Denominator = 1;
    dxgiSwapChainDecs.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

    dxgiSwapChainDecs.OutputWindow = ghwnd;
    dxgiSwapChainDecs.SampleDesc.Count = 1;
    dxgiSwapChainDecs.SampleDesc.Quality = 0;
    dxgiSwapChainDecs.Windowed = TRUE;

    for (UINT driverTypesIndex = 0; driverTypesIndex < numDriverTypes; driverTypesIndex++)
    {
        d3dDriverType = d3dDriverTypes[driverTypesIndex];
        hr = D3D11CreateDeviceAndSwapChain(
            NULL,
            d3dDriverType,
            NULL,
            createDeviceFlags,
            &d3dFeatureLevel_required,
            numFeatureLevels,
            D3D11_SDK_VERSION,
            &dxgiSwapChainDecs,
            &gpIDXGISwapChain,
            &gpID3D11Device,
            &d3dFeatureLevel_acquired,
            &gpID3D11DeviceContext);

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

    /*
     * Create Vertext Shader Source Code
     */

    const char *vertexShaderSourceCode_PV =
        "cbuffer ConstantBuffer                                                                                         \n"
        "{                                                                                                              \n"
        "   float4x4 worldMatrix;                                                                                       \n"
        "   float4x4 viewMatrix;                                                                                        \n"
        "   float4x4 projectionMatrix;                                                                                  \n"
        "                                                                                                               \n"
        "   float3 la;                                                                                                  \n"
        "   float3 ld;                                                                                                  \n"
        "   float3 ls;                                                                                                  \n"
        "                                                                                                               \n"
        "   float3 ka;                                                                                                  \n"
        "   float3 kd;                                                                                                  \n"
        "   float3 ks;                                                                                                  \n"
        "                                                                                                               \n"
        "   float4 lightPosition;                                                                                       \n"
        "   float materialShinnines;                                                                                    \n"
        "                                                                                                               \n"
        "   uint key_pressed;                                                                                           \n"
        "}                                                                                                              \n"
        "struct vertex_output {                                                                                         \n"
        "   float4 position: SV_POSITION;                                                                               \n"
        "   float3 phong_ads_light: COLOR;                                                                              \n"
        "};                                                                                                             \n"
        "vertex_output main(float4 pos: POSITION, float4 normal: NORMAL)                                                \n"
        "{                                                                                                              \n"
        "   vertex_output output;                                                                                       \n"
        "   if(key_pressed == 1) {                                                                                      \n"
        "       float4 eyePosition = mul(worldMatrix, pos);                                                             \n"
        "       eyePosition = mul(viewMatrix, eyePosition);                                                             \n"
        "       float3 transformedNormal = normalize(mul((float3x3)worldMatrix, (float3)normal));                       \n"
        "       float3 lightDirection = (float3) normalize((lightPosition - eyePosition));                              \n"
        "       float3 reflectionVector = reflect(-lightDirection, transformedNormal);                                  \n"
        "       float3 viewerVector = normalize(-eyePosition.xyz);                                                       \n"
        "       float3 ambient = la * ka;                                                                               \n"
        "       float3 diffuse = ld * kd * max(dot(lightDirection, transformedNormal), 0.0);                            \n"
        "       float3 specular = ls * ks * pow(max(dot(reflectionVector, viewerVector), 0.0), materialShinnines);      \n"
        "       output.phong_ads_light = ambient + diffuse + specular;                                                  \n"
        "   } else {                                                                                                    \n"
        "       output.phong_ads_light = float3(1.0 ,1.0 , 1.0);                                                        \n"
        "   }                                                                                                           \n"
        "   float4 position = mul(worldMatrix, pos);                                                                    \n"
        "   position = mul(viewMatrix, position);                                                                       \n"
        "   position = mul(projectionMatrix, position);                                                                 \n"
        "   output.position = position;                                                                                 \n"
        "   return(output);                                                                                             \n"
        "}                                                                                                              \n";

    ID3DBlob *pID3DBlob_VertexShaderCode_PV = NULL;
    ID3DBlob *pID3DBlob_Error_PV = NULL;

    hr = D3DCompile(vertexShaderSourceCode_PV,
                    lstrlenA(vertexShaderSourceCode_PV) + 1,
                    "VS",
                    NULL,
                    D3D_COMPILE_STANDARD_FILE_INCLUDE,
                    "main",
                    "vs_5_0",
                    0,
                    0,
                    &pID3DBlob_VertexShaderCode_PV,
                    &pID3DBlob_Error_PV);

    if (FAILED(hr))
    {
        if (pID3DBlob_Error_PV != NULL)
        {
            fopen_s(&gpFile, gszFileName, "a+");
            fprintf_s(gpFile, "D3DCompile() Failed For Vertex Shader: %s.\n", (char *)pID3DBlob_Error_PV->GetBufferPointer());
            fclose(gpFile);
            pID3DBlob_Error_PV->Release();
            pID3DBlob_Error_PV = NULL;
            return (hr);
        }
        else
        {
            fopen_s(&gpFile, gszFileName, "a+");
            fprintf_s(gpFile, "COM-ERROR!\n");
            fclose(gpFile);
            return (hr);
        }
    }
    else
    {
        fopen_s(&gpFile, gszFileName, "a+");
        fprintf_s(gpFile, "D3DCompile() Succeeded!\n");
        fclose(gpFile);
    }

    hr = gpID3D11Device->CreateVertexShader(pID3DBlob_VertexShaderCode_PV->GetBufferPointer(),
                                            pID3DBlob_VertexShaderCode_PV->GetBufferSize(),
                                            NULL,
                                            &gpID3D11VertexShader_PV);
    if (FAILED(hr))
    {
        fopen_s(&gpFile, gszFileName, "a+");
        fprintf_s(gpFile, "ID3D11Device::CreateVertexShader() Failed!\n");
        fclose(gpFile);
        return (hr);
    }
    else
    {
        fopen_s(&gpFile, gszFileName, "a+");
        fprintf_s(gpFile, "ID3D11Device::CreateVertexShader() Succeeded!\n");
        fclose(gpFile);
    }

    gpID3D11DeviceContext->VSSetShader(gpID3D11VertexShader_PV, NULL, 0);

    /*
     * Create Pixel Shader Source Code
     */
    const char *pixelShaderSourceCode_PV =
        "struct vertex_output {                                                                             \n"
        "   float4 position: SV_POSITION;                                                                   \n"
        "   float3 phong_ads_light: COLOR;                                                                  \n"
        "};                                                                                                 \n"
        "float4 main(struct vertex_output input) : SV_TARGET                                                \n"
        "{                                                                                                  \n"
        "   float4 color = float4(input.phong_ads_light, 1.0);                                              \n"
        "   return(color);                                                                                  \n"
        "}                                                                                                  \n";

    ID3D10Blob *pID3DBlob_PixelShaderCode_PV = NULL;
    pID3DBlob_Error_PV = NULL;

    hr = D3DCompile(pixelShaderSourceCode_PV,
                    lstrlenA(pixelShaderSourceCode_PV) + 1,
                    "PS",
                    NULL,
                    D3D_COMPILE_STANDARD_FILE_INCLUDE,
                    "main",
                    "ps_5_0",
                    0,
                    0,
                    &pID3DBlob_PixelShaderCode_PV,
                    &pID3DBlob_Error_PV);

    if (FAILED(hr))
    {
        if (pID3DBlob_Error_PV != NULL)
        {
            fopen_s(&gpFile, gszFileName, "a+");
            fprintf_s(gpFile, "D3DCompile() Failed For Pixel Shader: %s.\n", (char *)pID3DBlob_Error_PV->GetBufferPointer());
            fclose(gpFile);
            pID3DBlob_Error_PV->Release();
            pID3DBlob_Error_PV = NULL;
            return (hr);
        }
        else
        {
            fopen_s(&gpFile, gszFileName, "a+");
            fprintf_s(gpFile, "COM-ERROR!\n");
            fclose(gpFile);
            return (hr);
        }
    }
    else
    {
        fopen_s(&gpFile, gszFileName, "a+");
        fprintf_s(gpFile, "D3DCompile() Succeeded!\n");
        fclose(gpFile);
    }

    hr = gpID3D11Device->CreatePixelShader(pID3DBlob_PixelShaderCode_PV->GetBufferPointer(),
                                           pID3DBlob_PixelShaderCode_PV->GetBufferSize(),
                                           NULL,
                                           &gpID3D11PixelShader_PV);
    if (FAILED(hr))
    {
        fopen_s(&gpFile, gszFileName, "a+");
        fprintf_s(gpFile, "ID3D11Device::CreateVertexShader() Failed!\n");
        fclose(gpFile);
        return (hr);
    }
    else
    {
        fopen_s(&gpFile, gszFileName, "a+");
        fprintf_s(gpFile, "ID3D11Device::CreateVertexShader() Succeeded!\n");
        fclose(gpFile);
    }

    gpID3D11DeviceContext->PSSetShader(gpID3D11PixelShader_PV, NULL, 0);

    if (gpID3D11PixelShader_PV)
    {
        gpID3D11PixelShader_PV->Release();
        gpID3D11PixelShader_PV = NULL;
    }

    // get sphere data
    getSphereVertexData(sphere_vertices, sphere_normals, sphere_textures, sphere_elements);

    num_of_sphere_vertices = getNumberOfSphereVertices();
    num_of_sphere_elements = getNumberOfSphereElements();

    // *Sequence Changes As Per Sirs Suggestion
    D3D11_INPUT_ELEMENT_DESC inputElementDesc_PV[2];

    // Position
    inputElementDesc_PV[0].SemanticName = "POSITION";
    inputElementDesc_PV[0].SemanticIndex = 0;
    inputElementDesc_PV[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    inputElementDesc_PV[0].AlignedByteOffset = 0;
    inputElementDesc_PV[0].InputSlot = 0;
    inputElementDesc_PV[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    inputElementDesc_PV[0].InstanceDataStepRate = 0;

    inputElementDesc_PV[1].SemanticName = "NORMAL";
    inputElementDesc_PV[1].SemanticIndex = 0;
    inputElementDesc_PV[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    inputElementDesc_PV[1].AlignedByteOffset = 0;
    inputElementDesc_PV[1].InputSlot = 1;
    inputElementDesc_PV[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    inputElementDesc_PV[1].InstanceDataStepRate = 0;

    hr = gpID3D11Device->CreateInputLayout(inputElementDesc_PV,
                                           _ARRAYSIZE(inputElementDesc_PV),
                                           pID3DBlob_VertexShaderCode_PV->GetBufferPointer(),
                                           pID3DBlob_VertexShaderCode_PV->GetBufferSize(),
                                           &gpID3D11InputLayout_PV);

    if (FAILED(hr))
    {
        fopen_s(&gpFile, gszFileName, "a+");
        fprintf_s(gpFile, "ID3D11Device::CreateInputLayout() Failed!\n");
        fclose(gpFile);
        if (pID3DBlob_VertexShaderCode_PV)
        {
            pID3DBlob_VertexShaderCode_PV->Release();
            pID3DBlob_VertexShaderCode_PV = NULL;
        }
        return (hr);
    }
    else
    {
        fopen_s(&gpFile, gszFileName, "a+");
        fprintf_s(gpFile, "ID3D11Device::CreateInputLayout() Succeeded!\n");
        fclose(gpFile);
        if (pID3DBlob_VertexShaderCode_PV)
        {
            pID3DBlob_VertexShaderCode_PV->Release();
            pID3DBlob_VertexShaderCode_PV = NULL;
        }
    }

    gpID3D11DeviceContext->IASetInputLayout(gpID3D11InputLayout_PV);

    // Create vertex buffer
    D3D11_BUFFER_DESC bufferDesc;
    ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufferDesc.ByteWidth = sizeof(float) * num_of_sphere_vertices * 3;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    hr = gpID3D11Device->CreateBuffer(&bufferDesc,
                                      NULL,
                                      &gpID3D11Buffer_VertexBuffer_Position);

    if (FAILED(hr))
    {
        fopen_s(&gpFile, gszFileName, "a+");
        fprintf_s(gpFile, "ID3D11Device::CreateBuffer() POSITION Failed!\n");
        fclose(gpFile);
        return (hr);
    }
    else
    {
        fopen_s(&gpFile, gszFileName, "a+");
        fprintf_s(gpFile, "ID3D11Device::CreateBuffer() POSITION Succeeded!\n");
        fclose(gpFile);
    }

    D3D11_MAPPED_SUBRESOURCE mappedSubResource;
    ZeroMemory((void *)&mappedSubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
    gpID3D11DeviceContext->Map(gpID3D11Buffer_VertexBuffer_Position,
                               0,
                               D3D11_MAP_WRITE_DISCARD,
                               0,
                               &mappedSubResource);

    memcpy(mappedSubResource.pData, sphere_vertices, sizeof(float) * num_of_sphere_vertices * 3);
    gpID3D11DeviceContext->Unmap(gpID3D11Buffer_VertexBuffer_Position, 0);

    // Pushed Normals Data
    ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufferDesc.ByteWidth = sizeof(float) * num_of_sphere_vertices * 3;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    hr = gpID3D11Device->CreateBuffer(&bufferDesc,
                                      NULL,
                                      &gpID3D11Buffer_VertexBuffer_Normals);

    if (FAILED(hr))
    {
        fopen_s(&gpFile, gszFileName, "a+");
        fprintf_s(gpFile, "ID3D11Device::CreateBuffer() Normal Failed!\n");
        fclose(gpFile);
        return (hr);
    }
    else
    {
        fopen_s(&gpFile, gszFileName, "a+");
        fprintf_s(gpFile, "ID3D11Device::CreateBuffer() Normal Succeeded!\n");
        fclose(gpFile);
    }

    ZeroMemory((void *)&mappedSubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
    gpID3D11DeviceContext->Map(gpID3D11Buffer_VertexBuffer_Normals,
                               0,
                               D3D11_MAP_WRITE_DISCARD,
                               0,
                               &mappedSubResource);

    memcpy(mappedSubResource.pData, sphere_normals, sizeof(float) * num_of_sphere_vertices * 3);
    gpID3D11DeviceContext->Unmap(gpID3D11Buffer_VertexBuffer_Normals, 0);

    // Create elements buffer
    ZeroMemory((void *)&bufferDesc, sizeof(D3D11_BUFFER_DESC));
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufferDesc.ByteWidth = sizeof(short) * num_of_sphere_elements;
    bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    hr = gpID3D11Device->CreateBuffer(&bufferDesc,
                                      NULL,
                                      &gpID3D11Buffer_IndexBuffer_Elements);

    if (FAILED(hr))
    {
        fopen_s(&gpFile, gszFileName, "a+");
        fprintf_s(gpFile, "ID3D11Device::CreateBuffer() Elements Failed!\n");
        fclose(gpFile);
        return (hr);
    }
    else
    {
        fopen_s(&gpFile, gszFileName, "a+");
        fprintf_s(gpFile, "ID3D11Device::CreateBuffer() Elements Succeeded!\n");
        fclose(gpFile);
    }

    ZeroMemory((void *)&mappedSubResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
    gpID3D11DeviceContext->Map(gpID3D11Buffer_IndexBuffer_Elements,
                               0,
                               D3D11_MAP_WRITE_DISCARD,
                               0,
                               &mappedSubResource);

    memcpy(mappedSubResource.pData, sphere_elements, sizeof(short) * num_of_sphere_elements);
    gpID3D11DeviceContext->Unmap(gpID3D11Buffer_IndexBuffer_Elements, 0);

    // Initialise and create constant buffer
    D3D11_BUFFER_DESC bufferDesc_ConstantBuffer_PV;
    ZeroMemory(&bufferDesc_ConstantBuffer_PV, sizeof(D3D11_BUFFER_DESC));
    bufferDesc_ConstantBuffer_PV.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc_ConstantBuffer_PV.ByteWidth = sizeof(CBUFFER);
    bufferDesc_ConstantBuffer_PV.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc_ConstantBuffer_PV.CPUAccessFlags = 0;

    hr = gpID3D11Device->CreateBuffer(&bufferDesc_ConstantBuffer_PV,
                                      nullptr,
                                      &gpID3D11Buffer_ConstantBuffer_PV);
    if (FAILED(hr))
    {
        fopen_s(&gpFile, gszFileName, "a+");
        fprintf_s(gpFile, "ID3D11Device::CreateBuffer() Failed!\n");
        fclose(gpFile);
        return (hr);
    }
    else
    {
        fopen_s(&gpFile, gszFileName, "a+");
        fprintf_s(gpFile, "ID3D11Device::CreateBuffer() Succeeded!\n");
        fclose(gpFile);
    }

    gpID3D11DeviceContext->VSSetConstantBuffers(0,
                                                1,
                                                &gpID3D11Buffer_ConstantBuffer_PV);

    // *Create and set rasterizer state
    D3D11_RASTERIZER_DESC d3d11RasterizerDesc;
    ZeroMemory((void *)&d3d11RasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
    d3d11RasterizerDesc.AntialiasedLineEnable = FALSE;
    d3d11RasterizerDesc.CullMode = D3D11_CULL_NONE;
    d3d11RasterizerDesc.DepthBias = 0;
    d3d11RasterizerDesc.DepthBiasClamp = 0.0f;
    d3d11RasterizerDesc.DepthClipEnable = TRUE;
    d3d11RasterizerDesc.FillMode = D3D11_FILL_SOLID;
    d3d11RasterizerDesc.FrontCounterClockwise = FALSE;
    d3d11RasterizerDesc.MultisampleEnable = FALSE;
    d3d11RasterizerDesc.ScissorEnable = FALSE;
    d3d11RasterizerDesc.SlopeScaledDepthBias = 0.0f;

    hr = gpID3D11Device->CreateRasterizerState(&d3d11RasterizerDesc, &gpID3D11RasterizerState);
    if (FAILED(hr))
    {
        fopen_s(&gpFile, gszFileName, "a+");
        fprintf_s(gpFile, "ID3D11Device::CreateRasterizerState() Failed!\n");
        fclose(gpFile);
        return (hr);
    }
    else
    {
        fopen_s(&gpFile, gszFileName, "a+");
        fprintf_s(gpFile, "ID3D11Device::CreateRasterizerState() Succeeded!\n");
        fclose(gpFile);
    }

    gpID3D11DeviceContext->RSSetState(gpID3D11RasterizerState);

    // *BLACK COLOR
    gfClearColor[0] = 0.05f;
    gfClearColor[1] = 0.05f;
    gfClearColor[2] = 0.05f;
    gfClearColor[3] = 1.0f;

    // *set projection matrix to identity matrix
    gPerspectiveProjectionMatrix = XMMatrixIdentity();

    // *call resize for first time
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
    // *CODE'
    HRESULT hr;

    // *Free Any Size-Dependent Resources
    if (gpID3D11RenderTargetView)
    {
        gpID3D11RenderTargetView->Release();
        gpID3D11RenderTargetView = NULL;
    }
    // *Free Any Size-Dependent Resources
    if (gpID3D11DepthStencilView)
    {
        gpID3D11DepthStencilView->Release();
        gpID3D11DepthStencilView = NULL;
    }

    // *Resize Swap Chain Buffers Accordingly
    gpIDXGISwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

    // *Again Get Back Buffers From Swap Chain
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

    // *For Depth Just Like needed for RTV, DSV also Need texture buffer
    D3D11_TEXTURE2D_DESC d3d11Texture2DDesc;
    ZeroMemory((void *)&d3d11Texture2DDesc, sizeof(D3D11_TEXTURE2D_DESC));
    d3d11Texture2DDesc.Width = (UINT)width;
    d3d11Texture2DDesc.Height = (UINT)height;
    d3d11Texture2DDesc.Format = DXGI_FORMAT_D32_FLOAT;
    d3d11Texture2DDesc.Usage = D3D11_USAGE_DEFAULT;
    d3d11Texture2DDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    d3d11Texture2DDesc.SampleDesc.Count = 1; // this can be 1 - 4
    d3d11Texture2DDesc.SampleDesc.Quality = 0;
    d3d11Texture2DDesc.ArraySize = 1;
    d3d11Texture2DDesc.MipLevels = 1;
    d3d11Texture2DDesc.CPUAccessFlags = 0;
    d3d11Texture2DDesc.MiscFlags = 0;

    ID3D11Texture2D *pID3D11Texture2D_DepthBuffer = NULL;
    hr = gpID3D11Device->CreateTexture2D(&d3d11Texture2DDesc, NULL, &pID3D11Texture2D_DepthBuffer);
    if (FAILED(hr))
    {
        fopen_s(&gpFile, gszFileName, "a+");
        fprintf_s(gpFile, "CreateTexture2D() Failed!\n");
        fclose(gpFile);
        return (hr);
    }
    else
    {
        fopen_s(&gpFile, gszFileName, "a+");
        fprintf_s(gpFile, "CreateTexture2D() Succeeded!\n");
        fclose(gpFile);
    }

    D3D11_DEPTH_STENCIL_VIEW_DESC d3d11DepthStencilViewDesc;
    ZeroMemory((void *)&d3d11DepthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
    d3d11DepthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
    d3d11DepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
    hr = gpID3D11Device->CreateDepthStencilView(pID3D11Texture2D_DepthBuffer, &d3d11DepthStencilViewDesc, &gpID3D11DepthStencilView);
    if (FAILED(hr))
    {
        fopen_s(&gpFile, gszFileName, "a+");
        fprintf_s(gpFile, "CreateDepthStencilView() Failed!\n");
        fclose(gpFile);
        return (hr);
    }
    else
    {
        fopen_s(&gpFile, gszFileName, "a+");
        fprintf_s(gpFile, "CreateDepthStencilView() Succeeded!\n");
        fclose(gpFile);
    }

    // *Set Render Target View As Render Target && depth Change in last parameter
    gpID3D11DeviceContext->OMSetRenderTargets(1, &gpID3D11RenderTargetView, gpID3D11DepthStencilView);

    // *Set Viewport
    D3D11_VIEWPORT d3dViewport;
    d3dViewport.TopLeftX = 0;
    d3dViewport.TopLeftY = 0;
    d3dViewport.Width = (float)width;
    d3dViewport.Height = (float)height;
    d3dViewport.MinDepth = 0.0f;
    d3dViewport.MaxDepth = 1.0f;
    gpID3D11DeviceContext->RSSetViewports(1, &d3dViewport);

    gPerspectiveProjectionMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), (float)width / (float)height, 0.01f, 100.0f);

    return (hr);
}

void display(void)
{
    void update(void);

    // *CODE

    // *Clear Render Target View To A Chosen Color
    gpID3D11DeviceContext->ClearRenderTargetView(gpID3D11RenderTargetView, gfClearColor);
    gpID3D11DeviceContext->ClearDepthStencilView(gpID3D11DepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

    // *select which vertext buffer to display
    UINT stride = sizeof(float) * 3;
    UINT offset = 0;

    gpID3D11DeviceContext->IASetVertexBuffers(0,
                                              1,
                                              &gpID3D11Buffer_VertexBuffer_Position,
                                              &stride,
                                              &offset);

    stride = sizeof(float) * 3;
    offset = 0;
    gpID3D11DeviceContext->IASetVertexBuffers(1,
                                              1,
                                              &gpID3D11Buffer_VertexBuffer_Normals,
                                              &stride,
                                              &offset);
    gpID3D11DeviceContext->IASetIndexBuffer(gpID3D11Buffer_IndexBuffer_Elements, DXGI_FORMAT_R16_UINT, 0);

    // *Set primitive topology
    gpID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // *Transformations
    XMMATRIX translationMatrix = XMMatrixTranslation(0.0f, 0.0f, 1.0f);
    XMMATRIX rotationMatrix1 = XMMatrixRotationX(gAngle);
    XMMATRIX rotationMatrix2 = XMMatrixRotationY(gAngle);
    XMMATRIX rotationMatrix3 = XMMatrixRotationZ(gAngle);
    XMMATRIX worldMatrix = translationMatrix;
    XMMATRIX viewMatrix = XMMatrixIdentity();

    // Final WorldViewMatrix
    XMMATRIX wvpMatrix = worldMatrix * viewMatrix * gPerspectiveProjectionMatrix;

    // *Load the data into constant buffer
    CBUFFER constantBuffer;
    constantBuffer.WorldMatrix = worldMatrix;
    constantBuffer.ViewMatrix = worldMatrix;
    constantBuffer.ProjectionMatrix = gPerspectiveProjectionMatrix;

    if (gbLights)
    {
        constantBuffer.KeyPressed = 1;

        constantBuffer.LA = XMVectorSet(light_ambient[0], light_ambient[1], light_ambient[2], light_ambient[3]);
        constantBuffer.LD = XMVectorSet(light_diffuse[0], light_diffuse[1], light_diffuse[2], light_diffuse[3]);
        constantBuffer.LS = XMVectorSet(light_specular[0], light_specular[1], light_specular[2], light_specular[3]);

        constantBuffer.KA = XMVectorSet(material_ambient[0], material_ambient[1], material_ambient[2], material_ambient[3]);
        constantBuffer.KD = XMVectorSet(material_diffuse[0], material_diffuse[1], material_diffuse[2], material_diffuse[3]);
        constantBuffer.KS = XMVectorSet(material_specular[0], material_specular[1], material_specular[2], material_specular[3]);

        constantBuffer.LightPosition = XMVectorSet(light_position[0], light_position[1], light_position[2], light_position[3]);
        constantBuffer.MaterialShininess = material_shininess;
    }
    else
    {
        constantBuffer.KeyPressed = 0;
    }

 

    gpID3D11DeviceContext->UpdateSubresource(gpID3D11Buffer_ConstantBuffer_PV,
                                             0,
                                             NULL,
                                             &constantBuffer,
                                             0,
                                             0);
    // *draw vertex buffer to render target
    gpID3D11DeviceContext->DrawIndexed(num_of_sphere_elements, 0, 0);

    // update frame
    if (gbAnimate)
    {
        update();
    }
    // *Switch Between Front & Back Buffers
    gpIDXGISwapChain->Present(0, 0);
}

void update(void)
{
    gAngle = gAngle + 0.001f;
    if (gAngle >= 360.0f)
    {
        gAngle = 0;
    }
}
void uninitialize(void)
{
    // *CODE
    if (gpID3D11Buffer_ConstantBuffer_PV)
    {
        gpID3D11Buffer_ConstantBuffer_PV->Release();
        gpID3D11Buffer_ConstantBuffer_PV = NULL;
    }

    if (gpID3D11InputLayout_PV)
    {
        gpID3D11InputLayout_PV->Release();
        gpID3D11InputLayout_PV = NULL;
    }

    if (gpID3D11Buffer_VertexBuffer_Position)
    {
        gpID3D11Buffer_VertexBuffer_Position->Release();
        gpID3D11Buffer_VertexBuffer_Position = NULL;
    }
    if (gpID3D11Buffer_VertexBuffer_Normals)
    {
        gpID3D11Buffer_VertexBuffer_Normals->Release();
        gpID3D11Buffer_VertexBuffer_Normals = NULL;
    }
    if (gpID3D11Buffer_IndexBuffer_Elements)
    {
        gpID3D11Buffer_IndexBuffer_Elements->Release();
        gpID3D11Buffer_IndexBuffer_Elements = NULL;
    }

    if (gpID3D11PixelShader_PV)
    {
        gpID3D11PixelShader_PV->Release();
        gpID3D11PixelShader_PV = NULL;
    }

    if (gpID3D11VertexShader_PV)
    {
        gpID3D11VertexShader_PV->Release();
        gpID3D11VertexShader_PV = NULL;
    }

    if (gpID3D11DepthStencilView)
    {
        gpID3D11DepthStencilView->Release();
        gpID3D11DepthStencilView = NULL;
    }

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
