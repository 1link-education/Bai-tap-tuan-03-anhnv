#include "studentController.h"
#include "studentIO.h"
#include "student.h"

#include <Windows.h>
#include <iostream>
#include <string>
#include <conio.h>
#include <iomanip>
#include <fstream>
#include <stdlib.h>

#define MAXSTD 32
#define MAXPATH 255

using namespace std;

// tao thu muc DIEM_THI_2016 va mot so thu muc temp
void mkDir() {
	/*CreateDirectory("C:\\Users\\Public\\DIEM_THI_2016", NULL);
	CreateDirectory("C:\\Users\\Public\\DT2016_temp", NULL);*/
	CreateDirectory("DIEM_THI_2016", NULL);
	CreateDirectory("DT2016_temp", NULL);
}

// lay thong tin truong de thao tac
void Schooling(string inSchool)
{
	
	cout << "\nenter school name to continue: ";
	fflush(stdin);
	getline(cin, inSchool);
	while(!checkString(inSchool)) {
		cout << "'" << inSchool << "' is not a valid school name. please re-enter school name\n";
		cout << "school name: ";
		fflush(stdin);
		getline(cin, inSchool);                         
	}
	mkFile(inSchool);							// check if entered school already has database?
}

// kiem tra trung ma thi sinh
bool checkID(string s, int i, Student st[])
{
	bool flag = true;
	int j;

	for(j = 1; j <= i-1; j++){
		if(st[j].getID().compare(s) == 0){
			flag = false;
		}
	}

	if(flag == false){
		cout << "\n\n hmm.. :-(\n it seem like the student id you've entered is already in the record list\n please try another\n\n";
	}

	return flag;
}

// them ban ghi tu ban phim
void addFK(int n, string schoolName)
{
	Student st[MAXSTD];
	ofstream outFile;

	char fileSCHOOL[MAX_PATH];
	const char * constSchl = schoolName.c_str();

	GetCurrentDirectory(MAX_PATH, fileSCHOOL);
	strcat_s(fileSCHOOL, constSchl);
	strcat_s(fileSCHOOL, "-001.INPUT");

	outFile.open(fileSCHOOL,ios::binary|ios::out);

	int i;
	for(i = 1; i <= n; i++){
		cout << "\nstudent number " << i << "\n----------------\n\n";
		st[i].importStd();
		while(!checkID(st[i].getID(), i, st)) {
			cout << "\nstudent number " << i << "\n----------------\n";
			st[i].importStd();
		}
		outFile.write(reinterpret_cast<char *> (&st[i]), sizeof(Student));
	}
	outFile.close();
	cout << "\n\nstudent record has been created successfully\n";
}

// them ban ghi tu file studentIN.txt
void addFF(int n, string schoolName)
{
	Student st;
	int i;
	fstream inFile;
	fstream outFile;

	char fileSCHOOL[MAX_PATH];
	const char * constSchl = schoolName.c_str();
	strcat_s(fileSCHOOL, constSchl);
	strcat_s(fileSCHOOL, "-001.INPUT");

	outFile.open(fileSCHOOL,ios::binary|ios::out);

	inFile.open("studentIN.txt",ios::binary|ios::in);
	if(!inFile)
	{
		cout << "file 'studentIN.txt' could not be open. press any key...";
		cin.ignore();
		cin.get();
		return;
	}
	if(!outFile)
	{
		cout << "file '" << fileSCHOOL << "' could not be open. press any key...";
		cin.ignore();
		cin.get();
		return;
	}
	
	for(i = 1; i <= n; i++){
		inFile.read(reinterpret_cast<char *> (&st), sizeof(Student));
		outFile.write(reinterpret_cast<char *> (&st), sizeof(Student));
	}
	outFile.close();
	inFile.close();
	cout << "\n" << n << " students successfully imported from studentin.txt to" << fileSCHOOL << endl;
	cin.get();
}

// showAdd()
void add(int n, string schoolName)
{
	char addOpt;

	cout << "press 'k' to add records from keyboard or 'f' to add from file: ";
	cin >> addOpt;

	switch(addOpt) {
		case 'k':
			addFK(n, schoolName);
			break;
		case 'f':
			addFF(n, schoolName);
			break;
		default:
			cout << "\ninvalid parameter. please choose [k/f] to add record from keyboard/file..\n";
	}

	cout << "press any key to continue..\n";
	cin.get();
}
 // hien thi toan bo cac ban ghi cua truong dang thao tac
