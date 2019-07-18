#ifndef EXPRESSIONS_H
#define EXPRESSIONS_H
#include <iostream>
#include <string>
#include <map>
#include<cctype>
#include <sstream>
#include <vector>
#include <utility>
#include<algorithm>//to use transform
using namespace std;
class expressions
{
    public:
        expressions();
        void evaluate(string operand);
        string errorMessage();
        string getAddress();
    protected:

    private:
        string add(string first, string second);
        string subtract(string first, string second);
        string multiply(string first, string second);
        string divide(string first, string second);
        string getValue(pair<string,string>firstnumber,pair<string,string>secondnumber,string op);
        string message;
        string address;
};

#endif // EXPRESSIONS_H
