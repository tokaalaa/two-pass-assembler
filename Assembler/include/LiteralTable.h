#ifndef LITERALTABLE_H
#define LITERALTABLE_H
#include <string>
#include<map>
#include <iostream>


class LiteralTable
{
    public:
        LiteralTable();
        static LiteralTable *getInstance();
        //virtual ~LiteralTable();
        void addLiteral(std::string literal);
        std::string setLiterals(std::string address);
        std::string getLiterals();
        std::string literals;
        std::map <std::string ,std::string*> getlitTbl();
    protected:

    private:
        static LiteralTable *instance;
        std::map <std::string ,std::string*> litTbl;
        std::string ASCIItoHEX(std::string ascii);
};

#endif // LITERALTABLE_H
