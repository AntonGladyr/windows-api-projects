#include "../interface.h"

void __cdecl main()
{
	char buffer[4096];
	HANDLE hSemaphore;
	HANDLE hMutex;
	HANDLE hEventData;
	HANDLE hEventCancel;
	HANDLE hFileMap;
	HANDLE hFileMem;

	/* Init console window */
    	AllocConsole();
    	SetConsoleTitle( "Client" );
    	system( "cls" );
    	
    	// MessageBox(0, "TEST", "TEST", MB_OK); 
    	
    	/* Open existing event */
    	hEventData = OpenEvent(
    		EVENT_ALL_ACCESS,	// The access to the event object
    		FALSE,				// The process do not inherit this handle
    		szEventDataName		// The name of the event to be opend
    	);
    	
    	if ( hEventData == NULL ) {
    		printf( "Open first event failed (%d)\n", GetLastError() );
    		while(getchar() != '\n');
		return;
    	}
    	
    	/* Open second existing event */
    	hEventCancel = OpenEvent(
    		EVENT_ALL_ACCESS,					// The access to the event object
    		FALSE,								// The process do not inherit this handle
    		szEventCancelName					// The name of the event to be opend
    	);
    	
    	if ( hEventCancel == NULL ) {
    		CloseHandle( hEventData );
    		printf( "Open second event failed (%d)\n", GetLastError() );
		return;
    	}
    	
    	hMutex = OpenMutex(
    		MUTEX_ALL_ACCESS,	// full access
    		FALSE,				// handle not inheritable
    		szMutexDataName		// object name
    	);
	
	if ( hMutex == NULL ) {
		CloseHandle( hEventCancel );
		CloseHandle( hEventData );
    		printf( "Open mutex failed (%d)\n", GetLastError() );
		return;
    	}
	
	hFileMap = OpenFileMapping(
		FILE_MAP_READ | FILE_MAP_WRITE,	// The access to the file mapping object
		FALSE,								// The handle cannot be inherited
		szMMFName							// The name of the file mapping object to be opened
	);
	
	if ( hFileMap == NULL ) {
		CloseHandle( hMutex );
		CloseHandle( hEventCancel );
		CloseHandle( hEventData );
    		printf( "Could not open file mapping object (%d).\n", GetLastError() );
    		return;
	}
	
	hFileMem = MapViewOfFile(
		hFileMap,							// file mapping object
		FILE_MAP_READ | FILE_MAP_WRITE,	// desired access
		0,									// file offset high
		0,									// file offset low
		0									// number of bytes to map
	);
	
	if ( hFileMem == NULL ) {
		printf( "Could not map view of file (%d).\n", GetLastError() );
		CloseHandle( hFileMap );
    		CloseHandle( hMutex );
		CloseHandle( hEventCancel );
		CloseHandle( hEventData );
		return;
	}
	
	hSemaphore = OpenSemaphore(
    		EVENT_ALL_ACCESS,
    		FALSE,
    		szSemaphoreName
    	);
    	
    	if ( hSemaphore == NULL ) {
    		CloseHandle( hFileMem);
    		CloseHandle( hFileMap );
    		CloseHandle( hMutex );
		CloseHandle( hEventCancel );
		CloseHandle( hEventData );
    		printf( "Open semaphore failed (%d)\n", GetLastError() );
		return;
    	}
    	
    	WaitForSingleObject( hSemaphore, INFINITE );
	
	printf( "Press <ENTER> to continue...\n\n" );
	
	while( TRUE ) {
		fgets( buffer, 4096, stdin );
		if( strlen( buffer ) > 1 ) {
			buffer[ strlen(buffer) - 1] = '\0';
		}
		if ( strcmp( buffer, "\n" ) == 0 ) {
			break;
		}
		WaitForSingleObject( hMutex, INFINITE);
		memcpy( hFileMem, buffer, sizeof( buffer ) );
		SetEvent( hEventData );
		Sleep(100 );
		ReleaseMutex( hMutex );
	}
	
	ReleaseSemaphore( 
    		hSemaphore,
    		1,
    		NULL	
    	 );
    	 
    	CloseHandle ( hSemaphore );
	UnmapViewOfFile( hFileMem );
	CloseHandle( hFileMem);
	CloseHandle( hFileMap );
	CloseHandle( hMutex );
	SetEvent( hEventCancel );
	CloseHandle( hEventCancel );
	CloseHandle( hEventData );	
	
	ExitProcess( 0 );
	
	return;
}
