//
// Created by x1c on 2018/12/6.
//

#pragma once
#include<exception>
#include<iostream>
#include<vector>
#include"Scanner.h"
#include"Store.h"
#define EMPTY -1
#define EMPTY_CHAIN 0


struct quaternary {
    const std::string ope;
    int ide_l;
    int ide_r;
    int ide;
};


class syntax_error :public std::runtime_error {
public:
    const unsigned pos;

    syntax_error(unsigned pos) :
            std::runtime_error("syntax error"),
            pos(pos) {}

    void print(std::ostream &out) {
        out << "NUM:" << pos << "R----"
            << what() << "\n";
    }
};


class type_error :public std::runtime_error {
public:
    const unsigned pos;
    const std::string name;

    type_error(unsigned pos,
               const std::string &name) :
            std::runtime_error("Error of variable type:"),
            pos(pos), name(name) {}

    void print(std::ostream &out) {
        out << "NUM: " << pos << "R----"
            << what() << " " << name << " \n";
    }
};


class declare_error :public std::runtime_error {
public:
    const unsigned pos;
    const std::string name;

    declare_error(unsigned pos,
                  const std::string &name) :
            std::runtime_error("Variables not declared:"),
            pos(pos), name(name) {}

    void print(std::ostream &out) {
        out << "NUM: " << pos << "R----"
            << what() << " " << name << " \n";
    }
};

//Parser类利用带回溯的自顶向下方法，实现语法分析
//在进行语法分析的同时进行中间代码生成，生成四元式结果
class Grammar {

    Storage *storage;
    //生成的中间代码串
    std::vector<quaternary> Code;
    //词法分析获得的Token串
    std::vector<token> Tokens;
    //记录文本代码中每行对应的第一个Token位置
    //为抛出异常时填写错误信息服务
    std::vector<unsigned> rowMark;

    unsigned getRow(size_t);

    //四元式生成
    unsigned Add(const std::string &ope,
                 int ide_l, int ide_r, int ide);

    //四元式代码链合并、回填
    void backpatch(unsigned, unsigned);
    unsigned merge(unsigned, unsigned);

    //判断下一个Token是否为对应非终结符
    bool isType(size_t);
    bool isBoolType(size_t);
    bool isIntType(size_t);
    bool isRelation(size_t);
    bool isBoolean(size_t);
    bool isEqual(const token &,
                 const std::string &);

    bool isProgram(size_t);
    bool isVDeclaration(size_t i);
    bool isVDefinitionEnd(size_t i);
    bool isCStatement(size_t i);
    bool isStatement(size_t);

    //每个非终结符对应一个处理函数
    //根据表达式生成四元式
    bool program(size_t &);
    bool vDefinition(size_t &i);
    bool identifierTable(size_t &, unsigned &);

    bool statement(size_t &, unsigned &);
    bool assignmentStatement(size_t &);
    bool ifStatement(size_t &, unsigned &);
    bool whileStatement(size_t &, unsigned &);
    bool repeatStatement(size_t &, unsigned &);
    bool cStatement(size_t &i);
    bool statementList(size_t &);

    bool amExpression(size_t &i, int &ide);
    bool item(size_t &, int &);
    bool divisor(size_t &, int &);
    bool amUnit(size_t &i, int &ide);
    bool booleanE(size_t &i, unsigned &tl, unsigned &fl);
    bool booleanI(size_t &i, unsigned &tl, unsigned &fl);
    bool booleanD(size_t &i, unsigned &tl, unsigned &fl);
    bool booleanU(size_t &, unsigned &, unsigned &);

public:
    Grammar(Storage *storage) :storage(storage) {}
    //对输入的文件进行词法分析和语法分析，生成四元式代码
    void parse(std::ifstream &);
    //输出生成的四元式代码
    void printCode(std::ostream &out);

};
