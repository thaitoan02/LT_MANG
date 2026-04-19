#include "library.h"

// ==========================================
// QUAN LY DOC GIA
// ==========================================
// Cac chuc nang:
// - listReaders(): Hien thi danh sach tat ca doc gia
// - addReader(): Them doc gia moi
// - updateReader(): Sua thong tin doc gia
// - deleteReader(): Xoa doc gia
// - lockReader(): Khoa / Mo khoa tai khoan doc gia
// - manageReader(): Menu quan ly doc gia

// HAM: HIEN THI DANH SACH DOC GIA
void listReaders() {
    clearScreen();
    cout << "          DANH SACH DOC GIA\n";
    cout << left
         << setw(5)  << "ID"
         << setw(15) << "Username"
         << setw(25) << "Ho ten"
         << setw(25) << "Email"
         << "Trang thai\n";
    cout << string(80, '-') << "\n";

    // Dem va in danh sach doc gia
    int count = 0;
    for (auto& u : gUsers) {
        // Chi hien thi user co role = "reader"
        if (u.role != "reader") continue;
        cout << left
             << setw(5)  << u.id
             << setw(15) << u.username.substr(0, 13)
             << setw(25) << u.fullname.substr(0, 23)
             << setw(25) << u.email.substr(0, 23)
             << u.status << "\n";
        count++;
    }
    cout << "\nTong: " << count << " doc gia.\n";
    pause();
}

// HAM: THEM DOC GIA MOI
void addReader() {
    clearScreen();
    cout << "          THEM DOC GIA MOI\n";

    cin.ignore();
    User u;
    int maxId = 0;
    for (auto& usr : gUsers) if (usr.id > maxId) maxId = usr.id;
    u.id   = maxId + 1;
    u.role = "reader";
    u.status = "active";

    cout << "  Ho ten    : "; getline(cin, u.fullname);
    cout << "  Username  : "; getline(cin, u.username);

    // Kiểm tra trùng username
    for (auto& usr : gUsers) {
        if (usr.username == u.username) {
            cout << "\n  Username da ton tai!\n";
            pause();
            return;
        }
    }

    cout << "  Password  : ";
    string pass; getline(cin, pass);
    u.password = hashPassword(pass);
    cout << "  Email     : "; getline(cin, u.email);

    gUsers.push_back(u);
    saveUsers();

    cout << "\n  Them doc gia thanh cong! ID = " << u.id << "\n";
    pause();
}

void updateReader() {
    clearScreen();
    cout << "       CAP NHAT THONG TIN DOC GIA\n";

    cout << "  Nhap ID doc gia can sua: ";
    int id; cin >> id;

    // Tim doc gia theo ID
    User* found = nullptr;
    for (auto& u : gUsers) {
        if (u.id == id && u.role == "reader") { found = &u; break; }
    }

    if (!found) {
        cout << "\n  Khong tim thay doc gia!\n";
        pause();
        return;
    }

    cin.ignore();
    // Cap nhat thong tin (Enter = giu nguyen)
    cout << "  Ho ten moi (Enter = giu nguyen): ";
    string input;
    getline(cin, input);
    if (!input.empty()) found->fullname = input;

    cout << "  Email moi  : ";
    getline(cin, input);
    if (!input.empty()) found->email = input;

    saveUsers();
    cout << "\n  Cap nhat thanh cong!\n";
    pause();
}

// HAM: KHOA / MO KHOA TAI KHOAN DOC GIA
void lockReader() {
    clearScreen();
    cout << "         KHOA / MO KHOA DOC GIA\n";
    cout << "  Nhap ID doc gia: ";
    int id; cin >> id;

    for (auto& u : gUsers) {
        if (u.id == id && u.role == "reader") {
            if (u.status == "active") {
                cout << "  Xac nhan khoa tai khoan \"" << u.username << "\"? (y/n): ";
                char c; cin >> c;
                if (c == 'y' || c == 'Y') {
                    u.status = "locked";
                    saveUsers();
                    cout << "\n  Da khoa tai khoan!\n";
                }
            } else {
                cout << "  Mo khoa tai khoan \"" << u.username << "\"? (y/n): ";
                char c; cin >> c;
                if (c == 'y' || c == 'Y') {
                    u.status = "active";
                    saveUsers();
                    cout << "\n  Da mo khoa tai khoan!\n";
                }
            }
            pause();
            return;
        }
    }
    cout << "\n  Khong tim thay doc gia!\n";
    pause();
}

void deleteReader() {
    clearScreen();
    cout << "           XOA DOC GIA\n";

    cout << "  Nhap ID doc gia can xoa: ";
    int id; cin >> id;

    for (int i = 0; i < (int)gUsers.size(); i++) {
        if (gUsers[i].id == id && gUsers[i].role == "reader") {
            // Kiem tra doc gia co dang muon sach khong
            for (auto& slip : gBorrows) {
                if (slip.readerId == id && slip.status == "borrowing") {
                    cout << "\n  Doc gia dang muon sach, khong the xoa!\n";
                    pause();
                    return;
                }
            }
            // Xac nhan xoa
            cout << "  Xac nhan xoa doc gia \"" << gUsers[i].fullname << "\"? (y/n): ";
            char c; cin >> c;
            if (c == 'y' || c == 'Y') {
                gUsers.erase(gUsers.begin() + i);
                saveUsers();
                cout << "\n  Da xoa doc gia!\n";
            } else {
                cout << "\n  Huy bo.\n";
            }
            pause();
            return;
        }
    }
    cout << "\n  Khong tim thay doc gia!\n";
    pause();
}

// HAM: MENU QUAN LY DOC GIA
void manageReader() {
    int choice;
    do {
        clearScreen();
        cout << "          QUAN LY DOC GIA\n";
        cout << "  1. Xem danh sach doc gia\n";
        cout << "  2. Them doc gia moi\n";
        cout << "  3. Sua thong tin doc gia\n";
        cout << "  4. Khoa / Mo khoa tai khoan\n";
        cout << "  5. Xoa doc gia\n";
        cout << "  0. Quay lai\n";
        cout << "  Chon: ";
        cin >> choice;

        switch (choice) {
            case 1: listReaders();  break;
            case 2: addReader();    break;
            case 3: updateReader(); break;
            case 4: lockReader();   break;
            case 5: deleteReader(); break;
        }
    } while (choice != 0);
}
