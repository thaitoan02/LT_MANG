#ifndef ADMIN_H
#define ADMIN_H

#include "User.h"
#include <vector>
using namespace std;

// ==========================================
// CLASS: Admin - Quan tri vien (ke thua User)
// ==========================================
class Admin : public User {
public:
    Admin();
    Admin(int id, const string& username, const string& password,
          const string& status, const string& fullname, const string& email);

    // Quan ly tai khoan
    void listUsers(const vector<User*>& users) const;
    void addUser(vector<User*>& users);
    void lockUser(vector<User*>& users, int targetId, const User* currentUser);
    void unlockUser(vector<User*>& users, int targetId);
    void updateUser(vector<User*>& users);

    void displayInfo() const override;
};

#endif
