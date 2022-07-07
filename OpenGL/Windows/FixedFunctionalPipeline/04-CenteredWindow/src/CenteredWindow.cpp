#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

#define APP_WIDHT  800
#define APP_HEIGHT 600

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow) {

	//Variables
	int iScreenWidth = 0;
	int iScreenHeight = 0;
	int x, y;

	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("ClassCenteredWindow");


	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground =(HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	//register wndclass

	if (!RegisterClassEx(&wndclass)) {
		MessageBox(NULL, TEXT("Window Registration Failed !"), TEXT("ERROR !"), MB_OK | MB_ICONERROR);
		return 0;
	}

	iScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	iScreenHeight= GetSystemMetrics(SM_CYSCREEN);

	x = (iScreenWidth - APP_WIDHT)/2;		//x co-ordinate for Application window's origin
	y = (iScreenHeight - APP_HEIGHT)/2;		//y co-ordinate for Application window's origin

	hwnd = CreateWindow(szAppName,
		TEXT("Centered Window"),
		WS_OVERLAPPEDWINDOW,
		x,
		y,
		APP_WIDHT,
		APP_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);

	if (hwnd == NULL) {
		MessageBox(NULL, TEXT("Window Creation Failed !"), TEXT("ERROR !"), MB_OK | MB_ICONERROR);
		return 0;
	}
	
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return((int)msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam,LPARAM lParam) {

	switch (iMsg) {

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			break;
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}


