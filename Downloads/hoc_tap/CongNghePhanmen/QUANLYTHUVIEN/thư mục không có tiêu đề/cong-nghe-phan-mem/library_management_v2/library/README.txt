
  HỆ THỐNG QUẢN LÝ THƯ VIỆN - C++


CÁCH COMPILE & CHẠY:
  Linux/Mac:
    g++ -std=c++11 -o library main.cpp utils.cpp fileio.cpp auth.cpp book.cpp reader.cpp borrow.cpp fine_report.cpp admin.cpp
    ./library

  Hoặc dùng Makefile:
    make
    ./library

  Windows (MinGW):
    g++ -std=c++11 -o library.exe main.cpp utils.cpp fileio.cpp auth.cpp book.cpp reader.cpp borrow.cpp fine_report.cpp admin.cpp
    library.exe

-------------------------------------
TÀI KHOẢN MẶC ĐỊNH (lần đầu chạy):
  Admin    : admin    / admin123
  Thủ thư  : thuthu   / thu123
  Độc giả  : docgia1  / doc123
-------------------------------------

CẤU TRÚC FILE:
  library.h       - Header: định nghĩa struct, hằng số, khai báo hàm
  main.cpp        - Entry point, khởi tạo dữ liệu
  utils.cpp       - Hàm tiện ích (ngày tháng, hash, ...)
  fileio.cpp      - Đọc/ghi dữ liệu từ file .txt
  auth.cpp        - Đăng nhập, đổi mật khẩu
  book.cpp        - Quản lý sách
  reader.cpp      - Quản lý độc giả
  borrow.cpp      - Mượn/trả sách
  fine_report.cpp - Tiền phạt + Báo cáo thống kê
  admin.cpp       - Quản lý user + Menu chính
  data/           - Thư mục lưu dữ liệu (.txt)

-------------------------------------
CHỨC NĂNG THEO ROLE:

  READER (Độc giả):
    - Tìm kiếm sách (theo tên/tác giả/thể loại)
    - Xem danh sách sách
    - Xem lịch sử mượn của mình
    - Đổi mật khẩu

  LIBRARIAN (Thủ thư):
    - Quản lý sách (thêm/sửa/xóa)
    - Quản lý độc giả (thêm/sửa/khóa/xóa)
    - Tạo phiếu mượn sách
    - Xác nhận trả sách (tự động tính tiền phạt nếu trễ)
    - Quản lý tiền phạt

  ADMIN (Quản trị):
    - Quản lý tài khoản (thêm nhân viên/khóa/mở khóa)
    - Quản lý sách
    - Quản lý độc giả
    - Xem báo cáo thống kê

-------------------------------------
QUY TẮC HỆ THỐNG:
  - Phí phạt trả trễ: 2,000 VND/ngày
  - Hạn mượn mặc định: 14 ngày
  - Tối đa mượn: 5 cuốn/lần
  - Password được hash trước khi lưu
  - Không cho mượn nếu hết sách
  - Không xóa sách đang được mượn
  - Không xóa độc giả đang mượn sách


