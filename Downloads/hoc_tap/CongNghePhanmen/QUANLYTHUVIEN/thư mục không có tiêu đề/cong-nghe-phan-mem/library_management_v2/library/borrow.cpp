#include "library.h"


// ==========================================
// QUAN LY MUON / TRA SACH
// ==========================================
// Cac chuc nang:
// - createBorrowSlip(): Tao phieu muon sach
// - confirmReturn(): Xac nhan tra sach (tinh phat neu tre)
// - listBorrows(): Hien thi danh sach phieu muon
// - viewBorrowHistory(): Xem lich su muon cua doc gia
// - manageBorrow(): Menu quan ly muon/tra
//
// Cac ham ho tro:
// - getReaderName(): Tim ten doc gia theo ID
// - getBookTitle(): Tim ten sach theo ID

// HAM HO TRO: TIM TEN DOC GIA THEO ID
string getReaderName(int readerId) {
    for (auto& u : gUsers)
        if (u.id == readerId) return u.fullname;
    return "Khong ro";
}

// HAM HO TRO: TIM TEN SAC THEO ID
string getBookTitle(int bookId) {
    for (auto& b : gBooks)
        if (b.id == bookId) return b.title;
    return "Khong ro";
}

void listBorrows() {
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

    // Duyet danh sach phieu muon va in ra
    for (auto& slip : gBorrows) {
        string readerName = getReaderName(slip.readerId);
        cout << left
             << setw(6)  << slip.id
             << setw(20) << readerName.substr(0, 18)
             << setw(12) << slip.borrowDate
             << setw(12) << slip.dueDate
             << setw(12) << (slip.returnDate.empty() ? "---" : slip.returnDate)
             << slip.status << "\n";
    }
    cout << "\nTong: " << gBorrows.size() << " phieu.\n";
    pause();
}

void viewBorrowHistory(int readerId) {
    clearScreen();
    int targetId = readerId;
    if (targetId == -1) {
        // Nếu là reader đang đăng nhập
        targetId = gCurrentUser->id;
    }

    cout << "  Doc gia: " << getReaderName(targetId) << "\n";

    int count = 0;
    for (auto& slip : gBorrows) {
        if (slip.readerId != targetId) continue;
        count++;

        cout << "\nPhieu #" << slip.id
             << " | Muon: " << slip.borrowDate
             << " | Han: "  << slip.dueDate
             << " | Tra: "  << (slip.returnDate.empty() ? "Chua tra" : slip.returnDate)
             << " | " << slip.status << "\n";

        for (auto& d : slip.details) {
            cout << "   - " << getBookTitle(d.bookId)
                 << " (x" << d.quantity << ")\n";
        }
    }

    if (count == 0) cout << "  Chua co lich su muon.\n";
    pause();
}

// HAM: TAO PHIEU MUON SACH MOI
// Quy trinh:
// 1. Chon doc gia (kiem tra hop le va khong bi khoa)
// 2. Kiem tra doc gia co phieu dang muon chua tra
// 3. Them cac cuon sach vao phieu (max MAX_BORROW cuon)
// 4. Giam so luong con lai cua sach
// 5. Luu phieu vao file
void createBorrowSlip() {
    clearScreen();
    cout << "          TAO PHIEU MUON SACH\n";

    // Chọn độc giả
    cout << "  Nhap ID doc gia: ";
    int readerId; cin >> readerId;

    User* reader = nullptr;
    for (auto& u : gUsers) {
        if (u.id == readerId && u.role == "reader") {
            reader = &u; break;
        }
    }
    if (!reader) {
        cout << "\n  Khong tim thay doc gia!\n";
        pause();
        return;
    }
    if (reader->status == "locked") {
        cout << "\n  Tai khoan doc gia bi khoa!\n";
        pause();
        return;
    }

    // Kiểm tra còn phiếu đang mượn chưa trả không
    int currentBorrow = 0;
    for (auto& slip : gBorrows) {
        if (slip.readerId == readerId && slip.status == "borrowing")
            currentBorrow++;
    }
    if (currentBorrow > 0) {
        cout << "\n  Doc gia dang co " << currentBorrow << " phieu chua tra!\n";
        cout << "  Vui long tra sach truoc khi muon them.\n";
        pause();
        return;
    }

    cout << "  Doc gia: " << reader->fullname << "\n\n";

    BorrowSlip slip;
    int maxId = 0;
    for (auto& s : gBorrows) if (s.id > maxId) maxId = s.id;
    slip.id       = maxId + 1;
    slip.readerId = readerId;
    slip.borrowDate = getCurrentDate();
    slip.dueDate    = addDays(slip.borrowDate, 14); // hạn 14 ngày
    slip.status     = "borrowing";

    // Thêm sách vào phiếu
    int totalBooks = 0;
    while (true) {
        cout << "  Nhap ID sach can muon (0 = xong): ";
        int bookId; cin >> bookId;
        if (bookId == 0) break;

        Book* book = nullptr;
        for (auto& b : gBooks) {
            if (b.id == bookId) { book = &b; break; }
        }
        if (!book) { cout << "  Khong tim thay sach!\n"; continue; }
        if (book->available <= 0) {
            cout << "  Sach \"" << book->title << "\" da het, khong the muon!\n";
            continue;
        }

        cout << "  So luong muon (con lai: " << book->available << "): ";
        int qty; cin >> qty;

        if (qty <= 0 || qty > book->available) {
            cout << "  So luong khong hop le!\n";
            continue;
        }
        if (totalBooks + qty > MAX_BORROW) {
            cout << "  Vuot qua gioi han muon toi da (" << MAX_BORROW << " cuon)!\n";
            continue;
        }

        BorrowDetail detail;
        detail.bookId   = bookId;
        detail.quantity = qty;
        slip.details.push_back(detail);

        // Trừ số lượng sách
        book->available -= qty;
        totalBooks += qty;

        cout << "  Da them: " << book->title << " x" << qty << "\n";
    }

    if (slip.details.empty()) {
        cout << "\n  Khong co sach nao duoc chon. Huy phieu.\n";
        // Hoàn lại số lượng
        for (auto& d : slip.details) {
            for (auto& b : gBooks)
                if (b.id == d.bookId) b.available += d.quantity;
        }
        pause();
        return;
    }

    gBorrows.push_back(slip);
    saveBooks();
    saveBorrows();

    cout << "     PHIEU MUON #" << slip.id << " DA DUOC TAO\n";
    cout << "  Doc gia  : " << reader->fullname << "\n";
    cout << "  Ngay muon: " << slip.borrowDate  << "\n";
    cout << "  Han tra  : " << slip.dueDate      << "\n";
    cout << "  Sach da muon:\n";
    for (auto& d : slip.details)
        cout << "    - " << getBookTitle(d.bookId) << " x" << d.quantity << "\n";
    pause();
}