void show(string schoolName)
{
	char fileSCHOOL[MAX_PATH];
	const char * constSchl = schoolName.c_str();
	strcat_s(fileSCHOOL, constSchl);
	strcat_s(fileSCHOOL, ".INPUT");
	
	Student st;
	ifstream inFile;
	inFile.open(fileSCHOOL, ios::in|ios::binary);
	if(!inFile){
		cout << "file '" << fileSCHOOL << "' could not be open. press any key...";
		cin.ignore();
		cin.get();
		return;
	}
	cout << "\n\n\t\t---show all records---\n\n";
	int stdnumber = 1;
	while(inFile.read(reinterpret_cast<char *> (&st), sizeof(Student))) {
		cout << "student number " << stdnumber << "\n----------------\n\n";
		st.exportStd();
		cout << "\n";
		stdnumber++;
	}
	inFile.close();
	cin.get();
}

// sua thong tin 1 ban ghi
void update(string schoolName)
{
	char fileSCHOOL[MAX_PATH];
	const char * constSchl = schoolName.c_str();
	strcat_s(fileSCHOOL, constSchl);
	strcat_s(fileSCHOOL, ".INPUT");
	
	bool found=false;
	Student st;
	fstream File;
	File.open(fileSCHOOL,ios::binary|ios::in|ios::out);
	if(!File)
	{
		cout << "file '" << fileSCHOOL << "' could not be open. press any key...";
		cin.ignore();
		cin.get();
		return;
	}

	string s;
	cout << "\nenter the id you want to update info: ";
	fflush(stdin);
	getline(cin,s);
	while(!File.eof() && found==false)
	{

		File.read(reinterpret_cast<char *> (&st), sizeof(Student));
		if(st.getID().compare(s) == 0)
		{
			st.exportStd();
			cout<<"\n\nupdate student's info\n";
			st.importStd();
			int pos=(-1)*static_cast<int>(sizeof(st));
			File.seekp(pos,ios::cur);
			File.write(reinterpret_cast<char *> (&st), sizeof(Student));
			cout<<"\n\t record has been succuessfully upadated";
			found=true;
		}
	}
	File.close();
	if(found==false)
		cout << "\nrecord not found\n";
	cin.ignore();
	cin.get();
}

// xoa 1 ban ghi - tao temp.INPUT la clone cua database, ngoai tru ban ghi can xoa 
// -> xoa database sau do doi ten temp->databse
void del(string schoolName)
{
	char fileSCHOOL[MAX_PATH];
	const char * constSchl = schoolName.c_str();
	strcat_s(fileSCHOOL, constSchl);
	strcat_s(fileSCHOOL, ".INPUT");
	
	bool found=false;
	Student st;
	fstream inFile;
	inFile.open(fileSCHOOL,ios::binary|ios::in|ios::out);
	if(!inFile)
	{
		cout << "file '" << fileSCHOOL << "' could not be open. press any key...";
		cin.ignore();
		cin.get();
		return;
	}

	string s;
	cout << "\nenter the id you want to delete: ";
	fflush(stdin);
	getline(cin,s);
	while(!inFile.eof() && found == false)
	{

		inFile.read(reinterpret_cast<char *> (&st), sizeof(Student));
		if(st.getID().compare(s) == 0)
		{
			st.exportStd();
			cout<<"\n\ndo you really delete this student record? [Y/n] ";
			char confirm;
			cin >> confirm;
			if(confirm == 'Y' || confirm == 'y') {
				ofstream outFile;
				outFile.open("temporary.INPUT",ios::out);
				inFile.seekg(0,ios::beg);
				while(inFile.read(reinterpret_cast<char *> (&st), sizeof(Student)))
				{
					if(st.getID().compare(s) != 0)
					{
						outFile.write(reinterpret_cast<char *> (&st), sizeof(Student));
					}
				}
				outFile.close();
				inFile.close();
				remove(fileSCHOOL);
				rename("temporary.INPUT", fileSCHOOL);
				cout<<"\n\n\t record has been successfully deleted!";
				cin.get();
			}
			found = true;
		}
	}
	if(found == false)
		cout<<"\nrecord not found\n";
	cin.ignore();
	cin.get();
}

