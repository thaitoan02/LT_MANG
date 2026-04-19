#include "Utils.h"
#include <iostream>
#include <ctime>
#include <cstdio>
using namespace std;

// ==========================================
// IMPLEMENTS: Utils
// ==========================================

string getCurrentDate() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char buf[30];
    snprintf(buf, sizeof(buf), "%02d/%02d/%04d",
        ltm->tm_mday, ltm->tm_mon + 1, ltm->tm_year + 1900);
    return string(buf);
}

static int dateToDay(const string& date) {
    int d, m, y;
    sscanf(date.c_str(), "%d/%d/%d", &d, &m, &y);
    return y * 365 + m * 30 + d;
}

int daysBetween(const string& date1, const string& date2) {
    return dateToDay(date2) - dateToDay(date1);
}

string addDays(const string& date, int days) {
    int d, m, y;
    sscanf(date.c_str(), "%d/%d/%d", &d, &m, &y);
    int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    d += days;
    while (d > daysInMonth[m]) {
        d -= daysInMonth[m];
        m++;
        if (m > 12) { m = 1; y++; }
    }
    char buf[30];
    snprintf(buf, sizeof(buf), "%02d/%02d/%04d", d, m, y);
    return string(buf);
}

string hashPassword(const string& password) {
    string hashed = "";
    int key = 37;
    for (char c : password) {
        int code = (int)c + key;
        hashed += to_string(code) + "-";
        key = code % 97;
    }
    return hashed;
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pauseScreen() {
    cout << "\n  Nhan Enter de tiep tuc...";
    cin.ignore();
    cin.get();
}
