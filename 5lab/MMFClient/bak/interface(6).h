#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

/* names of the event objects */
char szEventDataName[] = "$EventDataName";
char szEventCancelName[] = "$EventCancelName";
/* the name of the memory-mapped file */
char szMMFName[] = "$InterprocessCommunicationMMF$";
/* names of mutex and semaphore objects */
char szSemaphoreName[] = "$SemaphoreName";
char szMutexName[] = "$MutexName";

/* HANDLES */
HANDLE hSemaphore;
HANDLE hEventData;
HANDLE hEventCancel;
HANDLE hFileMap;
HANDLE hFileMem;