void swap(Student &st1, Student &st2){
	Student temp;
	memcpy(&temp, &st1, sizeof(Student));
	memcpy(&st1, &st2, sizeof(Student));
	memcpy(&st2, &temp, sizeof(Student));
}

void sortID(int n, Student st[]){
	for(int i = 1; i <= n; i++){
		for(int j = i+1; j <= n; j++){
			if(st[i].getID() > st[j].getID()){
				swap(st[i], st[j]);
			}
		}
	}
}

void sortTotal(int n, Student st[]){
	for(int i = 1; i <= n; i++){
		for(int j = i+1; j <= n; j++){
			if(st[i].getTotal() > st[j].getTotal()){
				swap(st[i], st[j]);
			}
		}
	}
}

void sortName(int n, Student st[]){
	for(int i = 1; i <= n; i++){
		for(int j = i+1; j <= n; j++){
			if(st[i].getName() > st[j].getName()){
				swap(st[i], st[j]);
			}
		}
	}
}

// showSort()
void sort(int resultcount, Student result[]){
	int sw;
	cout << "1-sort by id | 2-sort by name | 3-sort by total of point\n";
	cout << "your choice: ";
	cin >> sw;
	cout << endl;
	switch(sw) {
			   case 1:
				   sortID(resultcount, result);
				   for(int i = 1; i <= resultcount; i++){
					   result[i].exportStd();
				   }
				   break;
			   case 2:
				   sortName(resultcount, result);
				   for(int i = 1; i <= resultcount; i++){
					   result[i].exportStd();
				   }
				   break;
			   case 3:
				   sortTotal(resultcount, result);
				   for(int i = 1; i <= resultcount; i++){
					   result[i].exportStd();
				   }
				   break;
	}
}

void searchID(string schoolName)
{
	char fileSCHOOL[MAX_PATH];
	const char * constSchl = schoolName.c_str();
	strcat_s(fileSCHOOL, constSchl);
	strcat_s(fileSCHOOL, ".INPUT");
	
	Student st;
	Student result[MAXSTD];
	int resultcount = 0;
	ifstream inFile;
	inFile.open(fileSCHOOL, ios::in|ios::binary);
	if(!inFile)
	{
		cout << "file '" << fileSCHOOL << "' could not be open. press any key...";
		cin.ignore();
		cin.get();
		return;
	}
	bool flag=false;
	string s;
	cout << "\nenter the id you want to search: ";
	fflush(stdin);
	getline(cin,s);
	while(inFile.read(reinterpret_cast<char *> (&st), sizeof(Student)))
	{
		if(st.getID().compare(s) == 0)
		{
			resultcount++;
			memcpy(&result[resultcount], &st, sizeof(Student));
			flag=true;
		}
	}
	inFile.close();
	if(flag==false)
		cout << "\nrecord not found\n";
	else{
		sort(resultcount, result);
		char confirmExp;
		cout << "\ndo you want to export searching result to file? [Y/n] ";
		cin >> confirmExp;
		if(confirmExp == 'Y' || confirmExp == 'y') {
			writeFile(resultcount, result);
			cout << "searching results have been successfully saved to student_search_result.txt\n";
		}
	}
	cin.ignore();
	cin.get();
}

void searchName(string schoolName)
{
	char fileSCHOOL[MAX_PATH];
	const char * constSchl = schoolName.c_str();
	strcat_s(fileSCHOOL, constSchl);
	strcat_s(fileSCHOOL, ".INPUT");
	
	Student st;
	Student result[MAXSTD];
	int resultcount = 0;
	ifstream inFile;
	inFile.open(fileSCHOOL, ios::in|ios::binary);
	if(!inFile)
	{
		cout << "file '" << fileSCHOOL << "' could not be open. press any key...";
		cin.ignore();
		cin.get();
		return;
	}
	bool flag=false;
	string s;
	cout<<"\nenter the name you want to search: ";
	fflush(stdin);
	getline(cin,s);
	while(inFile.read(reinterpret_cast<char *> (&st), sizeof(Student)))
	{
		if(st.getName().compare(s) == 0)
		{
			resultcount++;
			memcpy(&result[resultcount], &st, sizeof(Student));
			flag=true;
		}
	}
	inFile.close();
	if(flag == false){
		cout << "\nrecord not found\n";
	}
	else{
		sort(resultcount, result);
		char confirmExp;
		cout << "\ndo you want to export searching result to file? [Y/n] ";
		cin >> confirmExp;
		if(confirmExp == 'Y' || confirmExp == 'y') {
			writeFile(resultcount, result);
			cout << "searching results have been successfully saved to student_search_result.txt\n";
		}
	}
}

