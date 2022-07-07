// Header
#include<windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow) {

	WNDCLASSEX wndclass;				
	HWND hwnd;								
	MSG msg;							
	TCHAR szAppName[] = TEXT("My App");		

	// Initilize WNDCLASSEX Structure
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

	//Register the WNDCLASSEX structure
	RegisterClassEx(&wndclass); 


	//Create Window in Memory 
	hwnd = CreateWindow(szAppName,					
		TEXT("Rohit's Window Application"),	
		WS_OVERLAPPEDWINDOW,						
		CW_USEDEFAULT,								
		CW_USEDEFAULT,								
		CW_USEDEFAULT,								
		CW_USEDEFAULT,								
		NULL,										
		NULL,										
		hInstance,									
		NULL);										

	//Display Window on Desktop
	ShowWindow(hwnd, iCmdShow); 					
	UpdateWindow(hwnd);								

	while(GetMessage(&msg, NULL, 0, 0)) {

		TranslateMessage(&msg);
		DispatchMessage(&msg);

	}

	return((int)msg.wParam);

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {

	switch (iMsg) {
        case WM_CREATE:
            MessageBox(hwnd, TEXT(" WM_CREATE IS RECIEVED "), TEXT("My Message!"),MB_OK);
            break;

        case WM_LBUTTONDOWN:
            MessageBox(hwnd, TEXT(" WM_LBUTTONDOWN IS RECIEVED "), TEXT("My Message!"),MB_OK);
            break;

        case WM_KEYDOWN:
            MessageBox(hwnd, TEXT(" WM_KEYDOWN IS RECIEVED "), TEXT("My Message!"),MB_OK);
            break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

        default:
            break;
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));

}
