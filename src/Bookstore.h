#ifndef BOOKSTORE_BOOKSTORE_H
#define BOOKSTORE_BOOKSTORE_H

#include <sstream>
#include <iterator>
#include "AccountSystem.h"
#include "LogSystem.h"
#include "BookSystem.h"
#include "KV_Database.h"
#include "MyString.h"

void Trim(string &s) {
    if (!s.empty()) {
        s.erase(0, s.find_first_not_of(' '));
        s.erase(s.find_last_not_of(' ') + 1);
    }
}

void Split(const string &str,
           vector<string> &tokens,
           const string delim = " ") {
    tokens.clear();
    auto start = str.find_first_not_of(delim, 0);
    auto position = str.find_first_of(delim, start);
    while (position != string::npos || start != string::npos) {
        tokens.emplace_back(std::move(str.substr(start, position - start)));
        start = str.find_first_not_of(delim, position);
        position = str.find_first_of(delim, start);
    }
}

bool ISBN_Checker(const string &str) {
    for (int i = 0; i < str.size(); i++) {
        if (!isdigit(str[i])) {
            return false;
        }
    }
    return true;
}


const int commandLen = 1025;


class Bookstore {
private:
    AccountSystem account_system;
    BookSystem book_system;
    LogSystem log_system;
public:
    Bookstore() = default;

    void work();// read and execute the commands

    void Quit();

    void Exit();

    void Su();

    void Logout();

    void Register();

    void Passwd();

    void Useradd();

    void Delete();

    void Show();

    void Buy();

    void Select();

    void Modify();

    void Import();

    void Show_Finance(int i);

    void Show_Finance();

    void Report();

    void Report_Myself();

    void Report_Finance();

    void Report_Employee();

    void Log();
};

void Bookstore::work() {
    string command;
    if (!(cin >> command)) Bookstore::Quit();
    if (command == "quit") { Bookstore::Quit(); }
    else if (command == "exit") { Bookstore::Exit(); }
    else if (command == "su") { Bookstore::Su(); }
    else if (command == "logout") { Bookstore::Logout(); }
    else if (command == "register") { Bookstore::Register(); }
    else if (command == "passwd") { Bookstore::Passwd(); }
    else if (command == "useradd") { Bookstore::Useradd(); }
    else if (command == "delete") { Bookstore::Delete(); }
    else if (command == "show") { Bookstore::Show(); }
    else if (command == "buy") { Bookstore::Buy(); }
    else if (command == "select") { Bookstore::Select(); }
    else if (command == "modify") { Bookstore::Modify(); }
    else if (command == "import") { Bookstore::Import(); }
//    else if (command == "report") { Bookstore::Report(); }
//    else if (command == "log") { Bookstore::Log(); }
    else {
        string a;
        getline(cin, a);
        throw "Invalid";
    }
}

void Bookstore::Quit() {
    throw 1;
}

void Bookstore::Exit() {
    throw 1;
}

void Bookstore::Su() {
    string statement;
    getline(cin, statement);
    Trim(statement);
    vector<string> tokens;
    Split(statement, tokens);
    if (tokens.size() == 0 || tokens.size() > 2) throw "Invalid";
    else if (tokens.size() == 1) { account_system.Login(MyString::to_MyString(tokens.front()), ""); }
    else { account_system.Login(MyString::to_MyString(tokens.front()), MyString::to_MyString(tokens.back())); }
};

void Bookstore::Logout() {
    account_system.Logout();
}

void Bookstore::Register() {
    string statement;
    getline(cin, statement);
    Trim(statement);
    vector<string> tokens;
    Split(statement, tokens);
    if (tokens.size() != 3) throw "Invalid";
    auto iter = tokens.begin() + 1;
    if (tokens.back().size() > stringLen || tokens.front().size() > stringLen
        || (*iter).size() > stringLen)
        throw "Invalid";
    account_system.Register(MyString::to_MyString(tokens.front()),
                            MyString::to_MyString(tokens.back()), MyString::to_MyString(*iter));
}