void searchMath(string schoolName)
{
	char fileSCHOOL[MAX_PATH];
	const char * constSchl = schoolName.c_str();
	strcat_s(fileSCHOOL, constSchl);
	strcat_s(fileSCHOOL, ".INPUT");
	
	Student st;
	Student result[MAXSTD];
	int resultcount = 0;
	ifstream inFile;
	inFile.open(fileSCHOOL, ios::in|ios::binary);
	if(!inFile)
	{
		cout << "file '" << fileSCHOOL << "' could not be open. press any key...";
		cin.ignore();
		cin.get();
		return;
	}
	bool flag=false;
	float x;
	cout << "\nenter math point you want to search: ";
	cin >> x;
	while(inFile.read(reinterpret_cast<char *> (&st), sizeof(Student)))
	{
		if(st.getMath() == x)
		{
			resultcount++;
			memcpy(&result[resultcount], &st, sizeof(Student));
			flag=true;
		}
	}
	inFile.close();
	if(flag==false)
		cout << "\nrecord not found\n";
	else{
		sort(resultcount, result);
		char confirmExp;
		cout << "\ndo you want to export searching result to file? [Y/n] ";
		cin >> confirmExp;
		if(confirmExp == 'Y' || confirmExp == 'y') {
			writeFile(resultcount, result);
			cout << "searching results have been successfully saved to student_search_result.txt\n";
		}
	}
	cin.ignore();
	cin.get();
}

void searchPhysics(string schoolName)
{
	char fileSCHOOL[MAX_PATH];
	const char * constSchl = schoolName.c_str();
	strcat_s(fileSCHOOL, constSchl);
	strcat_s(fileSCHOOL, ".INPUT");
	
	Student st;
	Student result[MAXSTD];
	int resultcount = 0;
	ifstream inFile;
	inFile.open(fileSCHOOL, ios::in|ios::binary);
	if(!inFile)
	{
		cout << "file '" << fileSCHOOL << "' could not be open. press any key...";
		cin.ignore();
		cin.get();
		return;
	}
	bool flag=false;
	float x;
	cout << "\nenter physics point you want to search: ";
	cin >> x;
	while(inFile.read(reinterpret_cast<char *> (&st), sizeof(Student)))
	{
		if(st.getPhys() == x)
		{
			resultcount++;
			memcpy(&result[resultcount], &st, sizeof(Student));
			flag=true;
		}
	}
	inFile.close();
	if(flag==false)
		cout << "\nrecord not found\n";
	else{
		sort(resultcount, result);
		char confirmExp;
		cout << "\ndo you want to export searching result to file? [Y/n] ";
		cin >> confirmExp;
		if(confirmExp == 'Y' || confirmExp == 'y') {
			writeFile(resultcount, result);
			cout << "searching results have been successfully saved to student_search_result.txt\n";
		}
	}
	cin.ignore();
	cin.get();
}

void searchChemistry(string schoolName)
{
	char fileSCHOOL[MAX_PATH];
	const char * constSchl = schoolName.c_str();
	strcat_s(fileSCHOOL, constSchl);
	strcat_s(fileSCHOOL, ".INPUT");
	
	Student st;
	Student result[MAXSTD];
	int resultcount = 0;
	ifstream inFile;
	inFile.open(fileSCHOOL, ios::in|ios::binary);
	if(!inFile)
	{
		cout << "file '" << fileSCHOOL << "' could not be open. press any key...";
		cin.ignore();
		cin.get();
		return;
	}
	bool flag=false;
	float x;
	cout << "\nenter chemistry point you want to search: ";
	cin >> x;
	while(inFile.read(reinterpret_cast<char *> (&st), sizeof(Student)))
	{
		if(st.getChem() == x)
		{
			resultcount++;
			memcpy(&result[resultcount], &st, sizeof(Student));
			flag=true;
		}
	}
	inFile.close();
	if(flag==false)
		cout << "\nrecord not found\n";
	else{
		sort(resultcount, result);
		char confirmExp;
		cout << "\ndo you want to export searching result to file? [Y/n] ";
		cin >> confirmExp;
		if(confirmExp == 'Y' || confirmExp == 'y') {
			writeFile(resultcount, result);
			cout << "searching results have been successfully saved to student_search_result.txt\n";
		}
	}
	cin.ignore();
	cin.get();
}

