#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

	/* File handlers */
    	HANDLE hFile1;
    	HANDLE hFile2;
    	
    	/*Handlers for file mapping creation*/
    	HANDLE hMemFile1;
    	HANDLE hMemFile2;
    	
    	/*  Map view of files */
    	char* hMemView1;
    	char* hMemView2;
    	
    	/* Original file size */
    	DWORD fileSizeHigh;
    	DWORD fileSizeLow;
    	DWORD secondFileSize;
    	
void __cdecl main( int argc, char *argv[] )
{
    	AllocConsole();
    	SetConsoleTitle("Memory-mapped file");
    	system("cls");
    	
    	if (argc != 3) {
    		printf("You have to pass two file names.\n");
    		return;
    	}
    	
    	hFile1 = CreateFile(argv[1],                			// name of the write
                       			 GENERIC_READ,          		// open for writing
                       			 FILE_SHARE_READ | FILE_SHARE_WRITE,   // sharing
                       			 0,                   			// default security
                       			 OPEN_EXISTING,             		// create new file only
                       			 FILE_ATTRIBUTE_NORMAL,  // normal file
                       			 0);
                       			 
          if (hFile1 == INVALID_HANDLE_VALUE) { 
		printf("Unable to open file: %s.\n", argv[1]);
		return;
	}
	
	fileSizeLow = GetFileSize(hFile1, &fileSizeHigh); 
	if (fileSizeLow == INVALID_FILE_SIZE) {
		printf("Unable to get file size: %s.\n", argv[1]);
		CloseHandle(hFile1);
		return;
	}
	
	hMemFile1 = CreateFileMapping(hFile1, 0, PAGE_READONLY, 0, fileSizeLow, 0);  
	if (hMemFile1 == NULL) {
		printf("Unable to map on memory first file: %s.\n", argv[1]);
		CloseHandle(hFile1);
		return;
	}
	
	hMemView1 = (char*) MapViewOfFile(hMemFile1,			// handle to map object
									     FILE_MAP_READ,	// read/write permission
									     0,
									     0,
									     0);
	if (hMemView1 == NULL) {
		printf("Unable to map view of first file: %s.\n", argv[1]);
		CloseHandle(hMemFile1);
		CloseHandle(hFile1);
		return;
	}
	
	
	/* Handlers for second file */
	
	hFile2 = CreateFile(argv[2],		// open file to write
                       			 GENERIC_READ | GENERIC_WRITE,
                       			 FILE_SHARE_WRITE |  FILE_SHARE_READ,
                       			 0,
                       			 OPEN_ALWAYS,
                       			 FILE_ATTRIBUTE_NORMAL,
                       			 0); // if the file does not exist it will be created
                       			 
           if (hFile2 == INVALID_HANDLE_VALUE) { 
		printf("Unable to open file: %s.\n", argv[2]);
		UnmapViewOfFile(hMemView1);
		CloseHandle(hMemFile1);
		CloseHandle(hFile1);
		return;
	}
	
	secondFileSize = GetFileSize(hFile2, NULL); 
	if (secondFileSize == INVALID_FILE_SIZE) {
		printf("Unable to get size of second file: %s.\n", argv[2]);
		CloseHandle(hFile2);
		UnmapViewOfFile(hMemView1);
		CloseHandle(hMemFile1);
		CloseHandle(hFile1);
		return;
	}
	
	hMemFile2 = CreateFileMapping(hFile2, 0, PAGE_READWRITE, 0, fileSizeLow + secondFileSize, 0);  
	if (hMemFile2 == NULL) {
		printf("Unable to map on memory second file: %s.\n", argv[2]);
		CloseHandle(hFile2);
		UnmapViewOfFile(hMemView1);
		CloseHandle(hMemFile1);
		CloseHandle(hFile1);
		return;
	}
	
	// LPSTR - long pointer to string(Windows type)
	hMemView2 = (char*) MapViewOfFile(hMemFile2,			// handle to map object
									     FILE_MAP_WRITE,	// read/write permission 
									     0,
									     0,
									     0);  //fileSizeLow + secondFileSize
	if (hMemView2 == NULL) {
		printf("Unable to map view of second file: %s.\n", argv[2]);	
		CloseHandle(hMemFile2);
		CloseHandle(hFile2);
		UnmapViewOfFile(hMemView1);
		CloseHandle(hMemFile1);
		CloseHandle(hFile1);
		return;
	}
    	

    	/*while (fileSizeLow / 4096 != 0) {
    		memcpy(hMemView2, hMemView1, 4096);
    		fileSizeLow -= 4096;
    		hMemView2 += 4096;
    		hMemView1 += 4096;
    	}*/
    	memcpy(hMemView2 + secondFileSize, hMemView1, fileSizeLow);
    	//CopyMemory(hMemView2 + secondFileSize, hMemView1, fileSizeLow);
    	UnmapViewOfFile(hMemView2);
    	CloseHandle(hMemFile2);
	CloseHandle(hFile2);
	UnmapViewOfFile(hMemView1);
	CloseHandle(hMemFile1);
	CloseHandle(hFile1);
    	printf("\nPress <ENTER> to continue...");
    	while(getchar() != '\n');
   	 return;
}