void Bookstore::Passwd() {
    string statement;
    getline(cin, statement);
    Trim(statement);
    vector<string> tokens;
    Split(statement, tokens);
    if (tokens.size() < 2 || tokens.size() > 3) throw "Invalid";
    else if (tokens.size() == 2) {
        account_system.Change_Password_Root(MyString::to_MyString(tokens.front()),
                                            MyString::to_MyString(tokens.back()));
        if (tokens.back().size() > stringLen || tokens.front().size() > stringLen) throw "Invalid";
    } else {
        auto iter = tokens.begin() + 1;
        if (tokens.back().size() > stringLen || tokens.front().size() > stringLen
            || (*iter).size() > stringLen)
            throw "Invalid";
        account_system.Change_Password(MyString::to_MyString(tokens.front()),
                                       MyString::to_MyString(*iter), MyString::to_MyString(tokens.back()));
    }
}

void Bookstore::Useradd() {
    string statement;
    getline(cin, statement);
    Trim(statement);
    vector<string> tokens;
    Split(statement, tokens);
    if (tokens.size() != 4) throw "Invalid";
    else {
        auto iter1 = tokens.begin() + 1;
        auto iter2 = iter1 + 1;
        if (tokens.back().size() > stringLen || tokens.front().size() > stringLen
            || (*iter1).size() > stringLen || (*iter2).size() > stringLen)
            throw "Invalid";
        account_system.Creat_Account(MyString::to_MyString(tokens.front()), MyString::to_MyString(tokens.back()),
                                     MyString::to_MyString(*iter1), atoi(iter2->c_str()));
    }
}

void Bookstore::Delete() {
    string statement;
    getline(cin, statement);
    if (account_system.currentAccount.Get_Rank() < 7) throw "Invalid";
    Trim(statement);
    vector<string> tokens;
    Split(statement, tokens);
    if (tokens.size() != 1) throw "Invalid";
    else account_system.Delete_Account(MyString::to_MyString(tokens.front()));
}

void Bookstore::Buy() {
    string statement;
    getline(cin, statement);
    if (account_system.currentAccount.Get_Rank() < 1) throw "Invalid";
    Trim(statement);
    vector<string> tokens;
    Split(statement, tokens);
    if (tokens.size() != 2) throw "Invalid";
    vector<Book> ans;
    ans = book_system.Query_Book(MyString::to_MyString(tokens.front()), 1);
    if (ans.empty()) throw "Invalid";
    double a = 0;
    try {
        a = book_system.Sell_Book(ans.front(), std::atoi(tokens.back().c_str()));
    }
    catch (int) {
        throw "Invalid";
    }
    log_system.Save_Finance(a, true);
}

void Bookstore::Select() {
    string statement;
    getline(cin, statement);
    if (account_system.currentAccount.Get_Rank() < 3) throw "Invalid";
    Trim(statement);
    vector<string> tokens;
    Split(statement, tokens);
    if (tokens.size() != 1) throw "Invalid";
    account_system.Select_Book(MyString::to_MyString(tokens.front()), book_system);
}

void Bookstore::Show() {
    string statement;
    getline(cin, statement);
    if (account_system.accountStack.size() == 1) throw "Invalid";
    if (account_system.currentAccount.Get_Rank() < 1) throw "Invalid";
    Trim(statement);
    vector<string> tokens;
    Split(statement, tokens);
    if (tokens.size() != 0 && tokens.front() == "finance") {
        if (tokens.size() == 2) Show_Finance(std::atoi(tokens.back().c_str()));
        else Show_Finance();
        return;
    };
    if (tokens.size() > 1) throw "Invalid";
    else if (tokens.size() == 0) {
        vector<Book> ans = book_system.Show_All();
        for (auto iter = ans.begin(); iter != ans.end(); ++iter) {
            iter->Display();
        }
    } else {
        vector<string> tmp;
        Split(tokens.front(), tmp, "=");
        vector<Book> ans;
        if (tmp.front() == "-ISBN") {
            ans = book_system.Query_Book(MyString::to_MyString(tmp.back()), 1);
        } else if (tmp.front() == "-name") {
            ans = book_system.Query_Book(MyString::to_MyString(tmp.back().substr(1, tmp.back().length() - 2)), 2);
        } else if (tmp.front() == "-author") {
            ans = book_system.Query_Book(MyString::to_MyString(tmp.back().substr(1, tmp.back().length() - 2)), 3);
        } else if (tmp.front() == "-keyword") {
            vector<string> list;
            Split(tmp.back(), list, "=");
            if (list.size() != 1) throw "Invalid";
            else ans = book_system.Query_Book(MyString::to_MyString(tmp.back().substr(1, tmp.back().length() - 2)), 4);
        } else throw "Invalid";
        if (ans.empty()) cout << "\n";
        else
            for (auto iter = ans.begin(); iter != ans.end();
                 ++iter) {
                iter->Display();
            }
    }
}

