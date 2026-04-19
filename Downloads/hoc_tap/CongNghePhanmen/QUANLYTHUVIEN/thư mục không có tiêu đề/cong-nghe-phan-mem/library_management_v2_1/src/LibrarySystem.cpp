#include "LibrarySystem.h"
#include "FileManager.h"
#include "Utils.h"
#include "Constants.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <map>
#include <sys/stat.h>
using namespace std;

// ==========================================
// IMPLEMENTS: LibrarySystem
// ==========================================

LibrarySystem::LibrarySystem() : currentUser(nullptr) {}

LibrarySystem::~LibrarySystem() {
    for (auto u : users) delete u;
}

// ========== HELPER ==========

User* LibrarySystem::findUserByUsername(const string& username) {
    for (auto u : users)
        if (u->getUsername() == username) return u;
    return nullptr;
}

User* LibrarySystem::findUserById(int id) {
    for (auto u : users)
        if (u->getId() == id) return u;
    return nullptr;
}

string LibrarySystem::getReaderName(int readerId) const {
    for (auto u : users)
        if (u->getId() == readerId) return u->getFullname();
    return "Khong ro";
}

string LibrarySystem::getBookTitle(int bookId) const {
    for (const auto& b : books)
        if (b.getId() == bookId) return b.getTitle();
    return "Khong ro";
}

// ========== KHOI TAO DU LIEU ==========

void LibrarySystem::initDefaultData() {
    // Neu file users da ton tai -> da khoi tao roi, bo qua
    ifstream check(DATA_USERS);
    if (check.is_open()) { check.close(); return; }

    // Tao thu muc data
#ifdef _WIN32
    system("mkdir data 2>nul");
#else
    mkdir("data", 0777);
#endif

    // 3 tai khoan mac dinh
    users.push_back(new Admin    (1, "admin",   hashPassword("admin123"), "active", "Quan Tri Vien",  "admin@library.com"));
    users.push_back(new Librarian(2, "thuthu",  hashPassword("thu123"),   "active", "Thu Thu Nguyen", "thuthu@library.com"));
    users.push_back(new Reader   (3, "docgia1", hashPassword("doc123"),   "active", "Nguyen Van A",   "docgia1@gmail.com"));
    users.push_back(new Reader   (4, "docgia2", hashPassword("doc123"),   "active", "Tran Thi B",     "docgia2@gmail.com"));
    users.push_back(new Reader   (5, "docgia3", hashPassword("doc123"),   "locked", "Le Van C",       "docgia3@gmail.com"));

    // 8 sach mau
    auto mk = [](int id, string ti, string au, string pub, int yr,
                 string cat, int qty, int avl, string loc) {
        Book b;
        b.setId(id); b.setTitle(ti); b.setAuthor(au); b.setPublisher(pub);
        b.setYear(yr); b.setCategory(cat); b.setQuantity(qty);
        b.setAvailable(avl); b.setLocation(loc);
        return b;
    };
    books.push_back(mk(1,"Lap Trinh C++ Co Ban",          "Bjarne Stroustrup","NXB KHKT",2020,"Lap trinh",5,4,"A1-01"));
    books.push_back(mk(2,"Co So Du Lieu",                 "Ramez Elmasri",   "NXB GD",  2018,"CSDL",     3,2,"A1-02"));
    books.push_back(mk(3,"Giai Tich 1",                   "Nguyen Dinh Tri", "NXB DHQG",2019,"Toan hoc", 4,4,"B2-01"));
    books.push_back(mk(4,"Tri Tue Nhan Tao",              "Stuart Russell",  "NXB KHKT",2021,"AI",       2,1,"A2-01"));
    books.push_back(mk(5,"Cau Truc Du Lieu & Giai Thuat", "Thomas Cormen",   "NXB DHQG",2017,"Lap trinh",3,3,"A1-03"));
    books.push_back(mk(6,"Lap Trinh Python",              "Eric Matthes",    "NXB KHKT",2022,"Lap trinh",4,4,"A1-04"));
    books.push_back(mk(7,"Machine Learning Co Ban",       "Vu Huu Tiep",     "NXB KHKT",2021,"AI",       3,3,"A2-02"));
    books.push_back(mk(8,"Giao Tiep Hieu Qua",            "Dale Carnegie",   "NXB Tre", 2018,"Ky nang",  5,5,"C1-01"));

    // 3 phieu muon mau
    {
        BorrowSlip s1;
        s1.setId(1); s1.setReaderId(3);
        s1.setBorrowDate("01/04/2026"); s1.setDueDate("15/04/2026");
        s1.setReturnDate(""); s1.setStatus("borrowing");
        s1.addDetail(BorrowDetail(1, 1));
        s1.addDetail(BorrowDetail(4, 1));
        borrows.push_back(s1);

        BorrowSlip s2;
        s2.setId(2); s2.setReaderId(4);
        s2.setBorrowDate("10/03/2026"); s2.setDueDate("24/03/2026");
        s2.setReturnDate("05/04/2026"); s2.setStatus("returned");
        s2.addDetail(BorrowDetail(2, 1));
        borrows.push_back(s2);

        BorrowSlip s3;
        s3.setId(3); s3.setReaderId(3);
        s3.setBorrowDate("01/03/2026"); s3.setDueDate("15/03/2026");
        s3.setReturnDate("20/03/2026"); s3.setStatus("overdue");
        s3.addDetail(BorrowDetail(2, 1));
        borrows.push_back(s3);
    }

    // 1 phieu phat mau
    {
        Fine f;
        f.setId(1); f.setBorrowId(3); f.setReaderId(3);
        f.setAmount(10000); f.setReason("Tra sach tre 5 ngay");
        f.setStatus("unpaid"); f.setCreatedDate("20/03/2026");
        fines.push_back(f);
    }

    FileManager::saveAll(users, books, borrows, fines);

    cout << "\n   DU LIEU KHOI TAO LAN DAU\n";
    cout << "  Admin    : admin   / admin123\n";
    cout << "  Thu thu  : thuthu  / thu123\n";
    cout << "  Doc gia  : docgia1 / doc123\n\n";
    pauseScreen();
}

