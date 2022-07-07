// Header
#include<windows.h>

// Global Function Declerations
// LRESULT	:	Return Value		>> typedef LONG LRESULT				>> typedef long LONG 
// CALLBACK	:	Calling Convention	>> typedef _far_pascal CALLBACK
// WndProc	:	Identifier			>> Callback function

/* @Parameter : 1] HWND		:	Handle To Window	>>	typedef HANDLE HWND		>>	typedef UINT HANDLE		>>	typedef unsigend int UINT
				2] UINT		:	typedef UINT HANDLE	>>	typedef unsigend int UINT
				3] WPARAM	:	typedef WORD WPARAM	>>	typedef UINT WORD		>>	typedef unsigend int UINT
				4] LPARAM	:	typedef LONG LPARAM	>>	typedef long LONG 
*/

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// int			:	Return Value	
// WINAPI		:	Calling Convention	>>	typedef _far_pascal WINAPI
// WinMain		:	Identifier			>>	Entry-Point Function >> typedef _WinMainCRTStartup WinMain >> typedef  _mainCRTStartup main		>>	main start
// HINSTANCE	:	1] (hInstance		: Identifier) : Handle for Instance		>> typedef HANDLE HINSTANCE		>>	typedef UINT HANDLE		>>	typedef unsigend int UINT
//          	 	2] (hPrevInstance	: Identifier) : NULL Co-operative multitasking
// LPSTR		:	lpszCmdLine	>>	Long Pointer To String Ending With '\0'	>>	typedef _far char* LPSTR : Command- line Arguments >> argv[]
// int			:	iCmdShow >> How Window Should Be Displayed


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow) {

	WNDCLASSEX wndclass;				// struct 12 Member
	HWND hwnd;								// Handle To Window
	MSG msg;							// struct  6 Member >> HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, POINT pt(struct {LONG x,LONG y}), DWORD time, DWORD lPrivate
	TCHAR szAppName[] = TEXT("My App");		// typedef wchar_t TCHAR >> wide-character >> vs2001 to vs2010 we have to write '#define UNICODE' explicitly but now included in 'windows.h' >>PUERLY UNICODE COMPLIENT

	// Initilize WNDCLASSEX Structure
	wndclass.cbSize = sizeof(WNDCLASSEX);				// newly added // size of WNDCLASSEX in count of bytes  

	wndclass.style = CS_HREDRAW | CS_VREDRAW;			// Style of Window Must Be of Type Class Style Horizentally Redrawable | Class Style Vertically Redrawable 

	wndclass.cbClsExtra = 0;							// If 12 Member Cannot Satisfy Your need then Extra information of Class can be send through this parametter

	wndclass.cbWndExtra = 0;							// If Any Extra information for Window

	wndclass.lpfnWndProc = WndProc;						// Long Pointer To Function -> Callback WndProc Address given to member >> Giving Address of our Callback Function

	wndclass.hInstance = hInstance;						// Current Instance Of Runnig Application

	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);	// Which Colour To Be Given To App's Window 
																	// GetStockObject : Function >> Prototype In windows.h
																	//				  : Return Value >> HGDIOBJ >>Handle To Graphics Device Interface Object
																	//				  : Parameter >> int >> #define BLACK_BRUSH 4
																	// BLACK_BRUSH	  : Black Coloured Brush should be used While Colouring Background of Application's Window
																	// HBRUSH		  : HGDIOBJ Type casted in HBRUSH 

	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);	// Icon to Be Used >> LoadIcon : Function (MACRO)
														// NULL (if want use default Icon given by os) >> HINSTANCE (if want to use customized Icon)
														// IDI_APPLICATION : ID of ICON Used in Application >> MACRO (type LPCTSTR long pointer to char string)

	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);		// LoadCursor
														// NULL for default cursor
														// IDC_ARROW ID for Cursor For Current Arrow

	wndclass.lpszClassName = szAppName;					// What name Should be Give to Your Class of which window you will be using >> used by os

	wndclass.lpszMenuName = NULL;						// Menu Name >> curently no menu

	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION); //new added same as of previous

	//Register the WNDCLASSEX structure
	RegisterClassEx(&wndclass); //By registering you are finally giving all 12 members address to structure .i.e Registering your class to os


	//Create Window in Memory : CreateWindow() >> @param: 11. return Value HWND hend (unsigned int 32 bit window)

	hwnd = CreateWindow(szAppName,					// wchar_t szAppName class name of window we have registered the same name in wndclass (type LPCTSTR)
		TEXT("Rohit's Window Application"),			// Window's Caption >> UNICODE string must be in TEXT() MACRO (type LPCTSTR)
		WS_OVERLAPPEDWINDOW,						// What should be the type of Window Style (WS) WS_OVERLAPPEDWINDOW >> (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX) (type DWORD)
		CW_USEDEFAULT,								// x MACRO (CW Create Window)#define CW_USEDEFAULT ((int)0x80000000)  (type int)
		CW_USEDEFAULT,								// y (type int)
		CW_USEDEFAULT,								// width (type int)
		CW_USEDEFAULT,								// height (type int)
		NULL,										// Current window's parent window if NULL then Parent window is DESKTOP (type HWND)
		NULL,										// Menu (type HMENU)
		hInstance,									// Which instance is this window  is? (type HINSTANCE)
		NULL);										// type LPVOID (long pointer to void ) Additinoal Data to Application (TYPE LPARAM)

	//Display Window on Desktop
	ShowWindow(hwnd, iCmdShow); 					// return value BOOL - TRUE(1), FALSE(0)  | bool - true, false (cpp's bool)
													// param 1: Which window to show, hwnd
													// param 2: How to show window (SW_SHOWNORMAL, SW_MAXIMIZE, SW_MINIMIZE) (type int) SW >> Show Window
	UpdateWindow(hwnd);								// return value BOOL - TRUE(1), FALSE(0) >> ex.update winodws backgound with color which is register with wndclass

	// Start The Heart Of Your Application (Message Loop)
	// GetMessage Return Value:BOOL @param MSG msg, HWND hWnd, UINT MessageFilterRangeMin, UINT MessageFilterRangeMax
	while(GetMessage(&msg, NULL, 0, 0)) {

		TranslateMessage(&msg);
		DispatchMessage(&msg);

	}

	return((int)msg.wParam);

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {

	switch (iMsg) {
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));

}
