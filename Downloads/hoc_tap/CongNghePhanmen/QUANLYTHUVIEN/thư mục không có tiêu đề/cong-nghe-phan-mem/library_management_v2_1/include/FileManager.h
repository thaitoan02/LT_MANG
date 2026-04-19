#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "User.h"
#include "Book.h"
#include "BorrowSlip.h"
#include "Fine.h"
#include <vector>
using namespace std;

// ==========================================
// CLASS: FileManager - Quan ly doc/ghi file
// ==========================================
// Tat ca phuong thuc la static -> goi truc tiep
// khong can tao doi tuong
// Vi du: FileManager::saveBooks(books);
class FileManager {
public:
    // --- Users ---
    static void saveUsers(const vector<User*>& users);
    static void loadUsers(vector<User*>& users);

    // --- Books ---
    static void saveBooks(const vector<Book>& books);
    static void loadBooks(vector<Book>& books);

    // --- BorrowSlips ---
    static void saveBorrows(const vector<BorrowSlip>& borrows);
    static void loadBorrows(vector<BorrowSlip>& borrows);

    // --- Fines ---
    static void saveFines(const vector<Fine>& fines);
    static void loadFines(vector<Fine>& fines);

    // --- Toan bo ---
    static void saveAll(const vector<User*>& users,
                        const vector<Book>& books,
                        const vector<BorrowSlip>& borrows,
                        const vector<Fine>& fines);

    static void loadAll(vector<User*>& users,
                        vector<Book>& books,
                        vector<BorrowSlip>& borrows,
                        vector<Fine>& fines);
};

#endif
