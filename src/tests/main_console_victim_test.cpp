
#include <Windows.h>
#include <stdio.h>

int main()
{
	int nPid = GetCurrentProcessId();

	printf("pid=%d\n", nPid);

	SleepEx(INFINITE, TRUE);

	return 0;
}
