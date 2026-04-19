#include "LibrarySystem.h"
#include "FileManager.h"
#include "Utils.h"
#include <iostream>
#include <iomanip>
using namespace std;

// ==========================================
// IMPLEMENTS: LibrarySystem - Menu
// ==========================================

// ========== MENU READER ==========

void LibrarySystem::readerSearchBook() {
    clearScreen();
    cout << "\n           TIM KIEM SACH\n\n";
    cout << "  1. Tim theo ten sach\n";
    cout << "  2. Tim theo tac gia\n";
    cout << "  3. Tim theo the loai\n";
    cout << "  0. Quay lai\n";
    cout << "  Chon: ";
    int choice; cin >> choice;
    if (choice == 0) return;

    cin.ignore();
    cout << "  Nhap tu khoa: ";
    string keyword; getline(cin, keyword);

    Reader* reader = dynamic_cast<Reader*>(currentUser);
    if (!reader) return;

    auto results = reader->searchBook(books, keyword, choice);

    cout << "\n--- KET QUA TIM KIEM ---\n";
    cout << left
         << setw(5)  << "ID"
         << setw(30) << "Ten sach"
         << setw(20) << "Tac gia"
         << setw(15) << "The loai"
         << "Con lai\n";
    cout << string(75, '-') << "\n";
    for (auto b : results) {
        cout << left
             << setw(5)  << b->getId()
             << setw(30) << b->getTitle().substr(0,28)
             << setw(20) << b->getAuthor().substr(0,18)
             << setw(15) << b->getCategory().substr(0,13)
             << b->getAvailable() << "\n";
    }

    if (results.empty()) {
        cout << "  Khong tim thay ket qua phu hop.\n";
        pauseScreen();
    } else {
        cout << "\n  Tim thay " << results.size() << " sach.\n";
        cout << "  Nhap ID sach de xem chi tiet (0 = bo qua): ";
        int id; cin >> id;
        if (id != 0) {
            for (const auto& b : books)
                if (b.getId() == id) { clearScreen(); b.display(); pauseScreen(); break; }
        }
    }
}

void LibrarySystem::readerViewHistory() {
    clearScreen();
    Reader* reader = dynamic_cast<Reader*>(currentUser);
    if (reader) reader->viewBorrowHistory(borrows, books);
    pauseScreen();
}

void LibrarySystem::menuReader() {
    int choice;
    do {
        clearScreen();
        cout << "    MENU DOC GIA: " << currentUser->getFullname() << "\n\n";
        cout << "  1. Tim kiem sach\n";
        cout << "  2. Xem danh sach sach\n";
        cout << "  3. Xem lich su muon cua toi\n";
        cout << "  4. Doi mat khau\n";
        cout << "  0. Dang xuat\n\n";
        cout << "  Chon: ";
        cin >> choice;

        switch (choice) {
            case 1: readerSearchBook();  break;
            case 2: listBooks();         break;
            case 3: readerViewHistory(); break;
            case 4: changePasswordMenu(); break;
        }
    } while (choice != 0);
    currentUser->logout();
    currentUser = nullptr;
}

// ========== MENU LIBRARIAN ==========

void LibrarySystem::libManageBook() {
    Librarian* lib = dynamic_cast<Librarian*>(currentUser);
    if (!lib) return;

    int choice;
    do {
        clearScreen();
        cout << "\n          QUAN LY SACH\n\n";
        cout << "  1. Xem danh sach sach\n";
        cout << "  2. Tim kiem sach\n";
        cout << "  3. Them sach moi\n";
        cout << "  4. Sua thong tin sach\n";
        cout << "  5. Xoa sach\n";
        cout << "  0. Quay lai\n\n";
        cout << "  Chon: ";
        cin >> choice;

        switch (choice) {
            case 1: listBooks(); break;
            case 2: readerSearchBook(); break;
            case 3:
                lib->addBook(books);
                FileManager::saveBooks(books);
                pauseScreen();
                break;
            case 4:
                lib->updateBook(books);
                FileManager::saveBooks(books);
                pauseScreen();
                break;
            case 5:
                lib->deleteBook(books, borrows);
                FileManager::saveBooks(books);
                break;
        }
    } while (choice != 0);
}

