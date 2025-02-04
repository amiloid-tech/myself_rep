#include <iostream>
#include <string>
#include <algorithm>
#include <cmath>


int toDecimal(const std::string& number, int base) {
    int decimal = 0;
    int power = 1;
    for (int i = number.length() - 1; i >= 0; --i) {
        char ch = number[i];
        int value;
        if (ch >= '0' && ch <= '9') {
            value = ch - '0';
        } else if (ch >= 'A' && ch <= 'Z') {
            value = ch - 'A' + 10;
        } else if (ch >= 'a' && ch <= 'z') {
            value = ch - 'a' + 10;
        } else {
            std::cerr << "Некорректный символ в числе: " << ch << std::endl;
            return -1;
        }
        if (value >= base) {
            std::cerr << "Некорректное число для системы с основанием " << base << std::endl;
            return -1;
        }
        decimal += value * power;
        power *= base;
    }
    return decimal;
}


std::string fromDecimal(int decimal, int base) {
    if (decimal == 0) {
        return "0";
    }
    std::string result;
    while (decimal > 0) {
        int remainder = decimal % base;
        char ch;
        if (remainder < 10) {
            ch = '0' + remainder;
        } else {
            ch = 'A' + (remainder - 10);
        }
        result.push_back(ch);
        decimal /= base;
    }
    std::reverse(result.begin(), result.end());
    return result;
}

int main() {
    std::string number;
    int fromBase, toBase;

    
    std::cout << "Введите число: ";
    std::cin >> number;
    std::cout << "Введите исходную систему счисления (2-36): ";
    std::cin >> fromBase;
    std::cout << "Введите целевую систему счисления (2-36): ";
    std::cin >> toBase;

    
    if (fromBase < 2 || fromBase > 36 || toBase < 2 || toBase > 36) {
        std::cerr << "Некорректная система счисления. Допустимый диапазон: 2-36." << std::endl;
        return 1;
    }

    
    int decimal = toDecimal(number, fromBase);
    if (decimal == -1) {
        return 1;
    }

    
    std::string result = fromDecimal(decimal, toBase);

    
    std::cout << "Результат: " << result << std::endl;

    return 0;
}