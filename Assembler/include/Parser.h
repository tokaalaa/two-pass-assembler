#include<string>

#ifndef PARSER_H
#define PARSER_H
#include"Program.h"
#include"Address.h"
#include"Output.h"
#include"SymbolTable.h"
#include"Line.h"

using namespace std;
//using namespace std::regex_constants;

class Parser
{
    public:
        Parser();
        virtual ~Parser();
        void parseLine(std::string line);
        bool parseStart(std::string line);
        void parseEnd(std::string line);

    protected:

    private:
        bool opCode(std::string s);
        bool noOperand(std::string s);
        static string opCodes[51];
        static string noOperanCodes[7];
        //void addToSymbolTable();
        Line l;
};

#endif // PARSER_H
