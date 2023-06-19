#include <iostream>
#include <string>
#include <fstream>
#include <list>
#include <sstream>
#include <ctime>


#include "user.h"
#include "reader.h"
#include "library.h"

//物流管理
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
        std::cout << "借书还书信息加载成功！" << std::endl;
    } else {
        std::cout << "无法打开借书还书信息文件！" << std::endl;
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
        std::cout << "借书还书信息保存成功！" << std::endl;
    } else {
        std::cout << "无法打开借书还书信息文件！" << std::endl;
    }
}

//借书
int findBookIndex(const Library& library, const std::string& ISBN) {
    for (int i = 0; i < library.bookCount; i++) {
        if (library.books[i].ISBN == ISBN) {
            return i; // 返回找到的图书索引
        }
    }
    return -1; // 图书未找到，返回 -1
}
//获取当前时间
std::string getCurrentDate() {
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);

    int year = localTime->tm_year + 1900;
    int month = localTime->tm_mon + 1;
    int day = localTime->tm_mday;

    // 将日期格式化为字符串
    std::string currentDate = std::to_string(year) + "-" + std::to_string(month) + "-" + std::to_string(day);

    return currentDate;
}
//计算归还日期
std::string calculateReturnDate(const std::string& borrowDate, int loanPeriod) {
    // 将借书日期解析为年、月、日
    int borrowYear, borrowMonth, borrowDay;
    sscanf(borrowDate.c_str(), "%d-%d-%d", &borrowYear, &borrowMonth, &borrowDay);

    // 获取当前日期
    std::time_t now = std::time(nullptr);
    std::tm* localTime = std::localtime(&now);

    int currentYear = localTime->tm_year + 1900;
    int currentMonth = localTime->tm_mon + 1;
    int currentDay = localTime->tm_mday;

    // 计算还书日期
    int returnYear = borrowYear;
    int returnMonth = borrowMonth;
    int returnDay = borrowDay + loanPeriod;

    // 调整还书日期，确保月份和年份的正确性
    if (returnDay > 31) {
        returnDay -= 31;
        returnMonth++;
    }
    if (returnMonth > 12) {
        returnMonth -= 12;
        returnYear++;
    }

    // 将还书日期格式化为字符串
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

    // 检查图书可借数量
    int bookIndex = findBookIndex(library, ISBN);
    if (bookIndex == -1) {
        std::cout << "Book not found!" << std::endl;
        return;
    }

    if (library.books[bookIndex].copies - library.books[bookIndex].borrowed <= 0) {
        std::cout << "No copies available for borrowing!" << std::endl;
        return;
    }


    // 记录借书日期和还书预计日期
    std::string borrowDate = getCurrentDate();
    std::string returnDate = calculateReturnDate(borrowDate,90);

    // 更新图书的借出数量
    library.books[bookIndex].borrowed++;

    // 创建借书记录并添加到借书还书信息列表中
    BorrowRecord borrowInfo;
    borrowInfo.readerID = readerID;
    borrowInfo.ISBN = ISBN;
    borrowInfo.borrowDate = borrowDate;
    borrowInfo.returnDate = returnDate;
    borrowInfo.overdue = false;
    library.borrowList.push_back(borrowInfo);

    // 保存更新后的图书信息和借书还书信息到文件
    saveBooksToFile("book_main_file.txt", library);
    saveBorrowListToFile("borrow_list.txt", library.borrowList);

    std::cout << "Book borrowed successfully!" << std::endl;
}

void borrowBookMenu(){
    borrowBook(library);
}

//还书
//检查特定读者是否借阅了指定的图书
bool isBookBorrowedByReader(const Library& library, const std::string& readerID, const std::string& ISBN) {
    for (const BorrowRecord& borrowInfo : library.borrowList) {
        if (borrowInfo.readerID == readerID && borrowInfo.ISBN == ISBN) {
            return true;
        }
    }
    return false;
}
//借书记录是否逾期
bool isOverdue(const std::string& currentDate, const std::string& returnDate) {
    // 假设日期格式为YYYY-MM-DD，比较日期字符串大小进行判断
    return returnDate < currentDate;
}


void returnBook(Library& library) {
    std::string readerID, ISBN;
    std::cout << "Enter reader ID: ";
    std::cin >> readerID;
    std::cout << "Enter book ISBN: ";
    std::cin >> ISBN;

    // 检查读者是否借阅了该书
    if (!isBookBorrowedByReader(library, readerID, ISBN)) {
        std::cout << "该读者未借阅此书！" << std::endl;
        return;
    }

    // 获取当前日期
    std::string currentDate = getCurrentDate();

    // 更新借书记录列表
    for (auto& borrowInfo : library.borrowList) {
        if (borrowInfo.readerID == readerID && borrowInfo.ISBN == ISBN) {
            borrowInfo.returnDate = currentDate;
            borrowInfo.overdue = isOverdue(currentDate, borrowInfo.returnDate);
            break;
        }
    }

    // 更新图书的借出数
    for (auto& book : library.books) {
        if (book.ISBN == ISBN) {
            book.borrowed--;
            break;
        }
    }

    // 保存借书记录到文件
    saveBorrowListToFile("borrow_list.txt", library.borrowList);

    // 保存图书信息到文件
    saveBooksToFile("book_main_file.txt", library);

    std::cout << "还书处理完成！" << std::endl;
}

void returnBookMenu(){
    returnBook(library);
}

