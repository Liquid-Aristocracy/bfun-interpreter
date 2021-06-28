#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include "Interpreter.h"

void printusage()
{
    std::cout << "bfun - a brainfuck interpreter." << std::endl;
    std::cout << "Usage: bfun [FILE] {s|d[DEFAULT]}" << std::endl;
    std::cout << "where [FILE] is the bf source file." << std::endl;
    std::cout << "Mode -s: silent, mode -d: debug (default mode)." << std::endl;
}

int main (int argc, char *argv[])
{
    std::ifstream codefile;
    std::string code;
    bool silent = false;
    std::string arg;
    if (argc == 3)
    {
        std::string s (argv[2]);
        arg = s;
    }
    if (argc == 2 || (argc == 3 && (arg == "-s" || arg == "-d")))
    {
        try
        {
            codefile.open(argv[1], std::ios::in);
            std::stringstream buffer;
            buffer << codefile.rdbuf();
            code = buffer.str();
        } catch (std::ifstream::failure& e) {
            std::cout << "Exception opening/reading file " << argv[1] << "." << std::endl;
        }
        if (argc == 3 && arg == "-s")
        {
            silent = true;
        }
    } else {
        printusage();
        return 0;
    }
    
    Interpreter interpreter(code);
    if (silent)
    {
        interpreter.runs();
    } else {
        std::cout << argv[1] << ":" << std::endl;
        interpreter.printcode();
        interpreter.run();
    }
    
    codefile.close();
    return 0;
}


