#ifndef LIBRARYSYSTEM_H
#define LIBRARYSYSTEM_H

#include "User.h"
#include "Reader.h"
#include "Librarian.h"
#include "Admin.h"
#include "Book.h"
#include "BorrowSlip.h"
#include "Fine.h"
#include <vector>
using namespace std;

// ==========================================
// CLASS: LibrarySystem - He thong chinh
// ==========================================
// Dieu phoi toan bo: dang nhap, menu, nghiep vu,
// luu tru. Day la lop duy nhat giu trang thai
// chuong trinh (users, books, borrows, fines).
class LibrarySystem {
private:
    vector<User*>      users;
    vector<Book>       books;
    vector<BorrowSlip> borrows;
    vector<Fine>       fines;
    User*              currentUser;

    // --- Helper ---
    User* findUserByUsername(const string& username);
    User* findUserById(int id);
    string getReaderName(int readerId) const;
    string getBookTitle(int bookId)    const;

    // --- Khoi tao du lieu mac dinh (lan dau chay) ---
    void initDefaultData();

    // --- Dang nhap / dang xuat ---
    bool doLogin();
    void registerAccount();

    // --- Hien thi danh sach ---
    void listBooks()   const;
    void listBorrows() const;
    void listReaders() const;

    // --- Bao cao ---
    void reportBookStat()     const;
    void reportOverdue()      const;
    void reportTopBorrower()  const;

    // --- Doi mat khau (dung chung) ---
    void changePasswordMenu();

    // === Menu Reader ===
    void menuReader();
    void readerSearchBook();
    void readerViewHistory();

    // === Menu Librarian ===
    void menuLibrarian();
    void libManageBook();
    void libManageReader();
    void libManageBorrow();
    void libManageFine();

    // === Menu Admin ===
    void menuAdmin();
    void adminManageUser();
    void adminViewReport();

public:
    LibrarySystem();
    ~LibrarySystem();

    // Vong lap chinh - diem khoi chay tu main()
    void run();

    // Man hinh dang nhap
    bool login();
};

#endif
