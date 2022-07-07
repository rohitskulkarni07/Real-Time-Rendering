#include <stdio.h>
#include <d3d11.h>
#include <math.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

int main(void)
{
    /**
     * * local variable declaration
     */
    IDXGIFactory *pIDXGIFactory = NULL;
    IDXGIAdapter *pIDXGIAdapter = NULL;
    DXGI_ADAPTER_DESC dxgiAdapterDesc;
    HRESULT hr;
    char str[255];

    /**
     * *Code
     * *COM Helper Function
     */
    hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void **)&pIDXGIFactory);

    /**
     *  *Error Checking
     *  *Check If CreateDXGIFactoy is Failed
     */
    if (FAILED(hr))
    {
        printf("CreateDXGIFactory() Failed\n");
        goto cleanup;
    }

    /**
     *  *Enumerate From List of Adapters
     *  *Check For Valid Adapter
     */
    if (pIDXGIFactory->EnumAdapters(0, &pIDXGIAdapter) == DXGI_ERROR_NOT_FOUND)
    {
        printf("IDXGIAdapter Cannot Be Found!\n");
    }

    /**
     * *Zero Out Structure Memory
     */
    ZeroMemory((void *)&dxgiAdapterDesc, sizeof(DXGI_ADAPTER_DESC));

    /**
     * *Get The Filled Structure
     */
    hr = pIDXGIAdapter->GetDesc(&dxgiAdapterDesc);

    /**
     * *Convert Wide Charater String To ASCII Using ANSI Code Page
     */
    WideCharToMultiByte(CP_ACP, 0, dxgiAdapterDesc.Description, 255, str, 255, NULL, NULL);

    printf("\n************************************************\n");
    printf("\nGraphics Card   : %s\n", str);
    printf("VRAM (BYTES)    : %I64d Bytes\n", (__int64)dxgiAdapterDesc.DedicatedVideoMemory);
    printf("VRAM (GB)       : %1.lf GB\n", ceil(((((__int64)dxgiAdapterDesc.DedicatedVideoMemory / 1024.0) / 1024.0) / 1024.0)));
    printf("\n************************************************\n");

cleanup:
    if (pIDXGIAdapter)
    {
        pIDXGIAdapter->Release();
        pIDXGIAdapter = NULL;
    }

    if (pIDXGIFactory)
    {
        pIDXGIFactory->Release();
        pIDXGIFactory = NULL;
    }

    return (0);
}
