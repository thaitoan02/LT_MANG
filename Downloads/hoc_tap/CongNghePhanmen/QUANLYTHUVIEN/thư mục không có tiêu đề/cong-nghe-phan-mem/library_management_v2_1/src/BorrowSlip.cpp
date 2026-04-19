#include "BorrowSlip.h"
#include "Utils.h"
#include "Constants.h"

// ==========================================
// IMPLEMENTS: BorrowDetail
// ==========================================

BorrowDetail::BorrowDetail() : bookId(0), quantity(0) {}

BorrowDetail::BorrowDetail(int bId, int qty) : bookId(bId), quantity(qty) {}

// ==========================================
// IMPLEMENTS: BorrowSlip
// ==========================================

BorrowSlip::BorrowSlip() : id(0), readerId(0) {}

void BorrowSlip::addDetail(const BorrowDetail& d) {
    details.push_back(d);
}

int BorrowSlip::calculateLateDays(const string& today) const {
    int late = daysBetween(dueDate, today);
    return (late > 0) ? late : 0;
}

double BorrowSlip::calculateFine(const string& today) const {
    return calculateLateDays(today) * FINE_PER_DAY;
}
