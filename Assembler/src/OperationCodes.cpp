#include <iostream>
#include <map>
#include <string>
#include "OperationCodes.h"
#include<regex>
#include "FormatChecker.h"

using namespace std;
using namespace std::regex_constants;

string OperationCodes::assemblerDirectives[]= {"EQU","ORG","BASE","NOBASE","LTORG","START","END"};
string OperationCodes::storageDirectives[]= {"RESW","RESB","WORD","BYTE"};
//map for unimplemented codes

OperationCodes::OperationCodes()
{
    //ctor
    this->myMap["and"] = "3";
    this->myMap["or"] = "3";
    this->myMap["rmo"] = "2";
    this->myMap["clear"] = "1";
    this->myMap["lda"] = "3";
    this->myMap["ldx"] = "3";
    this->myMap["lds"] = "3";
    this->myMap["ldt"] = "3";
    this->myMap["ldb"] = "3";
    this->myMap["ldl"] = "3";
    this->myMap["sta"] = "3";
    this->myMap["stx"] = "3";
    this->myMap["stb"] = "3";
    this->myMap["stl"] = "3";
    this->myMap["sts"] = "3";
    this->myMap["stt"] = "3";
    this->myMap["ldch"] = "3";
    this->myMap["stch"] = "3";
    this->myMap["add"] = "3";
    this->myMap["sub"] = "3";
    this->myMap["mul"] = "3";
    this->myMap["div"] = "3";
    this->myMap["addr"] = "2";
    this->myMap["subr"] = "2";
    this->myMap["mulr"] = "2";
    this->myMap["divr"] = "2";
    this->myMap["compr"] = "2";
    this->myMap["comp"] = "3";
    this->myMap["j"] = "3";
    this->myMap["jeq"] = "3";
    this->myMap["jlt"] = "3";
    this->myMap["jgt"] = "3";
    this->myMap["tix"] = "3";
    this->myMap["tixr"] = "1";
    this->myMap["jsub"] = "3";
    this->myMap["rsub"] = "";
    this->myMap["td"] = "3";
    this->myMap["rd"] = "3";
    this->myMap["wd"] = "3";
}

OperationCodes::~OperationCodes()
{
    //dtor
}

map<string, string> OperationCodes::getMap()
{
    //string n = myMap["wd"];
    return this->myMap;
}

bool OperationCodes::check(std::string opcode, std::string operand)
{
    FormatChecker formatChecker;
    string s = opcode;
    std::transform(s.begin(), s.end(), s.begin(), ::toupper);
    for (int i = 0; i < 4; i++)
    {
        if (s == storageDirectives[i])
        {
            return formatChecker.storageDirectives(opcode, operand);
        }
    }
    std::transform(opcode.begin(), opcode.end(), opcode.begin(), ::tolower);
    std::transform(operand.begin(), operand.end(), operand.begin(), ::tolower);
    cout<< opcode;
    if (opcode.at(0) == '+') {
        opcode = opcode.substr (1,opcode.length());
        //cout<< opcode;
    }
    string format = OperationCodes::getMap()[opcode];
    if (format.compare("2") == 0)
    {
        return formatChecker.formatTwo(operand);
    }
    else if (format.compare("3") == 0)
    {
        return formatChecker.formatThree(operand);
    }
    else if (format.compare("1") == 0)
    {
        std::smatch m;
        std::regex e("(A|X|B|T|S|L)", ECMAScript | icase);
        if (std::regex_search(operand,m,e))
        {
            return true;
        }
        return false;
    }
    else if (format.compare("") == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool OperationCodes::isAssemblerDirective(string opCode)
{
    string s = opCode;
    std::transform(s.begin(), s.end(), s.begin(), ::toupper);
    for (int i = 0; i < 7; i++)
    {
        if (s == assemblerDirectives[i])
        {
            return true;
        }
    }
    return false;
}
