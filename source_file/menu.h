//
// Created by IcyFeather on 2023/6/5.
//

#ifndef BOOKMANAGE_MENU_H
#define BOOKMANAGE_MENU_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

// �˵�����
class MenuItem {
public:
    string name;      // �˵�������
    vector<MenuItem*> subMenus;  // �Ӳ˵�
    // ����ֵΪvoid������Ϊ�յ� ����ָ��
    // func = xxx ���Ը��Ƴɷ���ֵΪvoid������Ϊ�յ� ����
    // void print();
    // ���� func = print;
    // ִ�� func() = print();
    void (*func)();  // ִ�к���
    MenuItem* parent;    // ���˵���

    MenuItem();

    // ���캯��
    MenuItem(string name);

    // ���ƹ��캯��
    MenuItem(const MenuItem &other);

    MenuItem& operator=(const MenuItem &other);

    void showMenu();

    void addSubMenu(MenuItem* menu);

    void execFunc(MenuItem* menu);

};


#endif //BOOKMANAGE_MENU_H
