#include "Reader.h"
#include <iostream>
#include <algorithm>
using namespace std;

// ==========================================
// IMPLEMENTS: Reader
// ==========================================

Reader::Reader() {}

Reader::Reader(int _id, const string& _username, const string& _password,
               const string& _status, const string& _fullname, const string& _email)
    : User(_id, _username, _password, "reader", _status, _fullname, _email) {}

vector<Book*> Reader::searchBook(vector<Book>& books,
                                  const string& keyword,
                                  int searchType) {
    vector<Book*> results;

    // Chuyen tu khoa sang chu thuong
    string kw = keyword;
    transform(kw.begin(), kw.end(), kw.begin(), ::tolower);

    for (auto& b : books) {
        string field = "";
        if      (searchType == 1) field = b.getTitle();
        else if (searchType == 2) field = b.getAuthor();
        else if (searchType == 3) field = b.getCategory();

        string fieldLower = field;
        transform(fieldLower.begin(), fieldLower.end(), fieldLower.begin(), ::tolower);

        if (fieldLower.find(kw) != string::npos)
            results.push_back(&b);
    }
    return results;
}

void Reader::viewBorrowHistory(const vector<BorrowSlip>& borrows,
                                const vector<Book>& books) const {
    cout << "  Doc gia: " << fullname << "\n";
    int count = 0;

    for (const auto& slip : borrows) {
        if (slip.getReaderId() != id) continue;
        count++;

        cout << "\nPhieu #" << slip.getId()
             << " | Muon: " << slip.getBorrowDate()
             << " | Han: "  << slip.getDueDate()
             << " | Tra: "  << (slip.getReturnDate().empty() ? "Chua tra" : slip.getReturnDate())
             << " | "       << slip.getStatus() << "\n";

        for (const auto& d : slip.getDetails()) {
            string title = "Khong ro";
            for (const auto& b : books)
                if (b.getId() == d.getBookId()) { title = b.getTitle(); break; }
            cout << "   - " << title << " (x" << d.getQuantity() << ")\n";
        }
    }
    if (count == 0) cout << "  Chua co lich su muon.\n";
}

void Reader::displayInfo() const {
    User::displayInfo();
}
