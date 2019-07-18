#include "Program.h"
#include <iostream>
#include<fstream>
#include"Line.h"
#include"Parser.h"
#include<regex>
#include"Address.h"
#include "Output.h"
#include"LiteralTable.h"
#include"Parser_phase2.h"
#include"ObjectFile.h"
#include"ReportError.h"

using namespace std;
using namespace std::regex_constants;

Program *Program::instance=0;

Program::Program()
{
    //ctor
    baseRelative = true;
    baseRegister = "FFFFFF";
}

Program::~Program()
{
    //dtor
}

Program *Program::getInstance()
{
    if (instance==0)
    {
        instance = new Program();
    }
    return instance;
}

void Program::pass1(string path)
{
    bool endFlag;
    /*string path;
    cout << "Enter \"pass1 \" then source File Path:\n";
    string x;
    cin >> x >> path;*/
    //cout << path;
    ifstream input;
    input.open(path);

    string line;
    getline(input, line);
    while (line.at(0) == '.')
    {
        Line l;
        l.setCommentLine(true);
        l.setComment(line);
        l.write();
        getline(input, line);
    }
    std::smatch m;
    std::regex e("start", ECMAScript | icase);
    if(std::regex_search(line,m,e))
    {
        Parser parser;
        if (!parser.parseStart(line))
        {
            Parser p;
            Program::address.startCounter("0000");
            setstartingAdddress("0000");
            p.parseLine(line);
        }
    }
    else
    {
        Parser parser;
        Program::address.startCounter("0000");
        setstartingAdddress("0000");
        parser.parseLine(line);
    }
    while (getline(input, line))
    {
        std::smatch m;
        std::regex e("(\\w+)?\\s+end", ECMAScript | icase);
        if(std::regex_search(line,m,e))
        {
            if (m.position() == 0)
            {
                Parser parser;
                parser.parseEnd(line);
                endFlag=true;
                while (getline(input, line))
                {
                    Program::output.writeLine(line);
                }
            }
            else
            {
                Parser parser;
                parser.parseLine(line);
            }
        }
        else
        {
            Parser parser;
            parser.parseLine(line);
        }
    }
    if (!endFlag)
    {
        Program::output.writeLine("Error! No End ..");
    }
    input.close();
    SymbolTable* symbolTable = SymbolTable::getInstance();
    output.printSymbolt(symbolTable->GetSymTab());
}

void Program::assemble()
{
    if (this->pass1_error)
    {
        ofstream o;
        o.open("Pass2.txt");
        o.close();
        ReportError* reportError = ReportError::getInstance();
        reportError->printReport();
        return;
    }
    ObjectFile* objectFile = ObjectFile::getInstance();
    objectFile->Header();
    ofstream o;
    o.open("Pass2.txt");
    o.close();
    Parser_phase2 p;
    p.parse();
    ReportError* reportError = ReportError::getInstance();
    reportError->printReport();
}

void Program::setName(string name)
{
    this->name = name;
}
void Program::setstartingAdddress(string startingAdddress)
{
    this->startingAdddress = startingAdddress;
}
void Program::setLength(string length)
{
    this->length = length;
}
void Program::setBaseRegister(string address)
{
    this->baseRegister = address;
}
void Program::setBaseRelative(bool baseRelative)
{
    this->baseRelative = baseRelative;
}
string Program::getName()
{
    return this->name;
}
string Program::getStartingAddress()
{
    return this->startingAdddress;
}
string Program::getLength()
{
    return this->length;
}
string Program::getBaseRegister()
{
    return this->baseRegister;
}
bool Program::isBaseRealative()
{
    return this->baseRelative;
}
Address Program::getAddressManager()
{
    return this->address;
}
Output Program::getOutputManager()
{
    return this->output;
}
LiteralTable Program::getLiteralTableManeger()
{
    return this->literalTable;
}
void Program::setPass1_errorFlag(bool flag)
{
    this->pass1_error = flag;
}
