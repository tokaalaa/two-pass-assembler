#include "SymbolTable.h"
#include <iostream>
#include <string>
#include <map>
#include<cctype>
#include <sstream>
#include <vector>
#include <utility>
#include<algorithm>//to use transform
using namespace std;
SymbolTable *SymbolTable::instance=0;
SymbolTable::SymbolTable()
{
    //ctor
}
SymbolTable *SymbolTable::getInstance()
{
    if (instance==0)
    {
        instance = new SymbolTable();
    }
    return instance;
}
map<string, string*> SymbolTable::GetSymTab()
{
    return symTab;
}
string SymbolTable::generateError(string symbol)
{
    return "symbol '" + symbol +"' is already defined";
}
bool SymbolTable::checkValidName(string symbol)
{
    if(isdigit(symbol[0]))
    {
        return false;
        //contain special characters
    }
    else if(symbol.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_") != std::string::npos)
    {
        return false;
    }
    return true;
}
string SymbolTable::addlabel(string symbol, string address)
{
    std::transform(symbol.begin(), symbol.end(), symbol.begin(), ::tolower);
    string message = "";
    if(!checkValidName(symbol))
    {
        message = "Invalid Label Name";
        return message;
    }
    else
    {
        map<string, string*>::iterator i = symTab.find(symbol);
        if(i == symTab.end()) //label not found
        {
            string* s = new string[3] {address,"R","NoError"};
            symTab[symbol] = s;
        }
        else
        {
            symTab[symbol] = new string[3] {i->second[0],"R","Error"};;
            message = generateError(symbol);
        }
    }
    return message;
}
string SymbolTable::addequ(string symbol,vector<string> operand)
{
    std::transform(symbol.begin(), symbol.end(), symbol.begin(), ::tolower);
    if(!checkValidName(symbol))
    {
        return "Invalid Label Name";
    }
    map<string, string*>::iterator i = symTab.find(symbol);
    if(i != symTab.end())
    {
        return generateError(symbol);
    }
    string p = checkEquOperand(symbol,operand);
    return p;
}
string SymbolTable::checkEquOperand(string symbol,vector<string> operand)
{
    if(operand.size() == 1) //one symbol or one number
    {
        if(operand[0].find_first_not_of("0123456789") == std::string::npos) //not symbolic operand
        {
            string* s = new string[3] {operand[0],"A","NoError"};
            symTab[symbol] = s;
            return "";
        }
        std::transform(operand[0].begin(), operand[0].end(), operand[0].begin(), ::tolower);
        map<string, string*>::iterator i = symTab.find(operand[0]);
        if(i != symTab.end()) //symbol found
        {
            string* s = new string[3] {i->second[0],i->second[1],"NoError"};
            symTab[symbol] = s;
            return "";
        }
        else
        {
            return "undefined symbol in operand field";
        }
    }
    else
    {
        return checkExpressionOperand(symbol,operand);
    }

}
string SymbolTable::checkExpressionOperand(string symbol, vector<string> operand)
{
    pair<string, string> firstnumber, secondnumber;
    if(operand.size() != 3)
    {
        return "extra characters in operand field";
    }
    else
    {
        if(operand[1] == "+" || operand[1] =="-") //check operator
        {
            if(operand[0].find_first_not_of("0123456789") != std::string::npos)  //first number is symbolic
            {
                std::transform(operand[0].begin(), operand[0].end(), operand[0].begin(), ::tolower);
                map<string, string*>::iterator i = symTab.find(operand[0]);
                if(i != symTab.end()) //symbol found
                {
                    firstnumber.first = i-> second[0];
                    firstnumber.second = i-> second[1];
                }
                else
                {
                    return "undefined symbol in operand field";
                }
                if(operand[2].find_first_not_of("0123456789") != std::string::npos)  //second number is symbolic
                {
                    std::transform(operand[2].begin(), operand[2].end(), operand[2].begin(), ::tolower);
                    map<string, string*>::iterator i = symTab.find(operand[2]);
                    if(i != symTab.end()) //symbol found
                    {
                        secondnumber.first = i-> second[0];
                        secondnumber.second = i-> second[1];
                    }
                    else  //symbol not found
                    {
                        return "undefined symbol in operand field";
                    }
                }
                else  //second number is not symbolic
                {
                    secondnumber.first = operand[2];
                    secondnumber.second = "A";
                }
                return evaluateExpression(symbol,firstnumber,secondnumber,operand[1]);
            }
            else return "undefined symbol in operand field";
        }
        else
        {
            return "undefined symbol in operand field";
        }
    }
}
string SymbolTable::evaluateExpression(string symbol, pair<string, string> firstnumber,pair<string, string> secondnumber,string op)
{
    string type1 = firstnumber.second;
    string type2 = secondnumber.second;
    string newtype,value;
    if(type1 == "R" && type2 =="R")
    {
        if(op == "+")
        {
            return "illegal address expression in operand field";
        }
        else
        {
            newtype = "A";
            value = subtract(firstnumber.first, secondnumber.first);
        }
    }
    else if(type1 == "A" && type2 == "R")
    {
        if(op == "-")
        {
            return "illegal address expression in operand field";
        }
        else
        {
            newtype = "R";
            value = add(firstnumber.first, secondnumber.first);
        }
    }
    else if(type1 == "R" && type2 == "A")
    {
        newtype = "R";
        if(op == "+")
        {
            value = add(firstnumber.first, secondnumber.first);
        }
        else
        {
            value = subtract(firstnumber.first, secondnumber.first);
        }
    }
    else
    {
        newtype ="A";
        if(op == "+")
        {
            value = add(firstnumber.first, secondnumber.first);
        }
        else
        {
            value = subtract(firstnumber.first, secondnumber.first);
        }
    }
    string* s = new string[3] {value,newtype,"NoError"};
    symTab[symbol] = s;
    return "";
}
string SymbolTable::add(string first, string second)
{
    std::stringstream str;
    int f = std::stoul(first, nullptr, 16);
    int s = std::stoul(second, nullptr, 16);
    std::stringstream ss;
    f = f+s;
    ss << std::hex << f;
    return ss.str();
}
string SymbolTable::subtract(string first, string second)
{
    std::stringstream str;
    int f = std::stoul(first, nullptr, 16);
    int s = std::stoul(second, nullptr, 16);
    std::stringstream ss;
    f = f-s;
    ss << std::hex << f;
    return ss.str();
}
string SymbolTable::getOrgAddress(string symbol)
{
    std::transform(symbol.begin(), symbol.end(), symbol.begin(), ::tolower);
    map<string, string*>::iterator i = symTab.find(symbol);
    if(i != symTab.end()) //symbol found
    {
        return i->second[0]; //return symbol address
    }
    return"";
}
string SymbolTable::getAddress(string label)
{
    std::transform(label.begin(), label.end(), label.begin(), ::tolower);
    map<string, string*>::iterator i = symTab.find(label);
    if(i != symTab.end())
    {
        return i->second[0];
    }
    return "";
}
