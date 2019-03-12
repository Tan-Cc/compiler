//
// Created by x1c on 2018/12/6.
//

#include<iostream>
#include<fstream>
#include<string>
#include"Data.h"
#include"Grammar.h"


unsigned Grammar::getRow(size_t i) {
    for (unsigned j = 0; j <= rowMark.size(); ++j)
        if (i < rowMark[j])
            return j+1;

    return rowMark.size();
}


unsigned Grammar::Add(const std::string &ope,
                      int ide_l, int ide_r, int ide) {

    Code.push_back(quaternary{
            ope, ide_l, ide_r, ide });
    return Code.size() - 1;
}


void Grammar::backpatch(unsigned p, unsigned t) {

    for (int i = p; i != EMPTY;) {
        int tmp = Code[i].ide;
        Code[i].ide = t;
        i = tmp;
    }
}


unsigned Grammar::merge(unsigned p1, unsigned p2) {

    if (p2 == EMPTY_CHAIN)
        return p1;

    else {
        int i;
        for (i = p2; Code[i].ide != EMPTY;)
            i = Code[i].ide;

        Code[i].ide = p1;
        return p2;
    }
}


bool Grammar::isType(size_t code) {

    for (const std::string &str : Data::type)
        if(code == Data::getCode(str))
            return true;

    return false;
}


bool Grammar::isBoolType(size_t code) {

    if (code == Data::getCode("bool"))
        return true;
    else
        return false;
}


bool Grammar::isIntType(size_t code) {

    if (code == Data::getCode("integer"))
        return true;
    else
        return false;
}


bool Grammar::isRelation(size_t code) {

    for (const std::string &str : Data::relation)
        if (code == Data::getCode(str))
            return true;

    return false;
}


bool Grammar::isBoolean(size_t code) {

    for (const std::string &str : Data::boolean)
        if (code == Data::getCode(str))
            return true;

    return false;
}


bool Grammar::isEqual(const token &t,
                     const std::string &str) {

    if (t.type_index == Data::getCode(str))
        return true;
    else
        return false;
}


bool Grammar::isProgram(size_t i) {

    if (i + 2 < Tokens.size() &&
        isEqual(Tokens[i], "program") &&
        isEqual(Tokens[i + 1], IDENTIFIER) &&
        isEqual(Tokens[i + 2], ";"))
        return true;
    else
        return false;
}


bool Grammar::isVDeclaration(size_t i) {

    if (i < Tokens.size() &&
        isEqual(Tokens[i], "var"))
        return true;
    else
        return false;
}


bool Grammar::isVDefinitionEnd(size_t i) {

    if (i + 2 < Tokens.size() &&
        isEqual(Tokens[i], ":") &&
        isType(Tokens[i + 1].type_index) &&
        isEqual(Tokens[i + 2], ";"))
        return true;
    else
        return false;
}


bool Grammar::isStatement(size_t i) {
    if (i < Tokens.size() && (
            isEqual(Tokens[i], IDENTIFIER) ||
            isEqual(Tokens[i], "if") ||
            isEqual(Tokens[i], "while") ||
            isEqual(Tokens[i], "repeat")))
        return true;
    else
        return false;
}


bool Grammar::isCStatement(size_t i) {

    if (i < Tokens.size() &&
        isEqual(Tokens[i], "begin"))
        return true;
    else
        return false;
}


bool Grammar::program(size_t &i) {

    //std::cout << "hhhhhhh" << Tokens[i + 1].val_index << '\n';
    //std::cout << storage->getName(1) << '\n';
    Add("program", Tokens[i + 1].val_index,
        EMPTY, EMPTY);



    i += 3;

    if (i < Tokens.size()) {

        if (isVDeclaration(i)) {
            if (!vDefinition(++i))
                return false;
        }
        else
            return false;

        if (isCStatement(i)) {
            if (!cStatement(i))
                return false;
        }
        else
            return false;
    }
    else
        return false;

    if (i == Tokens.size() - 1 &&
        isEqual(Tokens[i], ".")) {

        Add("sys", EMPTY, EMPTY, EMPTY);
        return true;
    }
    else
        return false;
}


bool Grammar::vDefinition(size_t &i) {

    bool mark = false;
    unsigned type = 0;

    while (i < Tokens.size() &&
           isEqual(Tokens[i], IDENTIFIER)) {
        mark = true;
        if (!identifierTable(i, type))
            return false;
    }

    return mark;
}


