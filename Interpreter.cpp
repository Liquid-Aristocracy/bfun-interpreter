#include <iostream>
#include "Interpreter.h"

std::string escape(char c)
{
    std::string ret;
    if(c < ' ')
    {
        ret.push_back('^');
        ret.push_back(c + '@');
    }
    else if(c == '\x7F')
        ret.append("^?");
    else
        ret.push_back(c);
    return ret;
}

void Interpreter::printcode(void)
{
    std::cout << code << std::endl;
}

void Interpreter::changestate(int nextstate)
{
    std::cout << "*****";
    if (startpoints.size() > 0)
    {
        std::cout << " layer " << startpoints.size();
    }
    std::cout << std::endl;
    switch (currentstate)
    {
        case 0: // init/fin
            std::cout << "Init the machine." << std::endl;
            std::cout << "Init the tape." << std::endl;
            break;
        case 1: // mv
            std::cout << "Code: " << codebuf << std::endl;
            std::cout << "Move to relative position " << mvstep << "." << std::endl;
            tm.mv(mvstep);
            mvstep = 0;
            break;
        case 2: // ca
            std::cout << "Code: " << codebuf << std::endl;
            if (positive)
            {
                std::cout << "Increase " << (int) caval << " to current cell." << std::endl;
                tm.inc(caval);
            } else {
                std::cout << "Decrease " << (int) caval << " from current cell." << std::endl;
                tm.dec(caval);
            }
            caval = 0;
            positive = true;
            break;
        case 3: // i/o
            std::cout << "Code: " << codebuf << std::endl;
            if (codebuf == ",") // i
            {
                std::cout << "Get input from user." << std::endl;
                char userin[1];
                std::cout << "Please input a character to store in current cell." << std::endl;
                std::cout << "If you prefer to input an ASCII code, press enter." << std::endl;
                std::cout << "Character: ";
                std::cin.get(userin, 1);
                if (userin[0] == '\n')
                {
                    int asciicode;
                    std::cout << "Ascii code: ";
                    std::cin >> asciicode;
                    userin[0] = (char) asciicode;
                }
                std::string chartoprint;
                chartoprint = escape(userin[0]);
                std::cout << "Got character: " << chartoprint << " with ASCII code " << (int) userin[0] << "." << std::endl;
                tm.input((unsigned char) userin[0]);
            } else if (codebuf == ".") // o
            {
                unsigned char out;
                out = tm.output();
                output.push_back((char) out);
                std::string chartoprint;
                chartoprint = escape((char) out);
                std::cout << "Output " << chartoprint << " is stored in the output string." << std::endl;
            }
            break;
        case 4: // loop
            std::cout << "Code: " << codebuf << std::endl;
            if (codebuf == "[") // start
            {
                startpoints.push_back(codeptr - 2);
                if (!tm.iszero())
                {
                    std::cout << "Entering layer. Current layer is " << startpoints.size() << "." << std::endl;
                } else {
                    std::cout << "Pass to the next ] in layer " << startpoints.size() << "."  << std::endl;
                    nextstate = 5;
                }
            } else if (codebuf == "]") // end
            {
                if (!tm.iszero())
                {
                    codeptr = startpoints.back();
                    startpoints.pop_back();
                    std::cout << "Return to the matching [." << std::endl;
                    nextstate = 4;
                } else {
                    startpoints.pop_back();
                    std::cout << "Exiting layer. Current layer is " << startpoints.size() << std::endl;
                }
            }
            mvstep = 0;
            caval = 0;
            positive = true;
            break;
        case 5: // pass
        default:
            break;
    }
    std::cout << "Current tape state: " << std::endl;
    tm.print();
    codebuf.clear();
    currentstate = nextstate;
    return;
}

void Interpreter::run()
{
    while (codeptr < code.length() - 1)
    {
        char c = code[codeptr];
        if (currentstate == 1 && (c == '>' || c == '<'))
        {
            codebuf.push_back(c);
            if (c == '>')
            {
                mvstep ++;
            } else {
                mvstep --;
            }
        } else if (currentstate == 2 && (c == '+' || c == '-'))
        {
            codebuf.push_back(c);
            if (positive)
            {
                if (c == '+')
                {
                    caval ++;
                } else {
                    if (caval > 0)
                    {
                        caval --;
                    } else {
                        caval = 1;
                        positive = false;
                    }
                }
            } else {
                if (c == '-')
                {
                    caval ++;
                } else {
                    if (caval > 0)
                    {
                        caval --;
                    } else {
                        caval = 1;
                        positive = true;
                    }
                }
            }
        } else if (currentstate == 5)
        {
            std::vector<size_t> passed;
            while (code[codeptr] != ']' && !passed.empty())
            {
                if (code[codeptr] == '[')
                {
                    passed.push_back(codeptr);
                } else if (code[codeptr] == '[')
                {
                    passed.pop_back();
                }
                codeptr ++;
            }
            c = code[codeptr];
            codebuf.push_back(c);
            changestate(4);
        } else {
            switch (c) {
                case '>':
                    changestate(1);
                    codebuf.push_back(c);
                    mvstep ++;
                    break;
                case '<':
                    changestate(1);
                    codebuf.push_back(c);
                    mvstep --;
                    break;
                case '+':
                    changestate(2);
                    codebuf.push_back(c);
                    caval ++;
                    break;
                case '-':
                    changestate(2);
                    codebuf.push_back(c);
                    caval ++;
                    positive = false;
                    break;
                case ',':
                case '.':
                    changestate(3);
                    codebuf.push_back(c);
                    break;
                case '[':
                case ']':
                    changestate(4);
                    codebuf.push_back(code[codeptr]);
                    break;
                default:
                    break;
            }
        }
        codeptr ++;
    }
    changestate(0);
    std::cout << "*****" << std::endl;
    std::cout << "Output string: " << output << std::endl;
}

