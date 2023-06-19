#include <iostream>
#include <string>
#include <fstream>
#include <list>
#include <sstream>
#include <ctime>


#include "user.h"
#include "reader.h"
#include "library.h"

//��������
void loadBorrowListFromFile(const std::string& filename, list<BorrowRecord>& borrowList) {
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            BorrowRecord borrowInfo;
            std::getline(ss, borrowInfo.readerID, ',');
            std::getline(ss, borrowInfo.ISBN, ',');
            std::getline(ss, borrowInfo.borrowDate, ',');
            std::getline(ss, borrowInfo.returnDate, ',');
            std::string overdueStr;
            std::getline(ss, overdueStr, ',');
            borrowInfo.overdue = (overdueStr == "1");
            borrowList.push_back(borrowInfo);
        }
        file.close();
        std::cout << "���黹����Ϣ���سɹ���" << std::endl;
    } else {
        std::cout << "�޷��򿪽��黹����Ϣ�ļ���" << std::endl;
    }
}

void saveBorrowListToFile(const std::string& filename, const std::list<BorrowRecord>& borrowList) {
    std::ofstream file(filename);
    if (file.is_open()) {
        for (const auto& borrowInfo : borrowList) {
            file << borrowInfo.readerID << ","
                 << borrowInfo.ISBN << ","
                 << borrowInfo.borrowDate << ","
                 << borrowInfo.returnDate << ","
                 << (borrowInfo.overdue ? "1" : "0") << std::endl;
        }
        file.close();
        std::cout << "���黹����Ϣ����ɹ���" << std::endl;
    } else {
        std::cout << "�޷��򿪽��黹����Ϣ�ļ���" << std::endl;
    }
}

//����
int findBookIndex(const Library& library, const std::string& ISBN) {
    for (int i = 0; i < library.bookCount; i++) {
        if (library.books[i].ISBN == ISBN) {
            return i; // �����ҵ���ͼ������
        }
    }
    return -1; // ͼ��δ�ҵ������� -1
}
//��ȡ��ǰʱ��
std::string getCurrentDate() {
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);

    int year = localTime->tm_year + 1900;
    int month = localTime->tm_mon + 1;
    int day = localTime->tm_mday;

    // �����ڸ�ʽ��Ϊ�ַ���
    std::string currentDate = std::to_string(year) + "-" + std::to_string(month) + "-" + std::to_string(day);

    return currentDate;
}
//����黹����
std::string calculateReturnDate(const std::string& borrowDate, int loanPeriod) {
    // ���������ڽ���Ϊ�ꡢ�¡���
    int borrowYear, borrowMonth, borrowDay;
    sscanf(borrowDate.c_str(), "%d-%d-%d", &borrowYear, &borrowMonth, &borrowDay);

    // ��ȡ��ǰ����
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);

    int currentYear = localTime->tm_year + 1900;
    int currentMonth = localTime->tm_mon + 1;
    int currentDay = localTime->tm_mday;

    // ���㻹������
    int returnYear = borrowYear;
    int returnMonth = borrowMonth;
    int returnDay = borrowDay + loanPeriod;

    // �����������ڣ�ȷ���·ݺ���ݵ���ȷ��
    if (returnDay > 31) {
        returnDay -= 31;
        returnMonth++;
    }
    if (returnMonth > 12) {
        returnMonth -= 12;
        returnYear++;
    }

    // ���������ڸ�ʽ��Ϊ�ַ���
    std::string returnDate = std::to_string(returnYear) + "-" + std::to_string(returnMonth) + "-" + std::to_string(returnDay);

    return returnDate;
}

void borrowBook(Library& library) {
    std::string readerID;
    std::string ISBN;

    std::cout << "Enter reader ID: ";
    std::cin >> readerID;

    std::cout << "Enter book ISBN: ";
    std::cin >> ISBN;

    // ���ͼ��ɽ�����
    int bookIndex = findBookIndex(library, ISBN);
    if (bookIndex == -1) {
        std::cout << "Book not found!" << std::endl;
        return;
    }

    if (library.books[bookIndex].copies - library.books[bookIndex].borrowed <= 0) {
        std::cout << "No copies available for borrowing!" << std::endl;
        return;
    }


    // ��¼�������ںͻ���Ԥ������
    std::string borrowDate = getCurrentDate();
    std::string returnDate = calculateReturnDate(borrowDate,90);

    // ����ͼ��Ľ������
    library.books[bookIndex].borrowed++;

    // ���������¼����ӵ����黹����Ϣ�б���
    BorrowRecord borrowInfo;
    borrowInfo.readerID = readerID;
    borrowInfo.ISBN = ISBN;
    borrowInfo.borrowDate = borrowDate;
    borrowInfo.returnDate = returnDate;
    borrowInfo.overdue = false;
    library.borrowList.push_back(borrowInfo);

    // ������º��ͼ����Ϣ�ͽ��黹����Ϣ���ļ�
    saveBooksToFile("book_main_file.txt", library);
    saveBorrowListToFile("borrow_list.txt", library.borrowList);

    std::cout << "Book borrowed successfully!" << std::endl;
}

void borrowBookMenu(){
    borrowBook(library);
}

//����
//����ض������Ƿ������ָ����ͼ��
bool isBookBorrowedByReader(const Library& library, const std::string& readerID, const std::string& ISBN) {
    for (const BorrowRecord& borrowInfo : library.borrowList) {
        if (borrowInfo.readerID == readerID && borrowInfo.ISBN == ISBN) {
            return true;
        }
    }
    return false;
}
//�����¼�Ƿ�����
bool isOverdue(const std::string& currentDate, const std::string& returnDate) {
    // �������ڸ�ʽΪYYYY-MM-DD���Ƚ������ַ�����С�����ж�
    return returnDate < currentDate;
}


void returnBook(Library& library) {
    std::string readerID, ISBN;
    std::cout << "Enter reader ID: ";
    std::cin >> readerID;
    std::cout << "Enter book ISBN: ";
    std::cin >> ISBN;

    // �������Ƿ�����˸���
    if (!isBookBorrowedByReader(library, readerID, ISBN)) {
        std::cout << "�ö���δ���Ĵ��飡" << std::endl;
        return;
    }

    // ��ȡ��ǰ����
    std::string currentDate = getCurrentDate();

    // ���½����¼�б�
    for (auto& borrowInfo : library.borrowList) {
        if (borrowInfo.readerID == readerID && borrowInfo.ISBN == ISBN) {
            borrowInfo.returnDate = currentDate;
            borrowInfo.overdue = isOverdue(currentDate, borrowInfo.returnDate);
            break;
        }
    }

    // ����ͼ��Ľ����
    for (auto& book : library.books) {
        if (book.ISBN == ISBN) {
            book.borrowed--;
            break;
        }
    }

    // ��������¼���ļ�
    saveBorrowListToFile("borrow_list.txt", library.borrowList);

    // ����ͼ����Ϣ���ļ�
    saveBooksToFile("book_main_file.txt", library);

    std::cout << "���鴦����ɣ�" << std::endl;
}

void returnBookMenu(){
    returnBook(library);
}

