#include "LiteralTable.h"
#include <string>
#include<map>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include"Program.h"
#include"Address.h"

LiteralTable *LiteralTable::instance=0;
LiteralTable::LiteralTable()
{
    //ctor
}

/*LiteralTable::~LiteralTable()
{
    //dtor
}*/
LiteralTable *LiteralTable::getInstance()
{
    if (instance==0)
    {
        instance = new LiteralTable();
    }
    return instance;
}

void LiteralTable::addLiteral(std::string literal)
{
    std::string length;
    if (literal[1] == 'c' || literal[1] == 'C')
    {
        int i = literal.size() - 4;
        std::stringstream ss;
        ss << i;
        length = ss.str();
    }
    else if (literal[1] == 'x' || literal[1] == 'X')
    {
        int i = (literal.size() - 4)/2;
        std::stringstream ss;
        ss << i;
        length = ss.str();
    }
    else if (literal[1] == 'w' || literal[1] == 'W')
    {
        length = "3";
    }
    //  int hexValue = std::stoi(literal, 0, 16);
    std::stringstream str;
    string hexValue = ASCIItoHEX(literal);
  //  str << literal;
   // str >> std::hex >> hexValue;
 //   cout << hexValue << "\n";
    std::map<string, std::string*>::iterator i = this->litTbl.find(hexValue);
    if(i == this->litTbl.end()) //label not found
    {
        std::string* s = new std::string[3] {literal,length," "};
        this->litTbl[hexValue] = s;
    }

}
std::string LiteralTable::setLiterals(std::string address)
{
    //cout << address << " ";
    literals = "";
    for (std::map< string, string*>::iterator it = this->litTbl.begin(); it != this->litTbl.end();
            ++it)
    {
        if (it->second[2] == " ")
        {
            it->second[2] = address;
            std::stringstream str;
            unsigned int addr = std::stoul(address, nullptr, 16);
            unsigned int l = std::stoul(it->second[1], nullptr, 16);

      /*      str << address;
            str >> std::hex >> addr;
            str << it->second[1];
            str >> std::hex >> l; */
            //cout << addr << " " << l  << " ";
            addr = addr + l;
            //cout << addr << "\n";
            std::stringstream ss;
            ss << std::hex << addr;
            address = ss.str();
            string out = address;
                if(address.length() < 4)
                {
                    for(int i = 0; i < (4 -address.length()); i++)
                    {
                        out = "0" + out;
                    }
                }
                address = out;
            literals = literals  +it->second[2]+"       *         " + it->second[0]+"\n" ;
        }
    }
    return address;
}

std::string LiteralTable::getLiterals()
{
    return literals;
}
    // function to convert ASCII to HEX
 std::string LiteralTable::ASCIItoHEX(std::string ascii)
    {
        std::string hex = "";
        for (int i = 0; i < ascii.length(); i++) {
            char ch = ascii[i];
            int in = (int)ch;
            std::stringstream stream;
            stream << std::hex << in;
            std::string result( stream.str() );
   //         std::string part = Integer.toHexString(in);
            hex += result;
        }
        return hex;
    }

std::map <std::string ,std::string*> LiteralTable::getlitTbl(){
    return this->litTbl;
}

