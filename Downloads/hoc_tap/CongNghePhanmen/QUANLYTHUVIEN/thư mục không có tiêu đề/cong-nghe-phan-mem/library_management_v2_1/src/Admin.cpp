#include "Admin.h"
#include "Reader.h"
#include "Librarian.h"
#include "Utils.h"
#include <iostream>
#include <iomanip>
using namespace std;

// ==========================================
// IMPLEMENTS: Admin
// ==========================================

Admin::Admin() {}

Admin::Admin(int _id, const string& _username, const string& _password,
             const string& _status, const string& _fullname, const string& _email)
    : User(_id, _username, _password, "admin", _status, _fullname, _email) {}

void Admin::listUsers(const vector<User*>& users) const {
    clearScreen();
    cout << "         DANH SACH TAI KHOAN\n";
    cout << left
         << setw(5)  << "ID"
         << setw(15) << "Username"
         << setw(25) << "Ho ten"
         << setw(12) << "Vai tro"
         << "Trang thai\n";
    cout << string(65, '-') << "\n";
    for (auto u : users) u->displayInfo();
    cout << "\nTong: " << users.size() << " tai khoan.\n";
    pauseScreen();
}

void Admin::addUser(vector<User*>& users) {
    clearScreen();
    cout << "       THEM TAI KHOAN NHAN VIEN\n";
    cin.ignore();

    int maxId = 0;
    for (auto u : users) if (u->getId() > maxId) maxId = u->getId();

    string fullname, username, pass, email, role;
    cout << "  Ho ten    : "; getline(cin, fullname);
    cout << "  Username  : "; getline(cin, username);

    for (auto u : users) {
        if (u->getUsername() == username) {
            cout << "\n  Username da ton tai!\n"; pauseScreen(); return;
        }
    }

    cout << "  Password  : "; getline(cin, pass);
    cout << "  Email     : "; getline(cin, email);
    cout << "  Vai tro (librarian/admin): "; getline(cin, role);

    if (role != "librarian" && role != "admin") {
        cout << "\n  Vai tro khong hop le!\n"; pauseScreen(); return;
    }

    User* newUser = nullptr;
    if (role == "librarian")
        newUser = new Librarian(maxId + 1, username, hashPassword(pass), "active", fullname, email);
    else
        newUser = new Admin(maxId + 1, username, hashPassword(pass), "active", fullname, email);

    users.push_back(newUser);
    cout << "\n  Them tai khoan thanh cong! ID = " << (maxId + 1) << "\n";
    pauseScreen();
}

void Admin::lockUser(vector<User*>& users, int targetId, const User* currentUser) {
    for (auto u : users) {
        if (u->getId() == targetId) {
            if (u->getId() == currentUser->getId()) {
                cout << "\n  Khong the tu khoa tai khoan cua chinh minh!\n";
                pauseScreen(); return;
            }
            u->setStatus("locked");
            cout << "\n  Da khoa tai khoan: " << u->getUsername() << "\n";
            pauseScreen(); return;
        }
    }
    cout << "\n  Khong tim thay tai khoan!\n";
    pauseScreen();
}

void Admin::unlockUser(vector<User*>& users, int targetId) {
    for (auto u : users) {
        if (u->getId() == targetId) {
            u->setStatus("active");
            cout << "\n  Da mo khoa tai khoan: " << u->getUsername() << "\n";
            pauseScreen(); return;
        }
    }
    cout << "\n  Khong tim thay tai khoan!\n";
    pauseScreen();
}

void Admin::updateUser(vector<User*>& users) {
    clearScreen();
    cout << "  Nhap ID tai khoan can sua: ";
    int id; cin >> id;

    User* found = nullptr;
    for (auto u : users) if (u->getId() == id) { found = u; break; }

    if (!found) { cout << "\n  Khong tim thay tai khoan!\n"; pauseScreen(); return; }

    cin.ignore();
    string input;
    cout << "  Ho ten moi (Enter = giu nguyen): "; getline(cin, input);
    if (!input.empty()) found->setFullname(input);
    cout << "  Email moi  : "; getline(cin, input);
    if (!input.empty()) found->setEmail(input);

    cout << "\n  Cap nhat thanh cong!\n";
    pauseScreen();
}

void Admin::displayInfo() const {
    User::displayInfo();
}
