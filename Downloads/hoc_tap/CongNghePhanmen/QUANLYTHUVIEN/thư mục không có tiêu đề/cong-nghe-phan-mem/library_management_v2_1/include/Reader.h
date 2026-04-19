#ifndef READER_H
#define READER_H

#include "User.h"
#include "Book.h"
#include "BorrowSlip.h"
#include <vector>
using namespace std;

// ==========================================
// CLASS: Reader - Doc gia (ke thua User)
// ==========================================
class Reader : public User {
public:
    Reader();
    Reader(int id, const string& username, const string& password,
           const string& status, const string& fullname, const string& email);

    // Nghiep vu cua Doc Gia
    vector<Book*> searchBook(vector<Book>& books,
                             const string& keyword,
                             int searchType);  // 1=ten, 2=tacgia, 3=theloai

    void viewBorrowHistory(const vector<BorrowSlip>& borrows,
                           const vector<Book>& books) const;

    void displayInfo() const override;
};

#endif
