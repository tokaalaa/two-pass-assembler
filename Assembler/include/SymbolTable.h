#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
#include <iostream>
#include<map>
#include <string>
#include <vector>
#include <sstream>
#include <utility>
using namespace std;

class SymbolTable
{
    public:
        SymbolTable();
        static SymbolTable *getInstance();
        string addlabel(std::string symbol, std::string address);
        string addequ(std::string symbol, vector<std::string> operand);
        string getOrgAddress(std::string symbol);
        map<std::string, std::string*> GetSymTab();
        string getAddress(string label);
    protected:

    private:
        static SymbolTable *instance;
        bool checkValidName(string symbol);
        string checkEquOperand(string symbol,vector<string> operand);
        string checkExpressionOperand(string symbol, vector<string> operand);
        string generateError(string symbol);
        string evaluateExpression(string symbol, pair<string, string> firstnumber,pair<string, string> secondnumber,std::string op);
        string add(string first, string second);
        string subtract(string first, string second);
        map<string, string*>symTab;
};

#endif // SYMBOLTABLE_H
