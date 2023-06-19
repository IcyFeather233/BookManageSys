#ifndef READER_H
#define READER_H

#include <string>
#include <list>


class Reader {
private:
    std::string id;
    std::string name;
    std::string unit;
    std::string contact;
    int maxBooks;
    int borrowedBooks;

public:
    // 添加无参构造函数
    Reader() : maxBooks(0), borrowedBooks(0) {}

    // 添加带参数的构造函数
    Reader(const std::string& id, const std::string& name, const std::string& unit, const std::string& contact)
            : id(id), name(name), unit(unit), contact(contact), maxBooks(0), borrowedBooks(0) {}

    std::string getId() const {
        return id;
    }

    std::string getName() const {
        return name;
    }

    std::string getUnit() const {
        return unit;
    }

    std::string getContact() const {
        return contact;
    }

    int getMaxBooks() const {
        return maxBooks;
    }

    int getBorrowedBooks() const {
        return borrowedBooks;
    }

    void setId(const std::string& id) {
        this->id = id;
    }

    void setName(const std::string& name) {
        this->name = name;
    }

    void setUnit(const std::string& unit) {
        this->unit = unit;
    }

    void setContact(const std::string& contact) {
        this->contact = contact;
    }

    void setMaxBooks(int maxBooks) {
        this->maxBooks = maxBooks;
    }

    void setBorrowedBooks(int borrowedBooks) {
        this->borrowedBooks = borrowedBooks;
    }

    void displayInfo() const {
        std::cout << "ID: " << id << std::endl;
        std::cout << "Name: " << name << std::endl;
        std::cout << "Unit: " << unit << std::endl;
        std::cout << "Contact: " << contact << std::endl;
        std::cout << "Max books: " << maxBooks << std::endl;
        std::cout << "Borrowed books: " << borrowedBooks << std::endl;
    }

    static std::list<Reader> readers; // 存储读者信息的单链表
};



void saveReadersToFile(const std::string& filename, const std::list<Reader>& readers);

void loadReadersFromFile(const std::string& filename, std::list<Reader>& readers);

void enterReaderInfo();

void deleteReader(std::list<Reader>& readers, const std::string& id);

void deleteReaderMenu();

void displayReaderInfo(const std::list<Reader>& readers);

void displayReaderMenu();

void editReaderMenu();

void findReaderMenu();


#endif
