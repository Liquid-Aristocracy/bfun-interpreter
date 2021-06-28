#include <iostream>
#include <cstring>
#include "TM.h"

TM::TM(void)
{
    tape.push_back(0);
    ptr = 0;
}

void TM::mv(int step /*=1*/)
{
    if (ptr + step <= 0)
    {
        ptr = 0;
    } else {
        ptr += step;
        while (ptr >= tape.size())
        {
            tape.push_back(0);
        }
    }
}

void TM::inc(unsigned char val /*=1*/)
{
    tape[ptr] += val;
}

void TM::dec(unsigned char val /*=1*/)
{
    tape[ptr] -= val;
}

void TM::input(unsigned char val)
{
    tape[ptr] = val;
}

unsigned char TM::output(void)
{
    return tape[ptr];
}

bool TM::iszero(void)
{
    if (tape[ptr] == 0)
    {
        return true;
    } else {
        return false;
    }
}

void TM::print(void)
{
    /* -0---1---2---3---
     * | 0 | 2 |245| 34|
     * ----------^---^--
     */
    
    for (size_t i = 0; i < tape.size(); i++)
    {
        std::cout << "-" << i << "--";
    }
    std::cout << "-" << std::endl;
    
    for (size_t i = 0; i < tape.size(); i++)
    {
        std::cout << "|";
        if (tape[i] / 100 == 0)
        {
            std::cout << ' ';
        }
        std::cout << (int) tape[i];
        if (tape[i] / 10 == 0)
        {
            std::cout << ' ';
        }
    }
    std::cout << "|" << std::endl;
    
    for (size_t i = 0; i < tape.size(); i++)
    {
        if (i == ptr) {
            std::cout << "--^-";
        } else {
            std::cout << "----";
        }
    }
    std::cout << "-" << std::endl;
}


