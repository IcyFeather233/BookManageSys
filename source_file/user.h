#ifndef USER_H
#define USER_H

#include <iostream>
#include <string>
#include <vector>


using namespace std;

class User {
private:
    string username;
    string password;
    int userType;  // 用户类型：1-用户管理员 2-图书管理员 3-普通读者

public:
    User(string username, string password, int userType);

    static std::vector<User> users;

    User() {}

    void changePassword();

    std::string getUsername() const {
        return username;
    }

    std::string getPassword() const {
        return password;
    }

    int getUserType() const {
        return userType;
    }

    void setUserType(int userType) {
        this->userType = userType;
    }

    void setPassword(const std::string& password) {
        this->password = password;
    }

    void setUsername(const std::string& username) {
        this->username = username;
    }

};

extern User authenticatedUser;


void saveUsersToFile(const string& filename, const vector<User>& users);
void enterUserInfo();
void deleteUser(vector<User>& users, const string& username);
void deleteUserMenu();
void loadUsersFromFile(const string& filename, vector<User>& users);
void displayUserInfo(const vector<User>& users);
void displayUserMenu();
void changeUserInfo(vector<User>& users, const string& username, const string& newPassword, const int& newType);
void editUserMenu();
bool authenticateUser(const vector<User>& users, const string& username, const string& password, User& authenticatedUser);
void editUserPasswd();


#endif  // USER_H
