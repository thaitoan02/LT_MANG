#include "Book.h"
#include <iostream>
#include <iomanip>
using namespace std;

// ==========================================
// IMPLEMENTS: Book
// ==========================================

Book::Book() : id(0), year(0), quantity(0), available(0) {}

bool Book::isAvailable() const {
    return available > 0;
}

void Book::borrowCopy(int qty) {
    available -= qty;
}

void Book::returnCopy(int qty) {
    available += qty;
}

void Book::display() const {
    cout << "\n";
    cout << "          CHI TIET SACH\n";
    cout << "\n";
    cout << "  ID          : " << id        << "\n";
    cout << "  Ten sach    : " << title     << "\n";
    cout << "  Tac gia     : " << author    << "\n";
    cout << "  NXB         : " << publisher << "\n";
    cout << "  Nam xuat ban: " << year      << "\n";
    cout << "  The loai    : " << category  << "\n";
    cout << "  Vi tri keo  : " << location  << "\n";
    cout << "  So luong    : " << quantity  << "\n";
    cout << "  Con lai     : " << available << "\n";
    cout << "  Trang thai  : " << (isAvailable() ? "Con sach" : "Het sach") << "\n";
}

void Book::displaySummary() const {
    cout << left
         << setw(5)  << id
         << setw(30) << title.substr(0, 28)
         << setw(20) << author.substr(0, 18)
         << setw(15) << category.substr(0, 13)
         << setw(8)  << quantity
         << setw(8)  << available
         << (isAvailable() ? "Con sach" : "Het sach") << "\n";
}
