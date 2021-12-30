#ifndef MAIN_CPP_MYSTRING_H
#define MAIN_CPP_MYSTRING_H

#include <iostream>
#include <cstring>

using std::string;
using std::ostream;

class MyString {
public:
    char str[65];

    MyString(const char* obj){
        strcpy(str,obj);
    }

    MyString(string obj){
        strcpy(str,obj.c_str());
    }

    MyString():MyString(""){
    }

    static MyString to_MyString(const string &obj){
        MyString a;
        strcpy(a.str,obj.c_str());
        return a;
    }

    friend bool operator<(const MyString &a,const MyString &b){
        if(strcmp(a.str,b.str)<0) return true;
        else return false;
    }

    friend bool operator>(const MyString &a,const MyString &b){
        if(strcmp(a.str,b.str)>0) return true;
        else return false;
    }

    friend bool operator==(const MyString &a,const MyString &b){
        if(strcmp(a.str,b.str)==0) return true;
        else return false;
    }

    friend bool operator!=(const MyString &a,const MyString &b){
        if(strcmp(a.str,b.str)==0) return false;
        else return true;
    }

    friend bool operator>=(const MyString &a,const MyString &b){
        return !(a<b);
    }

    friend bool operator<=(const MyString &a,const MyString &b){
        return !(a>b);
    }

    friend  ostream& operator<<(ostream &os, const MyString &obj){
        os<<obj.str;
        return os;
    }
};

#endif //MAIN_CPP_MYSTRING_H