void Interpreter::changestates(int nextstate)
{
    switch (currentstate)
    {
        case 0: // init/fin
            break;
        case 1: // mv
            tm.mv(mvstep);
            mvstep = 0;
            break;
        case 2: // ca
            if (positive)
            {
                tm.inc(caval);
            } else {
                tm.dec(caval);
            }
            caval = 0;
            positive = true;
            break;
        case 3: // i/o
            if (codebuf == ",") // i
            {
                char userin[1];
                std::cout << "Input character (enter for ASCII mode): ";
                std::cin.get(userin, 1);
                if (userin[0] == '\n')
                {
                    int asciicode;
                    std::cout << "Input ASCII code: ";
                    std::cin >> asciicode;
                    userin[0] = (char) asciicode;
                }
                std::string chartoprint;
                chartoprint = escape(userin[0]);
                std::cout << "Got character: " << chartoprint << " with ASCII code " << (int) userin[0] << "." << std::endl;
                tm.input((unsigned char) userin[0]);
            } else if (codebuf == ".") // o
            {
                unsigned char out;
                out = tm.output();
                output.push_back((char) out);
                std::cout << (char) out;
            }
            break;
        case 4: // loop
            if (codebuf == "[") // start
            {
                startpoints.push_back(codeptr - 2);
                if (!tm.iszero())
                {
                } else {
                    nextstate = 5;
                }
            } else if (codebuf == "]") // end
            {
                if (!tm.iszero())
                {
                    codeptr = startpoints.back();
                    startpoints.pop_back();
                    nextstate = 4;
                } else {
                    startpoints.pop_back();
                }
            }
            mvstep = 0;
            caval = 0;
            positive = true;
            break;
        case 5: // pass
        default:
            break;
    }
    codebuf.clear();
    currentstate = nextstate;
    return;
}

void Interpreter::runs()
{
    while (codeptr < code.length() - 1)
    {
        char c = code[codeptr];
        if (currentstate == 1 && (c == '>' || c == '<'))
        {
            codebuf.push_back(c);
            if (c == '>')
            {
                mvstep ++;
            } else {
                mvstep --;
            }
        } else if (currentstate == 2 && (c == '+' || c == '-'))
        {
            codebuf.push_back(c);
            if (positive)
            {
                if (c == '+')
                {
                    caval ++;
                } else {
                    if (caval > 0)
                    {
                        caval --;
                    } else {
                        caval = 1;
                        positive = false;
                    }
                }
            } else {
                if (c == '-')
                {
                    caval ++;
                } else {
                    if (caval > 0)
                    {
                        caval --;
                    } else {
                        caval = 1;
                        positive = true;
                    }
                }
            }
        } else if (currentstate == 5)
        {
            std::vector<size_t> passed;
            while (code[codeptr] != ']' && !passed.empty())
            {
                if (code[codeptr] == '[')
                {
                    passed.push_back(codeptr);
                } else if (code[codeptr] == '[')
                {
                    passed.pop_back();
                }
                codeptr ++;
            }
            c = code[codeptr];
            codebuf.push_back(c);
            changestates(4);
        } else {
            switch (c) {
                case '>':
                    changestates(1);
                    codebuf.push_back(c);
                    mvstep ++;
                    break;
                case '<':
                    changestates(1);
                    codebuf.push_back(c);
                    mvstep --;
                    break;
                case '+':
                    changestates(2);
                    codebuf.push_back(c);
                    caval ++;
                    break;
                case '-':
                    changestates(2);
                    codebuf.push_back(c);
                    caval ++;
                    positive = false;
                    break;
                case ',':
                case '.':
                    changestates(3);
                    codebuf.push_back(c);
                    break;
                case '[':
                case ']':
                    changestates(4);
                    codebuf.push_back(code[codeptr]);
                    break;
                default:
                    break;
            }
        }
        codeptr ++;
    }
    changestates(0);
}
