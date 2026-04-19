#include "library.h"
#include <sys/stat.h>

// ==========================================
// KHOI TAO DU LIEU MAC DINH
// ==========================================
// Ham nay se chay lan dau tien de tao:
// - Thu muc data
// - Tao 3 tai khoan mac dinh (admin, thu thu, doc gia)
// - Tao 5 cuon sach mau

// BIEN TOAN CUC - khoi tao du lieu
vector<User>       gUsers;
vector<Book>       gBooks;
vector<BorrowSlip> gBorrows;
vector<Fine>       gFines;
User*              gCurrentUser = nullptr;

//  KHỞI TẠO DỮ LIỆU MẶC ĐỊNH 
// Tạo tài khoản admin mặc định nếu chưa có file dữ liệu

void initDefaultData() {
    // Kiem tra file users da ton tai chua (neu co = da khoi tao roi)
    ifstream f(DATA_USERS);
    if (f.is_open()) { f.close(); return; }  // File da ton tai -> khong cau tao lai

    // Tao thu muc data neu chua co
    #ifdef _WIN32
        system("mkdir data 2>nul");
    #else
        mkdir("data", 0777);
    #endif

    // ===== TAO 3 TAI KHOAN MAC DINH =====
    
    // Tai khoan 1: Admin (quan ly he thong)
    User admin;
    admin.id       = 1;
    admin.username = "admin";
    admin.password = hashPassword("admin123");
    admin.role     = "admin";           // Vai tro quan tri
    admin.status   = "active";
    admin.fullname = "Quan Tri Vien";
    admin.email    = "admin@library.com";
    gUsers.push_back(admin);

    // Tai khoan 2: Librarian (Thu thu - quan ly sach va doc gia)
    User lib;
    lib.id       = 2;
    lib.username = "thuthu";
    lib.password = hashPassword("thu123");
    lib.role     = "librarian";        // Vai tro thu thu
    lib.status   = "active";
    lib.fullname = "Thu Thu Nguyen";
    lib.email    = "thuthu@library.com";
    gUsers.push_back(lib);

    // Tai khoan 3: Reader (Doc gia - muon sach)
    User reader;
    reader.id       = 3;
    reader.username = "docgia1";
    reader.password = hashPassword("doc123");
    reader.role     = "reader";        // Vai tro doc gia
    reader.status   = "active";
    reader.fullname = "Nguyen Van A";
    reader.email    = "docgia1@gmail.com";
    gUsers.push_back(reader);

    // ===== TAO 5 CUON SACH MAU =====
    Book b1 = {1, "Lap Trinh C++ Co Ban", "Bjarne Stroustrup", "NXB KHKT", 2020,
               "Lap trinh", 5, 5, "A1-01"};
    Book b2 = {2, "Co So Du Lieu", "Ramez Elmasri", "NXB GD", 2018,
               "CSDL", 3, 3, "A1-02"};
    Book b3 = {3, "Giai Tich 1", "Nguyen Dinh Tri", "NXB DHQG", 2019,
               "Toan hoc", 4, 4, "B2-01"};
    Book b4 = {4, "Tri Tue Nhan Tao", "Stuart Russell", "NXB KHKT", 2021,
               "AI", 2, 2, "A2-01"};
    Book b5 = {5, "Cau Truc Du Lieu & Giai Thuat", "Thomas Cormen", "NXB DHQG", 2017,
               "Lap trinh", 3, 3, "A1-03"};
    gBooks = {b1, b2, b3, b4, b5};

    // Luu tat ca du lieu vao file
    saveAllData();

    // In thong tin tai khoan mac dinh ra man hinh
    cout << "   DU LIEU MAN KHOI TAO \n";
    cout << "  Tai khoan mac dinh:\n";
    cout << "    Admin    : admin / admin123\n";
    cout << "    Thu thu  : thuthu / thu123\n";
    cout << "    Doc gia  : docgia1 / doc123\n";
    cout << "  \n\n";
    pause();
}

//  MAIN FUNCTION - DIEM VAO CUA CHUONG TRINH
// Chuc nang:
// 1. Khoi tao du lieu mac dinh neu lan dau
// 2. Tai du lieu tu file
// 3. Vong lap chinh: Dang nhap, hien menu, dang xuat
// 4. Luu du lieu truoc thoat
int main() {
    // Buoc 1: Khoi tao du lieu mac dinh tren lan chay dau tien
    initDefaultData();

    // Buoc 2: Tai tat ca du lieu tu file vao bo nho
    loadAllData();

    // Buoc 3: Vong lap chinh - xu ly dang nhap va menu
    while (true) {
        // Goi ham dang nhap (tra ve true neu dang nhap thanh cong)
        bool loggedIn = login();
        if (!loggedIn) break;  // Nguoi dung chon thoat

        // Dieu huong toi menu phu hop theo vai tro
        if (gCurrentUser->role == "reader") {
            // Menu cho Doc Gia
            menuReader();
        } else if (gCurrentUser->role == "librarian") {
            // Menu cho Thu Thu
            menuLibrarian();
        } else if (gCurrentUser->role == "admin") {
            // Menu cho Admin
            menuAdmin();
        }
        // Sau khi dang xuat, vong lap quay lai login screen
    }

    // Buoc 4: Luu tat ca du lieu truoc khi thoat
    saveAllData();
    cout << "\n  === Hen gap lai! ===\n\n";
    return 0;
}
