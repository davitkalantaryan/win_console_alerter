#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

//#define LIBRARY_PATH OUTPUT_DIR	"..\dll\lib_console_alerter.dll"
#define LIBRARY_PATH "D:\\davit\\dev\\win_console_alerter\\prj\\libs\\lib_console_alerter_vs\\..\\..\\..\\sys\\x64\\dll\\lib_console_alerter.dll"

#define		AllocMemOnRemoteProcess(__hProcess, __size) VirtualAllocEx((__hProcess),0,(__size),MEM_COMMIT,PAGE_READWRITE)
#define		DeleteMemOnRemoteProcess(__hProcess,__mem) VirtualFreeEx((__hProcess),(__mem), 0, MEM_RELEASE)

int main(int a_argc, char* a_argv[])
{
	//HINSTANCE libHandle = LoadLibraryA(LIBRARY_PATH);
	//
	//if(libHandle){
	//	FreeLibrary(libHandle);
	//}

	HANDLE hProcess = 0;
	HANDLE hThread = 0;
	int nPid;
	int nReturn = 1;
	DWORD dwThreadId;
	SIZE_T szWritten;
	const size_t cunLibraryNameLengthPlus1 = strlen(LIBRARY_PATH) + 1;
	void* pLibraryPathOnRemoteProcess = nullptr;

	if(a_argc<2){
		fprintf(stderr,"Pid of program is not provided!\n");
		return 1;
	}

	nPid = atoi(a_argv[1]);
	printf("Pid of process is: %d\n",nPid);

	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, (nPid));

	if(!hProcess){
		fprintf(stderr, "process with the pid %d does not exist or is not accessible!\n", nPid);
		return 1;
	}

	pLibraryPathOnRemoteProcess = AllocMemOnRemoteProcess(hProcess, cunLibraryNameLengthPlus1);
	if(!pLibraryPathOnRemoteProcess){
		goto returnPoint;
	}

	if(!WriteProcessMemory(hProcess,pLibraryPathOnRemoteProcess,LIBRARY_PATH,cunLibraryNameLengthPlus1,&szWritten)){
		goto returnPoint;
	}

	hThread = CreateRemoteThread(hProcess,nullptr,0,(LPTHREAD_START_ROUTINE)LoadLibraryA,pLibraryPathOnRemoteProcess,0, &dwThreadId);

	nReturn = hThread ? 0 : 1;
returnPoint:
	if(hThread){
		WaitForSingleObject(hThread,INFINITE);
		CloseHandle(hThread);
	}

	if(pLibraryPathOnRemoteProcess){
		DeleteMemOnRemoteProcess(hProcess, pLibraryPathOnRemoteProcess);
	}

	if(hProcess){
		CloseHandle(hProcess);
	}

	return nReturn;
}