// ========== DANG NHAP / DANG XUAT ==========

bool LibrarySystem::doLogin() {
    clearScreen();
    cout << "            DANG NHAP\n";
    string username, password;
    cout << "  Username: "; cin >> username;
    cout << "  Password: "; cin >> password;

    User* u = findUserByUsername(username);
    if (!u || !u->login(password)) {
        cout << "\n  Sai username hoac password!\n";
        return false;
    }
    if (u->isLocked()) {
        cout << "\n  Tai khoan da bi khoa! Lien he Admin.\n";
        pauseScreen(); return false;
    }

    currentUser = u;
    cout << "\n  Dang nhap thanh cong! Xin chao, " << u->getFullname() << "!\n";
    pauseScreen();
    return true;
}

void LibrarySystem::registerAccount() {
    clearScreen();
    cout << "\n          DANG KY TAI KHOAN\n\n";
    cout << "  1. Doc gia\n  2. Thu thu\n  0. Quay lai\n";
    cout << "  Chon: ";
    int choice; cin >> choice;
    if (choice == 0) return;
    if (choice != 1 && choice != 2) {
        cout << "\n  Lua chon khong hop le!\n"; pauseScreen(); return;
    }

    cin.ignore();
    string fullname, username, pass, confirm, email;

    cout << "  Ho ten day du : "; getline(cin, fullname);
    if (fullname.empty()) { cout << "\n  Ho ten khong duoc de trong!\n"; pauseScreen(); return; }

    cout << "  Username      : "; getline(cin, username);
    if (username.empty()) { cout << "\n  Username khong duoc de trong!\n"; pauseScreen(); return; }
    if (findUserByUsername(username)) {
        cout << "\n  Username \"" << username << "\" da ton tai!\n"; pauseScreen(); return;
    }

    cout << "  Password      : "; getline(cin, pass);
    if (pass.length() < 4) { cout << "\n  Password phai co it nhat 4 ky tu!\n"; pauseScreen(); return; }
    cout << "  Nhap lai pass : "; getline(cin, confirm);
    if (pass != confirm) { cout << "\n  Password khong khop!\n"; pauseScreen(); return; }

    cout << "  Email         : "; getline(cin, email);

    int maxId = 0;
    for (auto u : users) if (u->getId() > maxId) maxId = u->getId();

    User* newUser = nullptr;
    if (choice == 1)
        newUser = new Reader   (maxId+1, username, hashPassword(pass), "active", fullname, email);
    else
        newUser = new Librarian(maxId+1, username, hashPassword(pass), "active", fullname, email);

    users.push_back(newUser);
    FileManager::saveUsers(users);

    cout << "\n  DANG KY THANH CONG!\n";
    cout << "  Vai tro  : " << (choice==1 ? "Doc gia" : "Thu thu") << "\n";
    cout << "  Username : " << username << "\n";
    pauseScreen();
}

