#include "FormatChecker.h"
#include<regex>
#include<vector>
#include <iostream>
#include <algorithm>
#include <string>


using namespace std;
using namespace std::regex_constants;

FormatChecker::FormatChecker()
{
    //ctor
}

FormatChecker::~FormatChecker()
{
    //dtor
}

bool FormatChecker::formatTwo(std::string operand)
{
    std::smatch m;
    std::regex e("(A|X|B|T|S|L),(A|X|B|T|S|L)", ECMAScript | icase);
    if (std::regex_search(operand,m,e))
    {
        return true;
    }
    return false;
}
bool FormatChecker::formatThree(std::string operand)
{
    if (isExpression(operand)) {
        return true;
    }
    if (operand == "*")
    {
        return true;
    }
    std::smatch m;
    std::regex e("((=)?(C'(\\w|\\W)+')|(X'[A-F0-9]'))|((=)?W'(-)\\d+')", ECMAScript | icase);
    if (std::regex_search(operand,m,e))
    {
        int x = m.length();
        int y = operand.length();
        if(x==y)
        {
            return true;
        }
    }
    std::smatch m1;
    std::regex e1("((#)?(-)?[0-9]+)|((@)?[0-9A-F]+)|([0-9A-F]+(,X)?)", ECMAScript | icase);
    if (std::regex_search(operand,m1,e1))
    {
        int x = m1.length();
        int y = operand.length();
        if(x==y)
        {
            if (x <= 4)
                return true;
            if(x == 5 && operand.at(0) == '#' || operand.at(0) == '@')
                return true;
            if(x == 6 && operand.at(x-2)==',')
                return true;
            if (operand.at(0) == '#' || operand.at(0) == '@')
            {
                if (operand.at(1) >= '0' && operand.at(1) <= '9')
                {
                    return false;
                }
            }
            else if (operand.at(0) >= '0' && operand.at(0) <= '9')
            {
                return false;
            }
        }

    }
    std::smatch m2;
    std::regex e2("(\\w+(,X)?)|((#)\\w+)|((@)\\w+)", ECMAScript | icase);
    if (std::regex_search(operand,m2,e2))
    {
        int x = m2.length();
        int y = operand.length();
        if (x == y)
        {
            if (operand.at(0) == '#' || operand.at(0) == '@')
            {
                if (!(operand.at(1) >= 'a' && operand.at(1) <= 'z') && !(operand.at(1) >= 'A' && operand.at(1) <= 'Z'))
                {
                    return false;
                }
            }
            else if (!(operand.at(0) >= 'a' && operand.at(0) <= 'z') && !(operand.at(0) >= 'A' && operand.at(0) <= 'Z'))
            {
                return false;
            }
            return true;
        }

    }
    return false;
}

bool FormatChecker::validLabel(std::string label)
{
    std::smatch m;
    std::regex e("([a-z]|[A-Z])\\w+", ECMAScript | icase);
    if (std::regex_search(label,m,e))
    {
        int x = m.length();
        int y = label.length();
        if (x==y)
        {
            return true;
        }
    }
    return false;
}

vector<string> FormatChecker::expression(std::string operand)
{
    vector<string> out;
    if (isExpression(operand))
    {
        bool con = true;
        while(con)
        {
            string n3;
            std::smatch m;
            std::regex all("[/+-]|\\(|\\)", ECMAScript | icase);
            if (std::regex_search(operand,m,all))
            {
                out.push_back(m.prefix().str());
                // cout << m.prefix().str();
                //cout << "\n";
                out.push_back(m.str());
                //cout << m.str();
                // cout << "\n";
                n3 = m.suffix().str();
            }
            if(n3.find('+') != std::string::npos
                    || n3.find('-')!= std::string::npos
                    || n3.find('*')!= std::string::npos
                    || n3.find('/')!= std::string::npos
                    || n3.find('(')!= std::string::npos
                    || n3.find(')')!= std::string::npos)
            {
                operand = n3;
            }
            else
            {
                //cout << n3;
                out.push_back(n3);
                con = false;
            }
        }
        //cout << "enter";
    }
    return out;

//nfok el expression
}

bool FormatChecker::isExpression(std::string operand)
{
    cout << operand;
    std::smatch m;
    std::regex add("[+]", ECMAScript | icase);
    std::regex sub("[-]", ECMAScript | icase);
    std::regex div("[/]", ECMAScript | icase);
    std::regex all("[/+-]", ECMAScript | icase);

    if (operand.at(0)=='+' ||
            operand.at(0)=='-' ||
            operand.at(0)=='/')
    {
        //cout << "false1";
        return false;
    }
    if (operand.at(0)=='*' && operand.length() == 1)
    {
        // cout << "false2";
        return false;
    }
    else if (operand.at(0)=='*' && operand.at(1) != '(')
    {
        //cout << "false22";
        return false;
    }
    if (std::regex_search(operand,m,add))
    {
        string before = m.suffix().str();
        string after = m.prefix().str();
        if(before.empty() || after.empty())
        {
            //cout << "false3";
            return false;
        }
    }
    if (std::regex_search(operand,m,sub))
    {
        string before = m.suffix().str();
        string after = m.prefix().str();
        if(before.empty() || after.empty())
        {
            //cout << "false4";
            return false;
        }
    }
    if (std::regex_search(operand,m,div))
    {
        string before = m.suffix().str();
        string after = m.prefix().str();
        if(before.empty() || after.empty())
        {
            //cout << "false5";
            return false;
        }
    }
    if (std::regex_search(operand,m,all))
    {
        string before = m.suffix().str();
        string after = m.prefix().str();
        if(!before.empty() && !after.empty())
        {
            //cout << "true";
            return true;
        }
    }
    else
    {
        //cout << "falseend";
        return false;
    }

}

bool FormatChecker::noLabelCode(string opCode)
{
    std::transform(opCode.begin(), opCode.end(), opCode.begin(), ::toupper);
    if (opCode == "BASE" || opCode == "NOBASE" || opCode =="ORG" || opCode == "LTORG")
    {
        return true;
    }
    return false;
}

bool FormatChecker::storageDirectives(string opCode, string operand)
{
    std::transform(opCode.begin(), opCode.end(), opCode.begin(), ::toupper);
    if (opCode == "RESW" || opCode == "RESB" || opCode == "WORD")
    {
        std::smatch m;
        std::regex e("\\d+", ECMAScript | icase);
        if (std::regex_search(operand,m,e))
        {
            int x = m.length();
            int y = operand.length();
            if (x==y)
            {
                return true;
            }
        }
        return false;
    }
    else if (opCode == "BYTE")
    {
        std::smatch m;
        std::regex e("((C'(\\w|\\W)+')|(X'[A-F0-9]+'))", ECMAScript | icase);
        if (std::regex_search(operand,m,e))
        {
            int x = m.length();
            int y = operand.length();
            if (x==y)
            {
                return true;
            }
        }
        return false;
    }
    return false;
}
