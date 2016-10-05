#include "studentIO.h"

#include <fstream>
#include <iostream>
#include <iomanip>
#include <iterator>
#include <Windows.h>
#include <Winbase.h>
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>

#define MAX_DIRS 25
#define MAX_FILES 255
#define MAX_BUFFER 4096

using namespace std;

// tim file theo ten truong de import file <ten truong>-x.INPUT moi duoc them vao directory
void findSchDB(char fileSchlKey[MAX_PATH], char addedFile[MAX_PATH]){
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	strcat(fileSchlKey, ".INPUT");						//maybe unsafe, while strcat_s leads E:2660, Mr. ToanTV and ManhTD can help? 
	hFind = FindFirstFile(fileSchlKey, &FindFileData);

	if (hFind != INVALID_HANDLE_VALUE) {	
		merge(FindFileData.cFileName, addedFile);
	}
}

// import file <ten truong>-x.IMPUT vao file chung cua truong
void merge(TCHAR schoolDB[MAX_PATH], TCHAR smallFiles[MAX_PATH]){
	//_splitpath

	Student st;
	int i;
	fstream inFile;
	fstream outFile;

	inFile.open(smallFiles,ios::binary|ios::in);
	outFile.open(schoolDB,ios::binary|ios::out|ios::app);

	while(inFile.read(reinterpret_cast<char *> (&st), sizeof(Student)))
		outFile.write(reinterpret_cast<char *> (&st), sizeof(Student));

	outFile.close();
	inFile.close();
}


extern "C" {
	WINBASEAPI BOOL WINAPI
		ReadDirectoryChangesW( HANDLE hDirectory,
		LPVOID lpBuffer, DWORD nBufferLength,
		BOOL bWatchSubtree, DWORD dwNotifyFilter,
		LPDWORD lpBytesReturned,
		LPOVERLAPPED lpOverlapped,
		LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
		);
}

// chua thong tin thu muc, cung cap bo dem chua thong tin thay doi file
typedef struct _DIRECTORY_INFO {
	HANDLE hDir;
	TCHAR lpszDirName[MAX_PATH];
	CHAR lpBuffer[MAX_BUFFER];
	DWORD dwBufLength;
	OVERLAPPED Overlapped;
} DIRECTORY_INFO, *PDIRECTORY_INFO, *LPDIRECTORY_INFO;

DIRECTORY_INFO DirInfo[MAX_DIRS];						// buffer cho tat ca thu muc
TCHAR FileList[MAX_FILES*MAX_PATH];						// buffer cho tat ca file
DWORD numDirs;

// call on a separate thread to avoid blocking the main thread.  
void WatchDirectory() {
	TCHAR path[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, path);
	//strcat(path, "DIEM_THI_2016");			// go with mkDir(), remember that!

	char buf[2048];
	DWORD nRet;
	BOOL result = TRUE;
	char filename[MAX_PATH];
	DirInfo[0].hDir = CreateFile (path, GENERIC_READ|FILE_LIST_DIRECTORY, 
		FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,
		NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS|FILE_FLAG_OVERLAPPED,
		NULL);

	if(DirInfo[0].hDir == INVALID_HANDLE_VALUE) {
		//return;
		cout << "dir not found :-)\n";
	}

	lstrcpy( DirInfo[0].lpszDirName, path);
	OVERLAPPED PollingOverlap;

	FILE_NOTIFY_INFORMATION* pNotify;
	int offset;
	PollingOverlap.OffsetHigh = 0;
	PollingOverlap.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	while(result) {
		result = ReadDirectoryChangesW(
			DirInfo[0].hDir,											// handle to the directory to be watched
			&buf,														// pointer to the buffer to receive the read results
			sizeof(buf),												// length of lpBuffer
			TRUE,														// flag for monitoring directory or directory tree
			FILE_NOTIFY_CHANGE_FILE_NAME |
			FILE_NOTIFY_CHANGE_DIR_NAME |
			FILE_NOTIFY_CHANGE_SIZE,
			//FILE_NOTIFY_CHANGE_LAST_WRITE |
			//FILE_NOTIFY_CHANGE_LAST_ACCESS |
			//FILE_NOTIFY_CHANGE_CREATION,
			&nRet,// number of bytes returned
			&PollingOverlap,											// pointer to structure needed for overlapped I/O
			NULL);

		WaitForSingleObject(PollingOverlap.hEvent,INFINITE);
		offset = 0;
		int rename = 0;
		char oldName[260];
		char newName[260];
		size_t found;
		do {
			pNotify = (FILE_NOTIFY_INFORMATION*)((char*)buf + offset);
			strcpy(filename, "");
			int filenamelen = WideCharToMultiByte(CP_ACP, 0, pNotify->FileName, pNotify->FileNameLength/2, filename, sizeof(filename), NULL, NULL);
			filename[pNotify->FileNameLength/2] = '\0';
			string str(filename);
			switch(pNotify->Action) {
				case FILE_ACTION_ADDED:
					//cout << "\nthe file is added to the directory: "<< filname << "\n";
					char extractName[MAX_PATH];
					//found = filename.find_first_of("-");
					found = str.find("-"); 
					int i;
					for(i = 0; i < found; i++)
						extractName[i] = filename[i];
					findSchDB(extractName, filename);
					break;
				/*case FILE_ACTION_MODIFIED:
					printf("\nThe file is modified. This can be a change in the time stamp or attributes: [%s]\n", filename);
					break;*/
				default:
					break;
			}
			offset += pNotify->NextEntryOffset;
		} while(pNotify->NextEntryOffset);												//(offset != 0);
	}

	CloseHandle( DirInfo[0].hDir );
	//return 0;
}

