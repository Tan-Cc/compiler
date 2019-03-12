//
// Created by x1c on 2018/12/3.
//

#include <iostream>
#include"Store.h"
using iter = std::vector<std::string>::const_iterator;


unsigned Storage::setIdentifer(const std::string &val) {

    if (val == TEMPORARY_VARIABLE) {
        Identifers.push_back(
                ide{ std::string("T") +
                     std::to_string(tem_count), 0, true });
        ++tem_count;
        return Identifers.size() - 1;
    }

    if(Identifers.size() == 0) {
        Identifers.push_back(ide{"tjx", 0, false});
    }

    unsigned i;
    for (i = 1; i < Identifers.size(); ++i)
        if (val == Identifers[i].name)
            break;

    if (i == Identifers.size())
        Identifers.push_back(ide{ val, 0, false });

    //std::cout << "S:" << i << " " <<Identifers[i].name << '\n';

    return i;
}



void Storage::setDeclare(unsigned i) {

    if (i < Identifers.size())
        Identifers[i].is_declare = true;
}


void Storage::setType(unsigned i, unsigned type) {

    if (i < Identifers.size())
        Identifers[i].type = type;
}


bool Storage::isDeclare(unsigned i) {

    if (i < Identifers.size())
        return Identifers[i].is_declare;
    else
        return false;
}


bool Storage::isSameType(unsigned i, unsigned j) {

    if (i < Identifers.size() &&
        j < Identifers.size()) {

        if (i == j ||
            Identifers[i].type == Identifers[j].type)
            return true;
        else
            return false;
    }
    else
        return false;
}


unsigned Storage::getType(unsigned i) {

    if (i < Identifers.size())
        return Identifers[i].type;
    else
        return 0;
}


std::string Storage::getName(unsigned i) {

    if (i < Identifers.size())
        return Identifers[i].name;
    else
        return std::string();
}


void Storage::clear() {
    Identifers.clear();
}