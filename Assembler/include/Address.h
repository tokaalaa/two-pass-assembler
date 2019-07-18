#include<string>
#ifndef ADDRESS_H
#define ADDRESS_H

using namespace std;

class Address
{
    public:
        Address();
        virtual ~Address();
        std::string getAddress();
        void updateCounter(std::string opCode, std::string operand);
        void startCounter(std::string operand);

    protected:

    private:
        string opCode, operand, address;
        int counAdd;
};

#endif // ADDRESS_H