// moi khi phat hien 1 truong moi se mkFile luu toan bo ban ghi cua truong do
void mkFile(string inpSchl) {
	//HANDLE hThread = CreateThread(NULL, 0, WatchDirectory, NULL, 0, NULL);
	char fileSCHOOL[MAX_PATH];
	const char * constSchl = inpSchl.c_str();

	GetCurrentDirectory(MAX_PATH, fileSCHOOL);
	strcat_s(fileSCHOOL, _TRUNCATE, constSchl);
	strcat_s(fileSCHOOL, _TRUNCATE, ".INPUT");

	HANDLE hFile = INVALID_HANDLE_VALUE;
	hFile = CreateFile(fileSCHOOL,							// file name 
						GENERIC_READ,						// open for reading 
						0,									// do not share 
						NULL,								// default security 
						OPEN_EXISTING,						// existing file only 
						FILE_ATTRIBUTE_NORMAL,				// normal file 
						NULL);     
	//WaitForSingleObject(hThread, INFINITE);
	
	//fstream schFile;
	//schFile.open(fileSCHOOL,ios::binary|ios::in/*|ios::out*/);
	//schFile.close();
}

void writeFile(int n, Student st[]){
	fstream f;
	int i;

	f.open("student_search_result.txt", ios::out|ios::app);
	f << setw(6) << "id" << setw(30) << "name" << setw(30) << "address" << setw(9) << "sex" << setw(9) << "math" << setw(12) << "physics" << setw(16) << "chemistry\n\n";
	for(i = 1; i <= n; i++){
		f << setw(6) << st[i].getID()<< setw(30) << st[i].getName( )<< setw(30) << st[i].getAdd() << setw(9) << st[i].getSex() << setw(9) << st[i].getMath() << setw(12) << st[i].getPhys() << setw(14) << st[i].getChem() << endl;
	}
	f.close();
}

void readFile(Student st[])
{
	fstream f;
	f.open("studentin.txt", ios::in);
	int i = 1;

	while(!f.eof()) {
		cout << "\nstudent number " << i << "\n----------------\n";
		cout << "id: "; f >> st[i].id; cout << st[i].id << endl;
		cout << "name: "; f >> st[i].name; cout << st[i].name << endl;
		cout << "address: "; f >> st[i].address; cout << st[i].address << endl;
		cout << "sex: "; f >> st[i].sex;
		if(st[i].sex.compare("m") == 0)
			cout << "male\n";
		else
			cout << "female\n";
		cout << "math: "; f >> st[i].math; cout << st[i].math << endl;
		cout << "physics: "; f >> st[i].phys; cout << st[i].phys << endl;
		cout << "chemistry: "; f >> st[i].chem; cout << st[i].chem << endl << endl;
		i++;
	}

	f.close();
}

