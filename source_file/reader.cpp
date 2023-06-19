//
// Created by Administrator on 2023/6/13.
//
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <list>
#include <sstream>



#include "user.h"
#include "reader.h"

std::list<Reader> Reader::readers; // 静态成员变量需要在类外进行定义
//读者
void saveReadersToFile(const std::string& filename, const std::list<Reader>& readers) {
    std::ofstream file(filename);
    if (file.is_open()) {
        for (const auto& reader : readers) {
            file << reader.getId() << ","
                 << reader.getName() << ","
                 << reader.getUnit() << ","
                 << reader.getContact() << ","
                 << reader.getMaxBooks() << ","
                 << reader.getBorrowedBooks() << std::endl;
        }
        file.close();
        std::cout << "读者信息保存成功！" << std::endl;
    } else {
        std::cout << "无法打开读者信息文件！" << std::endl;
    }
}

void loadReadersFromFile(const std::string& filename, std::list<Reader>& readers) {
    std::ifstream file(filename);
    if (file.is_open()) {
        readers.clear();
        std::string line;
        while (std::getline(file, line)) {
            // 从每行中解析读者信息
            std::string id, name, unit, contact;
            int maxBooks, borrowedBooks;
            std::istringstream iss(line);
            std::string field;

            // 逐个解析字段
            if (std::getline(iss, field, ',')) id = field;
            if (std::getline(iss, field, ',')) name = field;
            if (std::getline(iss, field, ',')) unit = field;
            if (std::getline(iss, field, ',')) contact = field;
            if (std::getline(iss, field, ',')) maxBooks = std::stoi(field);
            if (std::getline(iss, field, ',')) borrowedBooks = std::stoi(field);

            // 创建 Reader 对象并添加到 readers 列表中
            Reader reader(id, name, unit, contact);
            reader.setMaxBooks(maxBooks);
            reader.setBorrowedBooks(borrowedBooks);
            readers.push_back(reader);
        }
        file.close();
    } else {
        std::cout << "无法打开读者信息文件！" << std::endl;
    }
}


void enterReaderInfo() {
    Reader newReader;

    // 输入读者号
    std::cout << "Enter reader ID: ";
    std::string id;
    std::cin >> id;

    // 根据读者号长度判断学生或教工
    if (id.length() == 8) {
        newReader.setMaxBooks(5);  // 学生可借书数为 5 本
    } else if (id.length() == 7) {
        newReader.setMaxBooks(10);  // 教工可借书数为 10 本
    } else {
        std::cout << "无效的读者号长度！" << std::endl;
        return;
    }

    // 输入其他读者信息
    newReader.setId(id);
    std::cout << "Enter reader name: ";
    std::string name;
    std::cin >> name;
    newReader.setName(name);

    std::cout << "Enter reader unit: ";
    std::string unit;
    std::cin >> unit;
    newReader.setUnit(unit);

    std::cout << "Enter reader contact: ";
    std::string contact;
    std::cin >> contact;
    newReader.setContact(contact);

    // 将新读者添加到读者列表中
    Reader::readers.push_back(newReader);
    saveReadersToFile("readers.txt", Reader::readers);

    std::cout << "读者信息输入成功！" << std::endl;
}

void deleteReader(std::list<Reader>& readers, const std::string& id) {
    for (auto it = readers.begin(); it != readers.end(); ++it) {
        if (it->getId() == id) {
            readers.erase(it);
            std::cout << "读者删除成功！" << std::endl;
            return;
        }
    }
    std::cout << "找不到指定的读者！" << std::endl;
}

void deleteReaderMenu() {
    std::cout << "Enter reader ID to delete: ";
    std::string id;
    std::cin >> id;
    deleteReader(Reader::readers, id);
    saveReadersToFile("readers.txt", Reader::readers);  // 保存读者信息到文件
}

void displayReaderInfo(const std::list<Reader>& readers) {
    // 使用lambda表达式定义排序规则
    auto compareByName = [](const Reader& a, const Reader& b) {
        return a.getName() < b.getName();
    };
    // 使用sort函数按照姓名排序
    std::list<Reader> sortedReaders(readers);
    sortedReaders.sort(compareByName);

    // 输出读者的信息
    for (const auto& reader : sortedReaders) {
        reader.displayInfo();
        std::cout << std::endl;
    }
}

void displayReaderMenu() {
    displayReaderInfo(Reader::readers);
}

void editReaderMenu() {
    std::cout << "Enter reader ID: ";
    std::string id;
    std::cin >> id;

    cout << "Enter new reader name: ";
    string newName;
    cin >> newName;

    cout << "Enter new Unit: ";
    string newUnit;
    cin >> newUnit;

    std::cout << "Enter new contact: ";
    std::string newContact;
    std::cin >> newContact;

    for (auto& reader : Reader::readers) {
        if (reader.getId() == id) {
            reader.setName(newName);
            reader.setUnit(newUnit);
            reader.setContact(newContact);
            std::cout << "读者信息修改成功！" << std::endl;
            return;
        }
    }
    std::cout << "找不到指定的读者！" << std::endl;
}

void findReaderMenu() {
    cout << "Enter reader ID or name to search: ";
    string keyword;
    cin >> keyword;

    // 根据关键词在readers单链表中查找匹配的读者信息
    vector<Reader> matchingReaders;
    for (const auto& reader : Reader::readers) {
        if (reader.getId() == keyword || reader.getName() == keyword) {
            matchingReaders.push_back(reader);
        }
    }

    // 显示查询结果
    if (matchingReaders.empty()) {
        cout << "No matching readers found." << endl;
    } else {
        cout << "Matching readers:" << endl;
        for (const auto& reader : matchingReaders) {
            cout << "Reader ID: " << reader.getId() << endl;
            cout << "Name: " << reader.getName() << endl;
            cout << "Unit: " << reader.getUnit() << endl;
            cout << "Contact: " << reader.getContact() << endl;
            cout << "Max Books: " << reader.getMaxBooks() << endl;
            cout << "Borrowed Books: " << reader.getBorrowedBooks() << endl;
            cout << "---------------------" << endl;
        }
    }
}