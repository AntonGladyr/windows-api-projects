#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>

#define	defaultFirstProcess		"C:\\WINDOWS\\SYSTEM32\\tasklist.exe /nh"
#define	defaultSecondProcess		"C:\\WINDOWS\\SYSTEM32\\sort.exe /+60 /R"

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
	char shellCommand[2 * MAX_PATH] = "";
	char firstCommand[MAX_PATH] = "";
	char secondCommand[ MAX_PATH] = "";
	char * pch;
	int index;
	
	AllocConsole();
	SetConsoleTitle( "Processes info" );
	system( "cls" );
	
	if ( argc == 1 ) {
		strcat( shellCommand,  defaultFirstProcess );
		strcat( shellCommand,  "=" );
		strcat( shellCommand,  defaultSecondProcess );
    	}
    	else if ( argc >= 2 ) {
    		int counter;
		for ( counter = 1; counter < argc; counter++ ) {
			strcat( shellCommand, argv[counter] );
			strcat( shellCommand, " " );
		}
    	}
    	
    	if ( strchr( shellCommand, '=' )) {
    		pch = strchr( shellCommand, '=' );
    	}
    	else {
    		printf("%s\n", "Wrong arguments...");
		return;
    	}
    	
    	index = ( int )( pch - shellCommand );
	strncpy( firstCommand, shellCommand,  index );
	printf("%s\n", firstCommand);
	strncpy( secondCommand, shellCommand + index + 1, strlen( shellCommand ) - index );
	printf("%s\n\n", secondCommand);
	
	PSA.nLength = sizeof( PSA );
	PSA.lpSecurityDescriptor =  NULL;
	PSA.bInheritHandle = TRUE;
	GetStartupInfo( &StartupInfo1 );
	CreatePipe( &hReadPipe, &hWritePipe, &PSA, 0 );
	StartupInfo1.hStdOutput = hWritePipe;
	StartupInfo1.dwFlags = STARTF_USESTDHANDLES;
	if ( !CreateProcess(
						NULL,						// No module name (use command line)
						firstCommand,				// Command line
						NULL,						// Process handle not inheritable
						NULL,						// Thread handle not inheritable
						TRUE,						// Set handle inheritance to TRUE
						0,							// The new process has a new console
						NULL,						// Use parent's environment block
						TEXT("C:\\WINDOWS\\SYSTEM32\\wbem"), // Use parent's starting directory
						&StartupInfo1,				// Pointer to STARTUPINFO structure
						&ProcessInfo1				// Pointer to PROCESS_INFORMATION structure
				)
	) {
		printf( "First process failed (%d).\n", GetLastError() );
        		return;
	}
	CloseHandle( hWritePipe );
	GetStartupInfo( &StartupInfo2 );
	StartupInfo2.hStdOutput = GetStdHandle( STD_OUTPUT_HANDLE );
	StartupInfo2.hStdInput  = hReadPipe;
	StartupInfo2.dwFlags = STARTF_USESTDHANDLES;
	if ( !CreateProcess(
						NULL,						// No module name (use command line)
						secondCommand,				// Command line
						NULL,						// Process handle not inheritable
						NULL,						// Thread handle not inheritable
						TRUE,						// Set handle inheritance to TRUE
						0,							// The new process has a new console
						NULL,						// Use parent's environment block
						TEXT("C:\\WINDOWS\\SYSTEM32\\wbem"),		// Use parent's starting directory
						&StartupInfo2,				// Pointer to STARTUPINFO structure
						&ProcessInfo2				// Pointer to PROCESS_INFORMATION structure
				)
	) {
		printf( "Second process failed (%d).\n", GetLastError() );
        		return;
	}
	
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
