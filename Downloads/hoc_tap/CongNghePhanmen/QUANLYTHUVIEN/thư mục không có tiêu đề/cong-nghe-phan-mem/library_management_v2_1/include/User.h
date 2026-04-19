#ifndef USER_H
#define USER_H

#include <string>
using namespace std;

// ==========================================
// CLASS: User - Lop co so (Base class)
// ==========================================
class User {
protected:
    int    id;
    string username;
    string password;   // Da hash
    string role;       // "admin" | "librarian" | "reader"
    string status;     // "active" | "locked"
    string fullname;
    string email;

public:
    User();
    User(int id, const string& username, const string& password,
         const string& role,   const string& status,
         const string& fullname, const string& email);

    virtual ~User() {}

    // Getters
    int    getId()       const { return id; }
    string getUsername() const { return username; }
    string getPassword() const { return password; }
    string getRole()     const { return role; }
    string getStatus()   const { return status; }
    string getFullname() const { return fullname; }
    string getEmail()    const { return email; }

    // Setters
    void setId(int v)                { id = v; }
    void setUsername(const string& v){ username = v; }
    void setPassword(const string& v){ password = v; }
    void setRole(const string& v)    { role = v; }
    void setStatus(const string& v)  { status = v; }
    void setFullname(const string& v){ fullname = v; }
    void setEmail(const string& v)   { email = v; }

    // Nghiep vu chung
    bool isActive()  const { return status == "active"; }
    bool isLocked()  const { return status == "locked"; }

    bool login(const string& inputPassword) const;
    void logout() const;
    bool changePassword(const string& oldPass, const string& newPass);

    // Hien thi (co the override)
    virtual void displayInfo() const;
};

#endif
