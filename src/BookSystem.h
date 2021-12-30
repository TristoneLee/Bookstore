#ifndef BOOKSTORE_BOOKSYSTEM_H
#define BOOKSTORE_BOOKSYSTEM_H

#include <string>
#include "KV_Database.h"
#include <iomanip>
#include "MyString.h"


const int bookNameLen = 60;
const int ISBNLen = 20;
const int keywordLen = 60;

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
    int Quantity = 0;
    double Price = 0;
    MyString Keyword;
    int KeywordsNum;

public:
    Book() = default;

    ~Book() = default;

    void Set_ISBN(const MyString &isbn);

    void Display();

    friend bool operator<(const Book &a, const Book &b) {
        return a.ISBN < b.ISBN;
    }

    friend bool operator==(const Book &a, const Book &b) {
        if(a.ISBN!=b.ISBN) return false;
        else if(a.BookName!=b.BookName) return false;
        else if(a.Author!=b.Author) return false;
        else if(a.Price!=b.Price) return false ;
        else if(a.KeywordsNum!=b.KeywordsNum) return false;
        else if(a.Quantity!=b.Quantity) return false;
        else for (int i=0;i<a.KeywordsNum;++i){
            if (a.Keywords[i]!=b.Keywords[i]) return false;
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

public:
    BookSystem();

    //flag (1:ISBN 2:Name 3:Author 4:Keyword 5:price)
    vector<Book> Query_Book(const MyString &restriction, const int &flag);

    vector<Book> Show_All();

    void Creat_Book(Book obj);

    void Change_Book(Book &obj, const MyString &changed_info, const int &flag);

    void Change_Book(Book &obj, const double &changed_info, const int &flag);

    void Change_Book(Book &obj, vector<MyString> changed_info, const int &flag);

    void Update_Book(const Book &obj);

    void Takeout_Book(const Book &obj);

    void Restock_Book(Book &obj, int quantity);

    double Sell_Book(Book &obj, int quantity);
};

BookSystem::BookSystem() : bookFileBasedOnISBN(BlockList<Book>("BookFileBasedOnISBN")),
                           bookFileBasedOnName(BlockList<Book>("BookFileBasedOnName")),
                           bookFileBasedOnAuthor(BlockList<Book>("BookFileBasedOnAuthor")),
                           bookFileBasedOnKeywords(BlockList<Book>("BookFileBasedOnKeywords")) {}

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
        default: {
            vector<Book> ans = bookFileBasedOnKeywords.Find(restriction);
            return ans;
        }
    }
}

void BookSystem::Change_Book(Book &obj, const MyString &changed_info, const int &flag) {
    switch (flag) {
        case 1: {
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
    int i=0;
    for(auto iter=changed_info.begin();iter!=changed_info.end();++iter){
        obj.Keywords[i]=*iter;
        ++i;
    }
    obj.KeywordsNum=i;
}

void BookSystem::Takeout_Book(const Book &obj) {
    bookFileBasedOnISBN.Delete(Pair(obj.ISBN, obj));
    bookFileBasedOnName.Delete(Pair(obj.BookName, obj));
    bookFileBasedOnAuthor.Delete(Pair(obj.Author, obj));
    for (int i=0;i<obj.KeywordsNum;++i) {
        bookFileBasedOnKeywords.Delete(Pair(obj.Keywords[i], obj));
    }
}

void BookSystem::Update_Book(const Book &obj) {
    bookFileBasedOnISBN.Add(Pair(obj.ISBN, obj));
    bookFileBasedOnName.Add(Pair(obj.BookName, obj));
    bookFileBasedOnAuthor.Add(Pair(obj.Author, obj));
    for (int i=0;i<obj.KeywordsNum;++i) {
        bookFileBasedOnKeywords.Add(Pair(obj.Keywords[i], obj));
    }
}

void BookSystem::Restock_Book(Book &obj, int quantity) {
    obj.Quantity += quantity;
}

double BookSystem::Sell_Book(Book &obj, int quantity) {
    if (obj.Quantity < quantity) throw "Invalid";
    else obj.Quantity -= quantity;
    cout << std::fixed << std::setprecision(2) << quantity * obj.Price << "\n";
    return quantity * obj.Price;
}

vector<Book> BookSystem::Show_All() {
    vector<Book> ans;
    ans = bookFileBasedOnISBN.Show();
    return ans;
}

void Book::Set_ISBN(const MyString &isbn) {
    ISBN = isbn;
}

void Book::Display() {
    cout << ISBN << '\t' << BookName << '\t' << Author << '\t';
    for (int i=0;i<KeywordsNum-1;++i) {
        cout << Keywords[i] << "|";
    }
    cout << Keywords[KeywordsNum-1] << '\t' << std::fixed << std::setprecision(2)
    << Price << '\t' << std::fixed << std::setprecision(2)<< Quantity << '\t' << '\n';
}


#endif //BOOKSTORE_BOOKSYSTEM_H
