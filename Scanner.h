//
// Created by x1c on 2018/12/3.
//

#pragma once
#include<exception>
#include<iostream>
#include<string>
#include<vector>
#include"Store.h"
#define VALUE_WORD -2
#define VALUE_NONE -1


struct token {
    unsigned type_index;
    int val_index;
};

std::ostream &operator<<(std::ostream &, const token &);
bool isEqual(const token &, const std::string &);


class anotation_error :public std::runtime_error {
public:
    const unsigned pos;
    const unsigned row;

    anotation_error(unsigned pos, unsigned row):
            std::runtime_error("the annotation without '*/' "),
            pos(pos), row(row) {}

    void print(std::ostream &out) {
        out << "NUM: " << row << "R, NUM: "
            << pos << "P----" << what() << "\n";
    }
};


class string_error :public std::runtime_error {
public:
    const unsigned pos;
    const unsigned row;

    string_error(unsigned pos, unsigned row):
            std::runtime_error("The string without '"),
            pos(pos), row(row) {}

    void print(std::ostream &out) {
        out << "NUM: " << row << "R, NUM: "
            << pos << "P----" << what() << "\n";
    }
};


class character_error :public std::runtime_error {
public:
    const unsigned pos;
    const unsigned row;
    const char c;

    character_error(unsigned pos, unsigned row,
                    char c) :std::runtime_error("illegal character: "),
                             pos(pos), row(row), c(c) {}

    void print(std::ostream &out) {
        out << "NUM: " << row << "R, NUM: "
            << pos << "P----" << what()
            << " '" << c << "'\n";
    }
};


class word_error :public std::runtime_error {
public:
    const unsigned pos;
    const unsigned row;
    const char c;

    word_error(unsigned pos, unsigned row,
               char c) :std::runtime_error("unexpected word: "),
                        pos(pos), row(row), c(c) {}

    void print(std::ostream &out) {
        out << "NUM: " << row << "R, NUM: "
            << pos << "P----" << what()
            << " '" << c << "'\n";
    }
};

//Scanner类利用状态机原理，实现词法分析，生成Token
class Scanner {

    Storage *storage;

    //Token生成
    token getResult(const std::string &, int);

    //状态判断
    bool isDigital(char);
    bool isString(char);
    bool isDelimiter(char);
    bool isChar(char);

    bool isEmpty(char);
    bool isDoubleCharDelimiter(const std::string &);
    bool isAnotation(const std::string &);
    bool isAnotationEnd(const std::string &);
    bool isReserveWord(const std::string &);

    //状态处理
    token identifier(const std::string &, size_t &);
    token delimiter(const std::string &, size_t &, unsigned);
    token integer(const std::string &, size_t &, unsigned);
    token string(const std::string &, size_t &, unsigned);

public:
    Scanner(Storage *storage) :storage(storage) {}
    //传入待分析的字符串及位置，每读取一个单词返回
    //为了更好地显示报错信息，需要传入字符串在代码文本中的行号
    token scan(const std::string &, size_t &, unsigned);
};
