#include <Windows.h>


#define WHEIGHT 600
#define WWIDTH 800

//prototype
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine,int iCmdShow) {

	//initialization
	
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[255] = TEXT("WindowWith_WM_PAINT");

	RECT rc;
	int x = 0, y = 0;

	SystemParametersInfo(SPI_GETWORKAREA, 0, &rc, 0);

	x = (rc.right - WWIDTH)/2;
	y = (rc.bottom - WHEIGHT)/2;

	//initilize struct wndclass

	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground =(HBRUSH) GetStockObject(BLACK_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	//register wndclassex

	RegisterClassEx(&wndclass);


	//create window in memory

	hwnd = CreateWindow(szAppName,
		TEXT("Hello, World !!! Application Using WM_PAINT And BeginPaint"),
		WS_OVERLAPPEDWINDOW,
		x,
		y,
		WWIDTH,
		WHEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);

	// show window

	ShowWindow(hwnd, iCmdShow);

	UpdateWindow(hwnd);

	//messageloop

	while (GetMessage(&msg, NULL, 0, 0)) {
		DispatchMessage(&msg);
		TranslateMessage(&msg);

	}

	return((int)msg.wParam);

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
	
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rc;
	TCHAR str[] = "Hello, World !!!";

	switch (iMsg) {
	case WM_PAINT:

		GetClientRect(hwnd,&rc);

		hdc = BeginPaint(hwnd, &ps);

		SetBkColor(hdc, RGB(0, 0, 0));

		SetTextColor(hdc, RGB(0, 255, 0));

		DrawText(hdc, str, -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		EndPaint(hwnd, &ps);
		
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		break;
	}

	return(DefWindowProc(hwnd, iMsg, wParam, lParam));

}



