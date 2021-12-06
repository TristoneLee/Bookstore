# 项目名称 / 文档作者
项目名称：书店管理系统

文档作者：毛逸洲
# 程序功能概述
对于不同用户（未登录用户、顾客、店员、店长）提供相应的功能，具体来说：
## 账户系统
0. 未登录用户：登录其他已注册的账户、注册顾客账户。
1. 顾客：未登录用户可以使用的所有功能、注销当前登录的账户、修改指定账户的密码（需要输入旧密码）、检索图书。
2. 店员：顾客可以使用的所有功能、创建权限低于自己的账户。
3. 店长：店员可以使用的所有功能、删除账户、修改指定账户的密码（无需输入旧密码）。
## 图书系统
1. 顾客：检索图书、购买图书。
2. 店员：顾客可以使用的所有功能、修改图书信息、图书进货。
3. 店长：店员可以使用的所有功能。

## 日志系统
2. 店员：生成员工自己的操作记录
3. 店长：店员可以使用的所有功能、查询财务记录、生成财务记录报告、生成全体员工工作情况报告、生成日志。

## 其他功能
> todo

# 主体逻辑说明
用一个```Bookstore```类保存所有与这个书店相关的信息（包括账户信息、图书信息）并实现所有操作。

将在```Bookstore```类中分别将上述三个系统中的操作封装在三个独立的类中，处理命令时```Bookstore```类中的```Work```函数分别调用三个类的接口完成指令。
# 代码文件结构
```
Bookstore/
├── AccountSystem.cpp
├── AccountSystem.h
├── BookSystem.cpp
├── BookSystem.h
├── Bookstore.cpp
├── Bookstore.h
├── LogSystem.cpp
├── LogSystem.h
└── main.cpp
```
# 各个类的接口及成员说明 

## ```Bookstore```类
书店管理系统类，存储书店信息并处理命令
```cpp
class Bookstore {
private:
    AccountSystem account_system;
    BookSystem book_system;
    LogSystem log_system;
    /* something here */
public:
    void init(); // initialize the whole system, called when the whole program is started
    void work(); // read and execute the commands
    /* something here */
};
```
## ```AccountSystem```类
账户系统类，储存账户信息、执行账户系统命令、为所有命令处理提供目前登录的账户信息
```cpp
class AccountSystem {
private:
    class Account {
        /* something here */
    };
    /* something here */
public:
    Account Get_Current_Account();
    void Login(std::string user_id,std::string password);
    void Logout();
    void Register(std::string user_id,std::string user_name,std::string password);
    void Creat_Account(std::string user_id,std::string user_name,std::string password,int pro);
    void Change_Password(std::string user_id,std::string old_password,std::string new_password);// can be only used by root
    void Change_Password_Root(std::string user_id,std::string new_password);
    void Delete_Account(std::string user_id);
};
```


## ```BookSystem```类
图书系统类，储存、维护图书信息
```cpp
class BookSystem {
private:
    class Book {
        /* something here */
    };
    /* something here */
public:
    Book Query_Book(std::string restriction); // maybe vector<Book>
	// or maybe void and print the information of the book(s) directly
    void Creat_Book(Book obj); // Creat a new book (maybe without information)
    void Select_Book(std::string restriction);
    void Change_Book(std::string restriction,std::string changed_info);
    void Restock_Book(std::string restriction,int quantity);
    void Sell_Book(std::string restriction,int quantity);
};
```
## ```Logsystem```类
日志系统类，维护日志、进行打印日志操作
```cpp
class LogSystem {
private:
    /* something here */
public:
    void Report_Personal_Log(std::string employee_name);
    void Show_Finance_Log(int time);
    void Report_Finance_Log();
    void Query_Employee_Log();
    void Query_All_Log();
};
```


# 具体算法说明
使用**块状链表**实现K-V对的查询、修改、删除操作，用以保存图书数据、账户数据等。