void LibrarySystem::libManageReader() {
    int choice;
    do {
        clearScreen();
        cout << "          QUAN LY DOC GIA\n\n";
        cout << "  1. Xem danh sach doc gia\n";
        cout << "  2. Them doc gia moi\n";
        cout << "  3. Sua thong tin doc gia\n";
        cout << "  4. Khoa / Mo khoa tai khoan\n";
        cout << "  5. Xoa doc gia\n";
        cout << "  0. Quay lai\n\n";
        cout << "  Chon: ";
        cin >> choice;

        switch (choice) {
            case 1: listReaders(); break;

            case 2: {
                clearScreen();
                cout << "          THEM DOC GIA MOI\n";
                cin.ignore();
                string fullname, username, pass, email;
                int maxId = 0;
                for (auto u : users) if (u->getId() > maxId) maxId = u->getId();

                cout << "  Ho ten    : "; getline(cin, fullname);
                cout << "  Username  : "; getline(cin, username);

                bool exists = false;
                for (auto u : users) if (u->getUsername() == username) { exists = true; break; }
                if (exists) { cout << "\n  Username da ton tai!\n"; pauseScreen(); break; }

                cout << "  Password  : "; getline(cin, pass);
                cout << "  Email     : "; getline(cin, email);

                users.push_back(new Reader(maxId+1, username, hashPassword(pass), "active", fullname, email));
                FileManager::saveUsers(users);
                cout << "\n  Them doc gia thanh cong! ID = " << (maxId+1) << "\n";
                pauseScreen();
                break;
            }

            case 3: {
                clearScreen();
                cout << "       CAP NHAT THONG TIN DOC GIA\n";
                cout << "  Nhap ID doc gia can sua: ";
                int id; cin >> id;
                User* found = nullptr;
                for (auto u : users) if (u->getId()==id && u->getRole()=="reader") { found=u; break; }
                if (!found) { cout << "\n  Khong tim thay doc gia!\n"; pauseScreen(); break; }
                cin.ignore();
                string input;
                cout << "  Ho ten moi (Enter = giu nguyen): "; getline(cin, input);
                if (!input.empty()) found->setFullname(input);
                cout << "  Email moi  : "; getline(cin, input);
                if (!input.empty()) found->setEmail(input);
                FileManager::saveUsers(users);
                cout << "\n  Cap nhat thanh cong!\n";
                pauseScreen();
                break;
            }

            case 4: {
                clearScreen();
                cout << "         KHOA / MO KHOA DOC GIA\n";
                cout << "  Nhap ID doc gia: ";
                int id; cin >> id;
                User* found = nullptr;
                for (auto u : users) if (u->getId()==id && u->getRole()=="reader") { found=u; break; }
                if (!found) { cout << "\n  Khong tim thay doc gia!\n"; pauseScreen(); break; }
                if (found->isActive()) {
                    cout << "  Xac nhan khoa \"" << found->getUsername() << "\"? (y/n): ";
                    char c; cin >> c;
                    if (c=='y'||c=='Y') { found->setStatus("locked"); FileManager::saveUsers(users); cout << "\n  Da khoa!\n"; }
                } else {
                    cout << "  Mo khoa \"" << found->getUsername() << "\"? (y/n): ";
                    char c; cin >> c;
                    if (c=='y'||c=='Y') { found->setStatus("active"); FileManager::saveUsers(users); cout << "\n  Da mo khoa!\n"; }
                }
                pauseScreen();
                break;
            }

            case 5: {
                clearScreen();
                cout << "           XOA DOC GIA\n";
                cout << "  Nhap ID doc gia can xoa: ";
                int id; cin >> id;
                for (int i = 0; i < (int)users.size(); i++) {
                    if (users[i]->getId()==id && users[i]->getRole()=="reader") {
                        bool hasBorrow = false;
                        for (const auto& s : borrows)
                            if (s.getReaderId()==id && s.isBorrowing()) { hasBorrow=true; break; }
                        if (hasBorrow) { cout << "\n  Doc gia dang muon sach, khong the xoa!\n"; pauseScreen(); break; }
                        cout << "  Xac nhan xoa \"" << users[i]->getFullname() << "\"? (y/n): ";
                        char c; cin >> c;
                        if (c=='y'||c=='Y') {
                            delete users[i];
                            users.erase(users.begin()+i);
                            FileManager::saveUsers(users);
                            cout << "\n  Da xoa doc gia!\n";
                        } else { cout << "\n  Huy bo.\n"; }
                        pauseScreen(); break;
                    }
                }
                break;
            }
        }
    } while (choice != 0);
}

