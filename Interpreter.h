#ifndef INTERPRETER_H_
#define INTERPRETER_H_

#include <cstring>
#include <vector>
#include "TM.h"

std::string escape(char c);

class Interpreter
{
    public:
        Interpreter(std::string c);
        void run(void);
        void runs(void);
        void printcode(void);
    
    private:
        TM tm;
        std::string code;
        size_t codeptr;
        
        std::string codebuf;
        std::string output;
        int currentstate;
        // 0: init/fin
        // 1: moving pointer (mv)
        // 2: cell accessing (ca)
        // 3: i/o
        // 4: handle loop
        // 5: pass (until meet ] )
        int mvstep;
        unsigned char caval;
        bool positive;
        
        std::vector<int> startpoints;
        
        void changestate(int nextstate);
        void changestates(int nextstate);
};

#endif
