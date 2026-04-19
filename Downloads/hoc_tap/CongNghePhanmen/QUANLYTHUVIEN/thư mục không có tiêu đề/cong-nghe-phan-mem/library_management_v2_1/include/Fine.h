#ifndef FINE_H
#define FINE_H

#include <string>
using namespace std;

// ==========================================
// CLASS: Fine - Phieu tien phat
// ==========================================
class Fine {
private:
    int    id;
    int    borrowId;    // ID phieu muon lien quan
    int    readerId;    // ID doc gia bi phat
    double amount;      // So tien phat (VND)
    string reason;      // Ly do phat
    string status;      // "unpaid" | "paid"
    string createdDate;

public:
    Fine();

    // Getters
    int    getId()          const { return id; }
    int    getBorrowId()    const { return borrowId; }
    int    getReaderId()    const { return readerId; }
    double getAmount()      const { return amount; }
    string getReason()      const { return reason; }
    string getStatus()      const { return status; }
    string getCreatedDate() const { return createdDate; }

    // Setters
    void setId(int v)                { id = v; }
    void setBorrowId(int v)          { borrowId = v; }
    void setReaderId(int v)          { readerId = v; }
    void setAmount(double v)         { amount = v; }
    void setReason(const string& v)  { reason = v; }
    void setStatus(const string& v)  { status = v; }
    void setCreatedDate(const string& v) { createdDate = v; }

    // Nghiep vu
    bool   isPaid()           const { return status == "paid"; }
    void   markPaid()               { status = "paid"; }
    double calculateAmount()  const { return amount; }
};

#endif
