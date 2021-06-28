#ifndef TM_H_
#define TM_H_

#include <vector>

class TM
{
    public:
        TM();
        void mv(int step=1);
        void inc(unsigned char val=1);
        void dec(unsigned char val=1);
        void input(unsigned char val);
        unsigned char output(void);
        bool iszero(void);
        
        void print(void);
    
    private:
        std::vector<unsigned char> tape;
        size_t ptr;
};

#endif
