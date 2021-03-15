#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <tlhelp32.h>

/*  PROTOTYPES */
BOOL CtrlHandler( DWORD fdwCtrlType );

/* GLOBAL VARIABLES */

    	HANDLE hSnapshot;
    	
    	PROCESSENTRY32 processInfo;
    	
    	/*
    	;	PROCESSENTRY32 STRUCT
	;	dwSize				DWORD ? ;
	;	cntUsage				DWORD ? ;
	;	th32ProcessID		DWORD ? ;
	;	th32DefaultHeapID	ULONG_PTR ? ;
	;	th32ModuleID			DWORD ? ;
	;	cntThreads			DWORD ? ;
	;	th32ParentProcessID	DWORD ? ;
	;	pcPriClassBase		LONG ? ;
	;	dwFlags				DWORD ? ;
	;    	szExeFile				CHAR MAX_PATH dup (?)
	;	PROCESSENTRY32 ENDS
    	*/

/* ENTRY POINT */
void __cdecl main( int argc, char *argv[] )
{
	SetConsoleCtrlHandler( ( PHANDLER_ROUTINE )  CtrlHandler, TRUE );
    	AllocConsole();
    	SetConsoleTitle( "Processes info" );
    	
    	while( TRUE ) {
		system( "cls" );
		/*  Header of columns */
		printf( "%5s %8s %9s %11s\n", 
								"PID",
								"Threads",
								" Priority",
								"Image name"
		);
		
		// Take a snapshot of all processes in the system.
		hSnapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 ); 
		if ( hSnapshot == INVALID_HANDLE_VALUE ) {
			printf( "Unable to create snapshot of processes..." );
			return;
		}
		
		//Set the size of the structure before using it.
		processInfo.dwSize = sizeof( PROCESSENTRY32 );
		
		// Retrieve information about the first process,
  		// and exit if unsuccessful
  		if( !Process32First( hSnapshot, &processInfo ) )
		{
			printf( "Unable to get info of the first process ..." );
			CloseHandle( hSnapshot );          // clean the snapshot object
			return;
		}
		
		// Now walk the snapshot of processes, and
  		// display information about each process in turn
  		do {
			printf( "%5d %5d %9d %3s %s\n", 
								processInfo.th32ProcessID,
								processInfo.cntThreads,
								processInfo.pcPriClassBase,
								"",
								processInfo.szExeFile
			);
			
  		} while( Process32Next( hSnapshot, &processInfo ) );

		
		printf( "\nPress <Ctrl-C> to continue..." );
		Sleep( 3000 );
		CloseHandle( hSnapshot );
    	}
    	
   	 return;
}

BOOL CtrlHandler( DWORD fdwCtrlType ) {
	CloseHandle( hSnapshot );
	return TRUE;
}    	