void Bookstore::Modify() {
    string statement;
    getline(cin, statement);
    if (account_system.currentAccount.Get_Rank() < 3) throw "Invalid";
    if (account_system.if_Select == false) throw "Invalid";
    Trim(statement);
    vector<string> tokens;
    Split(statement, tokens);
    bool DON[5] = {false};
    string restrictions[5] = {""};
    if (tokens.empty()) throw "Invalid";
    for (auto iter = tokens.begin(); iter != tokens.end(); ++iter) {
        vector<string> tmp;
        Split(*iter, tmp, "=");
        if (tmp.front() == "-ISBN") {
            if (DON[0] == true) throw "Invalid";
            DON[0] = true;
            restrictions[0] = tmp.back();
        }
        if (tmp.front() == "-name") {
            if (DON[1] == true) throw "Invalid";
            DON[1] = true;
            restrictions[1] = tmp.back().substr(1, tmp.back().length() - 2);
        }
        if (tmp.front() == "-author") {
            if (DON[2] == true) throw "Invalid";
            DON[2] = true;
            restrictions[2] = tmp.back().substr(1, tmp.back().length() - 2);
        }
        if (tmp.front() == "-keyword") {
            if (DON[3] == true) throw "Invalid";
            DON[3] = true;
            restrictions[3] = tmp.back().substr(1, tmp.back().length() - 2);
        }
        if (tmp.front() == "-price") {
            if (DON[4] == true) throw "Invalid";
            DON[4] = true;
            restrictions[4] = tmp.back();
        }
    }
    Book currentBook = account_system.Get_Book_Selected();
    book_system.Takeout_Book(currentBook);
    for (int i = 0; i < 5; ++i) {
        if (DON[i] == true && i != 3 && i != 4) {
            book_system.Change_Book(currentBook, MyString::to_MyString(restrictions[i]), i + 1);
        } else if (DON[i] == true && i == 3) {
            vector<string> _keywords;
            Split(restrictions[i], _keywords, "|");
            vector<MyString> _Keywords;
            for (auto iter = _keywords.begin(); iter != _keywords.end(); ++iter) {
                _Keywords.push_back(MyString::to_MyString(*iter));
            }
            book_system.Change_Book(currentBook, _Keywords, i + 1);
        } else if (DON[i] == true && i == 4) {
            book_system.Change_Book(currentBook, std::atof(restrictions[i].c_str()), i + 1);
        }
    }
    book_system.Update_Book(currentBook);
    account_system.Set_Book_Selected(currentBook);
}

void Bookstore::Import() {
    string statement;
    getline(cin, statement);
    if (account_system.currentAccount.Get_Rank() < 3) throw "Invalid";
    if (!account_system.if_Select) throw "Invalid";
    Trim(statement);
    vector<string> tokens;
    Split(statement, tokens);
    if (tokens.size() != 2) throw "Invalid";
    Book currentBook = account_system.Get_Book_Selected();
    book_system.Restock_Book(currentBook, std::atoi(tokens.front().c_str()));
    log_system.Save_Finance(std::atof(tokens.back().c_str()), false);
}

void Bookstore::Show_Finance(int i) {
    log_system.Show_Finance_Log(i);
}

void Bookstore::Show_Finance() {
    log_system.Show_Finance();
}


#endif //BOOKSTORE_BOOKSTORE_H
