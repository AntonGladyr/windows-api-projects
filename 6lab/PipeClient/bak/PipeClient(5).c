#include "../interface.h"

void __cdecl main()
{
	/* Local variables */
	HANDLE hPipe;

	/* Init console window */
    	AllocConsole();
    	SetConsoleTitle( L"Client of named pipe" );
    	system( "cls" );
    	
    	printf( "Connecting . . ." );
    	hPipe = CreateFile(
    		"test", //szPipeName
    		GENERIC_READ | GENERIC_WRITE,
    		0,
    		NULL,
    		OPEN_EXISTING,
    		0,
    		NULL
    	);
    	
    	if ( hPipe == INVALID_HANDLE_VALUE ) {
    		printf( "\b\b\b\b\b\b\b\bon refused : (%d)\n", GetLastError() );
    		while(getchar() != '\n');
    	}
	
	CloseHandle( hPipe );
	ExitProcess( 0 );
}
