//
// Created by IcyFeather on 2023/6/5.
//

#ifndef BOOKMANAGE_MENU_H
#define BOOKMANAGE_MENU_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

// 菜单项类
class MenuItem {
public:
    string name;      // 菜单项名称
    vector<MenuItem*> subMenus;  // 子菜单
    // 返回值为void，参数为空的 函数指针
    // func = xxx 可以复制成返回值为void，参数为空的 函数
    // void print();
    // 定义 func = print;
    // 执行 func() = print();
    void (*func)();  // 执行函数
    MenuItem* parent;    // 父菜单项

    MenuItem();

    // 构造函数
    MenuItem(string name);

    // 复制构造函数
    MenuItem(const MenuItem &other);

    MenuItem& operator=(const MenuItem &other);

    void showMenu();

    void addSubMenu(MenuItem* menu);

    void execFunc(MenuItem* menu);

};


#endif //BOOKMANAGE_MENU_H