bool LibrarySystem::login() {
    while (true) {
        clearScreen();
        cout << "      HE THONG QUAN LY THU VIEN\n\n";
        cout << "  1. Dang nhap\n";
        cout << "  2. Dang ky tai khoan moi\n";
        cout << "  0. Thoat\n\n";
        cout << "  Chon: ";
        int choice; cin >> choice;

        if (choice == 0) return false;
        if (choice == 2) { registerAccount(); continue; }
        if (choice != 1) { cout << "\n  Lua chon khong hop le!\n"; pauseScreen(); continue; }

        int attempts = 0;
        while (attempts < 3) {
            if (doLogin()) return true;
            attempts++;
            if (attempts < 3) {
                cout << "  Con " << (3-attempts) << " lan thu lai.\n";
                pauseScreen();
            }
        }
        cout << "\n  Nhap sai qua 3 lan. Quay lai menu.\n";
        pauseScreen();
    }
}

// ========== DOI MAT KHAU (dung chung) ==========

void LibrarySystem::changePasswordMenu() {
    clearScreen();
    cout << "           DOI MAT KHAU\n";
    string oldPass, newPass, confirmPass;
    cout << "  Mat khau cu  : "; cin >> oldPass;
    cout << "  Mat khau moi : "; cin >> newPass;
    cout << "  Xac nhan lai : "; cin >> confirmPass;

    if (newPass != confirmPass)      { cout << "\n  Mat khau moi khong khop!\n"; }
    else if (newPass.length() < 4)   { cout << "\n  Mat khau phai co it nhat 4 ky tu!\n"; }
    else if (!currentUser->changePassword(oldPass, newPass)) { cout << "\n  Mat khau cu khong dung!\n"; }
    else {
        FileManager::saveUsers(users);
        cout << "\n  Doi mat khau thanh cong!\n";
    }
    pauseScreen();
}

// ========== HIEN THI DANH SACH ==========

void LibrarySystem::listBooks() const {
    clearScreen();
    cout << "\n          DANH SACH SACH\n\n";
    cout << left
         << setw(5)  << "ID"
         << setw(30) << "Ten sach"
         << setw(20) << "Tac gia"
         << setw(15) << "The loai"
         << setw(8)  << "SL"
         << setw(8)  << "Con lai"
         << "Trang thai\n";
    cout << string(90, '-') << "\n";
    for (const auto& b : books) b.displaySummary();
    cout << "\nTong: " << books.size() << " cuon sach.\n";
    pauseScreen();
}

void LibrarySystem::listBorrows() const {
    clearScreen();
    cout << "       DANH SACH PHIEU MUON\n";
    cout << left
         << setw(6)  << "ID"
         << setw(20) << "Doc gia"
         << setw(12) << "Ngay muon"
         << setw(12) << "Han tra"
         << setw(12) << "Ngay tra"
         << "Trang thai\n";
    cout << string(80, '-') << "\n";
    for (const auto& slip : borrows) {
        cout << left
             << setw(6)  << slip.getId()
             << setw(20) << getReaderName(slip.getReaderId()).substr(0,18)
             << setw(12) << slip.getBorrowDate()
             << setw(12) << slip.getDueDate()
             << setw(12) << (slip.getReturnDate().empty() ? "---" : slip.getReturnDate())
             << slip.getStatus() << "\n";
    }
    cout << "\nTong: " << borrows.size() << " phieu.\n";
    pauseScreen();
}

