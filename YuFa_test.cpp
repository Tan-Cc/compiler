//
// Created by x1c on 2018/12/8.
//

#include<fstream>
#include<iostream>
#include<string>
#include"Grammar.h"


int YuFa_test() {
    std::string str, program;
    Storage *storage = new Storage();
    Grammar test(storage);
    std::ifstream is;
    std::cout << "tan_jun_xi" << '\t' << "JK1"
              << '\t' << "201630599333" << "\n\n";
    std::cout << "Input the program" << '\n';
    std::cin >> program;
    std::cin.get();
    // F:/compiler/yftest1.txt
    // F:/compiler/yftest2.txt
    // F:/compiler/yftest3.txt
    // F:/compiler/yftest4.txt
    // F:/compiler/yftest5.txt

    is.open(program);
    if (is) {
        std::cout << "\nresult:\n";
        try {
            test.parse(is);
        }
        catch (string_error e) {
            e.print(std::cerr);
            delete storage;
            std::cin.get();
            return 1;
        }
        catch (anotation_error e) {
            e.print(std::cerr);
            delete storage;
            std::cin.get();
            return 1;
        }
        catch (character_error e) {
            e.print(std::cerr);
            delete storage;
            std::cin.get();
            return 1;
        }
        catch (word_error e) {
            e.print(std::cerr);
            delete storage;
            std::cin.get();
            return 1;
        }
        catch (syntax_error e) {
            e.print(std::cerr);
            std::cin.get();
            delete storage;
            return 1;
        }
        catch (type_error e) {
            e.print(std::cerr);
            std::cin.get();
            delete storage;
            return 1;
        }
        catch (declare_error e) {
            e.print(std::cerr);
            std::cin.get();
            delete storage;
            return 1;
        }
    }
    else {
        delete storage;
        std::cerr << "failed" << '\n';
        return 1;
    }
    test.printCode(std::cout);
    std::cout << "\nend the program\n";
    std::cin.get();
    delete storage;
    return 0;
}


int main() {
	return YuFa_test();
}