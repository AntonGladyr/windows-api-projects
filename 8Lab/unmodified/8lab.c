#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

void __cdecl main( int argc, char *argv[] )
{
	STARTUPINFO StartupInfo1;
	STARTUPINFO StartupInfo2;
	HANDLE hReadPipe;
	HANDLE hWritePipe;
	PROCESS_INFORMATION ProcessInfo1;
	PROCESS_INFORMATION ProcessInfo2;
	SECURITY_ATTRIBUTES  PSA;
	HANDLE hProcess[2];
	
	AllocConsole();
	SetConsoleTitle( "Processes info" );
	system( "cls" );
	
	PSA.nLength = sizeof( PSA );
	PSA.lpSecurityDescriptor =  NULL;
	PSA.bInheritHandle = TRUE;
	GetStartupInfo( &StartupInfo1 );
	CreatePipe( &hReadPipe, &hWritePipe, &PSA, 0 );
	StartupInfo1.hStdOutput = hWritePipe;
	StartupInfo1.dwFlags = STARTF_USESTDHANDLES;
	CreateProcess(
						NULL,						// No module name (use command line)
						TEXT("C:\\WINDOWS\\SYSTEM32\\tasklist.exe /nh"),					// Command line
						NULL,						// Process handle not inheritable
						NULL,						// Thread handle not inheritable
						TRUE,						// Set handle inheritance to TRUE
						0,							// The new process has a new console
						NULL,						// Use parent's environment block
						TEXT("C:\\WINDOWS\\SYSTEM32\\wbem"), // Use parent's starting directory
						&StartupInfo1,				// Pointer to STARTUPINFO structure
						&ProcessInfo1				// Pointer to PROCESS_INFORMATION structure
				);
	CloseHandle( hWritePipe );
	GetStartupInfo( &StartupInfo2 );
	StartupInfo2.hStdOutput = GetStdHandle( STD_OUTPUT_HANDLE );
	StartupInfo2.hStdInput  = hReadPipe;
	StartupInfo2.dwFlags = STARTF_USESTDHANDLES;
	CreateProcess(
						NULL,						// No module name (use command line)
						TEXT("C:\\WINDOWS\\SYSTEM32\\sort.exe /+60 /R"),					// Command line
						NULL,						// Process handle not inheritable
						NULL,						// Thread handle not inheritable
						TRUE,						// Set handle inheritance to TRUE
						0,							// The new process has a new console
						NULL,						// Use parent's environment block
						TEXT("C:\\WINDOWS\\SYSTEM32\\wbem"),		// Use parent's starting directory
						&StartupInfo2,				// Pointer to STARTUPINFO structure
						&ProcessInfo2				// Pointer to PROCESS_INFORMATION structure
				);
	CloseHandle( hWritePipe );
	hProcess[0] = ProcessInfo1.hProcess;
	hProcess[1] = ProcessInfo2.hProcess;
	WaitForMultipleObjects( sizeof( hProcess ) / sizeof( HANDLE ), hProcess, TRUE, INFINITE );
	CloseHandle( ProcessInfo1.hProcess );
	CloseHandle( ProcessInfo2.hProcess );
	printf( "Press <ENTER> to continue..." );
    	while( getchar() != '\n' );
	return;
}
