#include <stdio.h>
#include <stdlib.h>
#include <windows.h>


// Pass "-h" paramter for displaying only hidden files.

//****  Declare global variables ****//
	
	// POSSIBLE ATTRIBUTES:  -nDadshr
	/*
		n - FILE_ATTRIBUTE_NORMAL
		D - FILE_ATTRIBUTE_DEVICE
		a - FILE_ATTRIBUTE_ARCHIVE
		d - FILE_ATTRIBUTE_DIRECTORY
		s - FILE_ATTRIBUTE_SYSTEM
		h - FILE_ATTRIBUTE_HIDDEN
		r - FILE_ATTRIBUTE_READONLY
	*/
    	char fileAttr[] = "-------- ";
    	HANDLE hFindNextFile;
    	WIN32_FIND_DATA dirInfo;
    	/****
    		WIN32_FIND_DATA STRUCT
		dwFileAttributes		DWORD ? ;
		ftCreationTime		FILETIME <> ;
		ftLastAccessTime		FILETIME <> ;
		ftLastWriteTime		FILETIME <> ;
		nFileSizeHigh			DWORD ? ;
		nFileSizeLow			DWORD ? ;
		dwReserved0			DWORD ? ;
		dwReserved1			DWORD ? ;
		cFileName			CHAR MAX_PATH dup (?);
		cAlternateFileName	CHAR 14 dup (?);
		WIN32_FIND_DATA ENDS
	****/
	
	/****
    	 	Private Const FILE_ATTRIBUTE_READONLY = 1
		Private Const FILE_ATTRIBUTE_HIDDEN = 2
		Private Const FILE_ATTRIBUTE_SYSTEM = 4
		Private Const FILE_ATTRIBUTE_DIRECTORY = 16
		Private Const FILE_ATTRIBUTE_ARCHIVE = 32
		Private Const FILE_ATTRIBUTE_DEVICE = 64
		Private Const FILE_ATTRIBUTE_NORMAL = 128
		Private Const FILE_ATTRIBUTE_TEMPORARY = 256
		Private Const FILE_ATTRIBUTE_SPARSE_FILE = 512
		Private Const FILE_ATTRIBUTE_REPARSE_POINT = 1024
		Private Const FILE_ATTRIBUTE_COMPRESSED = 2048
		Private Const FILE_ATTRIBUTE_OFFLINE = 4096
		Private Const FILE_ATTRIBUTE_NOT_CONTENT_INDEXED = 8192
		Private Const FILE_ATTRIBUTE_ENCRYPTED = 16384 
		Private Const FILE_ATTRIBUTE_INTEGRITY_STREAM = 32768 
		Private Const FILE_ATTRIBUTE_VIRTUAL = 65536 
    	 ****/
    	 
   	 SYSTEMTIME dateTimeInfo;	
   	 /*SYSTEMTIME STRUCT
			    wYear			WORD ?
			    wMonth		WORD ?
			    wDayOfWeek	WORD ?
			    wDay			WORD ?
			    wHour			WORD ?
			    wMinute		WORD ?
			    wSecond		WORD ?
			    wMilliseconds	WORD ?
			;SYSTEMTIME ENDS
   	 */
   	 char isOnlyHidden = 0;
   	 
void __cdecl main( int argc, char *argv[] )
{
	char buffer[2*MAX_PATH];
    	AllocConsole();
    	SetConsoleTitle("Directory info");
    	system("cls");
    	
    	if (argc == 1) {
    		sprintf(buffer, "*.*");
    	}
    	else if(argc == 3 && strcmp(argv[2], "-h") == 0){
    		sprintf(buffer, "%s\\*.*", argv[1]);
    		isOnlyHidden = 1;
    	} else {
    		sprintf(buffer, "%s\\*.*", argv[1]);	
    	}
    	
    	if ((hFindNextFile = FindFirstFile(buffer, &dirInfo)) == INVALID_HANDLE_VALUE) {
    		printf("%s has not be found.\n", buffer);
    		return;
    	}
    	
    	 do {
    	 		//Check for third paramter (only hidden files)
    	 		if (isOnlyHidden && !(dirInfo.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)) {
    	 			continue;
    	 		}
    	 		
    	 		if (dirInfo.dwFileAttributes & FILE_ATTRIBUTE_NORMAL) {
    	 			fileAttr[1] = 'n';
    	 		}
    	 		if (dirInfo.dwFileAttributes & FILE_ATTRIBUTE_ENCRYPTED) {
    	 			fileAttr[2] = 'D';	
    	 		}
    	 		if (dirInfo.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE) {
    	 			fileAttr[3] = 'a';	
    	 		}
    	 		if (dirInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
    	 			fileAttr[4] = 'd';	
    	 		}
    	 		if (dirInfo.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) {
    	 			fileAttr[5] = 's';	
    	 		}
    	 		if (dirInfo.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) {
    	 			fileAttr[6] = 'h';	
    	 		}
    	 		if (dirInfo.dwFileAttributes & FILE_ATTRIBUTE_READONLY) {
    	 			fileAttr[7] = 'r';	
    	 		}
    	 		
			FileTimeToSystemTime(&dirInfo.ftCreationTime, &dateTimeInfo);
			printf("%s", fileAttr); //print file attributes
    	 		printf("%02d/%02d/%04d %02d:%02d:%02d ", 
    	 				dateTimeInfo.wDay,
    	 				dateTimeInfo.wMonth,
    	 				dateTimeInfo.wYear,
    	 				dateTimeInfo.wHour,
    	 				dateTimeInfo.wMinute,
    	 				dateTimeInfo.wSecond
    	 		); // print datetime
    	 		printf("%10u ", dirInfo.nFileSizeLow);
    	 		printf("%s \n", dirInfo.cFileName);// print fileNmae
    	 		sprintf(fileAttr, "-------- ");
    	 		
    	} while(FindNextFile(hFindNextFile, &dirInfo));
    	
    	//Close handle
    	FindClose(hFindNextFile);
    	
    	printf("\nPress <ENTER> to continue...");
    	while(getchar() != '\n');
   	 return;
}
