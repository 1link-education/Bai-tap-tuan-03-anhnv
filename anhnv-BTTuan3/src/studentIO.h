#ifndef _STUDENTIO_H_
#define _STUDENTIO_H_

#include "student.h"
#include <tchar.h>

#define MAX_PATH 255

void mkFile(string);
void merge(TCHAR[MAX_PATH], TCHAR[MAX_PATH]);
void WatchDirectory();
void writeFile(int n, Student st[]);
void readFile(Student st[]);

#endif