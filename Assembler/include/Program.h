#ifndef PROGRAM_H
#define PROGRAM_H
#include<string>
#include<Address.h>
#include<Output.h>
#include<SymbolTable.h>
#include<Parser.h>
#include"LiteralTable.h"

using namespace std;

class Program
{
public:
    static Program *getInstance();
    virtual ~Program();
    void pass1(string path);
    void assemble();
    void setName(string name);
    void setstartingAdddress(string startingAdddress);
    void setLength(string length);
    void setBaseRegister(string address);
    void setBaseRelative(bool baseRelative);
    string getName();
    string getStartingAddress();
    string getLength();
    string getBaseRegister();
    bool isBaseRealative();
    Address getAddressManager();
    Output getOutputManager();
    LiteralTable getLiteralTableManeger();
    void setPass1_errorFlag(bool flag);

protected:

private:
    Program();
    static Program *instance;
    string name, startingAdddress, length,baseRegister;
    bool baseRelative, pass1_error;
    Address address;
    Output output;
    LiteralTable literalTable;
};

#endif // PROGRAM_H
