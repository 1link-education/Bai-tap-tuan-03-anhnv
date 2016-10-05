#include "student.h"
#include "studentIO.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string.h>
#include <sstream>

#define MAXSTD 20

using namespace std;

bool ValidateFloat(string);
int checkString(string);

void Student::importStd(){
	cout << "id: ";
	fflush(stdin);
	getline(cin, id);

	cout << "name: ";
	fflush(stdin);
	getline(cin, name);
	while(!checkString(name)) {
		cout << "'" << name << "' is not a valid name. please re-enter name\n";
		cout << "name: ";
		fflush(stdin);
		getline(cin, name);                         
	}

	cout << "address: ";
	fflush(stdin);
	getline(cin, address);
	while(!checkString(address)) {
		cout << "'" << address << "' is not a valid address. please re-enter address\n";
		cout << "address: ";
		fflush(stdin);
		getline(cin, address);                         
	}

	cout << "sex (m for male / f for female): ";
	getline(cin, sex);
	while(!(sex == "m" || sex == "f")) {
		cout << "invalid parameter. please re-enter sex value, 'm' for male and 'f' for female\n";
		getline(cin, sex);      
	}

	string inputM;
	cout << "math point: ";
	getline(cin, inputM);

	while ((!ValidateFloat(inputM))) {
		cin.clear();
		cout << "	!invalid number: " << inputM << endl;
		cout << "math point: ";
		getline(cin, inputM);
	}
	math = atof(inputM.c_str());
	cin.clear();

	while(!(0 <= math && math <= 10)) {
		cout << "	!a point value must be between 0 and 10. please re-enter math point\n";
		cout << "math point: ";
		getline(cin, inputM);

		while ((!ValidateFloat(inputM))) {
			cin.clear();
			cout << "	!invalid number: " << inputM << endl;
			cout << "math point: ";
			getline(cin, inputM);
		}
		math = atof(inputM.c_str());
		cin.clear();
	}

	string inputP;
	cout << "physics point: ";
	getline(cin, inputP);

	while ((!ValidateFloat(inputP))) {
		cin.clear();
		cout << "	!invalid number: " << inputP << endl;
		cout << "physics point: ";
		getline(cin, inputP);
	}
	phys = atof(inputP.c_str());
	cin.clear();

	while(!(0 <= phys && phys <= 10)) {
		cout << "	!a point value must be between 0 and 10. please re-enter physics point\n";
		cout << "physics point: ";
		getline(cin, inputP);

		while ((!ValidateFloat(inputP))) {
			cin.clear();
			cout << "	!invalid number: " << inputP << endl;
			cout << "physics point: ";
			getline(cin, inputP);
		}
		phys = atof(inputP.c_str());
		cin.clear();
	}

	string inputC;
	cout << "chemistry point: ";
	getline(cin, inputC);

	while ((!ValidateFloat(inputC))) {
		cin.clear();
		cout << "	!invalid number: " << inputC << endl;
		cout << "chemistry point: ";
		getline(cin, inputC);
	}
	chem = atof(inputC.c_str());
	cin.clear();

	while(!(0 <= chem && chem <= 10)) {
		cout << "	!a point value must be between 0 and 10. please re-enter math point\n";
		cout << "chemistry point: ";
		getline(cin, inputC);

		while ((!ValidateFloat(inputC))) {
			cin.clear();
			cout << "	!invalid number: " << inputC << endl;
			cout << "chemistry point: ";
			getline(cin, inputC);
		}
		chem = atof(inputC.c_str());
		cin.clear();
	}
}

void Student::exportStd(){

	cout << "id: " << id << endl;
	cout << "name: " << name << endl;
	cout << "address: " << address << endl;
	cout << "sex: ";
	if(sex.compare("m") == 0)
		cout << "male\n";
	else
		cout << "female\n";
	cout << "math: " << math << endl;
	cout << "physics: " << phys << endl;
	cout << "chemistry: " << chem << endl << endl;
}

void Student::setID(string x){
	id = x;
}
string Student::getID(){
	return id;
}		

void Student::setName(string x){
	name = x;
}

string Student::getName(){
	return name;
}		

void Student::setAdd(string x){
	address = x;
}		

string Student::getAdd(){
	return address;
}		

void Student::setSex(string x){
	sex = x;
}		

string Student::getSex(){
	if(sex.compare("m") == 0)
		return "male";
	else
		return "female";
}		

void Student::setMath(float x){
	math = x;
}		

float Student::getMath(){
	return math;
}

void Student::setPhys(float x){
	phys = x;
}		

float Student::getPhys(){
	return phys;
}

void Student::setChem(float x){
	chem = x;
}		

float Student::getChem(){
	return chem;
}

float Student::getTotal(){
	return(math + phys + chem);
}

int checkString(string s)
{
	int i;
	bool flag = false;
	for(i = 0; i <= s.length(); i++)
		if((65 <= s[i] && s[i] <= 90) || (97 <= s[i] && s[i] <= 122)) {
			flag = true;
			break;       
		}  
		if(flag == true)
			return 1;
		else
			return 0;
}

// kiem tra dau vao la so float khong am
bool ValidateFloat(string input) {
	bool valid = true;
	float vf;

	if (input.length() == 0) {
		valid = false;
	} else {
		for (int i = 0; i < input.length(); i++) {
			if (!isdigit(input[i])) {
				valid = false;
			}
		}
	}
	vf = atof(input.c_str());
	if (vf < 0) {
		valid = false;				//accept positive values only - VANH
	}
	return valid;
}