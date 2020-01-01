// 
// file:		dllmain_lib_console_alerter.cpp
// created on:	2020 Jan 01
// created by:	Davit Kalantaryan 
// 

#include <Windows.h>
#include <stdio.h>

BOOL WINAPI DllMain(_In_ HINSTANCE ,_In_ DWORD a_fdwReason,_In_ LPVOID a_lpvReserved)
{
	printf("DllMain a_lpvReserved=%p, a_fdwReason=%d\n", a_lpvReserved, (int)a_fdwReason);
	//FreeConsole();
	if((!a_lpvReserved)&&(a_fdwReason==DLL_PROCESS_ATTACH)){
		printf("In the if \n");
		if(GetConsoleWindow()){
			printf("Freeing console!\n");
			FreeConsole();
		}
		else{
			if(!AttachConsole(ATTACH_PARENT_PROCESS)){
				AllocConsole();
			}
			printf("Console is back!\n");
		}
	}

	return FALSE;
}
