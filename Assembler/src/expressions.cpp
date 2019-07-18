#include "expressions.h"
#include <iostream>
#include <string>
#include <map>
#include<cctype>
#include <sstream>
#include <vector>
#include <utility>
#include<algorithm>//to use transform
#include<FormatChecker.h>
#include<SymbolTable.h>
using namespace std;
expressions::expressions()
{
    //ctor
}
//return empty string if invalid
void expressions::evaluate(string operand)
{
    pair<string, string> firstnumber, secondnumber;
    FormatChecker formatChecker;
    if(formatChecker.isExpression(operand))
    {
        vector<string> expr = formatChecker.expression(operand);
        if(expr.size()== 3 && (expr[1]=="+" || expr[1]=="-" || expr[1]=="/" || expr[1]=="*"))
        {
            SymbolTable* symbolTable = SymbolTable::getInstance();
            map<string, string*> symTab = symbolTable->GetSymTab();
            if(expr[0].find_first_not_of("0123456789") != std::string::npos)  //first number is symbolic
            {
                cout<<"s :"<<expr[0]<<endl;
                std::transform(expr[0].begin(), expr[0].end(), expr[0].begin(), ::tolower);
                map<string, string*>::iterator i = symTab.find(expr[0]);
                if(i != symTab.end()) //symbol found
                {
                    firstnumber.first = i-> second[0];//address
                    firstnumber.second = i-> second[1];//type
                }
                else
                {
                    message = "undefined symbol in operand field";
                    this->address =  "";
                    return;
                }
            }
            else  //first number is numeric
            {
                cout<<"n : "<<expr[0]<<endl;
                firstnumber.first = expr[0];//address
                firstnumber.second = "A";//type
            }
            if(expr[2].find_first_not_of("0123456789") != std::string::npos)  //first number is symbolic
            {
                std::transform(expr[2].begin(), expr[2].end(), expr[2].begin(), ::tolower);
                map<string, string*>::iterator i = symTab.find(expr[2]);
                cout<<"s :"<<expr[2]<<endl;
                if(i != symTab.end()) //symbol found
                {
                    secondnumber.first = i-> second[0];//address
                    secondnumber.second = i-> second[1];//type
                }
                else
                {
                    message = "undefined symbol in operand field";
                    this->address =  "";
                    return;
                }
            }
            else  //second number is numeric
            {
                cout<<"n :"<<expr[2]<<endl;
                secondnumber.first = expr[2];//address
                secondnumber.second = "A";//type
            }
            cout<<"get value"<<endl;
            this->address =  getValue(firstnumber,secondnumber,expr[1]);
            return;
        }
        else
        {
            message = "undefined symbol in operand field";
            this->address =  "";
            return;
        }
    }
    else
    {
        message = "not Expression";
        this->address =  "";
        return;
    }

}
string expressions::getValue(pair<string,string>firstnumber,pair<string,string>secondnumber,string op)
{
    string type1,type2;
    type1 = firstnumber.second;
    type2 = secondnumber.second;
    string result;
    if(type1 == "R" && type2 == "R")
    {
        if(op == "-")
        {
            result = subtract(firstnumber.first, secondnumber.first);
        }
        else
        {
            message = "illegal expression in operand field";
            return "";
        }
    }
    else if(type1 == "A" && type2 == "A")
    {
        if(op == "-")
        {
            result = subtract(firstnumber.first, secondnumber.first);
        }
        else if(op == "+")
        {
            result = add(firstnumber.first, secondnumber.first);
        }
        else if(op == "*")
        {
            result = multiply(firstnumber.first, secondnumber.first);
        }
        else
        {
            result = divide(firstnumber.first, secondnumber.first);
        }
    }
    else  //one"R" & one"A"
    {
        if(op == "-")
        {
            result = subtract(firstnumber.first, secondnumber.first);
        }
        else if(op == "+")
        {
            result = add(firstnumber.first, secondnumber.first);
        }
        else
        {
            message = "illegal expression in operand field";
            return "";
        }
    }
    return result;
}

string expressions::add(string first, string second)
{
    std::stringstream str;
    int f = std::stoul(first, nullptr, 16);
    int s = std::stoul(second, nullptr, 16);
    std::stringstream ss;
    f = f+s;
    if(f < 0)
    {
        message = "Negative value for address or immediate data";
        return"";
    }
    ss << std::hex << f;
    return ss.str();
}
string expressions::subtract(string first, string second)
{
    std::stringstream str;
    int f = std::stoul(first, nullptr, 16);
    int s = std::stoul(second, nullptr, 16);
    std::stringstream ss;
    f = f-s;
    if(f < 0)
    {
        message = "Negative value for address or immediate data";
        return"";
    }
    ss << std::hex << f;
    return ss.str();
}
string expressions::multiply(string first, string second)
{
    std::stringstream str;
    int f = std::stoul(first, nullptr, 16);
    int s = std::stoul(second, nullptr, 16);
    std::stringstream ss;
    f = f*s;
    if(f < 0)
    {
        message = "Negative value for address or immediate data";
        return"";
    }
    ss << std::hex << f;
    return ss.str();
}
string expressions::divide(string first, string second)
{
    std::stringstream str;
    int f = std::stoul(first, nullptr, 16);
    int s = std::stoul(second, nullptr, 16);
    std::stringstream ss;
    f = f/s;
    if(f < 0)
    {
        message = "Negative value for address or immediate data";
        return"";
    }
    ss << std::hex << f;
    return ss.str();
}
string expressions::errorMessage()
{
    return message;
}

string expressions::getAddress()
{
    return this->address;
}

