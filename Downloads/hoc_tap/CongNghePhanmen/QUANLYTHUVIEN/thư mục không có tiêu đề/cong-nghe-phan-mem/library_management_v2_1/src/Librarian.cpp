#include "Librarian.h"
#include "Utils.h"
#include "Constants.h"
#include <iostream>
#include <iomanip>
using namespace std;

// ==========================================
// IMPLEMENTS: Librarian
// ==========================================

Librarian::Librarian() {}

Librarian::Librarian(int _id, const string& _username, const string& _password,
                     const string& _status, const string& _fullname, const string& _email)
    : User(_id, _username, _password, "librarian", _status, _fullname, _email) {}

// ---------- QUAN LY SACH ----------

void Librarian::addBook(vector<Book>& books) {
    clearScreen();
    cout << "\n            THEM SACH MOI\n\n";
    cin.ignore();

    Book b;
    int maxId = 0;
    for (const auto& bk : books) if (bk.getId() > maxId) maxId = bk.getId();
    b.setId(maxId + 1);

    string input;
    cout << "  Ten sach     : "; getline(cin, input); b.setTitle(input);
    cout << "  Tac gia      : "; getline(cin, input); b.setAuthor(input);
    cout << "  NXB          : "; getline(cin, input); b.setPublisher(input);

    int year; cout << "  Nam xuat ban : "; cin >> year; b.setYear(year);
    cin.ignore();

    cout << "  The loai     : "; getline(cin, input); b.setCategory(input);

    int qty; cout << "  So luong     : "; cin >> qty;
    b.setQuantity(qty);
    b.setAvailable(qty);
    cin.ignore();

    cout << "  Vi tri keo   : "; getline(cin, input); b.setLocation(input);

    books.push_back(b);
    cout << "\n  Them sach thanh cong! ID = " << b.getId() << "\n";
}

void Librarian::updateBook(vector<Book>& books) {
    clearScreen();
    cout << "\n         CAP NHAT THONG TIN SACH\n\n";
    cout << "  Nhap ID sach can sua: ";
    int id; cin >> id;

    Book* found = nullptr;
    for (auto& b : books) if (b.getId() == id) { found = &b; break; }

    if (!found) {
        cout << "\n  Khong tim thay sach!\n"; pauseScreen(); return;
    }

    cout << "  Sach hien tai: " << found->getTitle() << "\n\n";
    cin.ignore();

    string input;
    cout << "  Ten moi (Enter = giu nguyen)  : "; getline(cin, input);
    if (!input.empty()) found->setTitle(input);

    cout << "  Tac gia moi                   : "; getline(cin, input);
    if (!input.empty()) found->setAuthor(input);

    cout << "  NXB moi                       : "; getline(cin, input);
    if (!input.empty()) found->setPublisher(input);

    cout << "  The loai moi                  : "; getline(cin, input);
    if (!input.empty()) found->setCategory(input);

    cout << "  Vi tri keo moi                : "; getline(cin, input);
    if (!input.empty()) found->setLocation(input);

    cout << "  So luong them (0 = khong doi) : ";
    int addQty; cin >> addQty;
    if (addQty > 0) {
        found->setQuantity(found->getQuantity() + addQty);
        found->setAvailable(found->getAvailable() + addQty);
    }

    cout << "\n  Cap nhat thanh cong!\n";
}

void Librarian::deleteBook(vector<Book>& books, const vector<BorrowSlip>& borrows) {
    clearScreen();
    cout << "\n             XOA SACH\n\n";
    cout << "  Nhap ID sach can xoa: ";
    int id; cin >> id;

    for (int i = 0; i < (int)books.size(); i++) {
        if (books[i].getId() == id) {
            // Kiem tra con cuon nao dang duoc muon
            if (books[i].getAvailable() < books[i].getQuantity()) {
                cout << "\n  Sach dang duoc muon, khong the xoa!\n";
                pauseScreen(); return;
            }
            cout << "  Xac nhan xoa sach \"" << books[i].getTitle() << "\"? (y/n): ";
            char c; cin >> c;
            if (c == 'y' || c == 'Y') {
                books.erase(books.begin() + i);
                cout << "\n  Da xoa sach!\n";
            } else {
                cout << "\n  Huy bo.\n";
            }
            pauseScreen(); return;
        }
    }
    cout << "\n  Khong tim thay sach ID " << id << "\n";
    pauseScreen();
}

// ---------- QUAN LY MUON / TRA ----------

