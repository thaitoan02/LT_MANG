#include "library.h"

// ==========================================
// CAC HAM TIEN ICH / UTILITY FUNCTIONS
// ==========================================
// Cac ham ho tro:
// - getCurrentDate(): Lay ngay hom nay (dd/mm/yyyy)
// - daysBetween(): Tinh so ngay giua 2 ngay
// - addDays(): Cong them n ngay vao 1 ngay
// - hashPassword(): Hash mat khau (giai lap)
// - clearScreen(): Xoa man hinh console
// - pause(): Dung va doi nhan Enter
// - generateId(): Tao ID moi

// HAM: LAY NGAY HIEN TAI (DD/MM/YYYY)
string getCurrentDate() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char buf[20];
    snprintf(buf, sizeof(buf), "%02d/%02d/%04d",
        ltm->tm_mday,
        ltm->tm_mon + 1,
        ltm->tm_year + 1900);
    return string(buf);
}

// HAM HO TRO: Chuyen "dd/mm/yyyy" thanh so ngay
int dateToDay(string date) {
    int d, m, y;
    sscanf(date.c_str(), "%d/%d/%d", &d, &m, &y);
    // Công thức đơn giản tính số ngày
    int days = y * 365 + m * 30 + d;
    return days;
}

// HAM: TINH SO NGAY GIUA 2 NGAY
// Tra ve: date2 - date1 (so ngay)
int daysBetween(string date1, string date2) {
    return dateToDay(date2) - dateToDay(date1);
}

// HAM: CONG THEM N NGAY VAO MOT NGAY
// Dung de tinh han tra sach (+14 ngay) hay ngay tra co phat
string addDays(string date, int days) {
    int d, m, y;
    sscanf(date.c_str(), "%d/%d/%d", &d, &m, &y);

    // Mảng số ngày trong tháng
    int daysInMonth[] = {0, 31,28,31,30,31,30,31,31,30,31,30,31};

    d += days;
    // Xử lý tràn tháng
    while (d > daysInMonth[m]) {
        d -= daysInMonth[m];
        m++;
        if (m > 12) { m = 1; y++; }
    }

    char buf[20];
    snprintf(buf, sizeof(buf), "%02d/%02d/%04d", d, m, y);
    return string(buf);
}

// HAM: HASH MAT KHAU (Giai lap - khong phai hash thuc)
// Dung de luu mat khau mot cach an toan
string hashPassword(string password) {
    string hashed = "";
    int key = 37;
    for (char c : password) {
        int code = (int)c + key;
        hashed += to_string(code) + "-";
        key = code % 97;
    }
    return hashed;
}

// HAM: XOA MAN HINH CONSOLE
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// HAM: DUNG VA DOI NHAN ENTER
void pause() {
    cout << "\n  Nhan Enter de tiep tuc...";
    cin.ignore();
    cin.get();
}

// HAM: TAO ID MOI (max + 1)
int generateId(int maxId) {
    return maxId + 1;
}
