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
    cout << "������������: ";
    cin >> newPassword;
    this->password = newPassword;
    cout << "�����޸ĳɹ���" << endl;
}


//�洢�û���Ϣ
void saveUsersToFile(const string& filename, const vector<User>& users) {
    ofstream file(filename);
    if (file.is_open()) {
        for (const auto& user : users) {
            file << user.getUsername() << "," << user.getPassword() << "," << user.getUserType() << endl;
        }
        file.close();
        cout << "�û���Ϣ����ɹ���" << endl;
    } else {
        cout << "�޷����û���Ϣ�ļ���" << endl;
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

    cout << "Enter user type (1-�û�����Ա 2-ͼ�����Ա 3-��ͨ����): ";
    int userType;
    cin >> userType;
    newAccount.setUserType(userType);

    User::users.push_back(newAccount);
    saveUsersToFile("users.txt", User::users);

    cout << "�û���Ϣ����ɹ���" << endl;
}

//for (int i = 0; i  < n; i ++)
// for (int i = 0; i < n;) xxxxxx;i++;
//for (int i = 0; i  < n; ++ i)
// for (int i = 0; i < n;) xxxxxx;++i;

//�û�ɾ��
void deleteUser(vector<User>& users, const string& username) {
    for (auto it = users.begin(); it != users.end(); ++it) {
        if (it->getUsername() == username) {
            users.erase(it);
            cout << "�û�ɾ���ɹ���" << endl;
            return;
        }
    }
    cout << "�Ҳ���ָ�����û���" << endl;
}

// �û�����˵�
void deleteUserMenu() {
    cout << "Enter username to delete: ";
    string username;
    cin >> username;
    deleteUser(User::users, username);
    saveUsersToFile("users.txt", User::users);  // �����û���Ϣ���ļ�
}

//��ȡ�ļ�
void loadUsersFromFile(const string& filename, vector<User>& users) {
    ifstream file(filename);
    if (file.is_open()) {
        users.clear();
        string line;
        while (getline(file, line)) {
            // ��ÿ���н����û�����������û�����
            string username, password;
            int userType;
            size_t commaPos1 = line.find(',');
            size_t commaPos2 = line.rfind(',');

            if (commaPos1 != string::npos && commaPos2 != string::npos) {
                username = line.substr(0, commaPos1);
                password = line.substr(commaPos1 + 1, commaPos2 - commaPos1 - 1);
                userType = stoi(line.substr(commaPos2 + 1));

                // ���� User ������ӵ� users ������
                User user(username, password, userType);
                users.push_back(user);
            }
        }
        file.close();
    } else {
        cout << "�޷����û���Ϣ�ļ���" << endl;
    }
}

//�û���Ϣ��ʾ
void displayUserInfo(const vector<User>& users) {
    cout << "�û���Ϣ��" << endl;
    for (const auto& user : users) {
        cout << "�û�����" << user.getUsername() << endl;
        cout << "���룺" << user.getPassword() << endl;
        cout << "�û����ͣ�" << user.getUserType();
        switch (user.getUserType()) {
            case 1:
                cout << "���û�����Ա��" << endl;
                break;
            case 2:
                cout << "��ͼ�����Ա��" << endl;
                break;
            case 3:
                cout << "����ͨ���ߣ�" << endl;
                break;
        }
        cout << "---------------------" << endl;
    }
}


void displayUserMenu() {
    displayUserInfo(User::users);
}

//�޸�����
void changeUserInfo(vector<User>& users, const string& username, const string& newPassword, const int& newType) {
    for (auto& user : users) {
        if (user.getUsername() == username) {
            user.setPassword(newPassword);
            user.setUserType(newType);
            cout << "�û���Ϣ�޸ĳɹ���" << endl;
            return;
        }
    }
    cout << "�Ҳ���ָ�����û���" << endl;
}

// �û���Ϣ�޸�
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
    saveUsersToFile("users.txt", User::users);  // �����û���Ϣ���ļ�
}

// �û������޸�
void editUserPasswd() {
    cout << "Enter new password: ";
    string newPassword;
    cin >> newPassword;
    changeUserInfo(User::users, authenticatedUser.getUsername(), newPassword, authenticatedUser.getUserType());
    saveUsersToFile("users.txt", User::users);  // �����û���Ϣ���ļ�
}


// ��֤�û���¼
bool authenticateUser(const vector<User>& users, const string& username, const string& password, User& authenticatedUser) {
    for (const auto& user : users) {
        if (user.getUsername() == username && user.getPassword() == password) {
            authenticatedUser = user;
            return true;
        }
    }
    return false;
}


