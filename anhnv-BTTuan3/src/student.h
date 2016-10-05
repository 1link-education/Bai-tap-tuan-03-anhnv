#ifndef _STUDENT_H_
#define _STUDENT_H_

#include <string>
using std::string;

bool ValidateFloat(string);
int checkString(string);

class Student
{
public:
	string id;
	string name;
	string address;
	string sex;
	float math;
	float phys;
	float chem;
	void setID(string);
	string getID();
	void setName(string);
	string getName();
	void setAdd(string);
	string getAdd();
	void setSex(string);
	string getSex();
	void setMath(float);
	float getMath();
	void setPhys(float);
	float getPhys();
	void setChem(float);
	float getChem();
	float getTotal();
	void importStd();
	void exportStd();
};

#endif
