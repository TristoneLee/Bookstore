#ifndef KV_DATABASE_H
#define KV_DATABASE_H

#include "MyString.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

using std::string;
using std::fstream;
using std::lower_bound;
using std::vector;
using std::cout;
using std::cin;
using std::endl;

const int blockCap = 320;
const int indexCap = 500;

template<class T>
class Block;

template<class Type>
class iofile {
private:
    fstream file;
    string file_name;
    int sizeofT = sizeof(Type);
    int count = 0;
    std::vector<int> availablePlace;
public:
    iofile() = default;

    iofile(const string &_file_name) : file_name(_file_name) {
        file.open(file_name);
        if (file) {
            file.seekg(0);
            file.read(reinterpret_cast<char *>(&count), sizeof(int));
            file.close();
        } else {
            file.clear();
            file.open(file_name, std::fstream::out);
            file.seekp(0);
            file.write(reinterpret_cast<char *>(&count), sizeof(int));
            file.close();
        }
    }


    //在文件合适位置写入类对象t，并返回写入的位置索引index
    //位置索引意味着当输入正确的位置索引index，在以下三个函数中都能顺利的找到目标对象进行操作
    //位置索引index可以取为对象写入的起始位置
    int write(Type &t) {
        file.open(file_name);
        int tmp;
        if (availablePlace.empty()) {
            tmp = (count++) * sizeof(Type) + sizeof(int);
        } else {
            tmp = availablePlace.back();
            availablePlace.pop_back();
        }
        file.seekp(0);
        file.write(reinterpret_cast<char *>(&count), sizeof(int));
        file.seekp(tmp);
        file.write(reinterpret_cast<char *>(&t), sizeofT);
        file.close();
        return tmp;
    }

    void update(Type &t, const int index) {
        file.open(file_name);
        file.seekp(index);
        file.write(reinterpret_cast<char *>(&t), sizeof(Type));
        file.close();
    }

    //读出位置索引index对应的T对象的值并赋值给t，保证调用的index都是由write函数产生
    void read(Type &t, const int index) {
        file.open(file_name);
        file.seekg(index);
        file.read(reinterpret_cast<char *>(&t), sizeof(Type));
        file.close();
    }

    //删除位置索引index对应的对象(不涉及空间回收时，可忽略此函数)，保证调用的index都是由write函数产生
    void Delete(int index) {
        availablePlace.push_back(index);
    }

    int countOf() { return count; }
};


template<class T>
class Pair {
protected:
    MyString index;
    T value;

public:
    friend class Block<T>;

    Pair() = default;

    Pair(const MyString _str, const T &_value) : value(_value), index(_str) {
    };  //todo:_str超长时异常抛出

    MyString indexOf() const { return index; }

    T valueOf() const { return value; }

    friend bool operator<(const Pair &p, const Pair &q) {
        if (p.index < q.index) return true;
        if (p.index > q.index) return false;
        else return p.value < q.value;
    }

    friend bool operator==(const Pair &p, const Pair &q) {
        if (p.index != q.index) return false;
        else return p.value == q.value;
    }

    friend bool operator!=(const Pair &p, const Pair &q) {
        return !(p == q);
    }
};

template<class T>
class Block {
private:
    int size = 0;
    Pair<T> array[blockCap * 3] = {};

public:
    Block() = default;

    Block(Pair<T> *_array, const int &_size) : size(_size) {
        for (int i = 0; i < _size; ++i) {
            array[i] = _array[i];
        }
    }

    int sizeOf() const { return size; }

    const Pair<T> &Back() const { return array[size - 1]; }

    const Pair<T> &Front() const { return array[0]; }

    Block merge(const Block &otr) {
        memcpy(array + size, otr.array, otr.size * sizeof(Pair<T>));
        size += otr.size;
        return *this;
    }

    Block split() {
        size -= blockCap;
        auto newBlock = Block(array + size, blockCap);
        return newBlock;
    }

    bool addInBlock(const Pair<T> &obj) {
        int loc = std::lower_bound(array, array + size, obj) - array;
        if (array[loc] == obj &&loc!=size) throw "Invalid";
        ++size;
        for (int i = size - 1; i > loc; --i) array[i] = array[i - 1];
        array[loc] = obj;
        return size < (blockCap << 1);
    }

    bool deleteInBlock(const Pair<T> &obj) {
        int loc = std::lower_bound(array, array + size, obj) - array;
        if (array[loc] != obj) throw "Invalid";
        --size;
        for (int i = loc; i < size; ++i) array[i] = array[i + 1];
        return size > (blockCap / 2);
    }

    void findInBlock(MyString obj, vector<T> &ans) {
        for (int i = 0; i < size; ++i) {
            if (obj == array[i].indexOf()) {
                ans.push_back(array[i].valueOf());
            }
        }
    }

    void showInBlock(vector<T> &ans) {
        for (int i = 0; i < size; ++i)
            ans.push_back(array[i].valueOf());
    }