void confirmReturn() {
    clearScreen();
    cout << "           XAC NHAN TRA SACH\n";

    cout << "  Nhap ID phieu muon: ";
    int slipId; cin >> slipId;

    // Tim phieu muon theo ID
    BorrowSlip* slip = nullptr;
    for (auto& s : gBorrows) {
        if (s.id == slipId) { slip = &s; break; }
    }

    if (!slip) {
        cout << "\n  Khong tim thay phieu muon!\n";
        pause();
        return;
    }
    if (slip->status != "borrowing") {
        cout << "\n  Phieu nay da duoc tra roi!\n";
        pause();
        return;
    }

    // Hiện thông tin phiếu
    cout << "\n  Doc gia  : " << getReaderName(slip->readerId) << "\n";
    cout << "  Ngay muon: " << slip->borrowDate << "\n";
    cout << "  Han tra  : " << slip->dueDate    << "\n";
    cout << "  Sach:\n";
    for (auto& d : slip->details)
        cout << "    - " << getBookTitle(d.bookId) << " x" << d.quantity << "\n";

    string today = getCurrentDate();
    slip->returnDate = today;
    slip->status     = "returned";

    // Hoàn trả số lượng sách
    for (auto& d : slip->details) {
        for (auto& b : gBooks) {
            if (b.id == d.bookId) {
                b.available += d.quantity;
                break;
            }
        }
    }

    // Kiểm tra quá hạn
    int lateDays = daysBetween(slip->dueDate, today);
    if (lateDays > 0) {
        slip->status = "overdue";
        double fineAmount = lateDays * FINE_PER_DAY;

        Fine fine;
        int maxFineId = 0;
        for (auto& f : gFines) if (f.id > maxFineId) maxFineId = f.id;
        fine.id          = maxFineId + 1;
        fine.borrowId    = slip->id;
        fine.readerId    = slip->readerId;
        fine.amount      = fineAmount;
        fine.reason      = "Tra sach tre " + to_string(lateDays) + " ngay";
        fine.status      = "unpaid";
        fine.createdDate = today;
        gFines.push_back(fine);
        saveFines();

        cout << "\n  !!! TRA TRE " << lateDays << " NGAY !!!\n";
        cout << "  Tien phat: " << fixed << setprecision(0) << fineAmount << " VND\n";
        cout << "  (Phi: " << (int)FINE_PER_DAY << " VND/ngay)\n";
    } else {
        cout << "\n  Tra sach dung han!\n";
    }

    saveBooks();
    saveBorrows();

    cout << "\n  Xac nhan tra sach thanh cong!\n";
    pause();
}

void manageBorrow() {
    int choice;
    do {
        clearScreen();
        cout << "\n";
        cout << "         QUAN LY MUON/TRA SACH\n";
        cout << "\n";
        cout << "  1. Xem danh sach phieu muon\n";
        cout << "  2. Tao phieu muon sach\n";
        cout << "  3. Xac nhan tra sach\n";
        cout << "  4. Xem lich su muon (theo doc gia)\n";
        cout << "  0. Quay lai\n";
        cout << "  Chon: ";
        cin >> choice;

        switch (choice) {
            case 1: listBorrows();   break;
            case 2: createBorrowSlip(); break;
            case 3: confirmReturn();    break;
            case 4: {
                cout << "  Nhap ID doc gia: ";
                int rid; cin >> rid;
                viewBorrowHistory(rid);
                break;
            }
        }
    } while (choice != 0);
}
