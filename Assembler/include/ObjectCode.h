#ifndef OBJECTCODE_H
#define OBJECTCODE_H
#include "OperationCodes.h"
#include "SymbolTable.h"
#include "LiteralTable.h"
#include <map>
#include <string>
#include <iostream>
#include"Program.h"
#include "expressions.h"
#include "FormatChecker.h"

using namespace std;

class ObjectCode
{
    public:
        ObjectCode();
        virtual ~ObjectCode();
        map<string, string> getObjectMap();
        string getObjectCode();
        void calculateObjectCode(std::string address,std::string opCode, std::string operand);
        string convHexToBinary(std::string address);
        string convBinaryToHex(std::string address);
        void objectOrdinary(std::string address,std::string opCode, std::string operand, std::string orginal);
        string checkPCorB(std::string address, std::string operand);
        bool is_number(const std::string& s);
        bool existError();
        string geterror();
    protected:

    private:
        map<string, string> objectOfOPCode;
        string objectCode, operand, opCode;
        bool error = false;
        string err;
        expressions exp;
        FormatChecker checkexp;
};

#endif // OBJECTCODE_H
