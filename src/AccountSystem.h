#ifndef BOOKSTORE_ACCOUNTSYSTEM_H
#define BOOKSTORE_ACCOUNTSYSTEM_H

#include <vector>
#include "BookSystem.h"
#include "KV_Database.h"
#include <filesystem>

const int stringLen = 30;

class Account;

class AccountSystem;

class Account {

    friend class AccountSystem;

private:
    MyString ID;
    MyString Name;
    MyString Password;
    int Rank = 0;
    Book BookSelected;

public:
    Account() = default;

    ~Account()=default;

    Account(const MyString &id, const MyString &name, const MyString &password, int rank);

    Book Get_Book_Selected();

    void Set_Book_Selected(const Book &obj);

    friend bool operator<(const Account &a,const Account &b){
        if(a.ID<b.ID) return true;
        else return false;
    }

    friend bool operator==(const Account &a ,const Account &b){
        return a.ID==b.ID;
    }

    int Get_Rank(){
        return Rank;
    }

    bool if_Select=false;
};


class AccountSystem {
    friend class Bookstore;
private:
    std::vector<Account> accountStack;
    BlockList<Account> accountFile;

public:
    Account currentAccount;

    void Login(const MyString &user_id, const MyString &password);

    void Logout();

    void Register(const MyString &user_id, const MyString &user_name, const MyString &password);

    void Creat_Account(const MyString &user_id, const MyString &user_name, const MyString &password, int rank);

    void Change_Password(const MyString &user_id, const MyString &old_password, const MyString &new_password);

    void Change_Password_Root(const MyString &user_id, const MyString &new_password);

    void Delete_Account(const MyString &user_id);

    Account Find(const MyString &user_id);

    void Select_Book(const MyString &isbn,BookSystem &bookSystem);

    AccountSystem();
};

Account::Account(const MyString &id, const MyString &name, const MyString &password, int rank) :
        ID(id), Name(name), Password(password), Rank(rank) {}

Book Account::Get_Book_Selected() {
    return BookSelected;
}

void Account::Set_Book_Selected(const Book &obj) {
    BookSelected=obj;
}

void AccountSystem::Login(const MyString &user_id, const MyString &password) {
    Account obj = this->Find(user_id);
    if (currentAccount.Rank > obj.Rank) {
        accountStack.pop_back();
        accountStack.push_back(currentAccount);
        currentAccount = obj;
        accountStack.push_back(obj);
        return;
    } else if (obj.Password == password) {
        accountStack.pop_back();
        accountStack.push_back(currentAccount);
        currentAccount = obj;
        accountStack.push_back(obj);
        return;
    } else throw "Invalid";
}

void AccountSystem::Logout() {
    if (accountStack.size()==1) throw "Invalid";
    accountStack.pop_back();
    currentAccount = accountStack.back();
}

Account AccountSystem::Find(const MyString &user_id) {
    std::vector<Account> ans = accountFile.Find(user_id);
    if (ans.empty()) throw "Invalid";
    else return ans.front();
}

void AccountSystem::Register(const MyString &user_id, const MyString &user_name, const MyString &password) {
    std::vector<Account> ans = accountFile.Find(user_id);
    if (!ans.empty()) throw "Invalid";
    else accountFile.Add(Pair<Account>(user_id, Account(user_id, user_name, password, 1)));
}

void AccountSystem::Creat_Account(const MyString &user_id, const MyString &user_name, const MyString &password, int rank) {
    if (rank != 1 && rank != 3)
        if (currentAccount.Rank < 3 || currentAccount.Rank <= rank) throw "Invalid";
    std::vector<Account> ans = accountFile.Find(user_id);
    if (!ans.empty()) throw "Invalid";
    else accountFile.Add(Pair<Account>(user_id, Account(user_id, user_name, password, rank)));
}

void AccountSystem::Change_Password(const MyString &user_id, const  MyString &old_password, const MyString &new_password) {
    Account tmp = this->Find(user_id);
    if (tmp.Password == old_password) {
        tmp.Password = new_password;
        accountFile.Update(Pair<Account>(user_id, tmp), tmp);
    } else throw "Invalid";
}

void AccountSystem::Change_Password_Root(const MyString &user_id, const MyString &new_password) {
    if (currentAccount.Rank != 7) throw "Invalid";
    Account tmp = this->Find(user_id);
    tmp.Password = new_password;
    accountFile.Update(Pair<Account>(user_id, tmp), tmp);
}

void AccountSystem::Delete_Account(const MyString &user_id) {
    if (currentAccount.Rank != 7) throw "Invalid";
    for (auto iter = accountStack.begin(); iter != accountStack.end(); ++iter)
        if (iter->ID == user_id) throw "Invalid";
    Account tmp = this->Find(user_id);
    accountFile.Delete(Pair<Account>(user_id, tmp));
}

AccountSystem::AccountSystem() : accountFile(BlockList<Account>("AccountFile")),currentAccount(Account()) {
    if(!std::filesystem::exists("BookFileBasedOnISBN"))
    accountFile.Add(Pair("root",Account("root", "", "sjtu", 7)));
    accountStack.emplace_back(Account());
}

void AccountSystem::Select_Book(const MyString &isbn,BookSystem &bookSystem) {
    vector<Book> ans=bookSystem.Query_Book(isbn,1);
    currentAccount.if_Select=true;
    if(ans.empty()) {auto newBook=Book();
        newBook.Set_ISBN(isbn);
        bookSystem.Plus_Index();
        newBook.Set_Index(MyString(std::to_string(bookSystem.Get_Index())));
        bookSystem.Update_Book(newBook);
        bookSystem.Creat_Book(newBook);
        currentAccount.Set_Book_Selected(newBook);
    }
    else currentAccount.Set_Book_Selected(ans.front());
}


#endif //BOOKSTORE_ACCOUNTSYSTEM_H
