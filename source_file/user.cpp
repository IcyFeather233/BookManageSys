//
// Created by Administrator on 2023/6/8.
//
#include <fstream>
#include "user.h"

std::vector<User> User:: users;
User authenticatedUser;

User::User(string username, string password, int userType) {
    this->username = username;
    this->password = password;
    this->userType = userType;
}

void User::changePassword() {
    string newPassword;
    cout << "请输入新密码: ";
    cin >> newPassword;
    this->password = newPassword;
    cout << "密码修改成功！" << endl;
}


//存储用户信息
void saveUsersToFile(const string& filename, const vector<User>& users) {
    ofstream file(filename);
    if (file.is_open()) {
        for (const auto& user : users) {
            file << user.getUsername() << "," << user.getPassword() << "," << user.getUserType() << endl;
        }
        file.close();
        cout << "用户信息保存成功！" << endl;
    } else {
        cout << "无法打开用户信息文件！" << endl;
    }
}

void enterUserInfo() {
    User newAccount;
    cout << "Enter username: ";
    string username;
    cin >> username;
    newAccount.setUsername(username);

    cout << "Enter password: ";
    string password;
    cin >> password;
    newAccount.setPassword(password);

    cout << "Enter user type (1-用户管理员 2-图书管理员 3-普通读者): ";
    int userType;
    cin >> userType;
    newAccount.setUserType(userType);

    User::users.push_back(newAccount);
    saveUsersToFile("users.txt", User::users);

    cout << "用户信息输入成功！" << endl;
}

//for (int i = 0; i  < n; i ++)
// for (int i = 0; i < n;) xxxxxx;i++;
//for (int i = 0; i  < n; ++ i)
// for (int i = 0; i < n;) xxxxxx;++i;

//用户删除
void deleteUser(vector<User>& users, const string& username) {
    for (auto it = users.begin(); it != users.end(); ++it) {
        if (it->getUsername() == username) {
            users.erase(it);
            cout << "用户删除成功！" << endl;
            return;
        }
    }
    cout << "找不到指定的用户！" << endl;
}

// 用户管理菜单
void deleteUserMenu() {
    cout << "Enter username to delete: ";
    string username;
    cin >> username;
    deleteUser(User::users, username);
    saveUsersToFile("users.txt", User::users);  // 保存用户信息到文件
}

//读取文件
void loadUsersFromFile(const string& filename, vector<User>& users) {
    ifstream file(filename);
    if (file.is_open()) {
        users.clear();
        string line;
        while (getline(file, line)) {
            // 从每行中解析用户名、密码和用户类型
            string username, password;
            int userType;
            size_t commaPos1 = line.find(',');
            size_t commaPos2 = line.rfind(',');

            if (commaPos1 != string::npos && commaPos2 != string::npos) {
                username = line.substr(0, commaPos1);
                password = line.substr(commaPos1 + 1, commaPos2 - commaPos1 - 1);
                userType = stoi(line.substr(commaPos2 + 1));

                // 创建 User 对象并添加到 users 向量中
                User user(username, password, userType);
                users.push_back(user);
            }
        }
        file.close();
    } else {
        cout << "无法打开用户信息文件！" << endl;
    }
}

//用户信息显示
void displayUserInfo(const vector<User>& users) {
    cout << "用户信息：" << endl;
    for (const auto& user : users) {
        cout << "用户名：" << user.getUsername() << endl;
        cout << "密码：" << user.getPassword() << endl;
        cout << "用户类型：" << user.getUserType();
        switch (user.getUserType()) {
            case 1:
                cout << "（用户管理员）" << endl;
                break;
            case 2:
                cout << "（图书管理员）" << endl;
                break;
            case 3:
                cout << "（普通读者）" << endl;
                break;
        }
        cout << "---------------------" << endl;
    }
}


void displayUserMenu() {
    displayUserInfo(User::users);
}

//修改密码
void changeUserInfo(vector<User>& users, const string& username, const string& newPassword, const int& newType) {
    for (auto& user : users) {
        if (user.getUsername() == username) {
            user.setPassword(newPassword);
            user.setUserType(newType);
            cout << "用户信息修改成功！" << endl;
            return;
        }
    }
    cout << "找不到指定的用户！" << endl;
}

// 用户信息修改
void editUserMenu() {
    cout << "Enter username: ";
    string username;
    cin >> username;

    cout << "Enter new password: ";
    string newPassword;
    cin >> newPassword;

    cout << "Enter new user type: ";
    int newType;
    cin >> newType;

    changeUserInfo(User::users, username, newPassword, newType);
    saveUsersToFile("users.txt", User::users);  // 保存用户信息到文件
}

// 用户密码修改
void editUserPasswd() {
    cout << "Enter new password: ";
    string newPassword;
    cin >> newPassword;
    changeUserInfo(User::users, authenticatedUser.getUsername(), newPassword, authenticatedUser.getUserType());
    saveUsersToFile("users.txt", User::users);  // 保存用户信息到文件
}


// 验证用户登录
bool authenticateUser(const vector<User>& users, const string& username, const string& password, User& authenticatedUser) {
    for (const auto& user : users) {
        if (user.getUsername() == username && user.getPassword() == password) {
            authenticatedUser = user;
            return true;
        }
    }
    return false;
}


