//
// Created by Administrator on 2023/6/13.
//

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <list>
#include <sstream>
#include <unordered_map>

#include "user.h"
#include "library.h"


Library library;



//图书
void saveBooksToFile(const string& filename, const Library& library) {
    ofstream file(filename);
    if (file.is_open()) {
        for (int i = 0; i < library.bookCount; ++i) {
            const Book& book = library.books[i];
            file << book.ISBN << ","
                 << book.title << ","
                 << book.author << ","
                 << book.copies << ","
                 << book.borrowed << endl;
        }
        file.close();
        cout << "图书信息保存成功！" << endl;
    } else {
        cout << "无法打开图书信息文件！" << endl;
    }
}

void loadBooksFromFile(const string& filename, Library& library) {
    ifstream file(filename);
    if (file.is_open()) {
        library.bookCount = 0;
        library.bookIndex.titleIndex.clear();
        library.bookIndex.authorIndex.clear();
        library.bookIndex.publisherIndex.clear();

        string line;
        while (getline(file, line)) {
            if (library.bookCount >= MAX_BOOKS) {
                cout << "图书数量已达到最大限制，无法加载更多图书！" << endl;
                break;
            }

            // 从每行中解析图书信息
            string ISBN, title, author;
            int copies,borrowed;
            istringstream iss(line);
            string field;

            // 逐个解析字段
            if (getline(iss, field, ',')) ISBN = field;
            if (getline(iss, field, ',')) title = field;
            if (getline(iss, field, ',')) author = field;
            if (getline(iss, field, ',')) copies = stoi(field);
            if (getline(iss, field, ',')) borrowed = stoi(field);

            // 创建 Book 对象并添加到 library 中
            Book book;
            book.ISBN = ISBN;
            book.title = title;
            book.author = author;
            book.copies = copies;
            book.borrowed = borrowed;

            library.books[library.bookCount] = book;
            library.bookCount++;

            // 更新索引表
            library.bookIndex.titleIndex[title] = ISBN;
            library.bookIndex.authorIndex[author] = ISBN;
        }
        file.close();
    } else {
        cout << "无法打开图书信息文件！" << endl;
    }
}

void updateIndexTable(const string& indexFilename, const unordered_map<string, string>& indexTable) {
    ofstream file(indexFilename);
    if (file.is_open()) {
        for (const auto& entry : indexTable) {
            file << entry.first << "," << entry.second << endl;
        }
        file.close();
        cout << "索引表更新成功！" << endl;
    } else {
        cout << "无法打开索引表文件！" << endl;
    }
}

void createTitleIndex(const string& indexFilename, const Library& library) {
    unordered_map<string, string> titleIndex;
    for (int i = 0; i < library.bookCount; ++i) {
        const Book& book = library.books[i];
        titleIndex[book.title] = book.ISBN;
    }
    updateIndexTable(indexFilename, titleIndex);
}

void createAuthorIndex(const string& indexFilename, const Library& library) {
    unordered_map<string, string> authorIndex;
    for (int i = 0; i < library.bookCount; ++i) {
        const Book& book = library.books[i];
        authorIndex[book.author] = book.ISBN;
    }
    updateIndexTable(indexFilename, authorIndex);
}

void createPublisherIndex(const string& indexFilename, const Library& library) {
    unordered_map<string, string> publisherIndex;
    for (int i = 0; i < library.bookCount; ++i) {
        const Book& book = library.books[i];
        publisherIndex[book.publisher] = book.ISBN;
    }
    updateIndexTable(indexFilename, publisherIndex);
}

//输入信息
void enterBookInfo(Library& library) {
    if (library.bookCount >= MAX_BOOKS) {
        cout << "图书数量已达到最大限制，无法添加更多图书！" << endl;
        return;
    }

    Book newBook;

    cout << "Enter ISBN: ";
    cin >> newBook.ISBN;

    cout << "Enter title: ";
    getline(cin, newBook.title);

    cout << "Enter author: ";
    getline(cin, newBook.author);

    cout << "Enter copies: ";
    cin >> newBook.copies;

    cout << "Enter borrowed: ";
    cin >> newBook.borrowed;

    library.books[library.bookCount] = newBook;
    library.bookCount++;

    // 更新索引表
    library.bookIndex.titleIndex[newBook.title] = newBook.ISBN;
    library.bookIndex.authorIndex[newBook.author] = newBook.ISBN;
    library.bookIndex.publisherIndex[newBook.publisher] = newBook.ISBN;

    saveBooksToFile("book_main_file.txt", library);
    updateIndexTable("title_index.txt", library.bookIndex.titleIndex);
    updateIndexTable("publisher_index.txt", library.bookIndex.publisherIndex);
    updateIndexTable("author_index.txt", library.bookIndex.authorIndex);

    cout << "新图书信息已添加！" << endl;
}

