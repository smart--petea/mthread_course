/*
 * Задача простая - найти производную многочлена. Многочлен может быть большой, но только с положительными целыми
 * степенями и целыми коэффициентами. Подумайте, каким образом его лучше представить в памяти? Вывод должен быть без
 * пробелов и в порядке убывания степеней, хотя ввод этому правилу может не удовлетворять. Также на ввод может быть
 * подан многочлен с неприведенными слагаемыми.
 */

#include <iostream>
#include <sstream>
#include <map>

bool getSign(char& character, std::istream& is, int& sign)
{
    switch(character)
    {
        case '+':
            sign = 1;
            is.get(character);
            break;
        case '-':
            sign = -1;
            is.get(character);
            break;
        default:
            sign = 1;
            break;
    }

    return true;
}

bool getCoefficient(char& character, std::istream& is, int& coefficient)
{
    if(character >= '0' && character <= '9')
    {
        coefficient = 0;
        while(character >= '0' && character <= '9')
        {
            coefficient = coefficient * 10 + (character - '0');
            is.get(character);
        }

        if(character == '*') is.get(character); //?
    } else 
    {
        coefficient = 1;
    }

    return true;
}

bool preparePower(char& character, std::istream& is, int& power)
{
    if(character != 'x') 
    {
        power = 0;
        return false;
    }

    is.get(character);
    if(character != '^') 
    {
        power = 1;
        return false;
    }

    is.get(character);

    return true;
}


int main()
{
    char character;
    int pwr;
    int coefficient;
    int sign;
    std::map<int, int> polinom;

    character = std::cin.get();
    while(character != '\n')
    {

        getSign(character, std::cin, sign)
        &&
        getCoefficient(character, std::cin, coefficient)
        &&
        preparePower(character, std::cin, pwr)
        &&
        getCoefficient(character, std::cin, pwr);

        coefficient *= sign;

        polinom[pwr] += coefficient;
    }

    std::map<int, int>::iterator it = polinom.begin();
    std::map<int, int>::iterator end = polinom.end();
    bool isFirstLoop = true;
    for(;it != end; it++)
    {
        pwr = it->first - 1;
        coefficient = it->second * it->first;

        if(coefficient > 0) {
            if(isFirstLoop == false) std::cout << "+";
        } else if (coefficient < 0)
        {
            std::cout << "-";
        } else
        {
            continue;
        }

        std::cout << coefficient;
        if(pwr > 0) {
            std::cout << "*x";
            if(pwr > 1) std::cout << "^" << pwr;
        }

        isFirstLoop = false;
    }
    std::cout << std::endl;

    return 0;
}