void LibrarySystem::libManageBorrow() {
    Librarian* lib = dynamic_cast<Librarian*>(currentUser);
    if (!lib) return;

    int choice;
    do {
        clearScreen();
        cout << "\n         QUAN LY MUON/TRA SACH\n\n";
        cout << "  1. Xem danh sach phieu muon\n";
        cout << "  2. Tao phieu muon sach\n";
        cout << "  3. Xac nhan tra sach\n";
        cout << "  4. Xem lich su muon (theo doc gia)\n";
        cout << "  0. Quay lai\n\n";
        cout << "  Chon: ";
        cin >> choice;

        switch (choice) {
            case 1: listBorrows(); break;

            case 2:
                lib->createBorrowSlip(borrows, books, users);
                FileManager::saveBooks(books);
                FileManager::saveBorrows(borrows);
                pauseScreen();
                break;

            case 3:
                lib->confirmReturn(borrows, books, fines);
                FileManager::saveBooks(books);
                FileManager::saveBorrows(borrows);
                FileManager::saveFines(fines);
                pauseScreen();
                break;

            case 4: {
                cout << "  Nhap ID doc gia: ";
                int rid; cin >> rid;
                clearScreen();
                cout << "  Doc gia: " << getReaderName(rid) << "\n";
                int cnt = 0;
                for (const auto& slip : borrows) {
                    if (slip.getReaderId() != rid) continue;
                    cnt++;
                    cout << "\nPhieu #" << slip.getId()
                         << " | Muon: " << slip.getBorrowDate()
                         << " | Han: "  << slip.getDueDate()
                         << " | Tra: "  << (slip.getReturnDate().empty() ? "Chua tra" : slip.getReturnDate())
                         << " | "       << slip.getStatus() << "\n";
                    for (const auto& d : slip.getDetails())
                        cout << "   - " << getBookTitle(d.getBookId())
                             << " (x" << d.getQuantity() << ")\n";
                }
                if (cnt == 0) cout << "  Chua co lich su muon.\n";
                pauseScreen();
                break;
            }
        }
    } while (choice != 0);
}

void LibrarySystem::libManageFine() {
    int choice;
    do {
        clearScreen();
        cout << "         QUAN LY TIEN PHAT\n\n";
        cout << "  1. Xem danh sach tien phat\n";
        cout << "  2. Xac nhan thanh toan\n";
        cout << "  0. Quay lai\n\n";
        cout << "  Chon: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                clearScreen();
                cout << "          DANH SACH TIEN PHAT\n";
                cout << left
                     << setw(5)  << "ID"
                     << setw(8)  << "PhieuID"
                     << setw(22) << "Doc gia"
                     << setw(14) << "So tien"
                     << setw(28) << "Ly do"
                     << "Trang thai\n";
                cout << string(85, '-') << "\n";
                double totalUnpaid = 0;
                for (const auto& f : fines) {
                    cout << left
                         << setw(5)  << f.getId()
                         << setw(8)  << f.getBorrowId()
                         << setw(22) << getReaderName(f.getReaderId()).substr(0,20)
                         << setw(14) << fixed << setprecision(0) << f.getAmount()
                         << setw(28) << f.getReason().substr(0,26)
                         << f.getStatus() << "\n";
                    if (!f.isPaid()) totalUnpaid += f.getAmount();
                }
                cout << "\nTong chua thanh toan: " << fixed << setprecision(0)
                     << totalUnpaid << " VND\n";
                pauseScreen();
                break;
            }

            case 2: {
                clearScreen();
                cout << "        XAC NHAN THANH TOAN PHAT\n\n";
                bool hasUnpaid = false;
                for (const auto& f : fines) {
                    if (f.isPaid()) continue;
                    cout << "  ID " << f.getId()
                         << " | " << getReaderName(f.getReaderId())
                         << " | " << f.getReason()
                         << " | " << fixed << setprecision(0) << f.getAmount() << " VND\n";
                    hasUnpaid = true;
                }
                if (!hasUnpaid) { cout << "  Khong co khoan phat nao chua thanh toan.\n"; pauseScreen(); break; }

                cout << "\n  Nhap ID phieu phat can thanh toan: ";
                int fineId; cin >> fineId;
                for (auto& f : fines) {
                    if (f.getId() != fineId) continue;
                    if (f.isPaid()) { cout << "\n  Khoan nay da duoc thanh toan!\n"; }
                    else {
                        cout << "  Xac nhan thanh toan " << fixed << setprecision(0)
                             << f.getAmount() << " VND? (y/n): ";
                        char c; cin >> c;
                        if (c=='y'||c=='Y') {
                            f.markPaid();
                            FileManager::saveFines(fines);
                            cout << "\n  Thanh toan thanh cong!\n";
                        } else { cout << "\n  Huy bo.\n"; }
                    }
                    pauseScreen(); break;
                }
                break;
            }
        }
    } while (choice != 0);
}

