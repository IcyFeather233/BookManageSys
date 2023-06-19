#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "library.h"


#include "menu.h"
#include "user.h"
#include "reader.h"
#include "bookcm.h"

using namespace std;


// 全局变量


MenuItem m_main("主菜单");

MenuItem m_return_main("返回主菜单"), m_exit("退出系统");

// 主菜单下面的菜单
MenuItem m_user_manage("用户管理菜单"), m_reader_manage("读者管理菜单"),m_book_manage("图书管理"),m_book_circulation_manage("图书流通管理");
// 用户管理菜单
MenuItem m_user_input("用户信息输入"), m_user_edit("用户信息修改"), m_user_passwd_edit("用户密码修改"), m_user_delete("用户信息删除"),m_user_display("用户信息显示");
// 读者管理菜单
MenuItem m_reader_input("读者信息输入"),m_reader_delete("读者信息删除"),m_reader_find("读者信息查询"),m_reader_display("读者信息显示"),m_reader_edit("读者信息修改");
//图书管理菜单
MenuItem m_book_input("图书信息输入"), m_book_edit("图书信息修改"),m_book_find("图书信息查询"),m_book_total("汇总统计");
//图书物流管理
MenuItem m_book_circulation_borrow("借书处理"), m_book_circulation_return("还书处理");
//图书信息查询
MenuItem m_book_find_number("按书号查询"),m_book_find_name("按书名查询"),m_book_find_author("按作者查询"),m_book_find_publish("按出版社查询");
MenuItem m_cur;


void return_main() {
    m_cur = m_main;
}

void exit_pro() {
    exit(0);
}



void manage(int userType) {
    if (userType == 3) { // 普通读者
    }
    else if (userType == 2) { // 图书管理员
        m_reader_manage.addSubMenu(&m_reader_input);
        m_reader_manage.addSubMenu(&m_reader_edit);
        m_reader_manage.addSubMenu(&m_reader_delete);
        m_reader_manage.addSubMenu(&m_reader_find);
        m_reader_manage.addSubMenu(&m_reader_display);
        m_book_manage.addSubMenu(&m_book_input);
        m_book_manage.addSubMenu(&m_book_edit);
        m_book_circulation_manage.addSubMenu(&m_book_circulation_borrow);
        m_book_circulation_manage.addSubMenu(&m_book_circulation_return);
    }
    else if (userType == 1) { // 系统管理员
        m_user_manage.addSubMenu(&m_user_input);
        m_user_manage.addSubMenu(&m_user_delete);
        m_user_manage.addSubMenu(&m_user_display);
        m_user_manage.addSubMenu(&m_user_edit);
    }
    m_user_manage.addSubMenu(&m_user_passwd_edit);
    m_user_manage.addSubMenu(&m_return_main);
    m_reader_manage.addSubMenu(&m_return_main);
    m_book_manage.addSubMenu(&m_book_find);
    m_book_find.addSubMenu(&m_book_find_name);
    m_book_find.addSubMenu(&m_book_find_number);
    m_book_find.addSubMenu(&m_book_find_author);
    m_book_find.addSubMenu(&m_book_find_publish);
    m_book_manage.addSubMenu(&m_book_total);
    m_book_manage.addSubMenu(&m_return_main);
    m_book_circulation_manage.addSubMenu(&m_return_main);
}


int main() {
//    User authenticatedUser;
    int maxAttempts = 3;
    int attemptCount = 0;
    bool isAuthenticated = false;
    loadUsersFromFile("users.txt", User::users);  // 从文件加载用户信息
    loadReadersFromFile("readers.txt", Reader::readers);  // 从文件加载读者信息
    loadBooksFromFile("book_main_file.txt", library);
    loadBorrowListFromFile( "borrow_list.txt", library.borrowList);
    createTitleIndex("title_index.txt", library);
    createAuthorIndex("author_index.txt", library);
    createPublisherIndex("publisher_index.txt", library);
    while (attemptCount < maxAttempts) {
        std::string username, password;
        std::cout << "请输入用户名: ";
        std::cin >> username;
        std::cout << "请输入密码: ";
        std::cin >> password;

        // for (auto x: 迭代器)
        // for (int i = 0; i <= n; i++)
        // vector: 1-n
        // for (auto i: vector)
        for (const auto& user : User::users) {
            if (user.getUsername() == username && user.getPassword() == password) {
                isAuthenticated = true;
                authenticatedUser = user;
                break;
            }
        }
        if (authenticateUser(User::users, username, password, authenticatedUser)) {
            int userType = authenticatedUser.getUserType();
            m_main.addSubMenu(&m_user_manage);
            m_main.addSubMenu(&m_reader_manage);
            m_main.addSubMenu(&m_book_manage);
            m_main.addSubMenu(&m_book_circulation_manage);
            m_main.addSubMenu(&m_exit);
            manage(userType);
            break;
        } else {
            std::cout << "用户名或密码错误，请重试。\n";
            attemptCount++;
        }
    }

    if (attemptCount == maxAttempts) {
        std::cout << "尝试次数已达上限，程序退出。\n";
        // 可以在这里执行退出程序的逻辑
    }

    m_return_main.func = return_main;

    m_exit.func = exit_pro;

    m_user_input.func = enterUserInfo;

    m_user_delete.func = deleteUserMenu;

    m_user_display.func = displayUserMenu;

    m_user_edit.func = editUserMenu;

    m_user_passwd_edit.func = editUserPasswd;

    m_reader_input.func=enterReaderInfo;

    m_reader_edit.func=editReaderMenu;

    m_reader_delete.func=deleteReaderMenu;

    m_reader_find.func=findReaderMenu;

    m_reader_display.func=displayReaderMenu;

    m_book_input.func=enterBookInfoMenu;

    m_book_edit.func=modifyBookInfoMenu;

    m_book_find_name.func=searchBookByTitleMenu;

    m_book_find_number.func=searchBookByISBNMenu;

    m_book_find_author.func=searchByAuthorMenu;

    m_book_find_publish.func=searchByPublisherMenu;

    m_book_total.func=displayAllBooksMenu;

    m_book_circulation_borrow.func=borrowBookMenu;

    m_book_circulation_return.func=returnBookMenu;

    m_cur = m_main;

    int op = 0;

    m_cur.showMenu();
    while (cin >> op) {
        MenuItem m_select = *m_cur.subMenus[op - 1];
        if (m_select.func == NULL) {
            m_cur = m_select;
        }
        else {
            m_select.func();
        }
        m_cur.showMenu();
    }
    system("pause");
}