// Header Files
#include <Windows.h>
#include <string.h>
#include <memory.h>

//global Variable Declaration
bool gbFullscreen = false;
HWND ghwnd;
DWORD gdwStyle;
DEVMODE devmode;
WINDOWPLACEMENT wpPrev;


// Global Function Declaration
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// GUI Entry Point 
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow) {
	
	//Local Variables Declaration
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[255] = TEXT("Class_FullscreenWindowCDS");

	int iScreenWidth  = 800;
	int iScreenHeight = 600;
	
	//DEVMODE INITIALIZATION
	devmode.dmSize = sizeof(DEVMODE);
	EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devmode);
	devmode.dmPelsWidth = iScreenWidth;
	devmode.dmPelsHeight = iScreenHeight;
	devmode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

	//Initialize WNDCLASSEX Member

	int x = GetSystemMetrics(SM_CXSCREEN);
	int y = GetSystemMetrics(SM_CYSCREEN);

	x = (x - iScreenWidth) /2;
	y = (y - iScreenHeight)/2;

	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	//Register Class 
	RegisterClassEx(&wndclass);

	//Create Window In Memory
	hwnd = CreateWindow(szAppName,
		TEXT("Fullscreen Window Using ChangeDisplaySetting()"),
		WS_OVERLAPPEDWINDOW,
		x,
		y,
		iScreenWidth,
		iScreenHeight,
		NULL,
		NULL,
		hInstance,
		NULL);
	
	ghwnd = hwnd;

	// Show Window State
	ShowWindow(hwnd, iCmdShow);

	// Update Window With Color Given In wndclass Iitialization (Sends WM_PAINT Automatically)
	UpdateWindow(hwnd);

	//Message Loop

	while (GetMessage(&msg, NULL, 0, 0)) {

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return((int)msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
	
	HDC hdc;
	RECT rc;
	PAINTSTRUCT ps;
	TCHAR str[] = "H e l l o ,   W o r l d ! ! !";

	void ToggleFullscreen(void);

	switch (iMsg) {

		case WM_CREATE:
			break;

		case WM_PAINT:
			
			GetClientRect(hwnd, &rc);

			hdc = BeginPaint(hwnd, &ps);
			
			SetBkColor(hdc, RGB(0, 0, 0));
			SetTextColor(hdc, RGB(0, 255, 0));
			
			DrawText(hdc, str, -1, &rc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
		
			EndPaint(hwnd, &ps);
			break;
		case WM_KEYDOWN:
				switch (wParam) {
					case 0x46:	// 'f'
					case 0x66:	// 'F'
						ToggleFullscreen();
						break;
					default:
						break;
				}
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		
		default:
			break;
	}

	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}


void ToggleFullscreen() {

	if (gbFullscreen == false) {

		gdwStyle = GetWindowLong(ghwnd, GWL_STYLE);
		GetWindowPlacement(ghwnd, &wpPrev);

		if (ChangeDisplaySettings(&devmode, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
			gbFullscreen = false;
			MessageBox(ghwnd, TEXT("FAILED"), TEXT("FAILED"), MB_OK);
			return;
		}

		SetWindowLong(ghwnd, GWL_STYLE, gdwStyle & ~WS_OVERLAPPEDWINDOW);

		SetWindowPos(ghwnd,
			HWND_TOP,
			0, 0,
			GetSystemMetrics(SM_CXSCREEN),
			GetSystemMetrics(SM_CYSCREEN),
			SWP_NOZORDER | SWP_FRAMECHANGED);

		ShowCursor(FALSE);
		gbFullscreen = true;

	}
	else {


		if (ChangeDisplaySettings(NULL, CDS_RESET) == DISP_CHANGE_SUCCESSFUL) {

			SetWindowLong(ghwnd, GWL_STYLE, gdwStyle | WS_OVERLAPPEDWINDOW);
			SetWindowPlacement(ghwnd, &wpPrev);
			ShowCursor(TRUE);
			gbFullscreen = false;
		}
	}

}



/*
>>Compile:

cl.exe /c /EHsc FullscreenWindowUsingChangeDisplaySetting.cpp

>>Link:

link.exe FullscreenWindowUsingChangeDisplaySetting.obj kernel32.lib gdi32.lib user32.lib

*/

