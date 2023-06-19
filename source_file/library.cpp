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



//ͼ��
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
        cout << "ͼ����Ϣ����ɹ���" << endl;
    } else {
        cout << "�޷���ͼ����Ϣ�ļ���" << endl;
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
                cout << "ͼ�������Ѵﵽ������ƣ��޷����ظ���ͼ�飡" << endl;
                break;
            }

            // ��ÿ���н���ͼ����Ϣ
            string ISBN, title, author;
            int copies,borrowed;
            istringstream iss(line);
            string field;

            // ��������ֶ�
            if (getline(iss, field, ',')) ISBN = field;
            if (getline(iss, field, ',')) title = field;
            if (getline(iss, field, ',')) author = field;
            if (getline(iss, field, ',')) copies = stoi(field);
            if (getline(iss, field, ',')) borrowed = stoi(field);

            // ���� Book ������ӵ� library ��
            Book book;
            book.ISBN = ISBN;
            book.title = title;
            book.author = author;
            book.copies = copies;
            book.borrowed = borrowed;

            library.books[library.bookCount] = book;
            library.bookCount++;

            // ����������
            library.bookIndex.titleIndex[title] = ISBN;
            library.bookIndex.authorIndex[author] = ISBN;
        }
        file.close();
    } else {
        cout << "�޷���ͼ����Ϣ�ļ���" << endl;
    }
}

void updateIndexTable(const string& indexFilename, const unordered_map<string, string>& indexTable) {
    ofstream file(indexFilename);
    if (file.is_open()) {
        for (const auto& entry : indexTable) {
            file << entry.first << "," << entry.second << endl;
        }
        file.close();
        cout << "��������³ɹ���" << endl;
    } else {
        cout << "�޷����������ļ���" << endl;
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

//������Ϣ
void enterBookInfo(Library& library) {
    if (library.bookCount >= MAX_BOOKS) {
        cout << "ͼ�������Ѵﵽ������ƣ��޷���Ӹ���ͼ�飡" << endl;
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

    // ����������
    library.bookIndex.titleIndex[newBook.title] = newBook.ISBN;
    library.bookIndex.authorIndex[newBook.author] = newBook.ISBN;
    library.bookIndex.publisherIndex[newBook.publisher] = newBook.ISBN;

    saveBooksToFile("book_main_file.txt", library);
    updateIndexTable("title_index.txt", library.bookIndex.titleIndex);
    updateIndexTable("publisher_index.txt", library.bookIndex.publisherIndex);
    updateIndexTable("author_index.txt", library.bookIndex.authorIndex);

    cout << "��ͼ����Ϣ����ӣ�" << endl;
}

void enterBookInfoMenu(){
    enterBookInfo(library);
}

//��ʾ
void displayBookInfo(const Book& book) {
    cout << "ISBN: " << book.ISBN << endl;
    cout << "Title: " << book.title << endl;
    cout << "Author: " << book.author << endl;
    cout << "Copies: " << book.copies << endl;
    cout << "Borrowed: " << book.borrowed << endl;
    cout << "Publisher: " << book.publisher << endl;
    // ��������ֶε����
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


//������Ϣ
void modifyBookInfo(Library& library, const string& ISBN) {
    // ����ָ��ISBN��ͼ��
    int index = -1;
    for (int i = 0; i < library.bookCount; i++) {
        if (library.books[i].ISBN == ISBN) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        cout << "�Ҳ���ָ����ͼ�飡" << endl;
        return;
    }

    cout << "��ǰͼ����Ϣ��" << endl;
    displayBookInfo(library.books[index]);

    // �޸Ĳ�����
    int newCopies;
    cout << "Enter new copies: ";
    cin >> newCopies;
    library.books[index].copies = newCopies;

    // �޸Ľ����
    int newBorrowed;
    cout << "Enter new borrowed: ";
    cin >> newBorrowed;
    library.books[index].borrowed = newBorrowed;

    // ����������
    library.bookIndex.titleIndex[library.books[index].title] = ISBN;
    library.bookIndex.authorIndex[library.books[index].author] = ISBN;

    // ����ͼ����Ϣ���ļ�
    saveBooksToFile("book_main_file.txt", library);

    // �����������ļ�
    updateIndexTable("index_table.txt", library.bookIndex.titleIndex);

    cout << "ͼ����Ϣ���޸ģ�" << endl;
}

void modifyBookInfoMenu(){
    string ISBN;
    cout << "Enter the ISBN of the book to modify: ";
    cin >> ISBN;
    modifyBookInfo(library, ISBN);
}

//��ѯ
void searchBookByISBN(const Library& library, const string& ISBN) {
    bool found = false;

    for (int i = 0; i < library.bookCount; i++) {
        const Book& book = library.books[i];

        // ������Ž��в�ѯ
        if (book.ISBN == ISBN) {
            if (!found) {
                cout << "��ѯ�����" << endl;
                found = true;
            }

            displayBookInfo(book);
            break;  // �ҵ�ƥ�����ź����ѭ��
        }
    }

    if (!found) {
        cout << "�Ҳ��������ƥ���ͼ�飡" << endl;
    }
}

void searchBookByISBNMenu(){
    string ISBN;
    cout << "Enter the ISBN of the book to search: ";
    cin >> ISBN;

    searchBookByISBN(library, ISBN);
}
//���Դ�Сд
int compareIgnoreCase(const string& str1, const string& str2) {
    // ȷ�������ַ����ĳ�����ͬ
    if (str1.length() != str2.length()) {
        return (str1.length() < str2.length()) ? -1 : 1;
    }
}
//��������ѯ
void searchBookByTitle(const Library& library, const string& title) {
    bool found = false;

    for (int i = 0; i < library.bookCount; i++) {
        const Book& book = library.books[i];

        // ���������в�ѯ
        if (book.title == title) {
            if (!found) {
                cout << "��ѯ�����" << endl;
                found = true;
            }

            displayBookInfo(book);
        }
    }

    if (!found) {
        cout << "�Ҳ���������ƥ���ͼ�飡" << endl;
    }
}

void searchBookByTitleMenu(){
    string title;
    cout << "Enter the title of the book to search: ";
    cin.ignore(); // ������һ���������µĻ��з�
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
        cout << "�Ҳ���ָ�����ߵ�ͼ�飡" << endl;
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
        cout << "�Ҳ���ָ���������ͼ�飡" << endl;
    }
}

void searchByPublisherMenu(){
    string publisher;
    cout << "Enter publisher: ";
    getline(cin, publisher);

    searchByPublisher(library, publisher);
}
