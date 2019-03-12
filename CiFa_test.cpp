//
// Created by x1c on 2018/12/5.
//

#include<iostream>
#include<fstream>
#include<string>
#include"Scanner.h"
#include"Store.h"


int CiFa_test() {
    std::string str, program;
    Storage *storage = new Storage();
    Scanner test(storage);
    std::ifstream is;
    std::cout << "tan_jun_xi" << '\t' << "JK1"
              << '\t' << "201630599333" << "\n\n";
    std::cout << "please input the test program" << '\n';
    std::cin >> program;
    //   F:/compiler/cifatest1.txt
    //   F:/compiler/cifatest2.txt
    //   F:/compiler/cifatest3.txt
    //   F:/compiler/cifatest4.txt
    //   F:/compiler/cifatest5.txt
    std::cin.get();
    is.open(program);
    if (is) {
        std::cout << "CIFA result:\n";
        unsigned count = 0;
        unsigned row = 1;
        while (std::getline(is, str)) {
            //std::cout << str << '\t' << str.length() <<  '\n';
            for (unsigned i = 0; i < str.length();) {
                try {
                    token result = test.scan(str, i, row);
                    if (result.val_index != VALUE_NONE) {
                        ++count;
                        std::cout << result << '\t';
                        if (count % 5 == 0)
                            std::cout << '\n';
                    }
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
            }
            ++row;
        }
        is.close();
    }

    delete storage;
    std::cout << "\ninput a to end the program\n";
    std::cin.get();
    return 0;
}


//int main() {
//	return CiFa_test();
//}
