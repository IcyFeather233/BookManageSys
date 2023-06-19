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

std::list<Reader> Reader::readers; // ��̬��Ա������Ҫ��������ж���
//����
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
        std::cout << "������Ϣ����ɹ���" << std::endl;
    } else {
        std::cout << "�޷��򿪶�����Ϣ�ļ���" << std::endl;
    }
}

void loadReadersFromFile(const std::string& filename, std::list<Reader>& readers) {
    std::ifstream file(filename);
    if (file.is_open()) {
        readers.clear();
        std::string line;
        while (std::getline(file, line)) {
            // ��ÿ���н���������Ϣ
            std::string id, name, unit, contact;
            int maxBooks, borrowedBooks;
            std::istringstream iss(line);
            std::string field;

            // ��������ֶ�
            if (std::getline(iss, field, ',')) id = field;
            if (std::getline(iss, field, ',')) name = field;
            if (std::getline(iss, field, ',')) unit = field;
            if (std::getline(iss, field, ',')) contact = field;
            if (std::getline(iss, field, ',')) maxBooks = std::stoi(field);
            if (std::getline(iss, field, ',')) borrowedBooks = std::stoi(field);

            // ���� Reader ������ӵ� readers �б���
            Reader reader(id, name, unit, contact);
            reader.setMaxBooks(maxBooks);
            reader.setBorrowedBooks(borrowedBooks);
            readers.push_back(reader);
        }
        file.close();
    } else {
        std::cout << "�޷��򿪶�����Ϣ�ļ���" << std::endl;
    }
}


void enterReaderInfo() {
    Reader newReader;

    // ������ߺ�
    std::cout << "Enter reader ID: ";
    std::string id;
    std::cin >> id;

    // ���ݶ��ߺų����ж�ѧ����̹�
    if (id.length() == 8) {
        newReader.setMaxBooks(5);  // ѧ���ɽ�����Ϊ 5 ��
    } else if (id.length() == 7) {
        newReader.setMaxBooks(10);  // �̹��ɽ�����Ϊ 10 ��
    } else {
        std::cout << "��Ч�Ķ��ߺų��ȣ�" << std::endl;
        return;
    }

    // ��������������Ϣ
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

    // ���¶�����ӵ������б���
    Reader::readers.push_back(newReader);
    saveReadersToFile("readers.txt", Reader::readers);

    std::cout << "������Ϣ����ɹ���" << std::endl;
}

void deleteReader(std::list<Reader>& readers, const std::string& id) {
    for (auto it = readers.begin(); it != readers.end(); ++it) {
        if (it->getId() == id) {
            readers.erase(it);
            std::cout << "����ɾ���ɹ���" << std::endl;
            return;
        }
    }
    std::cout << "�Ҳ���ָ���Ķ��ߣ�" << std::endl;
}

void deleteReaderMenu() {
    std::cout << "Enter reader ID to delete: ";
    std::string id;
    std::cin >> id;
    deleteReader(Reader::readers, id);
    saveReadersToFile("readers.txt", Reader::readers);  // ���������Ϣ���ļ�
}

void displayReaderInfo(const std::list<Reader>& readers) {
    // ʹ��lambda���ʽ�����������
    auto compareByName = [](const Reader& a, const Reader& b) {
        return a.getName() < b.getName();
    };
    // ʹ��sort����������������
    std::list<Reader> sortedReaders(readers);
    sortedReaders.sort(compareByName);

    // ������ߵ���Ϣ
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
            std::cout << "������Ϣ�޸ĳɹ���" << std::endl;
            return;
        }
    }
    std::cout << "�Ҳ���ָ���Ķ��ߣ�" << std::endl;
}

void findReaderMenu() {
    cout << "Enter reader ID or name to search: ";
    string keyword;
    cin >> keyword;

    // ���ݹؼ�����readers�������в���ƥ��Ķ�����Ϣ
    vector<Reader> matchingReaders;
    for (const auto& reader : Reader::readers) {
        if (reader.getId() == keyword || reader.getName() == keyword) {
            matchingReaders.push_back(reader);
        }
    }

    // ��ʾ��ѯ���
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