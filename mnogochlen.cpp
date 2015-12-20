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
            break;
    }
    return true;
}

bool getCoefficient(char& character, std::istream& is, int& coefficient)
{
    coefficient = 0;
    while(character >= '0' && character <= '9')
    {
        coefficient = coefficient * 10 + character;
    }

    return true;
}

bool preparePower(char& character, std::istream& is)
{
    if(character != '*') return false;

    is.get(character);
    if(character != 'x') return false;

    is.get(character);
    if(character != '^') return false;

    is.get(character);
    return true;
}


int main()
{
    char character;
    int power;
    int coefficient;
    int sign;
    std::map<int, int> polinom;

    power = 1;
    coefficient = 1;
    while((character = std::cin.get()) != '=')
    {
        getSign(character, std::cin, sign)
        &&
        getCoefficient(character, std::cin, coefficient)
        &&
        preparePower(character, std::cin)
        &&
        getCoefficient(character, std::cin, power);

        coefficient *= sign;

        polinom[power] += coefficient;

    }

    return 0;
}
