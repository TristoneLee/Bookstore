#ifndef BOOKSTORE_BOOKSYSTEM_H
#define BOOKSTORE_BOOKSYSTEM_H

#include <string>
#include "KV_Database.h"
#include <iomanip>
#include "MyString.h"


const int bookNameLen = 60;
const int ISBNLen = 20;
const int keywordLen = 60;

BlockList<int> stockFile("stockFile");

vector<string> KeywordsParser(string keywords);

class Book;

class BookSystem;

class Book {
    friend class BookSystem;

private:
    MyString ISBN;
    MyString BookName;
    MyString Author;
    MyString Keywords[10];
    double Price = 0;
    MyString Keyword;
    int KeywordsNum;
    MyString Index;

public:
    Book() = default;

    ~Book() = default;

    void Set_ISBN(const MyString &isbn);

    void Set_Index(const MyString &index);

    MyString Get_Index();

    void Display();

    friend bool operator<(const Book &a, const Book &b) {
        return a.ISBN < b.ISBN;
    }

    friend bool operator==(const Book &a, const Book &b) {
        if (a.ISBN != b.ISBN) return false;
        else if (a.BookName != b.BookName) return false;
        else if (a.Author != b.Author) return false;
        else if (a.Price != b.Price) return false;
        else if (a.KeywordsNum != b.KeywordsNum) return false;
        else
            for (int i = 0; i < a.KeywordsNum; ++i) {
                if (a.Keywords[i] != b.Keywords[i]) return false;
            }
        return true;
    }
};

class BookSystem {
private:
    BlockList<Book> bookFileBasedOnISBN;
    BlockList<Book> bookFileBasedOnName;
    BlockList<Book> bookFileBasedOnAuthor;
    BlockList<Book> bookFileBasedOnKeywords;
    BlockList<Book> bookFileBasedOnIndex;
    int bookIndex = 0;
    std::fstream bookCount;

public:
    BookSystem();

    ~BookSystem();

    //flag (1:ISBN 2:Name 3:Author 4:Keyword 5:price)
    vector<Book> Query_Book(const MyString &restriction, const int &flag);

    vector<Book> Show_All();

    void Creat_Book(const Book &obj);

    void Plus_Index();

    int Get_Index();

    void Change_Book(Book &obj, const MyString &changed_info, const int &flag);

    void Change_Book(Book &obj, const double &changed_info, const int &flag);

    void Change_Book(Book &obj, vector<MyString> changed_info, const int &flag);

    void Update_Book(const Book &obj);

    void Takeout_Book(const Book &obj);

    void Restock_Book(Book &obj, int quantity);

    double Sell_Book(Book &obj, int quantity);

    void Change_ISBN(const MyString &former, const MyString &latter);
};

BookSystem::BookSystem() : bookFileBasedOnISBN(BlockList<Book>("BookFileBasedOnISBN")),
                           bookFileBasedOnName(BlockList<Book>("BookFileBasedOnName")),
                           bookFileBasedOnAuthor(BlockList<Book>("BookFileBasedOnAuthor")),
                           bookFileBasedOnKeywords(BlockList<Book>("BookFileBasedOnKeywords")),
                           bookFileBasedOnIndex(BlockList<Book>("BookFileBasedOnIndex")) {
    bookCount.open("bookIndexFile");
    if (!bookCount) {
        bookCount.clear();
        bookCount.open("bookIndexFile", std::fstream::out);
        bookCount.close();
    } else {
        bookCount.seekg(0);
        bookCount.read(reinterpret_cast<char *>(&bookIndex), sizeof(int));
        bookCount.close();
    }
}

vector<Book> BookSystem::Query_Book(const MyString &restriction, const int &flag) {
    switch (flag) {
        case 1: {
            vector<Book> ans = bookFileBasedOnISBN.Find(restriction);
            return ans;
        }
        case 2: {
            vector<Book> ans = bookFileBasedOnName.Find(restriction);
            return ans;
        }
        case 3: {
            vector<Book> ans = bookFileBasedOnAuthor.Find(restriction);
            return ans;
        }
        case 4: {
            vector<Book> ans = bookFileBasedOnKeywords.Find(restriction);
            return ans;
        }
        default: {
            vector<Book> ans = bookFileBasedOnIndex.Find(restriction);
            return ans;
        }
    }
}

