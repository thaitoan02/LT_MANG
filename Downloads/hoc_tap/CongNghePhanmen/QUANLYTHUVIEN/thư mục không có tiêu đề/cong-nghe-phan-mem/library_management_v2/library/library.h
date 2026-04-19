#ifndef LIBRARY_H
#define LIBRARY_H

// ==========================================
// FILE HEADER CHINH - HE THONG QUAN LY THU VIEN
// ==========================================
// File nay chua tat ca:
// - Dinh nghia cac struct du lieu
// - Khai bao cac hang so
// - Khai bao cac ham

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <iomanip>
using namespace std;

// ==========================================
// CAC HANG SO TOAN CUC
// ==========================================
// Duong dan den cac file du lieu
const string DATA_USERS    = "data/users.txt";    // File luu tong hop nguoi dung
const string DATA_BOOKS    = "data/books.txt";    // File luu danh sach sach
const string DATA_BORROWS  = "data/borrows.txt";  // File luu phieu muon tra
const string DATA_FINES    = "data/fines.txt";    // File luu tien phat

// Cac thong so he thong
const double FINE_PER_DAY  = 2000.0;  // Phat 2000 VND cho moi ngay tre
const int    MAX_BORROW    = 5;        // Toi da muon 5 cuon mot lan

// ==========================================
// CAC STRUCT DINH NGHIA DU LIEU
// ==========================================

// STRUCT 1: THONG TIN NGUOI DUNG (User, Admin, Thu Thu, Doc Gia)
struct User {
    int    id;           // ID duy nhat
    string username;     // Ten dang nhap
    string password;     // Mat khau (da hash)
    string role;         // Vai tro: "admin", "librarian", "reader"
    string status;       // Trang thai: "active" (hoat dong), "locked" (bi khoa)
    string fullname;     // Ho va ten day du
    string email;        // Dia chi email
};

// STRUCT 2: THONG TIN SACH
struct Book {
    int    id;          // ID duy nhat cua sach
    string title;       // Ten sach
    string author;      // Tac gia
    string publisher;   // Nha xuat ban
    int    year;        // Nam xuat ban
    string category;    // The loai (Toan, Lap trinh, Van hoc, v.v.)
    int    quantity;    // Tong so cuon trong thu vien
    int    available;   // So cuon con lai (chua bi muon)
    string location;    // Vi tri keo sach (VD: A1-01)
};

// STRUCT 3: CHI TIET SAC TRONG PHIEU MUON
// Luu thong tin tung cuon sach duoc muon trong 1 phieu
struct BorrowDetail {
    int bookId;     // ID cua sach duoc muon
    int quantity;   // So luong cuon muon
};

// STRUCT 4: PHIEU MUON SACH
struct BorrowSlip {
    int    id;                    // ID duy nhat cua phieu muon
    int    readerId;              // ID cua doc gia muon sach
    string borrowDate;            // Ngay muon (dinh dang: dd/mm/yyyy)
    string dueDate;               // Han tra (thong thuong la 14 ngay sau)
    string returnDate;            // Ngay tra thuc te (trong neu chua tra)
    string status;                // Trang thai: "borrowing" (dang muon), "returned" (da tra), "overdue" (tre han)
    vector<BorrowDetail> details; // Danh sach tung loai sach trong phieu
};

// STRUCT 5: PHIEU TIEN PHAT (cho sach tra tre)
struct Fine {
    int    id;         // ID duy nhat cua phieu phat
    int    borrowId;   // ID phieu muon co van de
    int    readerId;   // ID doc gia can thanh toan phat
    double amount;     // So tien phat (VND)
    string reason;     // Ly do phat (VD: "Tra sach tre 5 ngay")
    string status;     // Trang thai: "unpaid" (chua tra), "paid" (da tra)
    string createdDate; // Ngay tao phieu phat
};

// ==========================================
// CAC BIEN TOAN CUC
// ==========================================
// Danh sach du lieu chinh - luu trom trong bo nho ram
extern vector<User>       gUsers;      // Danh sach tat ca nguoi dung he thong
extern vector<Book>       gBooks;      // Danh sach tat ca sach trong thu vien
extern vector<BorrowSlip> gBorrows;    // Danh sach tat ca phieu muon tra sach
extern vector<Fine>       gFines;      // Danh sach tat ca phieu tien phat
extern User*              gCurrentUser; // Con tro den nguoi dung dang dang nhap

// ==========================================
// KHAI BAO CAC HAM
// ==========================================

// ===== PHAN 1: CAC HAM TIEN ICH (Utility Functions) =====
// Cac ham ho tro nhu tao id, tính toán ngày, hash password
string  getCurrentDate();
int     daysBetween(string date1, string date2);
string  addDays(string date, int days);
string  hashPassword(string password);
void    clearScreen();
void    pause();
int     generateId(int size);

// ===== PHAN 2: CAC HAM LUU/TAI DU LIEU (File I/O) =====
// Doc va ghi du lieu tu/vao file
void loadAllData();
void saveAllData();
void loadUsers();
void loadBooks();
void loadBorrows();
void loadFines();
void saveUsers();
void saveBooks();
void saveBorrows();
void saveFines();

// ===== PHAN 3: CAC HAM DANG NHAP/DANG XUAT (Authentication) =====
// Dang nhap, dang xuat, doi mat khau
bool login();
void logout();
void changePassword();

// ===== PHAN 4: CAC HAM QUAN LY SACH (Book Management) =====
// Them, sua, xoa, tim kiem sach
void manageBook();
void addBook();
void updateBook();
void deleteBook();
void listBooks();
void searchBook();
void viewBookDetail(int bookId);

// ===== PHAN 5: CAC HAM QUAN LY DOC GIA (Reader Management) =====
// Them, sua, xoa, khoa/mo khoa tai khoan doc gia
void manageReader();
void addReader();
void updateReader();
void deleteReader();
void lockReader();
void listReaders();

// ===== PHAN 6: CAC HAM QUAN LY MUON/TRA SACH (Borrow Management) =====
// Tao phieu muon, xac nhan tra sach, xem lich su
void manageBorrow();
void createBorrowSlip();
void confirmReturn();
void listBorrows();
void viewBorrowHistory(int readerId = -1);

// ===== PHAN 7: CAC HAM QUAN LY TIEN PHAT (Fine Management) =====
// Xem, thanh toan tien phat
void manageFine();
void listFines();
void payFine();

// ===== PHAN 8: CAC HAM BAO CAO THONG KE (Reporting) =====
// Thong ke trang thai sach, sach qua han, top nguoi muon
void viewReport();
void reportBookStat();
void reportOverdue();
void reportTopBorrower();

// ===== PHAN 9: CAC HAM QUAN LY TAI KHOAN (Admin Management) =====
// Quan ly tai khoan nguoi dung, khoa/mo khoa
void manageUser();
void listUsers();
void lockUser();
void unlockUser();

// ===== PHAN 10: CAC MENU CHINH (Menu Systems) =====
// Cac menu cho tung vai tro nguoi dung
void menuReader();
void menuLibrarian();
void menuAdmin();

#endif
