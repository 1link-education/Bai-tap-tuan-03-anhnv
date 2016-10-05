#include "studentController.h"
#include "studentIO.h"

#include <Windows.h>
#include <iostream>
#include <conio.h>

using namespace std;

CRITICAL_SECTION cs;

DWORD WINAPI monitor(LPVOID arg){
	EnterCriticalSection(&cs);
	while(1)
		WatchDirectory();
	LeaveCriticalSection(&cs);
	return 0;
}

int main()
{
	cout << "Anh Nguyen Viet - 3rd Week Pratices\n";
	cout << "-----------------------------------\n\n";

	//moi phien lam viec chi duoc thao tac voi 1 truong
	string initSchool;
	Schooling(initSchool);

	InitializeCriticalSection(&cs);
	HANDLE hThread = CreateThread(NULL, 0, monitor, NULL, 0, NULL);
	
	int n = 0;

	while(n < 32){
		cout << "\n\n	---MENU---\n";
		cout << " 1-add new student records\n";
		cout << " 2-show all student records\n";
		cout << " 3-update a student info\n";
		cout << " 4-delete a student record\n";
		cout << " 5-search for records\n";
		cout << " 6-do statistics\n";
		cout << " 0-exit program\n\n";

		int fearture;
		cout << "your choice: ";
		cin >> fearture;

		switch(fearture){
			case 1:
				cout << "\nnumber of records to add: ";
				cin >> n;
				add(n, initSchool);
				break;
			case 2:
				show(initSchool);
				break;
			case 3:
				update(initSchool);
				break;
			case 4:
				del(initSchool);
				break;
			case 5:
				search(initSchool);
				break;
			case 6:
				stat(initSchool);
				break;
			case 0:
				cout << "\nwill now exit program..";
				Sleep(3000);
				remove("student_search_result.txt");
				exit(0);
			default:
				cout << "invalid parameter. please choose one of following parameters..\n";
		}
	}

	WaitForSingleObject(hThread, INFINITE);
	DeleteCriticalSection(&cs);

	getch();
	return 0;
}