void LibrarySystem::menuLibrarian() {
    int choice;
    do {
        clearScreen();
        cout << "    MENU THU THU: " << currentUser->getFullname() << "\n\n";
        cout << "  1. Quan ly sach\n";
        cout << "  2. Quan ly doc gia\n";
        cout << "  3. Quan ly muon/tra sach\n";
        cout << "  4. Quan ly tien phat\n";
        cout << "  5. Doi mat khau\n";
        cout << "  0. Dang xuat\n\n";
        cout << "  Chon: ";
        cin >> choice;

        switch (choice) {
            case 1: libManageBook();    break;
            case 2: libManageReader();  break;
            case 3: libManageBorrow();  break;
            case 4: libManageFine();    break;
            case 5: changePasswordMenu(); break;
        }
    } while (choice != 0);
    currentUser->logout();
    currentUser = nullptr;
}

// ========== MENU ADMIN ==========

void LibrarySystem::adminManageUser() {
    Admin* admin = dynamic_cast<Admin*>(currentUser);
    if (!admin) return;

    int choice;
    do {
        clearScreen();
        cout << "         QUAN LY TAI KHOAN\n\n";
        cout << "  1. Xem danh sach tai khoan\n";
        cout << "  2. Them tai khoan nhan vien\n";
        cout << "  3. Khoa tai khoan\n";
        cout << "  4. Mo khoa tai khoan\n";
        cout << "  5. Cap nhat tai khoan\n";
        cout << "  0. Quay lai\n\n";
        cout << "  Chon: ";
        cin >> choice;

        switch (choice) {
            case 1: admin->listUsers(users); break;
            case 2:
                admin->addUser(users);
                FileManager::saveUsers(users);
                break;
            case 3: {
                clearScreen();
                cout << "  Nhap ID tai khoan can khoa: ";
                int id; cin >> id;
                admin->lockUser(users, id, currentUser);
                FileManager::saveUsers(users);
                break;
            }
            case 4: {
                clearScreen();
                cout << "  Nhap ID tai khoan can mo khoa: ";
                int id; cin >> id;
                admin->unlockUser(users, id);
                FileManager::saveUsers(users);
                break;
            }
            case 5:
                admin->updateUser(users);
                FileManager::saveUsers(users);
                break;
        }
    } while (choice != 0);
}

void LibrarySystem::adminViewReport() {
    int choice;
    do {
        clearScreen();
        cout << "          BAO CAO THONG KE\n\n";
        cout << "  1. Tinh trang sach\n";
        cout << "  2. Sach qua han tra\n";
        cout << "  3. Top nguoi muon nhieu nhat\n";
        cout << "  0. Quay lai\n\n";
        cout << "  Chon: ";
        cin >> choice;

        switch (choice) {
            case 1: reportBookStat();    break;
            case 2: reportOverdue();     break;
            case 3: reportTopBorrower(); break;
        }
    } while (choice != 0);
}

void LibrarySystem::menuAdmin() {
    int choice;
    do {
        clearScreen();
        cout << "    MENU ADMIN: " << currentUser->getFullname() << "\n\n";
        cout << "  1. Quan ly tai khoan\n";
        cout << "  2. Quan ly sach\n";
        cout << "  3. Quan ly doc gia\n";
        cout << "  4. Xem bao cao thong ke\n";
        cout << "  5. Doi mat khau\n";
        cout << "  0. Dang xuat\n\n";
        cout << "  Chon: ";
        cin >> choice;

        switch (choice) {
            case 1: adminManageUser();   break;
            case 2: libManageBook();     break;
            case 3: libManageReader();   break;
            case 4: adminViewReport();   break;
            case 5: changePasswordMenu(); break;
        }
    } while (choice != 0);
    currentUser->logout();
    currentUser = nullptr;
}

// ========== VONG LAP CHINH ==========

void LibrarySystem::run() {
    initDefaultData();
    FileManager::loadAll(users, books, borrows, fines);

    while (true) {
        if (!login()) break;

        if      (currentUser->getRole() == "reader")    menuReader();
        else if (currentUser->getRole() == "librarian") menuLibrarian();
        else if (currentUser->getRole() == "admin")     menuAdmin();
    }

    FileManager::saveAll(users, books, borrows, fines);
    cout << "\n  === Hen gap lai! ===\n\n";
}
