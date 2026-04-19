#ifndef BORROWSLIP_H
#define BORROWSLIP_H

#include <string>
#include <vector>
using namespace std;

// ==========================================
// CLASS: BorrowDetail - Chi tiet 1 cuon sach trong phieu muon
// ==========================================
class BorrowDetail {
private:
    int bookId;
    int quantity;

public:
    BorrowDetail();
    BorrowDetail(int bookId, int quantity);

    int getBookId()   const { return bookId; }
    int getQuantity() const { return quantity; }

    void setBookId(int v)   { bookId = v; }
    void setQuantity(int v) { quantity = v; }
};

// ==========================================
// CLASS: BorrowSlip - Phieu muon sach
// ==========================================
class BorrowSlip {
private:
    int    id;
    int    readerId;
    string borrowDate;
    string dueDate;
    string returnDate;
    string status;              // "borrowing" | "returned" | "overdue"
    vector<BorrowDetail> details;

public:
    BorrowSlip();

    // Getters
    int    getId()         const { return id; }
    int    getReaderId()   const { return readerId; }
    string getBorrowDate() const { return borrowDate; }
    string getDueDate()    const { return dueDate; }
    string getReturnDate() const { return returnDate; }
    string getStatus()     const { return status; }
    const vector<BorrowDetail>& getDetails() const { return details; }

    // Setters
    void setId(int v)                { id = v; }
    void setReaderId(int v)          { readerId = v; }
    void setBorrowDate(const string& v) { borrowDate = v; }
    void setDueDate(const string& v)    { dueDate = v; }
    void setReturnDate(const string& v) { returnDate = v; }
    void setStatus(const string& v)     { status = v; }

    // Nghiep vu
    void addDetail(const BorrowDetail& d);
    int    calculateLateDays(const string& today) const;
    double calculateFine(const string& today) const;
    bool   isBorrowing() const { return status == "borrowing"; }
};

#endif
