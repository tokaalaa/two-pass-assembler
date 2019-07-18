#include "ObjectFile.h"
#include<string>
#include<Program.h>
#include<fstream>
#include <sstream>
#include<regex>

using namespace std;
using namespace regex_constants;

ObjectFile *ObjectFile::instance = 0;

ObjectFile::ObjectFile()
{
    //ctor
    ofstream output;
    output.open("ObjectFile.txt");
    output.close();
}

ObjectFile::~ObjectFile()
{
    //dtor
}

ObjectFile *ObjectFile::getInstance()
{
    if (instance==0)
    {
        instance = new ObjectFile();
    }
    return instance;
}


void ObjectFile::appendObjectCode(string objectCode,string locationCounter)
{
    if (objectCode == " ") {
        return;
    }
    int length = objectCode.length();
    length/=2;
    if (length == 4)
    {
        string m = "M^";
        int a = currentLength+1;
        std::stringstream ss;
        ss << std::hex << currentLength;
        string loc = ss.str();
        while (loc.length()<6)
        {
            loc = "0" + loc;
        }
        m += loc + "^" + "05";
        modificationRecords.push_back(m);
    }
    currentLength+=length;
    if (currentLength > 30)
    {
        currentLength-=length*2;
        std::stringstream ss;
        ss << std::hex << currentLength;
        string recordLength = ss.str();
        currentTrecord = startTrecord + recordLength + currentTrecord;
        write(currentTrecord);
        startTrecord = "T^" + locationCounter+"^";
        currentTrecord = "^"+objectCode;
        currentLength = length;
        return;
    }
    currentTrecord+="^" + objectCode;
    if (currentLength == 30)
    {
        std::stringstream ss;
        ss << std::hex << currentLength;
        string recordLength = ss.str();
        currentTrecord = startTrecord + recordLength + currentTrecord;
        write(currentTrecord);
        startTrecord = "T^" + locationCounter+"^";
        currentTrecord = "";
        currentLength = 0;
    }
}
void ObjectFile::Header()
{
    Program* program = Program::getInstance();
    Hrecord = "H^" + program->getName();
    int spaces = 7 - program->getName().length();
    for (unsigned int i = 0; i < spaces; i++)
    {
        Hrecord+=" ";
    }
    Hrecord+="^"+program->getStartingAddress()+"^"+program->getLength();
    write(Hrecord);
    startTrecord = "T^" + program->getStartingAddress()+"^";
    currentTrecord = "";
    currentLength = 0;
}
void ObjectFile::End(string address)
{
    std::stringstream ss;
    ss << std::hex << currentLength;
    string recordLength = ss.str();
    currentTrecord = startTrecord + recordLength + currentTrecord;
    write(currentTrecord);
    for (unsigned int i = 0; i < modificationRecords.size(); i++)
    {
        write(modificationRecords[i]);
    }
    int l = address.length();
    if (l==0)
    {
        Erecord = "E";
    }
    else
    {
        std::smatch m;
        std::regex e("[A-F0-9]+", ECMAScript | icase);
        if (std::regex_search(address,m,e))
        {
            int x = m.length();
            int y = address.length();
            if (x!=y)
            {
                SymbolTable* symbolTable = SymbolTable::getInstance();
                address = symbolTable->getAddress(address);

            }
        }
        else
        {
            SymbolTable* symbolTable = SymbolTable::getInstance();
            address = symbolTable->getAddress(address);
        }
        Erecord = "E^" + address;
    }

    write(Erecord);
}
void ObjectFile::write(string s)
{
    ofstream output;
    output.open("ObjectFile.txt", std::ios::app);
    output << s << "\n";
    output.close();
}
