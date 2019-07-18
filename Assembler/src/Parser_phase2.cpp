#include "Parser_phase2.h"
#include "Input.h"
#include"Program.h"
#include"Output.h"
#include"ObjectCode.h"
#include"ObjectFile.h"
#include"ReportError.h"


Parser_phase2::Parser_phase2()
{
    //ctor
}

Parser_phase2::~Parser_phase2()
{
    //dtor
}
void Parser_phase2::parse()
{
    Input in;
    in.openFile();
    in.readTwolines();
    opcode = in.getOpCode();
    operand = in.getOperand();
    nextLoc = in.getNextLoc();
    ObjectFile* objectFile = ObjectFile::getInstance();
    Program* program= Program::getInstance();
    Output output = program->getOutputManager();
    while (!icompare(opcode,"end"))
    {
        if (icompare(opcode, "base"))
        {
            Program* program= Program::getInstance();
            program->setBaseRelative(true);
            string a;
            LiteralTable* literalTable= LiteralTable::getInstance();
            map <std::string,std::string*> litTbl = literalTable->getlitTbl();
            if(operand.at(0) == '=')
            {
                std::string hex = "";
                for (int i = 0; i < operand.length(); i++)
                {
                    char ch = operand[i];
                    int in = (int)ch;
                    std::stringstream stream;
                    stream << std::hex << in;
                    std::string result( stream.str() );
                    hex += result;
                }
                operand =  hex;

                for (std::map<string, string*>::iterator it = litTbl.begin(); it != litTbl.end();
                        ++it)
                {
                    if (operand.compare(it->first) == 0)
                    {
                        a = it->second[2];
                        break;
                    }
                }
            }
            else
            {
                SymbolTable* symtab = SymbolTable::getInstance();
                a = symtab->getAddress(operand);
                if (a.length() == 0)
                {

                    a = operand;
                }
            }
            program->setBaseRegister(a);
        }
        else if (icompare(opcode, "nobase"))
        {
            Program* program= Program::getInstance();
            program->setBaseRelative(false);
        }
        else
        {
            ObjectCode objectCode;
            objectCode.calculateObjectCode(nextLoc,opcode,operand);
            if (objectCode.existError())
            {
                ReportError* reportError = ReportError::getInstance();
                string errorLine = in.getCurrentLine() + "\n" + objectCode.geterror();
                reportError->addError(errorLine);
                output.pass2(in.getCurrentLine());
                output.pass2(objectCode.geterror());
            }
            else
            {
                string line = in.getCurrentLine() + objectCode.getObjectCode();
                output.pass2(line);
                objectFile->appendObjectCode(objectCode.getObjectCode(),nextLoc);
            }
        }
        in.newLine();
        opcode = in.getOpCode();
        operand = in.getOperand();
        nextLoc = in.getNextLoc();
    }
    output.pass2(in.getCurrentLine());
    objectFile->End(in.getOperand());
    in.closeFile();
}

bool Parser_phase2::icompare(std::string a, std::string b)
{
    string s = a;
    string q = b;
    if (a.length()==b.length())
    {
        std::transform(s.begin(), s.end(), s.begin(), ::toupper);
        std::transform(q.begin(), q.end(), q.begin(), ::toupper);

        return s == q;
    }
    else
    {
        return false;
    }
}
