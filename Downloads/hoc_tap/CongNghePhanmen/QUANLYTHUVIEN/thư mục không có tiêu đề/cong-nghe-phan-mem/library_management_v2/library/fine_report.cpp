#include "library.h"
#include <map>

// ==========================================
// QUAN LY TIEN PHAT VA BAO CAO THONG KE
// ==========================================
// PHAN 1: QUAN LY TIEN PHAT
// - listFines(): Hien thi danh sach tien phat
// - payFine(): Xac nhan thanh toan tien phat
// - manageFine(): Menu quan ly tien phat
//
// PHAN 2: BAO CAO THONG KE
// - reportBookStat(): Thong ke trang thai sach
// - reportOverdue(): Bao cao sach qua han tra
// - reportTopBorrower(): Top 10 nguoi muon nhieu nhat
// - viewReport(): Menu bao cao

// HAM HO TRO: Import tu borrow.cpp
extern string getReaderName(int readerId);
extern string getBookTitle(int bookId);

// ===== PHAN 1: QUAN LY TIEN PHAT =====

// HAM: HIEN THI DANH SACH TIEN PHAT
void listFines() {
    clearScreen();
    cout << "          DANH SACH TIEN PHAT\n";
    cout << left
         << setw(5)  << "ID"
         << setw(8)  << "BorrowID"
         << setw(20) << "Doc gia"
         << setw(15) << "So tien"
         << setw(30) << "Ly do"
         << "Trang thai\n";
    cout << string(90, '-') << "\n";

    double totalUnpaid = 0;
    for (auto& f : gFines) {
        string readerName = getReaderName(f.readerId);
        cout << left
             << setw(5)  << f.id
             << setw(8)  << f.borrowId
             << setw(20) << readerName.substr(0, 18)
             << setw(15) << fixed << setprecision(0) << f.amount
             << setw(30) << f.reason.substr(0, 28)
             << f.status << "\n";
        if (f.status == "unpaid") totalUnpaid += f.amount;
    }

    cout << "\nTong no chua thanh toan: " << fixed << setprecision(0) << totalUnpaid << " VND\n";
    pause();
}

void payFine() {
    clearScreen();
    cout << "        XAC NHAN THANH TOAN PHAT\n";
    // Hien thi danh sach tien phat chua thanh toan
    cout << "  Cac khoan phat chua thanh toan:\n";
    bool hasUnpaid = false;
    for (auto& f : gFines) {
        if (f.status == "unpaid") {
            cout << "  ID " << f.id
                 << " | Doc gia: " << getReaderName(f.readerId)
                 << " | " << f.reason
                 << " | " << fixed << setprecision(0) << f.amount << " VND\n";
            hasUnpaid = true;
        }
    }

    if (!hasUnpaid) {
        cout << "  Khong co khoan phat nao chua thanh toan.\n";
        pause();
        return;
    }

    cout << "\n  Nhap ID phieu phat can thanh toan: ";
    int fineId; cin >> fineId;

    for (auto& f : gFines) {
        if (f.id == fineId) {
            if (f.status == "paid") {
                cout << "\n  Khoan phat nay da duoc thanh toan roi!\n";
            } else {
                cout << "  Xac nhan thanh toan " << fixed << setprecision(0)
                     << f.amount << " VND? (y/n): ";
                char c; cin >> c;
                if (c == 'y' || c == 'Y') {
                    f.status = "paid";
                    saveFines();
                    cout << "\n  Thanh toan thanh cong!\n";
                } else {
                    cout << "\n  Huy bo.\n";
                }
            }
            pause();
            return;
        }
    }
    cout << "\n  Khong tim thay phieu phat!\n";
    pause();
}

// HAM: MENU QUAN LY TIEN PHAT
void manageFine() {
    int choice;
    do {
        clearScreen();
        cout << "         QUAN LY TIEN PHAT\n";
        cout << "  1. Xem danh sach tien phat\n";
        cout << "  2. Xac nhan thanh toan\n";
        cout << "  0. Quay lai\n";
        cout << "  Chon: ";
        cin >> choice;

        switch (choice) {
            case 1: listFines(); break;
            case 2: payFine();   break;
        }
    } while (choice != 0);
}

