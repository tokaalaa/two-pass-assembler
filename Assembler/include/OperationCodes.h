#ifndef OPERATIONCODES_H
#define OPERATIONCODES_H
#include <map>
#include <string>
#include <iostream>

using namespace std;

class OperationCodes
{
    public:
        OperationCodes();
        virtual ~OperationCodes();
        void geneError(std::string opcode, std::string operand, std::string address);
        bool check(std::string opcode, std::string operand);
        map<string, string> getMap();
        bool isAssemblerDirective(string opCode);
        static string storageDirectives[4];
        static string assemblerDirectives[7];

    protected:

    private:
        map<string, string> myMap;
};

#endif // OPERATIONCODES_H
