// Header
#include <stdio.h>
#include<windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//Gloabal Variable

FILE *gpFile = NULL;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow) {

	WNDCLASSEX wndclass = {};
	HWND hwnd;								
	MSG msg = {};
	TCHAR szAppName[] = TEXT("My App");	

	if(fopen_s(&gpFile, "Log.txt","w")) {

		MessageBox(NULL,TEXT("Cannot Open Desired File !!!"), TEXT("FILE IO ERROR"), MB_OK | MB_ICONERROR);
		exit(0);
	}

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

	fclose(gpFile);
	return((int)msg.wParam);

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {

	switch (iMsg) {
        case WM_CREATE:
			fprintf(gpFile,"India Is My Country.\n");
			break;
		case WM_DESTROY:
			fprintf(gpFile,"*** JAI HIND  ***\n");
			PostQuitMessage(0);
			break;

        default:
            break;
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));

}