    void updateInBlock(const Pair<T> &obj, T _value) {
        int loc = std::lower_bound(array, array + size, obj) - array;
        if (array[loc] != obj) throw "Invalid";
        array[loc].value = _value;
    }

};

template<class T>
class BlockList {
private:
    iofile<Block<T>> blockList;
    fstream indexList;
    int indexArray[indexCap] = {0};
    int blockCount = 0;
    string fileName;
public:
    BlockList() = default;

    BlockList(const string &file_name) : blockList(file_name), fileName(file_name) {
        indexList.open(file_name + "_index");
        if (indexList) {
            indexList.seekg(0);
            indexList.read(reinterpret_cast<char *>(&blockCount), sizeof(int));
            indexList.seekg(sizeof(int));
            indexList.read(reinterpret_cast<char *>(&indexArray), sizeof(indexArray));
            indexList.close();
        } else {
            indexList.clear();
            indexList.open(file_name + "_index", std::fstream::out);
            indexList.close();
        }
    }

    ~BlockList() {
        indexList.open(fileName + "_index");
        indexList.seekg(0);
        indexList.write(reinterpret_cast<char *>(&blockCount), sizeof(int));
        indexList.seekg(sizeof(int));
        indexList.write(reinterpret_cast<char *>(&indexArray), sizeof(indexArray));
        indexList.close();
    }

//    int binary_search(const Block<T> &obj) {
//        Block<T> tmp;
//        int ans=-1;
//        int lp=0, rp=blockCount-1,mid;
//        while(lp<=rp){
//            mid=lp+(rp-lp)/2;
//            blockList.read(tmp,indexArray[mid])
//        }
//    }


    void Add(const Pair<T> &obj) {
        if (blockCount == 0) {
            Block<T> tmp;
            tmp.addInBlock(obj);
            indexArray[0] = blockList.write(tmp);
            ++blockCount;
            indexList.open(fileName + "_index");
            indexList.seekp(0);
            indexList.write(reinterpret_cast<char *>(&blockCount), sizeof(int));
            indexList.seekp(sizeof(int));
            indexList.write(reinterpret_cast<char *>(&indexArray), sizeof(indexArray));
            indexList.close();
        } else {
            Block<T> tmp;
            int presentBlock = 0;
            do {
                ++presentBlock;
                if (presentBlock == blockCount) break;
                blockList.read(tmp, indexArray[presentBlock - 1]);
            } while (tmp.Back() < obj);
            blockList.read(tmp, indexArray[presentBlock - 1]);
            if (!tmp.addInBlock(obj)) {
                Block<T> newBlock = tmp.split();
                for (int i = blockCount; i > presentBlock; --i) indexArray[i] = indexArray[i - 1];
                indexArray[presentBlock] = blockList.write(newBlock);
                ++blockCount;
            }
            blockList.update(tmp, indexArray[presentBlock - 1]);
        }
    }

    void Delete(const Pair<T> &obj) {
        Block<T> tmp;
        if (blockCount == 0) throw "Invalid";
        int presentBlock = 0;
        do {
            ++presentBlock;
            if (presentBlock == blockCount) break;
            blockList.read(tmp, indexArray[presentBlock - 1]);
        } while (tmp.Back() < obj);
        blockList.read(tmp, indexArray[presentBlock - 1]);
        if (!tmp.deleteInBlock(obj) && presentBlock != blockCount) {
            Block<T> nextBlock;
            blockList.read(nextBlock, indexArray[presentBlock]);
            blockList.Delete(indexArray[presentBlock]);
            --blockCount;
            tmp.merge(nextBlock);
            for (int i = presentBlock; i < blockCount; ++i) indexArray[i] = indexArray[i + 1];
        }
        blockList.update(tmp, indexArray[presentBlock - 1]);
    }

    std::vector<T> Find(MyString obj) {
        if (blockCount == 0) return vector<T>();
        int presentBlock = 0;
        vector<T> ans;
        Block<T> tmp;
        for (presentBlock; presentBlock < blockCount; ++presentBlock) {
            blockList.read(tmp, indexArray[presentBlock]);
            if (tmp.Back().indexOf() < obj) continue;
            if (obj < tmp.Front().indexOf()) break;
            tmp.findInBlock(obj, ans);
        }
        return ans;
    }

    std::vector<T> Show() {
        int presentBlock = 0;
        vector<T> ans;
        Block<T> tmp;
        for (presentBlock; presentBlock < blockCount; ++presentBlock) {
            blockList.read(tmp, indexArray[presentBlock]);
            tmp.showInBlock(ans);
        }
        return ans;
    }

    void Update(const Pair<T> &obj, T _value) {
        Block<T> tmp;
        if (blockCount == 0) throw "Invalid";
        int presentBlock = 0;
        do {
            ++presentBlock;
            if (presentBlock == blockCount) break;
            blockList.read(tmp, indexArray[presentBlock - 1]);
        } while (tmp.Back() < obj);
        blockList.read(tmp, indexArray[presentBlock - 1]);
        tmp.updateInBlock(obj, _value);
        blockList.update(tmp, indexArray[presentBlock - 1]);
    }
};


#endif
