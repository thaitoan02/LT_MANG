#include "library.h"

// ==========================================
// QUAN LY TAI KHOAN VA MENU CHINH
// ==========================================
// Cac chuc nang:
// - listUsers(): Hien thi danh sach tat ca tai khoan
// - lockUser(): Khoa tai khoan
// - unlockUser(): Mo khoa tai khoan
// - addStaffAccount(): Them tai khoan nhan vien (Thu Thu, Admin)
// - manageUser(): Menu quan ly tai khoan
// - menuReader(): Menu cho Doc Gia
// - menuLibrarian(): Menu cho Thu Thu
// - menuAdmin(): Menu cho Admin

// HAM: HIEN THI DANH SACH TAI KHOAN
void listUsers() {
    clearScreen();
    cout << "         DANH SACH TAI KHOAN\n";
    cout << left
         << setw(5)  << "ID"
         << setw(15) << "Username"
         << setw(25) << "Ho ten"
         << setw(12) << "Vai tro"
         << "Trang thai\n";
    cout << string(65, '-') << "\n";

    for (auto& u : gUsers) {
        cout << left
             << setw(5)  << u.id
             << setw(15) << u.username.substr(0, 13)
             << setw(25) << u.fullname.substr(0, 23)
             << setw(12) << u.role
             << u.status << "\n";
    }
    cout << "\nTong: " << gUsers.size() << " tai khoan.\n";
    pause();
}

void lockUser() {
    clearScreen();
    cout << "          KHOA TAI KHOAN\n";
    cout << "  Nhap ID tai khoan can khoa: ";
    int id; cin >> id;

    for (auto& u : gUsers) {
        if (u.id == id) {
            // Khong the tu khoa tai khoan cua minh
            if (u.id == gCurrentUser->id) {
                cout << "\n  Khong the tu khoa tai khoan cua chinh minh!\n";
                pause();
                return;
            }
            u.status = "locked";
            saveUsers();
            cout << "\n  Da khoa tai khoan: " << u.username << "\n";
            pause();
            return;
        }
    }
    cout << "\n  Khong tim thay tai khoan!\n";
    pause();
}

// HAM: MO KHOA TAI KHOAN
void unlockUser() {
    clearScreen();
    cout << "          MO KHOA TAI KHOAN\n";

    cout << "  Nhap ID tai khoan can mo khoa: ";
    int id; cin >> id;

    for (auto& u : gUsers) {
        if (u.id == id) {
            u.status = "active";
            saveUsers();
            cout << "\n  Da mo khoa tai khoan: " << u.username << "\n";
            pause();
            return;
        }
    }
    cout << "\n  Khong tim thay tai khoan!\n";
    pause();
}

void addStaffAccount() {
    clearScreen();
    cout << "       THEM TAI KHOAN NHAN VIEN\n";

    cin.ignore();
    User u;
    // Tao ID moi
    int maxId = 0;
    for (auto& usr : gUsers) if (usr.id > maxId) maxId = usr.id;
    u.id     = maxId + 1;
    u.status = "active";

    cout << "  Ho ten    : "; getline(cin, u.fullname);
    cout << "  Username  : "; getline(cin, u.username);

    // Kiem tra username trung
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
    cout << "  Vai tro (librarian/admin): "; getline(cin, u.role);

    // Kiem tra vai tro hop le
    if (u.role != "librarian" && u.role != "admin") {
        cout << "\n  Vai tro khong hop le!\n";
        pause();
        return;
    }

    gUsers.push_back(u);
    saveUsers();
    cout << "\n  Them tai khoan thanh cong! ID = " << u.id << "\n";
    pause();
}

void manageUser() {
    int choice;
    do {
        clearScreen();
        cout << "         QUAN LY TAI KHOAN\n";
        cout << "  1. Xem danh sach tai khoan\n";
        cout << "  2. Them tai khoan nhan vien\n";
        cout << "  3. Khoa tai khoan\n";
        cout << "  4. Mo khoa tai khoan\n";
        cout << "  0. Quay lai\n";
        cout << "  Chon: ";
        cin >> choice;

        switch (choice) {
            case 1: listUsers();      break;
            case 2: addStaffAccount(); break;
            case 3: lockUser();       break;
            case 4: unlockUser();     break;
        }
    } while (choice != 0);
}

// ===== MENU CHO TAC DOC GIA =====
void menuReader() {
    int choice;
    do {
        clearScreen;
        cout << "    MENU DOC GIA: " << gCurrentUser->fullname << "\n";
        cout << "  1. Tim kiem sach\n";
        cout << "  2. Xem danh sach sach\n";
        cout << "  3. Xem lich su muon cua toi\n";
        cout << "  4. Doi mat khau\n";
        cout << "  0. Dang xuat\n";
        cout << "  Chon: ";
        cin >> choice;

        switch (choice) {
            case 1: searchBook();          break;
            case 2: listBooks();           break;
            case 3: viewBorrowHistory();   break;
            case 4: changePassword();      break;
        }
    } while (choice != 0);
    logout();
}

// ===== MENU CHO THU THU =====
void menuLibrarian() {
    int choice;
    do {
        clearScreen();
        cout << "    MENU THU THU: " << gCurrentUser->fullname << "\n";
        cout << "  1. Quan ly sach\n";
        cout << "  2. Quan ly doc gia\n";
        cout << "  3. Quan ly muon/tra sach\n";
        cout << "  4. Quan ly tien phat\n";
        cout << "  5. Doi mat khau\n";
        cout << "  0. Dang xuat\n";
        cout << "  Chon: ";
        cin >> choice;

        switch (choice) {
            case 1: manageBook();   break;
            case 2: manageReader(); break;
            case 3: manageBorrow(); break;
            case 4: manageFine();   break;
            case 5: changePassword(); break;
        }
    } while (choice != 0);
    logout();
}

// ===== MENU CHO ADMIN =====
void menuAdmin() {
    int choice;
    do {
        clearScreen();
        cout << "    MENU ADMIN: " << gCurrentUser->fullname << "\n";
        cout << "  1. Quan ly tai khoan\n";
        cout << "  2. Quan ly sach\n";
        cout << "  3. Quan ly doc gia\n";
        cout << "  4. Xem bao cao thong ke\n";
        cout << "  5. Doi mat khau\n";
        cout << "  0. Dang xuat\n";
        cout << "  Chon: ";
        cin >> choice;

        switch (choice) {
            case 1: manageUser();   break;
            case 2: manageBook();   break;
            case 3: manageReader(); break;
            case 4: viewReport();   break;
            case 5: changePassword(); break;
        }
    } while (choice != 0);
    logout();
}
