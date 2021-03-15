#include "../interface.h"

#define BUFSIZE 512
#define BAD_COMMAND_TEXT "Bad command\n"
#define EXIT_COMMAND_TEXT "exit\n"


void __cdecl wmain()
{
	/* Local variables */
	HANDLE hPipe;
	DWORD nSendReciveBytes;
	SYSTEMTIME dateTimeInfo;	
	char inputBuffer[512];
	char outputBuffer[512];
	
	/* Init console window */
    	AllocConsole();
    	SetConsoleTitle( L"Server of named pipe" );
    	system( "cls" );
	
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
	
	printf( "Connecting . . ." );
	ConnectNamedPipe( hPipe, NULL );
	printf( "\b\b\b\b\bis established\n" );
	
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
   		printf("%s %d", inputBuffer, sizeof( inputBuffer ));
   		
   		GetLocalTime( &dateTimeInfo );
		if ( strcmp( inputBuffer, "exit" ) == 0 ) {
			WriteFile(
	 				hPipe,						// handle to pipe 
	 				EXIT_COMMAND_TEXT,			// buffer to write from
	 				sizeof( EXIT_COMMAND_TEXT ),	// number of bytes to write
	 				&nSendReciveBytes,			// number of bytes written
	 				NULL						// not overlapped I/O
	 			);
	 		break;
		}
		
		else if ( strcmp( inputBuffer, "date" ) == 0 ) {
			sprintf( outputBuffer, "%02d/%02d/%04d",
					dateTimeInfo.wDay,
	 					dateTimeInfo.wMonth,
	 					dateTimeInfo.wYear
	 			);
	 			WriteFile(
	 				hPipe,				// handle to pipe 
	 				outputBuffer,			// buffer to write from
	 				BUFSIZE,				// number of bytes to write
	 				&nSendReciveBytes,	// number of bytes written
	 				NULL				// not overlapped I/O
	 			);
		}
		
		else if ( strcmp( inputBuffer, "time" ) == 0 ) {
			sprintf( outputBuffer, "%02d:%02d:%02d",
					dateTimeInfo.wHour,
	 					dateTimeInfo.wMinute,
	 					dateTimeInfo.wSecond
	 			);
	 			WriteFile(
	 				hPipe,				// handle to pipe 
	 				outputBuffer,			// buffer to write from
	 				BUFSIZE,				// number of bytes to write
	 				&nSendReciveBytes,	// number of bytes written
	 				NULL				// not overlapped I/O
	 			);
		}
		
		/*else if ( strcmp( inputBuffer, "help" ) ) {
			
		}*/
		
		else {
			WriteFile(
	 				hPipe,						// handle to pipe 
	 				BAD_COMMAND_TEXT,			// buffer to write from
	 				sizeof( BAD_COMMAND_TEXT ),	// number of bytes to write
	 				&nSendReciveBytes,			// number of bytes written
	 				NULL						// not overlapped I/O
	 			);
   		}
	}
	
	CloseHandle( hPipe );
	ExitProcess(0);
}
