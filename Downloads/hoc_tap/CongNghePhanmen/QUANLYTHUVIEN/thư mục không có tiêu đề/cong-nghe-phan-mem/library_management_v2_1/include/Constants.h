#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>
using namespace std;

// ==========================================
// HANG SO TOAN CUC
// ==========================================

// Duong dan file du lieu
const string DATA_USERS   = "data/users.txt";
const string DATA_BOOKS   = "data/books.txt";
const string DATA_BORROWS = "data/borrows.txt";
const string DATA_FINES   = "data/fines.txt";

// Thong so he thong
const double FINE_PER_DAY = 2000.0;  // 2000 VND / ngay tre
const int    MAX_BORROW   = 5;       // Toi da 5 cuon / lan muon

#endif
