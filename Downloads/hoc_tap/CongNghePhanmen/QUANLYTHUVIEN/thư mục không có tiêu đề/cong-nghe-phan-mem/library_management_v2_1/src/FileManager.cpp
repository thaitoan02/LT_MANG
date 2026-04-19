#include "FileManager.h"
#include "Reader.h"
#include "Librarian.h"
#include "Admin.h"
#include "Constants.h"
#include <fstream>
#include <sstream>
using namespace std;

// ==========================================
// IMPLEMENTS: FileManager
// ==========================================

// ========== USERS ==========
// Format: id|username|password|role|status|fullname|email

void FileManager::saveUsers(const vector<User*>& users) {
    ofstream f(DATA_USERS);
    for (auto u : users) {
        f << u->getId()       << "|"
          << u->getUsername() << "|"
          << u->getPassword() << "|"
          << u->getRole()     << "|"
          << u->getStatus()   << "|"
          << u->getFullname() << "|"
          << u->getEmail()    << "\n";
    }
    f.close();
}

void FileManager::loadUsers(vector<User*>& users) {
    // Giai phong bo nho cu truoc khi load lai
    for (auto u : users) delete u;
    users.clear();

    ifstream f(DATA_USERS);
    if (!f.is_open()) return;

    string line;
    while (getline(f, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string token;
        vector<string> parts;
        while (getline(ss, token, '|')) parts.push_back(token);
        if (parts.size() < 7) continue;

        int    id       = stoi(parts[0]);
        string username = parts[1];
        string password = parts[2];  // Da hash san trong file
        string role     = parts[3];
        string status   = parts[4];
        string fullname = parts[5];
        string email    = parts[6];

        User* u = nullptr;
        if      (role == "reader")    u = new Reader   (id, username, "", status, fullname, email);
        else if (role == "librarian") u = new Librarian(id, username, "", status, fullname, email);
        else if (role == "admin")     u = new Admin    (id, username, "", status, fullname, email);
        else                          u = new Reader   (id, username, "", status, fullname, email);

        // Set password hash truc tiep (khong qua hashPassword)
        u->setPassword(password);
        users.push_back(u);
    }
    f.close();
}

// ========== BOOKS ==========
// Format: id|title|author|publisher|year|category|quantity|available|location

void FileManager::saveBooks(const vector<Book>& books) {
    ofstream f(DATA_BOOKS);
    for (const auto& b : books) {
        f << b.getId()        << "|"
          << b.getTitle()     << "|"
          << b.getAuthor()    << "|"
          << b.getPublisher() << "|"
          << b.getYear()      << "|"
          << b.getCategory()  << "|"
          << b.getQuantity()  << "|"
          << b.getAvailable() << "|"
          << b.getLocation()  << "\n";
    }
    f.close();
}

void FileManager::loadBooks(vector<Book>& books) {
    books.clear();
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
        b.setId(stoi(parts[0]));
        b.setTitle(parts[1]);
        b.setAuthor(parts[2]);
        b.setPublisher(parts[3]);
        b.setYear(stoi(parts[4]));
        b.setCategory(parts[5]);
        b.setQuantity(stoi(parts[6]));
        b.setAvailable(stoi(parts[7]));
        b.setLocation(parts[8]);
        books.push_back(b);
    }
    f.close();
}

// ========== BORROWS ==========
// Format:
//   SLIP|id|readerId|borrowDate|dueDate|returnDate|status
//   DETAIL|bookId|quantity
//   END

void FileManager::saveBorrows(const vector<BorrowSlip>& borrows) {
    ofstream f(DATA_BORROWS);
    for (const auto& slip : borrows) {
        f << "SLIP|"
          << slip.getId()         << "|"
          << slip.getReaderId()   << "|"
          << slip.getBorrowDate() << "|"
          << slip.getDueDate()    << "|"
          << slip.getReturnDate() << "|"
          << slip.getStatus()     << "\n";

        for (const auto& d : slip.getDetails())
            f << "DETAIL|" << d.getBookId() << "|" << d.getQuantity() << "\n";

        f << "END\n";
    }
    f.close();
}

void FileManager::loadBorrows(vector<BorrowSlip>& borrows) {
    borrows.clear();
    ifstream f(DATA_BORROWS);
    if (!f.is_open()) return;

    string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        if (line.substr(0, 4) != "SLIP") continue;

        stringstream ss(line);
        string token;
        vector<string> parts;
        while (getline(ss, token, '|')) parts.push_back(token);

        BorrowSlip slip;
        slip.setId(stoi(parts[1]));
        slip.setReaderId(stoi(parts[2]));
        slip.setBorrowDate(parts[3]);
        slip.setDueDate(parts[4]);
        slip.setReturnDate(parts[5]);
        slip.setStatus(parts[6]);

        // Doc cac dong DETAIL cho den END
        string detailLine;
        while (getline(f, detailLine)) {
            if (detailLine == "END") break;
            if (detailLine.substr(0, 6) != "DETAIL") continue;

            stringstream ds(detailLine);
            string dt;
            vector<string> dp;
            while (getline(ds, dt, '|')) dp.push_back(dt);

            slip.addDetail(BorrowDetail(stoi(dp[1]), stoi(dp[2])));
        }
        borrows.push_back(slip);
    }
    f.close();
}

// ========== FINES ==========
// Format: id|borrowId|readerId|amount|reason|status|createdDate

void FileManager::saveFines(const vector<Fine>& fines) {
    ofstream f(DATA_FINES);
    for (const auto& fine : fines) {
        f << fine.getId()          << "|"
          << fine.getBorrowId()    << "|"
          << fine.getReaderId()    << "|"
          << fine.getAmount()      << "|"
          << fine.getReason()      << "|"
          << fine.getStatus()      << "|"
          << fine.getCreatedDate() << "\n";
    }
    f.close();
}

void FileManager::loadFines(vector<Fine>& fines) {
    fines.clear();
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
        fine.setId(stoi(parts[0]));
        fine.setBorrowId(stoi(parts[1]));
        fine.setReaderId(stoi(parts[2]));
        fine.setAmount(stod(parts[3]));
        fine.setReason(parts[4]);
        fine.setStatus(parts[5]);
        fine.setCreatedDate(parts[6]);
        fines.push_back(fine);
    }
    f.close();
}

// ========== SAVE / LOAD ALL ==========

void FileManager::saveAll(const vector<User*>& users,
                          const vector<Book>& books,
                          const vector<BorrowSlip>& borrows,
                          const vector<Fine>& fines) {
    saveUsers(users);
    saveBooks(books);
    saveBorrows(borrows);
    saveFines(fines);
}

void FileManager::loadAll(vector<User*>& users,
                          vector<Book>& books,
                          vector<BorrowSlip>& borrows,
                          vector<Fine>& fines) {
    loadUsers(users);
    loadBooks(books);
    loadBorrows(borrows);
    loadFines(fines);
}
