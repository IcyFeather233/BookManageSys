//
// Created by Administrator on 2023/6/13.
//

#ifndef TEST_BOOKCM_H
#define TEST_BOOKCM_H
#include "library.h"

void loadBorrowListFromFile(const std::string& filename, list<BorrowRecord>& borrowList);

void saveBorrowListToFile(const std::string& filename, const std::list<BorrowRecord>& borrowList);

int findBookIndex(const Library& library, const std::string& ISBN);

std::string getCurrentDate();

std::string calculateReturnDate(const std::string& borrowDate, int loanPeriod);

void borrowBook(Library& library);

void borrowBookMenu();

void returnBookMenu();

#endif //TEST_BOOKCM_H
