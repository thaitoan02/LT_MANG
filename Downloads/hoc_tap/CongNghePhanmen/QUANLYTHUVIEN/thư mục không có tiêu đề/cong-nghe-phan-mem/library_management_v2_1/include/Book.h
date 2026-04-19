#ifndef BOOK_H
#define BOOK_H

#include <string>
using namespace std;

// ==========================================
// CLASS: Book - Thong tin sach
// ==========================================
class Book {
private:
    int    id;
    string title;
    string author;
    string publisher;
    int    year;
    string category;
    int    quantity;   // Tong so cuon
    int    available;  // So cuon con lai
    string location;   // Vi tri keo sach

public:
    Book();

    // Getters
    int    getId()        const { return id; }
    string getTitle()     const { return title; }
    string getAuthor()    const { return author; }
    string getPublisher() const { return publisher; }
    int    getYear()      const { return year; }
    string getCategory()  const { return category; }
    int    getQuantity()  const { return quantity; }
    int    getAvailable() const { return available; }
    string getLocation()  const { return location; }

    // Setters
    void setId(int v)              { id = v; }
    void setTitle(const string& v)     { title = v; }
    void setAuthor(const string& v)    { author = v; }
    void setPublisher(const string& v) { publisher = v; }
    void setYear(int v)            { year = v; }
    void setCategory(const string& v)  { category = v; }
    void setQuantity(int v)        { quantity = v; }
    void setAvailable(int v)       { available = v; }
    void setLocation(const string& v)  { location = v; }

    // Nghiep vu
    bool isAvailable() const;
    void borrowCopy(int qty);   // Giam so cuon khi muon
    void returnCopy(int qty);   // Tang so cuon khi tra

    // Hien thi
    void display()        const;  // Hien thi day du
    void displaySummary() const;  // Hien thi 1 dong
};

#endif
