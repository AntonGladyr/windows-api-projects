#include "../interface.h"

#define BUFSIZE 512

void __cdecl main()
{
	/* Local variables */
	HANDLE hPipe;
	DWORD nSendReciveBytes;
	char inputBuffer[512];
	char outputBuffer[512];

	/* Init console window */
    	AllocConsole();
    	SetConsoleTitle( "Client of named pipe" );
    	system( "cls" );
    	
    	printf( "Connecting . . ." );
    	hPipe = CreateFile(
    		szPipeName,
    		GENERIC_READ | GENERIC_WRITE,
    		0,
    		NULL,
    		OPEN_EXISTING,
    		0,
    		NULL
    	);
    	
    	if ( hPipe == INVALID_HANDLE_VALUE ) {
    		printf( "\b\b\b\b\b\b\b\bon refused : (%d)\n", GetLastError() );
    		return;
    	}
	
	printf( "\b\b\b\b\b\b\b\bon is established\n" );
	
	while ( TRUE ) {
		printf("Command$ ");
		scanf("%s", outputBuffer);
		WriteFile(
			hPipe,				// handle to pipe
			outputBuffer,			// buffer to write from
			strlen( outputBuffer ),	// number of bytes to write
			&nSendReciveBytes,	// number of bytes written
			NULL				// not overlapped I/O
		);
		ReadFile(
			hPipe,				// handle to pipe
			inputBuffer,			// buffer to receive data
			BUFSIZE,				// size of buffer
			&nSendReciveBytes,	// number of bytes read
			NULL				// not overlapped I/O
		);
		
		printf( "%s",  inputBuffer );
		if ( strcmp( inputBuffer, "exit" ) == 0 ) break;
	}
	
	CloseHandle( hPipe );
	ExitProcess( 0 );
}
