//
// Created by IcyFeather on 2023/6/5.
//

#include "menu.h"

MenuItem::MenuItem() {
    this->name = "null menu";
    func = NULL;
}

MenuItem::MenuItem(string name) {
    this->name = name;
    func = NULL;
}

MenuItem::MenuItem(const MenuItem &other) {
    name = other.name;
    subMenus = other.subMenus;
    func = other.func;
    parent = other.parent;
}

MenuItem &MenuItem::operator=(const MenuItem &other) {
    name = other.name;
    subMenus = other.subMenus;
    func = other.func;
    parent = other.parent;
    return *this;
}

void MenuItem::showMenu() {
    cout << "**********************************" << endl;
    for (int i = 0; i < subMenus.size(); i++) {
        cout << i + 1 << ". " << subMenus[i]->name << endl;
    }
    cout << "**********************************" << endl;
    cout << "ÇëÑ¡Ôñ:";
}

void MenuItem::execFunc(MenuItem* menu) {
    func();
}

void MenuItem::addSubMenu(MenuItem *menu) {
    subMenus.push_back(menu);
    menu->parent = this;
}