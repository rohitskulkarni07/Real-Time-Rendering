// Header Files
#include <Windows.h>

// MACRO
#define WIDTH_WINDOW 800
#define HEIGHT_WINDOW 600

// Global Function Declaration
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


// Global Variable Declaration
DWORD dwStyle;													// double word(4 bytes)
WINDOWPLACEMENT wpPrev = { sizeof(WINDOWPLACEMENT) };			// Contains information about the placement of a window on the screen
bool gbFullscreen = false;										// c++'s boolean type
HWND ghwnd = NULL;



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow) {

	// Local Variable Declaration
	WNDCLASSEX wndclass = {sizeof(WNDCLASSEX)};
	MSG msg;
	HWND hwnd;
	TCHAR szAppName[255] = TEXT("ClassFullscreenWindow");

	int iScreenWidth  = 0; 
	int iScreenHeight = 0;
	int x = 0, y = 0;			// screen x axis and screen y axis

	// Initialize WNDCLASS Structure
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

	// Register Class
	RegisterClassEx(&wndclass);

	// Get Screen's Widht And Height
	 iScreenWidth  = GetSystemMetrics(SM_CXSCREEN);
	 iScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	 // Find Co-ordinates for Application's Window For Centering
	 x = (iScreenWidth - WIDTH_WINDOW)  / 2;
	 y = (iScreenHeight - HEIGHT_WINDOW) / 2;

	// Create Window In Memory
	hwnd = CreateWindow(szAppName,
		TEXT("RSK's Application: Fullscreen Window"),
		WS_OVERLAPPEDWINDOW,
		x,
		y,
		WIDTH_WINDOW,
		HEIGHT_WINDOW,
		NULL,
		NULL,
		hInstance,
		NULL);

	//Assign HWND to ghwnd For Future Use
	ghwnd = hwnd;

	// Display Window At Center
	ShowWindow(hwnd, iCmdShow);

	// Update Window With Given Color
	UpdateWindow(hwnd);

	//Message Loop ...Heart Of Program
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return((int)msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {
	
	// Local Function Declaration
	void ToggleFullscreen(void);

	// Local Variable Declaration
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rc;
	TCHAR str[] = TEXT("H e l l o ,  W o r l d ! ! !");


	switch (iMsg) {

		case WM_PAINT:
		
			GetClientRect(hwnd, &rc);

			hdc = BeginPaint(hwnd, &ps);

			SetBkColor(hdc, RGB(0, 0, 0));
			SetTextColor(hdc, RGB(0, 255, 0));

			DrawText(hdc, str, -1, &rc,DT_SINGLELINE | DT_VCENTER | DT_CENTER);

			EndPaint(hwnd, &ps);

			break;

		case WM_KEYDOWN:

			switch (wParam) {

				case 0x46:					// Key >> Small		'f' ASCII >> 70
				case 0x66:					// Key >> Capital	'F' ASCII >> 102

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

void ToggleFullscreen(void) {
	
	//Local Variable Declaration
	MONITORINFO mi = { sizeof(MONITORINFO) };

	//code
	if (gbFullscreen == false) {
		
		dwStyle = GetWindowLong(ghwnd, GWL_STYLE);			// Retrieves information about the specified window , Here Retrives Current Styel Of Given Window

		//Check If Window Has WS_OVERLAPPEDWINDOW (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX)
		if (dwStyle & WS_OVERLAPPEDWINDOW) {
			
			// Check If Window Placement is in Normal Position To Primary Monitors Screen
			if ((GetWindowPlacement(ghwnd, &wpPrev)) && (GetMonitorInfo(MonitorFromWindow(ghwnd, MONITORINFOF_PRIMARY), &mi))) {

				// Set New Style 'or' We Can Say That Remove WS_OVERLAPPEDWIDOW Style
				SetWindowLong(ghwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);

				// As We Have To Set Fullscreen Set Window Position Accordingly
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

		// Set Orignal or Previous Style to Window
		SetWindowLong(ghwnd, GWL_STYLE, (dwStyle | WS_OVERLAPPEDWINDOW));

		SetWindowPlacement(ghwnd, &wpPrev);

		SetWindowPos(ghwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_FRAMECHANGED);

		ShowCursor(TRUE);

		gbFullscreen = false;

	}

}


/*
SWP_NOMOVE			:	Retains the current position (ignores the X and Y parameters).
SWP_NOSIZE			:	Retains the current size (ignores the cx and cy parameters).
SWP_NOOWNERZORDER	:	Does not change the owner window's position in the Z order.
SWP_NOZORDER		:	Retains the current Z order (ignores the hWndInsertAfter parameter).

SWP_FRAMECHANGED	:	Sends a WM_NCCALCSIZE message to the window, even if the window's size is not being changed. 
						If this flag is not specified, WM_NCCALCSIZE is sent only when the window's size is being changed.
*/