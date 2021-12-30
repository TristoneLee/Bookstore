#ifndef BOOKSTORE_LOGSYSTEM_H
#define BOOKSTORE_LOGSYSTEM_H

#include "BookSystem.h"
#include "AccountSystem.h"
#include "KV_Database.h"


class Finance;

class LogSystem;

class Finance {
    friend class LogSystem;

private:
    double Income = 0;
    double Expenditure = 0;
    int Time = 0;

public:
    Finance(double income, double expenditure, int time);

    Finance() = default;

    friend bool operator<(const Finance &a, const Finance &b) {
        return a.Time < b.Time;
    }

    friend bool operator==(const Finance &a, const Finance &b) {
        return a.Time == b.Time;
    }
};

class Operation {
    friend class LogSystem;

private:
    string _operator;
    string operate;
    string detail;

public:
};

class LogSystem {
private:
    BlockList<Operation> operationDocument;
    BlockList<Finance> financeDocument;
    std::fstream basicData;
    int time = 0;
    Finance currentFinance;
public:
    LogSystem();

    ~LogSystem();

    void Report_Personal_Log(std::string employee_name);

    void Show_Finance_Log(int _time);

    void Report_Finance_Log();

    void Query_Employee_Log();

    void Query_All_Log();

    void Save_Operation(const string &operation, const string &_operator);

    void Save_Finance(double amount, bool sign);

    void Show_Finance();
};

Finance::Finance(double income, double expenditure, int time) : Income(income), Expenditure(expenditure), Time(time) {}

LogSystem::LogSystem() : financeDocument("FinanceFile"), operationDocument("OperationFlie") {
    basicData.open("basicData");
    if (!basicData) {
        basicData.clear();
        basicData.open("basicData", std::fstream::out);
        basicData.close();
    } else {
        basicData.seekg(0);
        basicData.read(reinterpret_cast<char *>(&time), sizeof(int));
        basicData.seekg(sizeof(int));
        basicData.read(reinterpret_cast<char *>(&currentFinance), sizeof(Finance));
    }
}

void LogSystem::Save_Finance(double amount, bool sign) {
    if (sign == true) {
        currentFinance.Income += amount;
        currentFinance.Time++;
        time++;
        financeDocument.Add(Pair(MyString::to_MyString(std::to_string(time)), currentFinance));
    } else {
        currentFinance.Expenditure += amount;
        currentFinance.Time++;
        time++;
        financeDocument.Add(Pair(MyString::to_MyString(std::to_string(time)), currentFinance));
    }
}

void LogSystem::Show_Finance_Log(int _time) {
    if (_time > time) throw "Invalid";
    else if (_time==time) {
        cout << "+ " << std::fixed << std::setprecision(2) << currentFinance.Income
             << " - " << currentFinance.Expenditure << '\n';
    } else {
        vector<Finance> ans;
        ans = financeDocument.Find(MyString::to_MyString(std::to_string(time - _time)));
        cout << "+ " << std::fixed << std::setprecision(2) << currentFinance.Income - ans.front().Income
             << " - " << currentFinance.Expenditure - ans.front().Expenditure << '\n';
    }
}


void LogSystem::Show_Finance() {
    cout << "+ " << std::fixed << std::setprecision(2) << currentFinance.Income
         << " - " << currentFinance.Expenditure << '\n';
}

LogSystem::~LogSystem() {
    basicData.open("basicData");
    basicData.seekp(0);
    basicData.write(reinterpret_cast<char *>(&time), sizeof(int));
    basicData.seekp(sizeof(int));
    basicData.write(reinterpret_cast<char *>(&currentFinance), sizeof(Finance));
}

void Save_Operation() {}

#endif //BOOKSTORE_LOGSYSTEM_H
