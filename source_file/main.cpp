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


// ȫ�ֱ���


MenuItem m_main("���˵�");

MenuItem m_return_main("�������˵�"), m_exit("�˳�ϵͳ");

// ���˵�����Ĳ˵�
MenuItem m_user_manage("�û�����˵�"), m_reader_manage("���߹���˵�"),m_book_manage("ͼ�����"),m_book_circulation_manage("ͼ����ͨ����");
// �û�����˵�
MenuItem m_user_input("�û���Ϣ����"), m_user_edit("�û���Ϣ�޸�"), m_user_passwd_edit("�û������޸�"), m_user_delete("�û���Ϣɾ��"),m_user_display("�û���Ϣ��ʾ");
// ���߹���˵�
MenuItem m_reader_input("������Ϣ����"),m_reader_delete("������Ϣɾ��"),m_reader_find("������Ϣ��ѯ"),m_reader_display("������Ϣ��ʾ"),m_reader_edit("������Ϣ�޸�");
//ͼ�����˵�
MenuItem m_book_input("ͼ����Ϣ����"), m_book_edit("ͼ����Ϣ�޸�"),m_book_find("ͼ����Ϣ��ѯ"),m_book_total("����ͳ��");
//ͼ����������
MenuItem m_book_circulation_borrow("���鴦��"), m_book_circulation_return("���鴦��");
//ͼ����Ϣ��ѯ
MenuItem m_book_find_number("����Ų�ѯ"),m_book_find_name("��������ѯ"),m_book_find_author("�����߲�ѯ"),m_book_find_publish("���������ѯ");
MenuItem m_cur;


void return_main() {
    m_cur = m_main;
}

void exit_pro() {
    exit(0);
}



void manage(int userType) {
    if (userType == 3) { // ��ͨ����
    }
    else if (userType == 2) { // ͼ�����Ա
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
    else if (userType == 1) { // ϵͳ����Ա
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
    loadUsersFromFile("users.txt", User::users);  // ���ļ������û���Ϣ
    loadReadersFromFile("readers.txt", Reader::readers);  // ���ļ����ض�����Ϣ
    loadBooksFromFile("book_main_file.txt", library);
    loadBorrowListFromFile( "borrow_list.txt", library.borrowList);
    createTitleIndex("title_index.txt", library);
    createAuthorIndex("author_index.txt", library);
    createPublisherIndex("publisher_index.txt", library);
    while (attemptCount < maxAttempts) {
        std::string username, password;
        std::cout << "�������û���: ";
        std::cin >> username;
        std::cout << "����������: ";
        std::cin >> password;

        // for (auto x: ������)
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
            std::cout << "�û�����������������ԡ�\n";
            attemptCount++;
        }
    }

    if (attemptCount == maxAttempts) {
        std::cout << "���Դ����Ѵ����ޣ������˳���\n";
        // ����������ִ���˳�������߼�
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