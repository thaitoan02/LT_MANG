#include "User.h"
#include "Utils.h"
#include <iostream>
#include <iomanip>
using namespace std;

// ==========================================
// IMPLEMENTS: User
// ==========================================

User::User() : id(0) {}

User::User(int _id, const string& _username, const string& _password,
           const string& _role,   const string& _status,
           const string& _fullname, const string& _email)
    : id(_id), username(_username), password(_password),
      role(_role), status(_status), fullname(_fullname), email(_email) {}

bool User::login(const string& inputPassword) const {
    return password == hashPassword(inputPassword);
}

void User::logout() const {
    cout << "\n  Da dang xuat.\n";
}

bool User::changePassword(const string& oldPass, const string& newPass) {
    if (hashPassword(oldPass) != password) return false;
    password = hashPassword(newPass);
    return true;
}

void User::displayInfo() const {
    cout << left
         << setw(5)  << id
         << setw(15) << username.substr(0, 13)
         << setw(25) << fullname.substr(0, 23)
         << setw(12) << role
         << status   << "\n";
}