// ===== PHAN 2: BAO CAO THONG KE =====

// HAM: BAO CAO TINH TRANG SACH
// Thong ke tong so dau sach, cuon da muon, chua muon, phieu muon
void reportBookStat() {
    clearScreen();
    cout << "       BAO CAO TINH TRANG SACH\n";

    int totalBooks = 0, totalAvailable = 0, totalBorrowed = 0;
    for (auto& b : gBooks) {
        totalBooks     += b.quantity;
        totalAvailable += b.available;
        totalBorrowed  += (b.quantity - b.available);
    }

    cout << "  Tong so dau sach     : " << gBooks.size()   << "\n";
    cout << "  Tong so cuon sach    : " << totalBooks       << "\n";
    cout << "  Sach dang co         : " << totalAvailable   << "\n";
    cout << "  Sach dang duoc muon  : " << totalBorrowed    << "\n";
    cout << "  Tong phieu muon      : " << gBorrows.size()  << "\n";

    int borrowing = 0;
    for (auto& s : gBorrows)
        if (s.status == "borrowing" || s.status == "overdue") borrowing++;
    cout << "  Phieu chua tra       : " << borrowing << "\n";
    pause();
}

// HAM: BAO CAO SACH QUA HAN TRA
// Hien thi cac phieu muon co ngay tra > han tra
void reportOverdue() {
    clearScreen();
    cout << "         SACH QUA HAN TRA\n";

    string today = getCurrentDate();
    int count = 0;

    cout << left
         << setw(6)  << "PhieuID"
         << setw(20) << "Doc gia"
         << setw(12) << "Han tra"
         << "So ngay tre\n";
    cout << string(55, '-') << "\n";

    for (auto& slip : gBorrows) {
        if (slip.status != "borrowing") continue;
        int late = daysBetween(slip.dueDate, today);
        if (late > 0) {
            cout << left
                 << setw(6)  << slip.id
                 << setw(20) << getReaderName(slip.readerId).substr(0, 18)
                 << setw(12) << slip.dueDate
                 << late << " ngay\n";
            count++;
        }
    }

    if (count == 0) cout << "  Khong co sach qua han!\n";
    else cout << "\nTong: " << count << " phieu qua han.\n";
    pause();
}

// HAM: BAO CAO TOP 10 NGUOI MUON NHIEU NHAT
// Dem so lan muon cua tung doc gia va sap xep giam dan
void reportTopBorrower() {
    clearScreen();
    cout << "      TOP NGUOI MUON NHIEU NHAT\n";

    // Đếm số lần mượn theo readerId
    map<int, int> borrowCount;
    for (auto& slip : gBorrows) {
        borrowCount[slip.readerId]++;
    }

    // Sắp xếp giảm dần
    vector<pair<int, int>> sorted(borrowCount.begin(), borrowCount.end());
    sort(sorted.begin(), sorted.end(), [](const pair<int,int>& a, const pair<int,int>& b) {
        return a.second > b.second;
    });

    int rank = 1;
    cout << left
         << setw(5) << "Top"
         << setw(25) << "Doc gia"
         << "So lan muon\n";
    cout << string(40, '-') << "\n";

    for (auto& p : sorted) {
        if (rank > 10) break;
        cout << left
             << setw(5)  << rank
             << setw(25) << getReaderName(p.first).substr(0, 23)
             << p.second << "\n";
        rank++;
    }

    if (sorted.empty()) cout << "  Chua co du lieu.\n";
    pause();
}

// HAM: MENU BAO CAO THONG KE
void viewReport() {
    int choice;
    do {
        clearScreen();
        cout << "          BAO CAO THONG KE\n";
        cout << "  1. Tinh trang sach\n";
        cout << "  2. Sach qua han tra\n";
        cout << "  3. Top nguoi muon nhieu nhat\n";
        cout << "  0. Quay lai\n";
        cout << "  Chon: ";
        cin >> choice;

        switch (choice) {
            case 1: reportBookStat();    break;
            case 2: reportOverdue();     break;
            case 3: reportTopBorrower(); break;
        }
    } while (choice != 0);
}
