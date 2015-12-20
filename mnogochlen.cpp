/*
 * Задача простая - найти производную многочлена. Многочлен может быть большой, но только с положительными целыми
 * степенями и целыми коэффициентами. Подумайте, каким образом его лучше представить в памяти? Вывод должен быть без
 * пробелов и в порядке убывания степеней, хотя ввод этому правилу может не удовлетворять. Также на ввод может быть
 * подан многочлен с неприведенными слагаемыми.
 */

#include <iostream>
#include <sstream>

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
            return true;
    }
}

int main()
{
    int character;
    int power;
    int coefficient;

    power = 1;
    coefficient = 1;
    while((character = std::cin.get()) != '=')
    {
 //       coefficient = getCoeficient();
    }

    return 0;
}
