#ifndef LIBRARIAN_H
#define LIBRARIAN_H

#include "User.h"
#include "Book.h"
#include "BorrowSlip.h"
#include "Fine.h"
#include <vector>
using namespace std;

// ==========================================
// CLASS: Librarian - Thu Thu (ke thua User)
// ==========================================
class Librarian : public User {
public:
    Librarian();
    Librarian(int id, const string& username, const string& password,
              const string& status, const string& fullname, const string& email);

    // Quan ly sach
    void addBook(vector<Book>& books);
    void updateBook(vector<Book>& books);
    void deleteBook(vector<Book>& books, const vector<BorrowSlip>& borrows);

    // Quan ly muon / tra
    void createBorrowSlip(vector<BorrowSlip>& borrows,
                          vector<Book>& books,
                          const vector<User*>& users);

    void confirmReturn(vector<BorrowSlip>& borrows,
                       vector<Book>& books,
                       vector<Fine>& fines);

    void displayInfo() const override;
};

#endif
