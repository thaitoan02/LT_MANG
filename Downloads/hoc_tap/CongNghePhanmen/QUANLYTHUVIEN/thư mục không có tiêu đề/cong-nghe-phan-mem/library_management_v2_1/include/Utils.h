#ifndef UTILS_H
#define UTILS_H

#include <string>
using namespace std;

// ==========================================
// HAM TIEN ICH CHUNG
// ==========================================

string getCurrentDate();
int    daysBetween(const string& date1, const string& date2);
string addDays(const string& date, int days);
string hashPassword(const string& password);
void   clearScreen();
void   pauseScreen();

#endif