void BookSystem::Change_Book(Book &obj, const MyString &changed_info, const int &flag) {
    switch (flag) {
        case 1: {
            Change_ISBN(obj.ISBN, changed_info);
            obj.ISBN = changed_info;
            break;
        }
        case 2: {
            obj.BookName = changed_info;
            break;
        }
        default:
            obj.Author = changed_info;
    }
}

void BookSystem::Change_Book(Book &obj, const double &changed_info, const int &flag) {
    obj.Price = changed_info;
}

void BookSystem::Change_Book(Book &obj, vector<MyString> changed_info, const int &flag) {
    int i = 0;
    for (auto iter = changed_info.begin(); iter != changed_info.end(); ++iter) {
        obj.Keywords[i] = *iter;
        ++i;
    }
    obj.KeywordsNum = i;
}

void BookSystem::Takeout_Book(const Book &obj) {
    bookFileBasedOnISBN.Delete(Pair(obj.ISBN, obj));
    bookFileBasedOnName.Delete(Pair(obj.BookName, obj));
    bookFileBasedOnAuthor.Delete(Pair(obj.Author, obj));
    bookFileBasedOnIndex.Delete(Pair(obj.Index, obj));
    for (int i = 0; i < obj.KeywordsNum; ++i) {
        bookFileBasedOnKeywords.Delete(Pair(obj.Keywords[i], obj));
    }
}

void BookSystem::Update_Book(const Book &obj) {
    bookFileBasedOnISBN.Add(Pair(obj.ISBN, obj));
    bookFileBasedOnName.Add(Pair(obj.BookName, obj));
    bookFileBasedOnAuthor.Add(Pair(obj.Author, obj));
    bookFileBasedOnIndex.Add(Pair(obj.Index, obj));
    for (int i = 0; i < obj.KeywordsNum; ++i) {
        bookFileBasedOnKeywords.Add(Pair(obj.Keywords[i], obj));
    }
}

void BookSystem::Restock_Book(Book &obj, int quantity) {
    int Quantity = stockFile.Find(obj.ISBN).front();
    Quantity += quantity;
    stockFile.Update(Pair(obj.ISBN, Quantity - quantity), Quantity);
}

double BookSystem::Sell_Book(Book &obj, int quantity) {
    int Quantity = stockFile.Find(obj.ISBN).front();
    if (Quantity < quantity) throw 1;
    else {
        Quantity -= quantity;
        stockFile.Update(Pair(obj.ISBN, Quantity + quantity), Quantity);
        cout << std::fixed << std::setprecision(2) << quantity * obj.Price << "\n";
        return quantity * obj.Price;
    }
}

vector<Book> BookSystem::Show_All() {
    vector<Book> ans;
    ans = bookFileBasedOnISBN.Show();
    return ans;
}

void BookSystem::Creat_Book(const Book &obj) {
    stockFile.Add(Pair(obj.ISBN, 0));
}

void BookSystem::Change_ISBN(const MyString &former, const MyString &latter) {
    int a = stockFile.Find(former).front();
    stockFile.Delete(Pair(former, a));
    stockFile.Add(Pair(latter, a));
}

void BookSystem::Plus_Index() {
    ++bookIndex;
}

int BookSystem::Get_Index() {
    return bookIndex;
}

BookSystem::~BookSystem() {
    bookCount.open("bookIndexFile");
    bookCount.seekg(0);
    bookCount.write(reinterpret_cast<char *>(&bookIndex), sizeof(int));
    bookCount.close();
}

void Book::Set_ISBN(const MyString &isbn) {
    ISBN = isbn;
}

void Book::Display() {
    cout << ISBN << '\t' << BookName << '\t' << Author << '\t';
    for (int i = 0; i < KeywordsNum - 1; ++i) {
        cout << Keywords[i] << "|";
    }
    if (KeywordsNum != 0)
        cout << Keywords[KeywordsNum - 1];
    cout << '\t' << std::fixed << std::setprecision(2)
         << Price << '\t' << std::fixed << std::setprecision(2) << stockFile.Find(ISBN).front() << '\n';
}

MyString Book::Get_Index() {
    return Index;
}

void Book::Set_Index(const MyString &index) {
    Index = index;
}


#endif //BOOKSTORE_BOOKSYSTEM_H
