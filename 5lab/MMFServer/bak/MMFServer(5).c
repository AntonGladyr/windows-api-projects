#include "../interface.h"

#define MAX_SEM_COUNT 3

void __cdecl main( int argc, char *argv[] )
{
	/* LOCAL VARIABLES */
	PROCESS_INFORMATION processInfo;
	STARTUPINFO startInfo;
	HANDLE hSemaphore;
	HANDLE hMutex;
	HANDLE hEventData[2];
	//HANDLE hEventData;
	//HANDLE hEventCancel;
	HANDLE hFileMap;
	HANDLE hFileMem;
	
	/* Init console window */
    	AllocConsole();
    	SetConsoleTitle( "Server" );
    	system( "cls" );
    	
	// Create an auto-reset event object. The write thread sets this
	// object to the signaled state when it finishes writing to a 
	// shared buffer.
	
    	hEventData[0] = CreateEvent(
    		NULL,				// default security attributes
    		FALSE,				// auto-reset event
    		FALSE,				// initial state is nonsignaled
    		szEventDataName		// object name
    	);
    	
    	if ( hEventData[0] == NULL ) { 
    		MessageBox(0, "CreateEvent failed", &szEventDataName, MB_OK | MB_ICONERROR | MB_APPLMODAL); 
		//printf( "CreateEvent failed (%d)\n", GetLastError() );
		return;
	}
	
	hEventData[1] = CreateEvent(
    		NULL,				// default security attributes
    		FALSE,				// auto-reset event
    		FALSE,				// initial state is nonsignaled
    		szEventCancelName		// object name
    	);
    	
    	if ( hEventData[1] == NULL ) { 
		printf( "CreateEvent failed (%d)\n", GetLastError() );
		CloseHandle( hEventData[0] );
		return;
	}
	
	// Create second auto-reset event object.
	 /*hEventCancel = CreateEvent(
    		NULL,				// default security attributes
    		FALSE,				// auto-reset event
    		FALSE,				// initial state is nonsignaled
    		szEventCancelName	// object name
    	);
    	
    	if ( hEventCancel == NULL ) { 
		printf( "CreateEvent failed (%d)\n", GetLastError() );
		CloseHandle( hEventData );
		return;
	}*/
    	
    	hSemaphore = CreateSemaphore(
    		NULL, 					// default security attributes
    		MAX_SEM_COUNT,		// inital count for the semaphore object
    		MAX_SEM_COUNT,		// the maximum count for the semaphore object
    		szSemaphoreName
    	);
    	
    	if ( hSemaphore == NULL ) { 
		printf( "CreateSemaphore failed (%d)\n", GetLastError() );
		CloseHandle( hEventData[1] );
		CloseHandle( hEventData[0] );
		//CloseHandle( hEventCancel );
		//CloseHandle( hEventData );
		return;
	}
	
	hMutex = CreateMutex(
    		NULL,				// default security attributes
    		FALSE,				// non-owned
    		szMutexName			// object name
    	);
    	
    	if ( hMutex == NULL ) { 
		printf( "CreateMutex failed (%d)\n", GetLastError() );
		CloseHandle( hSemaphore );
		CloseHandle( hEventData[1] );
		CloseHandle( hEventData[0] );
		//CloseHandle( hEventCancel );
		//CloseHandle( hEventData );
		return;
	}
	
	
	/* Create memory-mapped file object */
	hFileMap = CreateFileMapping(
		INVALID_HANDLE_VALUE,		// use paging file
		NULL,						// default security
		PAGE_READWRITE,			// read/write access
		0,							// maximum object size (high-order DWORD)
		4096,						// maximum object size (low-order DWORD)
		szMMFName					// name of mapping object
	);
	
	if ( hFileMap == NULL ) {
		printf( "Could not create file mapping object (%d).\n", GetLastError() );
		CloseHandle( hMutex );
		CloseHandle( hSemaphore );
		//CloseHandle( hEventCancel );
		//CloseHandle( hEventData );
		CloseHandle( hEventData[1] );
		CloseHandle( hEventData[0] );
		return;
	}
	
	/* Map view of file */
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
		CloseHandle( hSemaphore );
		//CloseHandle( hEventCancel );
		//CloseHandle( hEventData );
		CloseHandle( hEventData[1] );
		CloseHandle( hEventData[0] );
		return;
	}
	
	// After modification this code will be deleted
	
	/* Initialize console window */
	/*
	GetStartupInfo(&startInfo);
	startInfo.dwFillAttribute = 0x1F;
	startInfo.dwX = 0;
	startInfo.dwY = 0;
	startInfo.dwXSize = 800;
	startInfo.dwYSize = 300;
	startInfo.dwFlags = STARTF_USEPOSITION | STARTF_USESIZE | STARTF_USEFILLATTRIBUTE;
	
	
	if ( !CreateProcess(
		NULL, 					// No module name (use command line)
		"MMFClient.exe",			// Command line
		NULL,					// Process handle not inheritable
		NULL,					// Thread handle not inheritable
		FALSE,					// Set handle inheritance to FALSE
		CREATE_NEW_CONSOLE,	// The new process has a new console, instead of inheriting its parent's console (the default)
		NULL,					// Use parent's environment block
		NULL,					// Use parent's starting directory
		&startInfo,				// Pointer to STARTUPINFO structure
		&processInfo				// Pointer to PROCESS_INFORMATION structure
	) ) {
		printf( "CreateProcess failed (%d).\n", GetLastError() );
		UnmapViewOfFile( hFileMem );
		CloseHandle( hFileMap );
	    	CloseHandle( hEventCancel );
	    	CloseHandle( hEventData );
		return;
	}
	
	CloseHandle( processInfo.hProcess ); //close handle of process
	*/
	
	while ( TRUE ) {
		/*int res = WaitForMultipleObjects( 2, hEventData, FALSE, INFINITE );
		if ( res - WAIT_OBJECT_0 == 1 ) break;
		printf( "%s\n", hFileMem );*/
		if ( WaitForMultipleObjects( 2, hEventData, FALSE, INFINITE ) - WAIT_OBJECT_0 == 1)  break;
		printf( "%s\n", hFileMem );
	}
	
	UnmapViewOfFile( hFileMem );
	CloseHandle( hFileMap );
	CloseHandle( hMutex );
	CloseHandle( hSemaphore );
	//CloseHandle( hEventCancel );
	//CloseHandle( hEventData );
    	CloseHandle( hEventData[0] );
    	CloseHandle( hEventData[1] );
    	
   	 return;
}