void LibrarySystem::listReaders() const {
    clearScreen();
    cout << "          DANH SACH DOC GIA\n";
    cout << left
         << setw(5)  << "ID"
         << setw(15) << "Username"
         << setw(25) << "Ho ten"
         << setw(25) << "Email"
         << "Trang thai\n";
    cout << string(80, '-') << "\n";
    int count = 0;
    for (auto u : users) {
        if (u->getRole() != "reader") continue;
        cout << left
             << setw(5)  << u->getId()
             << setw(15) << u->getUsername().substr(0,13)
             << setw(25) << u->getFullname().substr(0,23)
             << setw(25) << u->getEmail().substr(0,23)
             << u->getStatus() << "\n";
        count++;
    }
    cout << "\nTong: " << count << " doc gia.\n";
    pauseScreen();
}

// ========== BAO CAO ==========

void LibrarySystem::reportBookStat() const {
    clearScreen();
    cout << "       BAO CAO TINH TRANG SACH\n\n";
    int total=0, avail=0, borrowed=0;
    for (const auto& b : books) {
        total    += b.getQuantity();
        avail    += b.getAvailable();
        borrowed += b.getQuantity() - b.getAvailable();
    }
    int borrowing = 0;
    for (const auto& s : borrows)
        if (s.isBorrowing() || s.getStatus()=="overdue") borrowing++;

    cout << "  Tong so dau sach     : " << books.size()  << "\n";
    cout << "  Tong so cuon sach    : " << total          << "\n";
    cout << "  Sach dang co         : " << avail          << "\n";
    cout << "  Sach dang duoc muon  : " << borrowed       << "\n";
    cout << "  Tong phieu muon      : " << borrows.size() << "\n";
    cout << "  Phieu chua tra       : " << borrowing      << "\n";
    pauseScreen();
}

void LibrarySystem::reportOverdue() const {
    clearScreen();
    cout << "         SACH QUA HAN TRA\n\n";
    string today = getCurrentDate();
    int count = 0;
    cout << left
         << setw(7)  << "PhieuID"
         << setw(22) << "Doc gia"
         << setw(13) << "Han tra"
         << "So ngay tre\n";
    cout << string(55, '-') << "\n";
    for (const auto& slip : borrows) {
        if (!slip.isBorrowing()) continue;
        int late = daysBetween(slip.getDueDate(), today);
        if (late <= 0) continue;
        cout << left
             << setw(7)  << slip.getId()
             << setw(22) << getReaderName(slip.getReaderId()).substr(0,20)
             << setw(13) << slip.getDueDate()
             << late << " ngay\n";
        count++;
    }
    if (count == 0) cout << "  Khong co sach qua han!\n";
    else cout << "\nTong: " << count << " phieu qua han.\n";
    pauseScreen();
}

void LibrarySystem::reportTopBorrower() const {
    clearScreen();
    cout << "      TOP NGUOI MUON NHIEU NHAT\n\n";
    map<int,int> cnt;
    for (const auto& s : borrows) cnt[s.getReaderId()]++;

    vector<pair<int,int>> sorted(cnt.begin(), cnt.end());
    sort(sorted.begin(), sorted.end(),
         [](const pair<int,int>& a, const pair<int,int>& b){ return a.second > b.second; });

    cout << left << setw(5) << "Top" << setw(25) << "Doc gia" << "So lan muon\n";
    cout << string(40, '-') << "\n";
    int rank = 1;
    for (auto& p : sorted) {
        if (rank > 10) break;
        cout << left << setw(5) << rank
             << setw(25) << getReaderName(p.first).substr(0,23)
             << p.second << "\n";
        rank++;
    }
    if (sorted.empty()) cout << "  Chua co du lieu.\n";
    pauseScreen();
}
