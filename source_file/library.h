//
// Created by Administrator on 2023/6/13.
//

#ifndef TEST_LIBRARY_H
#define TEST_LIBRARY_H

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <unordered_map>


#include "user.h"


const int MAX_BOOKS = 100; // 最大图书数量

struct Book {
    string ISBN;
    string title;
    string author;
    string publisher;
    int copies;
    int borrowed;
};


struct BookIndex {
    unordered_map<string, string> titleIndex; // 书名索引表
    unordered_map<string, string> authorIndex; // 作者索引表
    unordered_map<string, string> publisherIndex; // 出版社索引表
};


struct BorrowRecord {
    std::string readerID;
    std::string ISBN;
    std::string borrowDate;
    std::string returnDate;
    bool overdue;

    BorrowRecord() : overdue(false) {}
};

struct Library {
    Book books[MAX_BOOKS];
    int bookCount;
    BookIndex bookIndex;

    Library() : bookCount(0) {}
    std::list<BorrowRecord> borrowList;
};

extern Library library; // 声明一个外部的全局变量

void saveBooksToFile(const string& filename, const Library& library);

void loadBooksFromFile(const string& filename, Library& library);

void updateIndexTable(const string& indexFilename, const unordered_map<string, string>& indexTable);

void createTitleIndex(const string& indexFilename, const Library& library);

void createAuthorIndex(const string& indexFilename, const Library& library);

void createPublisherIndex(const string& indexFilename, const Library& library);

void enterBookInfo(Library& library);

void enterBookInfoMenu();

void displayBookInfo(const Book& book);

void displayAllBooks(const Library& library);

void displayAllBooksMenu();

void modifyBookInfo(Library& library, const string& ISBN);

void modifyBookInfoMenu();

void searchBookByISBN(const Library& library, const string& ISBN);

void searchBookByISBNMenu();

int compareIgnoreCase(const string& str1, const string& str2);

void searchBookByTitle(const Library& library, const string& title);

void searchBookByTitleMenu();

void searchByAuthor(const Library& library, const string& author);

void searchByAuthorMenu();

void searchByPublisher(const Library& library, const string& publisher);

void searchByPublisherMenu();

#endif //TEST_LIBRARY_H
