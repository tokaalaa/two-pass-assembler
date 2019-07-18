#include "Parser.h"
#include "Line.h"
#include<regex>
#include<string>
#include <iostream>
#include"Address.h"
#include "OperationCodes.h"
#include"Output.h"
#include"Program.h"
#include"SymbolTable.h"

using namespace std;
using namespace std::regex_constants;

/*string Parser::opCodes[] = {"ADD", "ADDF", "ADDR", "AND", "CLEAR", "COMP", "COMPF", "COMPR","DIV","DIVF","DIVR",
            "J","JEQ","JLT","JGT","JSUB","LDA","LDB","LDCH","LDF","LDL","LDS","LDT","LDT","LDX","LPS","MUL","MULF",
            "MULR","OR","RD","RMO","SHIFTL","SHIFTR","SSK","STA","STB","STCH","STF","STI","STL","STS","STSW",
            "STT","STX","SUB","SUBF","SUBR","SVC","TD","TIX","TIXR","WD"};*/
string Parser:: noOperanCodes[] = {"FIX","FLOAT","HIO","NORM","RSUB","SIO","TIO"};

Parser::Parser()
{
    //ctor
}
Parser::~Parser()
{
    //dtor
}

bool Parser::parseStart(std::string line)
{
    std::smatch m1;
    std::regex e1("\\s+\\.", ECMAScript | icase);
    if(std::regex_search(line,m1,e1))
    {
        string c = m1.suffix().str();
        line = m1.prefix().str();
        l.setComment(c);
    }
    std::smatch m;
    std::regex e("(\\s+)?start(\\s+)?", ECMAScript | icase);
    if(std::regex_search(line,m,e))
    {
        string x = m.prefix().str();
        string o = m.suffix().str();
        l.setLabel(x);
        l.setOpCode("start");
        std::smatch m2;
        std::regex e2("[0-9A-F]+", ECMAScript | icase);
        if (std::regex_search(o,m2,e2))
        {
            int x = m2.length();
            int y = o.length();
            if(x==y && x <= 4)
            {
                l.setOperand(o);
            }
        }
        l.executer();
        l.write();
        return true;
    }
    else
    {

        return false;
    }
}

void Parser::parseEnd(std::string line)
{
    l.setInput(line);
    std::smatch m;
    std::regex e("\\w+\\s+end", ECMAScript | icase);
    if(std::regex_search(line,m,e))
    {
        if (m.position() == 0)
        {
            l.setErrorMessage("Illegal label before END ...");
            l.setErrorFound(true);
            l.write();
            return;
        }
    }
    std::smatch m1;
    std::regex e1("end(\\s+)?", ECMAScript | icase);
    if(std::regex_search(line,m1,e1))
    {
        string oc = line.substr(m1.position(), 3);
        l.setOpCode(oc);
        line = m1.suffix().str();
    }
    int len = line.length();
    if (len > 0)
    {
        if (line.at(0) == '.')
        {
            l.setComment(line);
            l.executer();
            l.write();
            return;
        }
        std::smatch m2;
        std::regex e2("\\s+\\.?", ECMAScript | icase);
        if(std::regex_search(line,m2,e2))
        {
            string c = m2.suffix().str();
            string o = m2.prefix().str();
            l.setComment(c);
            l.setOperand(o);
        } else {
            l.setOperand(line);
        }
    }
    l.executer();
    l.write();
}

void Parser::parseLine(std::string line)
{
    l.setInput(line);
    if (line.at(0) == '.')
    {
        l.setComment(line);
        l.setCommentLine(true);
        l.write();
        return;
    }
    std::smatch m1;
    std::regex e1("\\s+\\.", ECMAScript | icase);
    if(std::regex_search(line,m1,e1))
    {
        string c = m1.suffix().str();
        line = m1.prefix().str();
        l.setComment(c);
    }
    int i = 0;
    string word;
    std::smatch m;
    std::regex e("\\s+", ECMAScript | icase);
    while(std::regex_search(line,m,e))
    {
        int x = m.position();
        if (x != 0)
        {
            word = line.substr(0,x);
            switch (i)
            {
            case 0:
                if(noOperand(word))
                {
                    l.setOpCode(word);
                    i = 2;
                }
                else if (opCode(word))
                {
                    l.setOpCode(word);
                    i = 1;
                }
                else
                {
                    l.setLabel(word);
                }
                break;
            case 1:
                if(noOperand(word))
                {
                    l.setOpCode(word);
                    i = 2;
                }
                else if (opCode(word))
                {
                    l.setOpCode(word);
                }
                else
                {
                    l.setErrorMessage("Undefined opCode ..");
                    l.setErrorFound(true);
                    l.write();
                    return;
                }
                break;
            case 2:
                l.setOperand(word);
                break;
            }
            i++;
            if (i == 3)
            {
                l.executer();
                l.write();
                return;
            }
        }
        line = m.suffix().str();
    }
    if (i < 3)
    {
        switch (i)
        {
        case 0:
            if(noOperand(line) || opCode(line))
            {
                l.setOpCode(line);
            }
            else
            {
                l.setErrorFound(true);
                l.setErrorMessage("\tNo Operation Code ..");
                l.write();
                return;
            }
            break;
        case 1:
            if(noOperand(line) || opCode(line))
            {
                l.setOpCode(line);
            }
            else
            {
                l.setErrorMessage("\tUndefined operation code ..");
                l.setErrorFound(true);
                l.write();
                return;
            }
            break;
        case 2:
            l.setOperand(line);
        }
    }
    l.executer();
    l.write();
}

bool Parser::opCode(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    if (s.at(0) == '+') {
        s = s.substr (1,s.length());
    }
    OperationCodes operationCodes;
    map<string, string> codeMap = operationCodes.getMap();
    map<string,string>::iterator it = codeMap.find(s);
    if(it != codeMap.end())
    {
        return true;
    }
    std::transform(s.begin(), s.end(), s.begin(), ::toupper);
    for (int i = 0; i < 5; i++)
    {
        if (s == operationCodes.assemblerDirectives[i])
        {
            return true;
        }
    }
    for (int i = 0; i < 4; i++)
    {
        if (s == operationCodes.storageDirectives[i])
        {
            return true;
        }
    }
    return false;
}

bool Parser::noOperand(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(), ::toupper);
    if (s == "NOBASE" || s=="LTORG")
    {
        return true;
    }
    return false;
}
