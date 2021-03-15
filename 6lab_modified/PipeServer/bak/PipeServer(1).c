#include "../interface.h"

#define BUFSIZE 512
#define BAD_COMMAND_TEXT "Bad command\n"
#define EXIT_COMMAND_TEXT "exit"

VOID  InstanceThread();

CRITICAL_SECTION crtiticalSection;

void __cdecl wmain()
{	
	HANDLE threadHandleArray[3];
	DWORD threadIDArray[3];
	char i = 0;
	
	InitializeCriticalSection( &crtiticalSection );
	
	/* Init console window */
    	AllocConsole();
    	SetConsoleTitle( L"Server of named pipe" );
    	system( "cls" );
    	
    	for ( i = 0; i < 3; i++ )
    	{
    		threadHandleArray[i] = CreateThread(
    			NULL,					// no security attribute
    			0,						// default stack size
    			InstanceThread,			// thread proc
    			0,						// thread parameter
    			0,						// not suspended
    			threadIDArray[i]			// returns thread ID
    		);
    		
    		if ( threadHandleArray[i] == NULL ) {
    			printf( "CreateThread failed, GLE=%d.\n", GetLastError() );
    			return;
    		}
    	}
	
	while ( TRUE ) {
		int index = WaitForMultipleObjects( 3,  threadHandleArray, FALSE, INFINITE ) - WAIT_OBJECT_0;
		threadHandleArray[index] = CreateThread(
    			NULL,					// no security attribute
    			0,						// default stack size
    			InstanceThread,			// thread proc
    			0,						// thread parameter
    			0,						// not suspended
    			threadIDArray[index]		// returns thread ID
    		);
    		if ( threadHandleArray[index] == NULL ) {
			printf( "CreateThread failed, GLE=%d.\n", GetLastError() );
		}
		
		Sleep(100);
	}
	
	DeleteCriticalSection( &crtiticalSection );
	
	return;
}


VOID  InstanceThread() {
	HANDLE hPipe;
	DWORD nSendReciveBytes;
	SYSTEMTIME dateTimeInfo;	
	char inputBuffer[512];
	char outputBuffer[512];
	
	
	hPipe = CreateNamedPipe(
		szPipeName,						// pipe name
		PIPE_ACCESS_DUPLEX,			// read/write access
		PIPE_TYPE_MESSAGE | PIPE_WAIT,	// message type pipe; blocking mode
		PIPE_UNLIMITED_INSTANCES,		// max. instances
		BUFSIZE,							// output buffer size
		BUFSIZE,							// input buffer size
		0,								// client time-out
		NULL							// default security attribute
	);
	
	if ( hPipe == INVALID_HANDLE_VALUE ) {
		printf( "Unable to create named pipe (%d)\n", GetLastError() );
		printf( "\nPress <ENTER> to continue..." );
    		while( getchar() != '\n' );
		return;
	}
	
	printf( "Connecting . . .\n" );
	ConnectNamedPipe( hPipe, NULL );
	printf( "Connection is established\n" );
	
	while( TRUE ) {
	
		// Read client requests from the pipe. This simplistic code only allows messages
   		// up to BUFSIZE characters in length.
   		ReadFile(
   			hPipe,				// handle to pipe
   			inputBuffer,			// buffer to receive data
   			BUFSIZE,				// size of buffer
   			&nSendReciveBytes,	// number of bytes read
   			NULL				// not overlapped I/O
   		);
   		printf("%s\n", inputBuffer);
   		
   		GetLocalTime( &dateTimeInfo );
		if ( strcmp( inputBuffer, "exit" ) == 0 ) {
	 		break;
		}
		
		else if ( strcmp( inputBuffer, "date" ) == 0 ) {
			sprintf( outputBuffer, "%02d/%02d/%04d\n",
						dateTimeInfo.wDay,
	 					dateTimeInfo.wMonth,
	 					dateTimeInfo.wYear
	 			);
	 			EnterCriticalSection( &crtiticalSection );
	 			WriteFile(
	 				hPipe,				// handle to pipe 
	 				outputBuffer,			// buffer to write from
	 				BUFSIZE,				// number of bytes to write
	 				&nSendReciveBytes,	// number of bytes written
	 				NULL				// not overlapped I/O
	 			);
	 			 LeaveCriticalSection( &crtiticalSection );
		}
		
		else if ( strcmp( inputBuffer, "time" ) == 0 ) {
			sprintf( outputBuffer, "%02d:%02d:%02d\n",
					dateTimeInfo.wHour,
	 					dateTimeInfo.wMinute,
	 					dateTimeInfo.wSecond
	 			);
	 			EnterCriticalSection( &crtiticalSection );
	 			WriteFile(
	 				hPipe,				// handle to pipe 
	 				outputBuffer,			// buffer to write from
	 				BUFSIZE,				// number of bytes to write
	 				&nSendReciveBytes,	// number of bytes written
	 				NULL				// not overlapped I/O
	 			);
	 			LeaveCriticalSection( &crtiticalSection );
		}
		
		else if ( strcmp( inputBuffer, "help" ) == 0 ) {
			sprintf( outputBuffer, "%s %s %s %s %s\n",
					"\nThere are available next commands:\n",
					"	date	- show current local date\n",
					"	time	- show current local time\n",
					"	help	- show all available commands\n",
					"	exit	- close program"
	 			);
	 		EnterCriticalSection( &crtiticalSection );
			WriteFile(
	 				hPipe,				// handle to pipe 
	 				outputBuffer,			// buffer to write from
	 				BUFSIZE,				// number of bytes to write
	 				&nSendReciveBytes,	// number of bytes written
	 				NULL				// not overlapped I/O
	 			);
	 		LeaveCriticalSection( &crtiticalSection );
		}
		
		else {
			EnterCriticalSection( &crtiticalSection );
			WriteFile(
	 				hPipe,						// handle to pipe 
	 				BAD_COMMAND_TEXT,			// buffer to write from
	 				sizeof( BAD_COMMAND_TEXT ),	// number of bytes to write
	 				&nSendReciveBytes,			// number of bytes written
	 				NULL						// not overlapped I/O
	 			);
	 		LeaveCriticalSection( &crtiticalSection )
   		}
	}
	
	
	EnterCriticalSection( &crtiticalSection );
	WriteFile(
	 				hPipe,						// handle to pipe 
	 				EXIT_COMMAND_TEXT,			// buffer to write from
	 				sizeof( EXIT_COMMAND_TEXT ),	// number of bytes to write
	 				&nSendReciveBytes,			// number of bytes written
	 				NULL						// not overlapped I/O
	 			);
	 LeaveCriticalSection( &crtiticalSection );
	 
	CloseHandle( hPipe );
	ExitThread( 0 );
	return;
}