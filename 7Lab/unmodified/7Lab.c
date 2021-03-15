#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include<Commctrl.h>
#include<Shellapi.h>
#include<string.h>

/* ID CONSTANTS */
#define	IDD_DIALOG			100
#define 	IDM_MENU			1000
#define 	IDM_FILE_EXIT		1001
#define 	IDM_HELP_ABOUT		1002
#define	IDD_MAINDIALOG		100
#define	IDC_IP				101
#define	IDC_OKBTN 			102
#define	IDC_CANCELBTN		103
#define	IDC_RESULTEDIT		104

#define	szClassName			"DLGCLASS"
#define	szApplicationName		"Ping GUI"
#define	szAboutMessage		"Ping GUI\nCopyright ©  ≤— ‘≈ ≤  Õ”"

LRESULT CALLBACK WndProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

int CALLBACK WinMain( HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow )
{
 	WNDCLASSEX wc;
	MSG msg;
	HWND hWnd;
        	HICON hi;
        
	wc.cbSize = sizeof( WNDCLASSEX );
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = DLGWINDOWEXTRA;
	wc.hInstance = hInstance;
	wc.hbrBackground = ( HBRUSH ) ( COLOR_BTNFACE + 1 );
	wc.lpszMenuName = MAKEINTRESOURCE( IDM_MENU );
	wc.lpszClassName = szClassName;
	hi = LoadIcon( NULL, IDI_APPLICATION );
	wc.hIcon = hi;
	wc.hIconSm = hi;
	wc.hCursor = LoadCursor( NULL, IDC_ARROW );
	RegisterClassEx( &wc );
	hWnd = CreateDialogParam( hInstance, MAKEINTRESOURCE( IDD_DIALOG ), 0, WndProc, 0 );
	ShowWindow ( hWnd, SW_SHOWNORMAL );
	UpdateWindow ( hWnd );
	while ( TRUE ) {
		if ( !GetMessage( &msg, 0, 0, 0 ) ) break;
		TranslateMessage(&msg); 
            	DispatchMessage(&msg);
	}
	
       return 0;
}

LRESULT CALLBACK WndProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	SECURITY_ATTRIBUTES Security;
	HANDLE phRead;
	HANDLE phWrite;
	char stdOutBuffer[4096];
	char OutputBuffer[ sizeof( stdOutBuffer ) ];
	STARTUPINFO StartInfo;
	PROCESS_INFORMATION ProcessInfo;
	char szExecute[65] = "cmd.exe /c c:\\windows\\system32\\ping.exe ";
	char IP[18];
	DWORD nBytesTransfer;
	
	switch ( uMsg ) {
	case WM_COMMAND:
		switch ( LOWORD(wParam) ) {
			case IDM_FILE_EXIT:
				 SendMessage( hwnd, WM_CLOSE, 0, 0 );
				 break;
			case IDM_HELP_ABOUT:
				ShellAbout( hwnd, szApplicationName, szAboutMessage, NULL );
				break;
			case IDC_CANCELBTN:
				SendMessage( hwnd, WM_CLOSE, 0, 0 );
				break;
			case IDC_OKBTN:
				Security.lpSecurityDescriptor = NULL;
				Security.bInheritHandle = TRUE;
				CreatePipe( &phRead, &phWrite, &Security, sizeof( stdOutBuffer ) );
				GetStartupInfo( &StartInfo );
				StartInfo.hStdOutput = phWrite;
				StartInfo.hStdError = phWrite;
				StartInfo.wShowWindow = SW_HIDE;
				StartInfo.dwFlags = STARTF_USESHOWWINDOW + STARTF_USESTDHANDLES;
				GetDlgItemText( hwnd, IDC_IP, IP, sizeof( IP ) );
				strcat( szExecute, IP );
				CreateProcess(
						NULL,						// No module name (use command line)
						szExecute,					// Command line
						NULL,						// Process handle not inheritable
						NULL,						// Thread handle not inheritable
						TRUE,						// Set handle inheritance to TRUE
						CREATE_NEW_CONSOLE,		// The new process has a new console, instead of inheriting its parent's console (the default)
						NULL,						// Use parent's environment block
						NULL,						// Use parent's starting directory
						&StartInfo,					// Pointer to STARTUPINFO structure
						&ProcessInfo				// Pointer to PROCESS_INFORMATION structure
				);
				WaitForSingleObject( ProcessInfo.hProcess, INFINITE );
				ReadFile( phRead, &stdOutBuffer, sizeof( stdOutBuffer ), &nBytesTransfer, NULL );
				OemToChar( stdOutBuffer, OutputBuffer );
				
				SetDlgItemText( hwnd, IDC_RESULTEDIT, OutputBuffer );
				break;
		}
		break;
	case WM_CLOSE:
		DestroyWindow( hwnd );
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc( hwnd, uMsg, wParam, lParam );
	}
	
	return 0;
}
