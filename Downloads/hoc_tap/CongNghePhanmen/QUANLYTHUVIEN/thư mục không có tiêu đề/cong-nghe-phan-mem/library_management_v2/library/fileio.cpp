#include "library.h"


// ==========================================
// LUU / TAI DU LIEU TU FILE
// ==========================================
// File nay quan ly xuat nhap file cho tat ca:
// - Users (loadUsers/saveUsers)
// - Books (loadBooks/saveBooks)
// - Borrows (loadBorrows/saveBorrows)
// - Fines (loadFines/saveFines)
//
// Format file: Dung ky tu '|' de phan cach cac truong
// Vi du: id|username|password|role|status|fullname|email

// HAM: TAI TAT CA DU LIEU TU FILE
// Goi lan dau khi khoi dong chuong trinh
void loadAllData() {
    loadUsers();
    loadBooks();
    loadBorrows();
    loadFines();
}

// HAM: LUU TAT CA DU LIEU XUONG FILE
// Goi truoc khi thoat chuong trinh hoac khi co thay doi
void saveAllData() {
    saveUsers();
    saveBooks();
    saveBorrows();
    saveFines();
}

// ========== PHAN 1: USERS (NGUOI DUNG) ==========
// Format file users.txt:
// id|username|password|role|status|fullname|email

// HAM: TAI DANH SACH USERS TU FILE
void loadUsers() {
    gUsers.clear();
    ifstream f(DATA_USERS);
    if (!f.is_open()) return;

    string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        User u;
        // Format: id|username|password|role|status|fullname|email
        sscanf(line.c_str(), "%d", &u.id);
        stringstream ss(line);
        string token;
        vector<string> parts;
        while (getline(ss, token, '|')) parts.push_back(token);

        if (parts.size() < 7) continue;
        u.id       = stoi(parts[0]);
        u.username = parts[1];
        u.password = parts[2];
        u.role     = parts[3];
        u.status   = parts[4];
        u.fullname = parts[5];
        u.email    = parts[6];
        gUsers.push_back(u);
    }
    f.close();
}

void saveUsers() {
    ofstream f(DATA_USERS);
    // Ghi tung user, dung | phan cach
    for (auto& u : gUsers) {
        f << u.id << "|" << u.username << "|" << u.password << "|"
          << u.role << "|" << u.status << "|" << u.fullname << "|"
          << u.email << "\n";
    }
    f.close();
}

// ========== PHAN 2: BOOKS (SACH) ==========
// Format file books.txt:
// id|title|author|publisher|year|category|quantity|available|location

// HAM: TAI DANH SACH SACH TU FILE
void loadBooks() {
    gBooks.clear();
    ifstream f(DATA_BOOKS);
    if (!f.is_open()) return;

    string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string token;
        vector<string> parts;
        while (getline(ss, token, '|')) parts.push_back(token);

        if (parts.size() < 9) continue;
        Book b;
        b.id        = stoi(parts[0]);
        b.title     = parts[1];
        b.author    = parts[2];
        b.publisher = parts[3];
        b.year      = stoi(parts[4]);
        b.category  = parts[5];
        b.quantity  = stoi(parts[6]);
        b.available = stoi(parts[7]);
        b.location  = parts[8];
        gBooks.push_back(b);
    }
    f.close();
}

void saveBooks() {
    ofstream f(DATA_BOOKS);
    // Ghi tung sach, dung | phan cach
    for (auto& b : gBooks) {
        f << b.id << "|" << b.title << "|" << b.author << "|"
          << b.publisher << "|" << b.year << "|" << b.category << "|"
          << b.quantity << "|" << b.available << "|" << b.location << "\n";
    }
    f.close();
}

// ========== PHAN 3: BORROWS (PHIEU MUON) ==========
// Format file borrows.txt:
// SLIP|id|readerId|borrowDate|dueDate|returnDate|status
// DETAIL|bookId|quantity
// (lap lai cho moi chi tiet)
// END

// HAM: TAI DANH SACH PHIEU MUON TU FILE
void loadBorrows() {
    gBorrows.clear();
    ifstream f(DATA_BORROWS);
    if (!f.is_open()) return;

    string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        // Format phiếu mượn:
        // SLIP|id|readerId|borrowDate|dueDate|returnDate|status
        // DETAIL|bookId|quantity
        // END

        if (line.substr(0, 4) == "SLIP") {
            stringstream ss(line);
            string token;
            vector<string> parts;
            while (getline(ss, token, '|')) parts.push_back(token);

            BorrowSlip slip;
            slip.id         = stoi(parts[1]);
            slip.readerId   = stoi(parts[2]);
            slip.borrowDate = parts[3];
            slip.dueDate    = parts[4];
            slip.returnDate = parts[5];
            slip.status     = parts[6];

            // Đọc chi tiết
            string detailLine;
            while (getline(f, detailLine)) {
                if (detailLine == "END") break;
                if (detailLine.substr(0, 6) == "DETAIL") {
                    stringstream ds(detailLine);
                    string dt;
                    vector<string> dp;
                    while (getline(ds, dt, '|')) dp.push_back(dt);
                    BorrowDetail d;
                    d.bookId   = stoi(dp[1]);
                    d.quantity = stoi(dp[2]);
                    slip.details.push_back(d);
                }
            }
            gBorrows.push_back(slip);
        }
    }
    f.close();
}

void saveBorrows() {
    ofstream f(DATA_BORROWS);
    // Ghi tung phieu muon voi chi tiet
    for (auto& slip : gBorrows) {
        // Dong 1: Thong tin phieu muon
        f << "SLIP|" << slip.id << "|" << slip.readerId << "|"
          << slip.borrowDate << "|" << slip.dueDate << "|"
          << slip.returnDate << "|" << slip.status << "\n";
        // Nhieu dong: Chi tiet sach
        for (auto& d : slip.details) {
            f << "DETAIL|" << d.bookId << "|" << d.quantity << "\n";
        }
        // Dong cuoi: Ket thuc phieu
        f << "END\n";
    }
    f.close();
}

// ========== PHAN 4: FINES (TIEN PHAT) ==========
// Format file fines.txt:
// id|borrowId|readerId|amount|reason|status|createdDate

// HAM: TAI DANH SACH TIEN PHAT TU FILE
void loadFines() {
    gFines.clear();
    ifstream f(DATA_FINES);
    if (!f.is_open()) return;

    string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string token;
        vector<string> parts;
        while (getline(ss, token, '|')) parts.push_back(token);

        if (parts.size() < 7) continue;
        Fine fine;
        fine.id          = stoi(parts[0]);
        fine.borrowId    = stoi(parts[1]);
        fine.readerId    = stoi(parts[2]);
        fine.amount      = stod(parts[3]);
        fine.reason      = parts[4];
        fine.status      = parts[5];
        fine.createdDate = parts[6];
        gFines.push_back(fine);
    }
    f.close();
}

void saveFines() {
    ofstream f(DATA_FINES);
    // Ghi tung phieu phat, dung | phan cach
    for (auto& fine : gFines) {
        f << fine.id << "|" << fine.borrowId << "|" << fine.readerId << "|"
          << fine.amount << "|" << fine.reason << "|" << fine.status << "|"
          << fine.createdDate << "\n";
    }
    f.close();
}
