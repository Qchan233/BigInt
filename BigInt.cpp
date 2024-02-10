#include <iostream>
#include <vector>
#include <limits.h>
#include <sstream>
#include <fstream>

#include "BigInt.h"

void add_tests() {
    std::ifstream inFile("tests/add.txt");
    if (!inFile) {
        std::cerr << "Unable to open file tests/add.txt";
        return;
    }

    std::string line;
    while (std::getline(inFile, line)) {
        std::istringstream iss(line);
        std::string num1, num2, expected;
        if (!(iss >> num1 >> num2 >> expected)) {
            std::cerr << "Error reading line: " << line << std::endl;
            continue; // skip this line
        }

        BigInt a(num1);
        BigInt b(num2);
        BigInt result = a + b;
        BigInt expectedBigInt(expected);

        if (result == expectedBigInt) {
        } else {
            std::cerr << "Test failed: " << num1 << " + " << num2 << " = " << result << ", expected " << expected << std::endl;
            abort();
        }
    }

    std::cout << "\033[32mAll Test Passed!\033[0m" << std::endl;
}

void sub_tests() {
    std::ifstream inFile("tests/sub.txt");
    if (!inFile) {
        std::cerr << "Unable to open file tests/add.txt";
        return;
    }

    std::string line;
    while (std::getline(inFile, line)) {
        std::istringstream iss(line);
        std::string num1, num2, expected;
        if (!(iss >> num1 >> num2 >> expected)) {
            std::cerr << "Error reading line: " << line << std::endl;
            continue; // skip this line
        }

        BigInt a(num1);
        BigInt b(num2);
         
        BigInt result = a - b;
        BigInt expectedBigInt(expected);

        if (result == expectedBigInt) {
        } else {
            std::cerr << "Test failed: " << num1 << " - " << num2 << " = " << result << ", expected " << expected << std::endl;
            abort();
        }
    }

    std::cout << "\033[32mAll Test Passed!\033[0m" << std::endl;
}

void multbase_tests() {
    std::ifstream inFile("tests/multbase.txt");
    if (!inFile) {
        std::cerr << "Unable to open file tests/add.txt";
        return;
    }

    std::string line;
    while (std::getline(inFile, line)) {
        std::istringstream iss(line);
        std::string num1, num2, expected;
        if (!(iss >> num1 >> num2 >> expected)) {
            std::cerr << "Error reading line: " << line << std::endl;
            continue; // skip this line
        }

        BigInt a(num1);
        std::stringstream ss;
        ss << std::hex << num2;
        unsigned int b;
        ss >> b;
         
        BigInt result = a.mult_base(a, b);
        BigInt expectedBigInt(expected);

        if (result == expectedBigInt) {
        } else {
            std::cerr << "Test failed: " << num1 << " * " << num2 << " = " << result << ", expected " << expected << std::endl;
            abort();
        }
    }

    std::cout << "\033[32mAll Test Passed!\033[0m" << std::endl;
}

void mul_tests() {
    std::ifstream inFile("tests/mul.txt");
    if (!inFile) {
        std::cerr << "Unable to open file tests/add.txt";
        return;
    }

    std::string line;
    while (std::getline(inFile, line)) {
        std::istringstream iss(line);
        std::string num1, num2, expected;
        if (!(iss >> num1 >> num2 >> expected)) {
            std::cerr << "Error reading line: " << line << std::endl;
            continue; // skip this line
        }

        BigInt a(num1);
        BigInt b(num2);
         
        BigInt result = a * b;
        BigInt expectedBigInt(expected);

        if (result == expectedBigInt) {
        } else {
            std::cerr << "Test failed: " << num1 << " * " << num2 << " = " << result << ", expected " << expectedBigInt << std::endl;
            abort();
        }
    }

    std::cout << "\033[32mAll Test Passed!\033[0m" << std::endl;
}

void div_tests() {
    std::ifstream inFile("tests/div.txt");
    if (!inFile) {
        std::cerr << "Unable to open file tests/add.txt";
        return;
    }

    std::string line;
    while (std::getline(inFile, line)) {
        std::istringstream iss(line);
        std::string num1, num2, expected;
        if (!(iss >> num1 >> num2 >> expected)) {
            std::cerr << "Error reading line: " << line << std::endl;
            continue; // skip this line
        }

        BigInt a(num1);
        BigInt b(num2);
         
        BigInt result = a / b;
        BigInt expectedBigInt(expected);

        if (result == expectedBigInt) {
        } else {
            std::cerr << "Test failed: " << num1 << " / " << num2 << " = " << result << ", expected " << expectedBigInt << std::endl;
            abort();
        }
    }

    std::cout << "\033[32mAll Test Passed!\033[0m" << std::endl;
}

int main() {
    // 测试框架
    add_tests();
    sub_tests();
    mul_tests();
    div_tests();
    return 0;
}
