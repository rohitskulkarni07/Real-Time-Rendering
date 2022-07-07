del D3D11.exe
rc.exe rico.rc
cl.exe /EHsc D3D11.cpp /link rico.res /NODEFAULTLIB:msvcrt.lib
D3D11.exe