bool Grammar::identifierTable(size_t &i, unsigned &type) {

    storage->setDeclare(Tokens[i].val_index);
    size_t mark = i;

    if (i + 1 < Tokens.size() &&
        isEqual(Tokens[i + 1], ",")) {

        i += 2;

        if (i < Tokens.size() &&
            isEqual(Tokens[i], IDENTIFIER)) {

            if (identifierTable(i, type)) {
                storage->setType(
                        Tokens[mark].val_index, type);
                return true;
            }
            else
                return false;
        }
        else
            return false;
    }

    else if (i + 3 < Tokens.size() &&
             isEqual(Tokens[i + 1], ":") &&
             isType(Tokens[i + 2].type_index) &&
             isEqual(Tokens[i + 3], ";")) {

        type = Tokens[i + 2].type_index;
        storage->setType(
                Tokens[mark].val_index, type);
        i += 4;
        return true;
    }

    else
        return false;

}


bool Grammar::statement(size_t &i, unsigned &chain) {

    if (i < Tokens.size()) {
        if (isEqual(Tokens[i], IDENTIFIER))
            return assignmentStatement(i);
        else if (isEqual(Tokens[i], "if"))
            return ifStatement(i, chain);
        else if (isEqual(Tokens[i], "while"))
            return whileStatement(i, chain);
        else if (isEqual(Tokens[i], "repeat"))
            return repeatStatement(i, chain);
    }

    return false;
}


bool Grammar::assignmentStatement(size_t &i) {

    i += 1;

    if (i < Tokens.size() &&
        isEqual(Tokens[i], ":=")) {

        unsigned mark = i;
        int ide = EMPTY;
        if (amExpression(++i, ide)) {

            if (!storage->isSameType(
                    Tokens[mark - 1].val_index, ide))
                throw type_error(getRow(mark - 1),
                                 storage->getName(
                                         Tokens[mark - 1].val_index));

            else {
                Add(":=", ide,
                    EMPTY, Tokens[mark - 1].val_index);
                return true;
            }
        }
    }

    return false;
}


bool Grammar::ifStatement(size_t &i, unsigned &chain) {

    i += 1;
    unsigned tl, fl, then_chain = EMPTY_CHAIN,
            else_chain = EMPTY_CHAIN;

    if (booleanE(i, tl, fl))

        if (i < Tokens.size() &&
            isEqual(Tokens[i], "then")) {

            backpatch(tl, Code.size());

            if (statement(++i, then_chain)) {
                if (i < Tokens.size() &&
                    isEqual(Tokens[i], "else")) {

                    unsigned mark = Code.size();
                    Add("j", EMPTY, EMPTY, EMPTY);
                    backpatch(fl, Code.size());
                    unsigned tmp_chain = merge(mark, then_chain);

                    if (statement(++i, else_chain)) {
                        chain = merge(tmp_chain, else_chain);
                        return true;
                    }
                }
                else {
                    chain = merge(fl, then_chain);
                    return true;
                }
            }
        }

    return false;
}


bool Grammar::whileStatement(size_t &i, unsigned &chain) {

    i += 1;
    unsigned tl, fl, tmp_chain = EMPTY_CHAIN,
            mark = Code.size();

    if (booleanE(i, tl, fl)) {

        if (i < Tokens.size() &&
            isEqual(Tokens[i], "do")) {

            backpatch(tl, Code.size());

            if (statement(++i, tmp_chain)) {

                backpatch(tmp_chain, mark);
                Add("j", EMPTY, EMPTY, mark);
                chain = fl;
                return true;
            }
            else
                return false;
        }
        else
            return false;
    }
    else
        return false;
}


bool Grammar::repeatStatement(size_t &i, unsigned &chain) {

    i += 1;
    unsigned tl, fl;
    unsigned mark = Code.size();

    if (statement(i, chain)) {

        if (i < Tokens.size() &&
            isEqual(Tokens[i], "until")) {
            if (booleanE(++i, tl, fl)) {
                chain = tl;
                backpatch(fl, mark);
                return true;
            }
            else
                return false;
        }
        else
            return false;
    }
    else
        return false;
}


bool Grammar::cStatement(size_t &i) {

    i += 1;
    if (statementList(i)) {

        if (i < Tokens.size() &&
            isEqual(Tokens[i], "end")) {
            ++i;
            return true;
        }
        else
            return false;
    }
    else
        return false;
}


bool Grammar::statementList(size_t &i) {

    i -= 1;
    unsigned chain = EMPTY_CHAIN;
    do {
        ++i;
        if (!statement(i, chain))
            return false;
        else {
            if (chain != EMPTY_CHAIN)
                backpatch(chain, Code.size());
        }
    } while (i < Tokens.size() &&
             isEqual(Tokens[i], ";"));

    return true;
}


