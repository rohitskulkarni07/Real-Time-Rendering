#include <Windows.h>
#include "MyWindowWithIcon.h"
#include <stdio.h>

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

// Global Function Prototype
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Global Variable
FILE *gpFile = NULL;
bool gbFullscreen = false;
HWND ghwnd = NULL;
DWORD dwStyle = NULL;
WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) };


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow) {
	
	WNDCLASSEX wndclass = { sizeof(WNDCLASSEX) };
	MSG msg = {};
	HWND hwnd;
	TCHAR szAppName[255] = TEXT("CLASS_MYWINDOWICON");

	// Initilize WNDCLASS

	if (fopen_s(&gpFile,"log.txt","w") != 0) {
		MessageBox(NULL, TEXT("FILE CREATION FAILED PROGRAM WILL EXIT..."), TEXT("ERROR"), MB_ICONEXCLAMATION | MB_OK);
		exit(0);
	}

	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
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
	fprintf(gpFile, "Class Registered Successfully\n");
	// Geting x,y for Centering Window 

	//Create Window In Memory

	hwnd = CreateWindow(szAppName,
		TEXT("MY WINDOW WITH ICON"),
		WS_OVERLAPPEDWINDOW,
		(GetSystemMetrics(SM_CXSCREEN) - WIN_WIDTH) / 2,
		(GetSystemMetrics(SM_CYSCREEN) - WIN_HEIGHT) / 2,
		800,
		600,
		NULL,
		NULL,
		hInstance,
		NULL);	
	
	// Assign global File Handle With Current Window Handle
	ghwnd = hwnd;
	fprintf(gpFile, "Create Window Successfull\n");
	
	// Show Window
	ShowWindow(hwnd, iCmdShow);
	fprintf(gpFile, "ShowWindow() Window Successfull\n");
	
	// Color Background
	UpdateWindow(hwnd);
	fprintf(gpFile, "UpdateWindow() Window Successfull\n");


	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	fprintf(gpFile, "*** End Of Message Loop Window Exited Program Executed Succesfully***\n");

	fclose(gpFile);
	return((int)msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
	
	// Local Function Prototype

	void ToggleFullscreen(void);

	// Local Variable
	PAINTSTRUCT ps = {};
	HDC hdc = NULL;
	RECT rc = {};
	TCHAR str[] = TEXT("H e l l o ,  W o r l d");

	switch (iMsg) {
		
	case WM_CREATE:

		fprintf(gpFile, "WM_CREATE Recieved");
		
		break;

	case WM_PAINT:

		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rc);

		SetBkColor(hdc, RGB(0, 0, 0));
		SetTextColor(hdc, RGB(0, 255, 0));

		DrawText(hdc, str, -1, &rc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

		EndPaint(hwnd, &ps);
		
		break;

	case WM_KEYDOWN:
		
		switch (wParam) {

		case 0x46:
		case 0x66:
			// Call ToggleFullscreen()
			ToggleFullscreen();
			break;

		default:
			break;
		}

		break;

	case WM_DESTROY:

		fprintf(gpFile, "WM_DESTROY Recieved\n");
		PostQuitMessage(0);
		fprintf(gpFile, "PostQuitMessage() Succesfull\n");
		
		break;

	default:
		break;
	}

	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}

void ToggleFullscreen(void) {

	//Local Variable
	MONITORINFO mi = { sizeof(MONITORINFO) };


	if (gbFullscreen == false) {

		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);			// Retrieves information about the specified window , Here Retrives Current Styel Of Given Window

		//Check If Window Has WS_OVERLAPPEDWINDOW (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX)
		if (dwStyle & WS_OVERLAPPEDWINDOW) {

			if ((GetWindowPlacement(ghwnd, &wpPrev)) && (GetMonitorInfo(MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY), &mi))) {

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

	} else {

		SetWindowLong(ghwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(ghwnd, &wpPrev);
		SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
		ShowCursor(TRUE);
		gbFullscreen = false;
	}

}