#ifndef _STUDENTCONTROLLER_H_
#define _STUDENTCONTROLLER_H_

#include "student.h"

void mkDir();
void Schooling(string);
bool checkID(string, int, Student);
void addFK(int, string);
void addFF(int, string);
void add(int, string);
void show(string);
void update(string);
void searchID(string);
void searchName(string);
void searchMath(string);
void searchPhysics(string);
void searchChemistry(string);
void searchTotal(string);
void search(string);
void swap();
void sortID(int, Student[]);
void sortName(int, Student[]);
void sortTotal(int, Student[]);
void sort(int, Student[]);
void del(string);
void stat(string);

#endif