void Librarian::createBorrowSlip(vector<BorrowSlip>& borrows,
                                  vector<Book>& books,
                                  const vector<User*>& users) {
    clearScreen();
    cout << "          TAO PHIEU MUON SACH\n";
    cout << "  Nhap ID doc gia: ";
    int readerId; cin >> readerId;

    // Kiem tra doc gia hop le
    User* reader = nullptr;
    for (auto u : users)
        if (u->getId() == readerId && u->getRole() == "reader") { reader = u; break; }

    if (!reader) {
        cout << "\n  Khong tim thay doc gia!\n"; pauseScreen(); return;
    }
    if (reader->isLocked()) {
        cout << "\n  Tai khoan doc gia bi khoa!\n"; pauseScreen(); return;
    }

    // Kiem tra con phieu chua tra
    int currentBorrow = 0;
    for (const auto& s : borrows)
        if (s.getReaderId() == readerId && s.isBorrowing()) currentBorrow++;

    if (currentBorrow > 0) {
        cout << "\n  Doc gia dang co " << currentBorrow << " phieu chua tra!\n";
        cout << "  Vui long tra sach truoc khi muon them.\n";
        pauseScreen(); return;
    }

    cout << "  Doc gia: " << reader->getFullname() << "\n\n";

    // Tao phieu moi
    BorrowSlip slip;
    int maxId = 0;
    for (const auto& s : borrows) if (s.getId() > maxId) maxId = s.getId();
    slip.setId(maxId + 1);
    slip.setReaderId(readerId);
    slip.setBorrowDate(getCurrentDate());
    slip.setDueDate(addDays(slip.getBorrowDate(), 14));
    slip.setStatus("borrowing");

    // Them sach vao phieu
    int totalBooks = 0;
    while (true) {
        cout << "  Nhap ID sach can muon (0 = xong): ";
        int bookId; cin >> bookId;
        if (bookId == 0) break;

        Book* book = nullptr;
        for (auto& b : books) if (b.getId() == bookId) { book = &b; break; }

        if (!book) { cout << "  Khong tim thay sach!\n"; continue; }
        if (!book->isAvailable()) {
            cout << "  Sach \"" << book->getTitle() << "\" da het!\n"; continue;
        }

        cout << "  So luong muon (con lai: " << book->getAvailable() << "): ";
        int qty; cin >> qty;

        if (qty <= 0 || qty > book->getAvailable()) {
            cout << "  So luong khong hop le!\n"; continue;
        }
        if (totalBooks + qty > MAX_BORROW) {
            cout << "  Vuot qua gioi han " << MAX_BORROW << " cuon!\n"; continue;
        }

        slip.addDetail(BorrowDetail(bookId, qty));
        book->borrowCopy(qty);
        totalBooks += qty;
        cout << "  Da them: " << book->getTitle() << " x" << qty << "\n";
    }

    if (slip.getDetails().empty()) {
        // Hoan lai so luong (neu co)
        cout << "\n  Khong co sach nao duoc chon. Huy phieu.\n";
        pauseScreen(); return;
    }

    borrows.push_back(slip);
    cout << "\n     PHIEU MUON #" << slip.getId() << " DA DUOC TAO\n";
    cout << "  Doc gia  : " << reader->getFullname() << "\n";
    cout << "  Ngay muon: " << slip.getBorrowDate()  << "\n";
    cout << "  Han tra  : " << slip.getDueDate()      << "\n";
}

void Librarian::confirmReturn(vector<BorrowSlip>& borrows,
                               vector<Book>& books,
                               vector<Fine>& fines) {
    clearScreen();
    cout << "           XAC NHAN TRA SACH\n";
    cout << "  Nhap ID phieu muon: ";
    int slipId; cin >> slipId;

    BorrowSlip* slip = nullptr;
    for (auto& s : borrows) if (s.getId() == slipId) { slip = &s; break; }

    if (!slip) {
        cout << "\n  Khong tim thay phieu muon!\n"; pauseScreen(); return;
    }
    if (!slip->isBorrowing()) {
        cout << "\n  Phieu nay da duoc tra roi!\n"; pauseScreen(); return;
    }

    string today = getCurrentDate();
    slip->setReturnDate(today);
    slip->setStatus("returned");

    // Hoan tra so luong sach
    for (const auto& d : slip->getDetails())
        for (auto& b : books)
            if (b.getId() == d.getBookId()) { b.returnCopy(d.getQuantity()); break; }

    // Kiem tra qua han
    int lateDays = slip->calculateLateDays(today);
    if (lateDays > 0) {
        slip->setStatus("overdue");

        Fine fine;
        int maxFineId = 0;
        for (const auto& f : fines) if (f.getId() > maxFineId) maxFineId = f.getId();
        fine.setId(maxFineId + 1);
        fine.setBorrowId(slip->getId());
        fine.setReaderId(slip->getReaderId());
        fine.setAmount(slip->calculateFine(today));
        fine.setReason("Tra sach tre " + to_string(lateDays) + " ngay");
        fine.setStatus("unpaid");
        fine.setCreatedDate(today);
        fines.push_back(fine);

        cout << "\n  !!! TRA TRE " << lateDays << " NGAY !!!\n";
        cout << "  Tien phat: " << fixed << setprecision(0)
             << fine.getAmount() << " VND\n";
        cout << "  (Phi: " << (int)FINE_PER_DAY << " VND/ngay)\n";
    } else {
        cout << "\n  Tra sach dung han!\n";
    }
    cout << "\n  Xac nhan tra sach thanh cong!\n";
}

void Librarian::displayInfo() const {
    User::displayInfo();
}