void enterBookInfoMenu(){
    enterBookInfo(library);
}

//显示
void displayBookInfo(const Book& book) {
    cout << "ISBN: " << book.ISBN << endl;
    cout << "Title: " << book.title << endl;
    cout << "Author: " << book.author << endl;
    cout << "Copies: " << book.copies << endl;
    cout << "Borrowed: " << book.borrowed << endl;
    cout << "Publisher: " << book.publisher << endl;
    // 添加其他字段的输出
}

void displayAllBooks(const Library& library) {
    for (int i = 0; i < library.bookCount; i++) {
        displayBookInfo(library.books[i]);
        cout << "--------------------------" << endl;
    }
}

void displayAllBooksMenu(){
    displayAllBooks(library);
}


//更改信息
void modifyBookInfo(Library& library, const string& ISBN) {
    // 查找指定ISBN的图书
    int index = -1;
    for (int i = 0; i < library.bookCount; i++) {
        if (library.books[i].ISBN == ISBN) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        cout << "找不到指定的图书！" << endl;
        return;
    }

    cout << "当前图书信息：" << endl;
    displayBookInfo(library.books[index]);

    // 修改藏书量
    int newCopies;
    cout << "Enter new copies: ";
    cin >> newCopies;
    library.books[index].copies = newCopies;

    // 修改借出数
    int newBorrowed;
    cout << "Enter new borrowed: ";
    cin >> newBorrowed;
    library.books[index].borrowed = newBorrowed;

    // 更新索引表
    library.bookIndex.titleIndex[library.books[index].title] = ISBN;
    library.bookIndex.authorIndex[library.books[index].author] = ISBN;

    // 保存图书信息到文件
    saveBooksToFile("book_main_file.txt", library);

    // 更新索引表文件
    updateIndexTable("index_table.txt", library.bookIndex.titleIndex);

    cout << "图书信息已修改！" << endl;
}

void modifyBookInfoMenu(){
    string ISBN;
    cout << "Enter the ISBN of the book to modify: ";
    cin >> ISBN;
    modifyBookInfo(library, ISBN);
}

//查询
void searchBookByISBN(const Library& library, const string& ISBN) {
    bool found = false;

    for (int i = 0; i < library.bookCount; i++) {
        const Book& book = library.books[i];

        // 按照书号进行查询
        if (book.ISBN == ISBN) {
            if (!found) {
                cout << "查询结果：" << endl;
                found = true;
            }

            displayBookInfo(book);
            break;  // 找到匹配的书号后结束循环
        }
    }

    if (!found) {
        cout << "找不到与书号匹配的图书！" << endl;
    }
}

void searchBookByISBNMenu(){
    string ISBN;
    cout << "Enter the ISBN of the book to search: ";
    cin >> ISBN;

    searchBookByISBN(library, ISBN);
}
//忽略大小写
int compareIgnoreCase(const string& str1, const string& str2) {
    // 确保两个字符串的长度相同
    if (str1.length() != str2.length()) {
        return (str1.length() < str2.length()) ? -1 : 1;
    }
}
//按书名查询
void searchBookByTitle(const Library& library, const string& title) {
    bool found = false;

    for (int i = 0; i < library.bookCount; i++) {
        const Book& book = library.books[i];

        // 按书名进行查询
        if (book.title == title) {
            if (!found) {
                cout << "查询结果：" << endl;
                found = true;
            }

            displayBookInfo(book);
        }
    }

    if (!found) {
        cout << "找不到与书名匹配的图书！" << endl;
    }
}

void searchBookByTitleMenu(){
    string title;
    cout << "Enter the title of the book to search: ";
    cin.ignore(); // 忽略上一次输入留下的换行符
    getline(cin, title);

    searchBookByTitle(library, title);
}

void searchByAuthor(const Library& library, const string& author) {
    bool found = false;
    for (int i = 0; i < library.bookCount; i++) {
        if (library.books[i].author == author) {
            displayBookInfo(library.books[i]);
            found = true;
        }
    }

    if (!found) {
        cout << "找不到指定作者的图书！" << endl;
    }
}

void searchByAuthorMenu(){
    string author;
    cout << "Enter author: ";
    getline(cin, author);
    searchByAuthor(library, author);
}

void searchByPublisher(const Library& library, const string& publisher) {
    bool found = false;
    for (int i = 0; i < library.bookCount; i++) {
        if (library.books[i].publisher == publisher) {
            displayBookInfo(library.books[i]);
            found = true;
        }
    }

    if (!found) {
        cout << "找不到指定出版社的图书！" << endl;
    }
}

void searchByPublisherMenu(){
    string publisher;
    cout << "Enter publisher: ";
    getline(cin, publisher);

    searchByPublisher(library, publisher);
}