void searchTotal(string schoolName)
{
	char fileSCHOOL[MAX_PATH];
	const char * constSchl = schoolName.c_str();
	strcat_s(fileSCHOOL, constSchl);
	strcat_s(fileSCHOOL, ".INPUT");
	
	Student st;
	Student result[MAXSTD];
	int resultcount = 0;
	ifstream inFile;
	inFile.open(fileSCHOOL, ios::in|ios::binary);
	if(!inFile)
	{
		cout << "file '" << fileSCHOOL << "' could not be open. press any key...";
		cin.ignore();
		cin.get();
		return;
	}
	bool flag=false;
	float x;
	cout << "\nenter point at total you want to search: ";
	cin >> x;
	while(inFile.read(reinterpret_cast<char *> (&st), sizeof(Student)))
	{
		if(st.getTotal() == x)
		{
			resultcount++;
			memcpy(&result[resultcount], &st, sizeof(Student));
			flag=true;
		}
	}
	inFile.close();
	if(flag==false)
		cout << "\nrecord not found\n";
	else{
		sort(resultcount, result);
		char confirmExp;
		cout << "\ndo you want to export searching result to file? [Y/n] ";
		cin >> confirmExp;
		if(confirmExp == 'Y' || confirmExp == 'y') {
			writeFile(resultcount, result);
			cout << "searching results have been successfully saved to student_search_result.txt\n";
		}
	}
	cin.ignore();
	cin.get();
}

void search(string schoolName)
{
		char searchOpt;
		cout << "\npress: 'i' to search with student id\n";
		cout << "       'n' to search with student name\n";
		cout << "       'm' to search with math point\n";
		cout << "       'p' to search with physics point\n";
		cout << "       'c' to search with chemistry point\n";
		cout << "       't' to search with point at total\n";
		cout << "your choice: ";
		cin >> searchOpt;

		switch(searchOpt){
			case 'i':
				searchID(schoolName);
				break;
			case 'n':
				searchName(schoolName);
				break;
			case 'm':
				searchMath(schoolName);
				break;
			case 'p':
				searchPhysics(schoolName);
				break;
			case 'c':
				searchChemistry(schoolName);
				break;
			case 't':
				searchTotal(schoolName);
				break;
			default:
				cout << "\ninvalid parameter. please choose [i/n/m/p/c/t] to search.. ";
		}
}

void stat(string schoolName)
{
	char fileSCHOOL[MAX_PATH];
	const char * constSchl = schoolName.c_str();
	strcat_s(fileSCHOOL, constSchl);
	strcat_s(fileSCHOOL, ".INPUT");
	
	Student st;
	int c15 = 0, c20 = 0, c25 = 0, c30 = 0;
	ifstream inFile;
	inFile.open(fileSCHOOL, ios::in|ios::binary);
	if(!inFile){
		cout << "file '" << fileSCHOOL << "' could not be open. press any key...";
		cin.ignore();
		cin.get();
		return;
	}
	cout << "\n\t\t---statistics---\n";
	while(inFile.read(reinterpret_cast<char *> (&st), sizeof(Student))) {
		if(st.getTotal() < 15)
			c15++;
		if(st.getTotal() >=15 && st.getTotal() < 20)
			c20++;
		if(st.getTotal() >=20 && st.getTotal() < 25)
			c25++;
		if(st.getTotal() >=25)
			c30++;
	}
	inFile.close();

	cout << "\nnumber of students whose total point is under 15: " << c15;
	cout << "\nnumber of students whose total point is between 15 and 20: " << c20;
	cout << "\nnumber of students whose total point is between 20 and 25: " << c25;
	cout << "\nnumber of students whose total point is higher than 25: " << c30 << endl << endl;
	cin.get();
}