bool Grammar::amExpression(size_t &i, int &ide) {

    if (item(i, ide)) {

        if (i < Tokens.size() && (
                isEqual(Tokens[i], "+") ||
                isEqual(Tokens[i], "-"))) {

            unsigned mark = i;
            int ide_r = EMPTY;
            if (amExpression(++i, ide_r)) {
                int index = storage->
                        setIdentifer(TEMPORARY_VARIABLE);
                storage->setType(index,
                                 storage->getType(ide));
                Add(Data::getValue(Tokens[mark].
                        type_index), ide, ide_r, index);
                ide = index;
                return true;
            }
            else
                return false;
        }
        else
            return true;
    }
    else
        return false;

}


bool Grammar::item(size_t &i, int &ide) {

    if (divisor(i, ide)) {

        if (i < Tokens.size() && (
                isEqual(Tokens[i], "*") ||
                isEqual(Tokens[i], "/"))) {

            unsigned mark = i;
            int ide_r = EMPTY;

            if (item(++i, ide_r)) {
                int index = storage->
                        setIdentifer(TEMPORARY_VARIABLE);
                storage->setType(index,
                                 storage->getType(ide));
                Add(Data::getValue(Tokens[mark].
                        type_index), ide, ide_r, index);
                ide = index;
                return true;
            }
            else
                return false;
        }
        else
            return true;

    }
    else
        return false;

}


bool Grammar::divisor(size_t &i, int &ide) {

    if (i < Tokens.size() &&
        isEqual(Tokens[i], "-")) {

        unsigned mark = i;

        if (divisor(++i, ide)) {
            int index = storage->
                    setIdentifer(TEMPORARY_VARIABLE);
            storage->setType(index,
                             storage->getType(ide));
            Add(Data::getValue(Tokens[mark].
                    type_index), ide, EMPTY, index);
            ide = index;
            return true;
        }
        else
            return false;
    }

    else
        return amUnit(i, ide);
}


bool Grammar::amUnit(size_t &i, int &ide) {

    if (i < Tokens.size()) {

        if (isEqual(Tokens[i], IDENTIFIER)) {

            if (storage->isDeclare(
                    Tokens[i].val_index) &&
                isIntType(storage->getType(
                        Tokens[i].val_index))) {

                ide = Tokens[i].val_index;
                ++i;
                return true;
            }
            else
                throw type_error(getRow(i),
                                    storage->getName(
                                            Tokens[i-2].val_index));

        }
        else if (isEqual(Tokens[i], INTEGER)) {

            storage->setType(Tokens[i].val_index,
                             Data::getCode("integer"));
            ide = Tokens[i].val_index;
            ++i;
            return true;
        }
        else if (isEqual(Tokens[i], "(")) {

            if (amExpression(++i, ide)) {
                if (isEqual(Tokens[i], ")")) {
                    ++i;
                    return true;
                }
            }
        }
    }

    return false;
}


bool Grammar::booleanE(size_t &i,
                       unsigned &tl, unsigned &fl) {

    unsigned tl_1, fl_1, tl_2, fl_2;

    if (booleanI(i, tl_1, fl_1)) {

        if (i < Tokens.size() &&
            isEqual(Tokens[i], "or")) {

            backpatch(fl_1, Code.size());
            booleanE(++i, tl_2, fl_2);
            fl = fl_2;
            tl = merge(tl_1, tl_2);
            return true;
        }
        else {
            tl = tl_1;
            fl = fl_1;
            return true;
        }
    }
    else
        return false;

}


bool Grammar::booleanI(size_t &i,
                       unsigned &tl, unsigned &fl) {

    unsigned tl_1, fl_1, tl_2, fl_2;

    if (booleanD(i, tl_1, fl_1)) {

        if (i < Tokens.size() &&
            isEqual(Tokens[i], "and")) {

            backpatch(tl_1, Code.size());
            booleanI(++i, tl_2, fl_2);
            tl = tl_2;
            fl = merge(fl_1, fl_2);
            return true;
        }
        else {
            tl = tl_1;
            fl = fl_1;
            return true;
        }
    }
    else
        return false;

}


bool Grammar::booleanD(size_t &i,
                       unsigned &tl, unsigned &fl) {

    if (i < Tokens.size() &&
        isEqual(Tokens[i], "not"))
        return booleanD(++i, fl, tl);
    else
        return booleanU(i, tl, fl);
}


