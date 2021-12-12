#ifndef KV_DATABASE_H
#define KV_DATABASE_H

#include <string>
#include <cstring>
#include <algorithm>
#include <fstream>

using std::string;
using std::fstream;
using std::lower_bound;
using std::vector;

const int blockCap = 320;
const int indexLen = 65;

template<class T>
class iofile {
private:
    fstream file;
    string file_name;
    int sizeofT = sizeof(T);
    int count = 0;
    std::vector<int> availablePlace;
public:
    iofile() = default;

    iofile(const string &file_name) : file_name(file_name) {}


    //在文件合适位置写入类对象t，并返回写入的位置索引index
    //位置索引意味着当输入正确的位置索引index，在以下三个函数中都能顺利的找到目标对象进行操作
    //位置索引index可以取为对象写入的起始位置
    int write(T &t) {
        file.open(file_name);
        int tmp;
        if (availablePlace.empty()) {
            tmp = (count++) * sizeof(T);
        } else {
            tmp = availablePlace.back();
            availablePlace.pop_back();
        }
        file.seekp(tmp);
        file.write(reinterpret_cast<char *>(&t), sizeofT);
        file.close();
        return tmp;
    }

    void update(T &t, const int index) {
        file.open(file_name);
        file.seekp(index);
        file.write(reinterpret_cast<char *>(&t), sizeof(T));
        file.close();
    }

    //读出位置索引index对应的T对象的值并赋值给t，保证调用的index都是由write函数产生
    void read(T &t, const int index) {
        file.open(file_name);
        file.seekg(index);
        file.read(reinterpret_cast<char *>(&t), sizeof(T));
        file.close();
    }

    //删除位置索引index对应的对象(不涉及空间回收时，可忽略此函数)，保证调用的index都是由write函数产生
    void Delete(int index) {
        availablePlace.push_back(index);
    }
};


template<class T>
class Pair {
protected:
    string index;
    T value;

public:
    Pair() = default;

    Pair(const string _str, const T _value) : index(_str), value(_value) {};  //todo:_str超长时异常抛出

    string indexOf() const { return index; }

    T valueOf() const { return value; }

    bool operator<(const Pair &otr) {
        if (index != otr.index) return index < otr.index;
        else return value < otr.value;
    }

    bool operator==(const Pair &otr) {
        if (index == otr.index) return true;
        else return value == otr.value;
    }

    bool operator!=(const Pair &otr) {
        if (index != otr.index)return true;
        else return value != otr.value;
    }
};

template<class T>
class Block {
private:
    int size = 0;
    Pair<T> array[blockCap << 1] = {};

public:
    Block() = default;

    Block(Pair<T> *_array, const int &_size) : size(_size) {
        for (int i = 0; i < _size; ++i) {
            array[i] = _array[i];
        }
    }

    int sizeOf() const { return size; }

    Pair<T> &Back() const { return array[size - 1]; }

    Block merge(const Block &otr) {
        memcpy(array + size, otr.array, otr.size * sizeof(Pair<T>));
        size += otr.size;
        return *this;
    }

    Block split() {
        size -= blockCap;
        auto newBlock = Block(array + size, blockCap);
        memset(array + size, Pair<T>(), sizeof(Pair<T>));
        return newBlock;
    }

    bool addInBlock(const Pair<T> &obj) {
        int loc = std::lower_bound(array, array + size, obj) - array;
        if (array[loc] == obj) throw "Already Have";
        ++size;
        for (int i = size - 1; i > loc; --i) array[i] = array[i - 1];
        array[loc] = obj;
        return size < (blockCap << 1);
    }

    bool deleteInBlock(const Pair<T> &obj, const Block &otr) {
        int loc = std::lower_bound(array, array + size, obj) - array;
        if (array[loc] != obj) throw "Cannot Find";
        --size;
        for (int i = loc; i < size; ++i) array[i] = array[i + 1];
        return size > (blockCap/2);
    }

    void findInBlock(const string &obj,vector<Pair<T>> &ans){
        for(int i=0;i<size;++i){if(array[i].index==obj) ans.push_back(array[i]);}
    }

};

template <class T>
class BlockList{
private:
    iofile<T> blockList;
public:
    BlockList()=default;

    explicit BlockList(const string &file_name){
        blockList(file_name);
    }

    void Add(const Pair<T> &obj){
        Block<T> tmp;
        do{
            blockList.
        }while (tmp.Back()<obj)
    }

    void Delete(){}

    void Find(){}
}


#endif