bool Grammar::booleanU(size_t &i,
                       unsigned &tl, unsigned &fl) {

    if (i + 2 < Tokens.size() &&
        isEqual(Tokens[i], IDENTIFIER) &&
        isRelation(Tokens[i + 1].type_index)
        && isEqual(Tokens[i + 2], IDENTIFIER)) {

        if (isIntType(storage->getType(
                Tokens[i].val_index)) &&
            storage->isSameType(Tokens[i].val_index,
                                Tokens[i + 2].val_index)) {

            tl = Code.size();
            Add(std::string("j") +
                Data::getValue(Tokens[i + 1].type_index),
                Tokens[i].val_index,
                Tokens[i + 2].val_index, EMPTY);

            fl = Code.size();
            Add("j", EMPTY, EMPTY, EMPTY);

            i += 3;
            return true;
        }
        else
            throw declare_error(getRow(i),
                             storage->getName(
                                     Tokens[i].val_index));
    }

    else if (i < Tokens.size()) {

        unsigned tmp = Code.size();

        int ide1, ide2;
        if (amExpression(i, ide1)) {
            unsigned mark = i;
            if (i < Tokens.size() &&
                isRelation(Tokens[i].
                        type_index))
                if (amExpression(++i, ide2)) {
                    tl = Code.size();
                    Add("j" + Data::getValue(
                            Tokens[mark].
                                    type_index), ide1, ide2,
                        EMPTY);
                    fl = Code.size();
                    Add("j", EMPTY, EMPTY, EMPTY);
                    return true;
                }
        }

        while (tmp < Code.size())
            Code.pop_back();

        if (isEqual(Tokens[i], IDENTIFIER)) {

            if (!isBoolType(storage->getType(
                    Tokens[i].val_index)) && !isIntType(
                    storage->getType(Tokens[i].val_index)))
                throw type_error(i, storage->getName(Tokens[i].val_index));

            else {
                tl = Code.size();
                Add("jnz", Tokens[i].val_index, EMPTY, EMPTY);
                fl = Code.size();
                Add("j", EMPTY, EMPTY, EMPTY);
                ++i;
                return true;
            }
        }

        else if (isEqual(Tokens[i], "(")) {

            unsigned tmp = i + 1;
            unsigned mark = Code.size();
            if (booleanE(++i, tl, fl))
                if (isEqual(Tokens[i], ")")) {
                    ++i;
                    return true;
                }
                else
                    return false;
            else {
                i = tmp;
                while (Code.size() > mark)
                    Code.pop_back();
            }
        }

        else if (isBoolean(Tokens[i].type_index)) {
            tl = Code.size();
            Add("jnz", Tokens[i].type_index, EMPTY, EMPTY);
            fl = Code.size();
            Add("j", EMPTY, EMPTY, EMPTY);
            ++i;
            return true;
        }

    }

    return false;
}


void Grammar::parse(std::ifstream &is) {

    std::string str;
    Scanner scanner(storage);
    unsigned row = 1;
    while (std::getline(is, str)) {
        size_t i = 0;
        while (i < str.length()) {
            //std::cout << str << '\n';
            token token = scanner.scan(str, i, row);
            if (token.val_index != VALUE_NONE) {
                Tokens.push_back(token);
                //std::cout << token.type_index << " " << token.val_index << '\n';
            }
        }
        rowMark.push_back(Tokens.size());
        ++row;
    }

    size_t i = 0;
    if (isProgram(i) && program(i))
        return;
    else
        throw syntax_error(getRow(i));
}


void Grammar::printCode(std::ostream &out) {

    unsigned i = 0;
    for (quaternary q : Code) {

        //out << "(" << q.ope << "," << q.ide_l << "," << q.ide_r << "," << q.ide << ")" << '\n';

        out << "(" << i << ")" << "\t(" << q.ope << ", ";

        if (q.ide_l == EMPTY)
            out << "-, ";
        else if (q.ope == "jnz") {
            if (isBoolean(q.ide_l))
                out << Data::getValue(q.ide_l) << ", ";
            else
                out << storage->getName(q.ide_l)<< ", ";
        }
        else
            out << storage->getName(q.ide_l) << ", ";

        if (q.ide_r == EMPTY)
            out << "-, ";
        else
            out << storage->getName(q.ide_r) << ", ";

        if (q.ope[0] == 'j')
            out << q.ide;
        else if (q.ide == EMPTY)
            out << "-";
        else
            out << storage->getName(q.ide);

        out << ")\n";

        ++i;
    }
}