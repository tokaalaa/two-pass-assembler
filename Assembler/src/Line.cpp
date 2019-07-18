#include "Line.h"
#include "Parser.h"
#include"Address.h"
#include"SymbolTable.h"
#include"Output.h"
#include"FormatChecker.h"
#include"LiteralTable.h"
#include"OperationCodes.h"
#include<string>
#include<vector>
#include<regex>
#include<ReportError.h>

using namespace std;
using namespace regex_constants;

Program* program= Program::getInstance();
Address address = program->getAddressManager();
Output output = program->getOutputManager();
LiteralTable literalTable = program->getLiteralTableManeger();
OperationCodes oc;

Line::Line()
{
    //ctor
    this->label = "";
    this->opCode = "";
    this->operand = "";
    this->comment = "";
    this->errorMessage="";
    this->input="";
    this->error=false;
    this->commentLine=false;
}

Line::~Line()
{
    //dtor
}

void Line::setLabel(string label)
{
    this->label = label;
}

void Line::setOpCode(string opCode)
{
    this->opCode = opCode;
}

void Line::setOperand(string operand)
{
    this->operand = operand;
}

void Line::setComment(string comment)
{
    this->comment = comment;
}

void Line::setLocationCounter(string locationCounter)
{
    this->locationCounter = locationCounter;
}

void Line::setErrorMessage(string errorMessage)
{
    this->errorMessage = errorMessage;
}
void Line::setInput(string input)
{
    this->input = input;
}
void Line::setCommentLine(bool commentLine)
{
    this->commentLine = commentLine;
}

void Line::setErrorFound(bool errorFound)
{
    this->error = errorFound;
}

string Line::getLabel()
{
    return this->label;
}

string Line::getOpCode()
{
    return this->opCode;
}

string Line::getOperand()
{
    return this->operand;
}

string Line::getComment()
{
    return this->comment;
}

string Line::getLocationCounter()
{
    return this->locationCounter;
}

string Line::getErrorMessage()
{
    return this->errorMessage;
}
string Line::getInput()
{
    return this->input;
}
bool Line::isComment()
{
    return this->commentLine;
}
bool Line::errorFound()
{
    return this->error;
}
bool Line::symbolicOperand()
{
    return this->symbol;
}
void Line::checker()
{
    map<string, string> codeMap = oc.getMap();
    map<string,string>::iterator it = codeMap.find(getOperand());
    if(it != codeMap.end())
    {
        setErrorMessage(getErrorMessage() + "\t\\\t" + "Invalid Operand ..");
        return;
    }
    for (int i = 0; i < 7; i++)
    {
        if (getOperand() == oc.assemblerDirectives[i])
        {
            setErrorMessage(getErrorMessage() + "\t\\\t" + "Invalid Operand ..");
            return;
        }
    }
    for (int i = 0; i < 4; i++)
    {
        if (getOperand() == oc.storageDirectives[i])
        {
            setErrorMessage(getErrorMessage() + "\t\\\t" + "Invalid Operand ..");
            return;
        }
    }
    if (!oc.check(getOpCode(), getOperand()))
    {
        setErrorFound(true);
        setErrorMessage(getErrorMessage() + "\t\\\t" + "Invalid Operand ..");
    }
}
void Line::executer()
{
    if(oc.isAssemblerDirective(getOpCode()))
    {
        handleSpecialCases();
    }
    else
    {
        checker();
        updateLocationCounter();
        addToSymbolTable();
        addToLiteralTable();
    }
}
void Line::addToSymbolTable()
{
    int z = getLabel().length();
    if (z > 0)
    {
        SymbolTable* symbolTable = SymbolTable::getInstance();
        string e = symbolTable->addlabel(getLabel(),getLocationCounter());
        z = e.length();
        if (z > 0)
        {
            setErrorFound(true);
            setErrorMessage(getErrorMessage() + "\t\\\t" + e);
        }
    }
}
void Line::addToLiteralTable()
{
    if(getOperand().length() > 0)
    {
        if (getOperand().at(0) == '=')
        {
            literalTable.addLiteral(operand);
        }
    }

}
void Line::updateLocationCounter()
{
    setLocationCounter(address.getAddress());
    address.updateCounter(getOpCode(), getOperand());
}
void Line::handleSpecialCases()
{
    string s = getOpCode();
    std::transform(s.begin(), s.end(), s.begin(), ::toupper);
    if (s == "START")
    {
        int len =getOperand().length();
        if (len > 0)
        {
            address.startCounter(getOperand());
        }
        else
        {
            address.startCounter("0000");
        }
        setLocationCounter(address.getAddress());
        program->setstartingAdddress(address.getAddress());
        return;
    }
    if (s == "END")
    {
        bool flag = false;
        updateLocationCounter();
        std::smatch m;
        std::regex e("[A-F0-9]+", ECMAScript | icase);
        if (std::regex_search(operand,m,e))
        {
            int x = m.length();
            int y = operand.length();
            if (x==y)
            {
                flag = true;
            }
        }
        SymbolTable* symb = SymbolTable::getInstance();
        string add = symb->getAddress(operand);
        if (add.length() > 0)
        {
            flag = true;
        }
        if (!flag)
        {
            setErrorFound(true);
            setErrorMessage(getErrorMessage() + "\t\\\t" + "Invalid Operand ..");
            output.makeLine(*this);
            string newAddress = literalTable.setLiterals(address.getAddress());
            output.writeLine(literalTable.getLiterals());
            address.startCounter(newAddress);
        }
        else
        {
            output.makeLine(*this);
            string newAddress = literalTable.setLiterals(address.getAddress());
            output.writeLine(literalTable.getLiterals());
            address.startCounter(newAddress);
            string current = address.getAddress();
            string start = program->getStartingAddress();
            int bgn,finish,l;
            stringstream ss1,ss2,ss3;
            ss1 << std::hex << start;
            ss1 >> bgn;
            ss2 << std::hex << current;
            ss2 >> finish;
            l = finish - bgn;
            ss3 << std::hex << l;
            string len = ss3.str();
            Program* program= Program::getInstance();
            program->setLength(len);
        }
        return;
    }
    if (s == "BASE")
    {
        int len1 = getOperand().length();
        int len2 = getLabel().length();
        if (len1 == 0 || len2 > 0)
        {
            setErrorMessage(getErrorMessage() + "\t\\\t" + "Undefined format for NOBASE ..");
        }
        return;
    }
    if (s == "NOBASE")
    {
        int len1 = getOperand().length();
        int len2 = getLabel().length();
        if (len1 > 0 || len2 > 0)
        {
            setErrorMessage(getErrorMessage() + "\t\\\t" + "Undefined format for NOBASE ..");
        }
        return;
    }
    if (s == "LTORG")
    {
        int len1 = getOperand().length();
        int len2 = getLabel().length();
        if (len1 > 0 || len2 > 0)
        {
            setErrorMessage(getErrorMessage() + "\t\\\t" + "Undefined format for LTORG ..");
            return;
        }
        string newAddress = literalTable.setLiterals(address.getAddress());
        output.makeLine(*this);
        output.writeLine(literalTable.getLiterals());
        address.startCounter(newAddress);
        return;
    }
    if (s == "ORG")
    {
        setLocationCounter(address.getAddress());
        int len = getLabel().length();
        if (len > 0)
        {
            setErrorFound(true);
            setErrorMessage(getErrorMessage() + "\t\\\t" + "ORG directive can not have label field ..");
        }
        else
        {
            SymbolTable* symbolTable = SymbolTable::getInstance();
            string a = symbolTable->getOrgAddress(getOperand());
            if (a == "")
            {
                setErrorMessage(getErrorMessage() + "\t\\\t" + "Undefined Operand ..");
                return;
            }
            address.startCounter(a);
        }
        return;
    }
    if (s == "EQU")
    {
        int len = getLabel().length();
        if (len > 0)
        {
            FormatChecker format;
            vector<string> exp;
            std::smatch m;
            std::regex e("[A-F0-9]+", ECMAScript | icase);
            if (std::regex_search(operand,m,e))
            {
                int x = m.length();
                int y = operand.length();
                if (x==y)
                {
                    exp.push_back(getOperand());
                }
            }
            else if (getOperand() == "*")
            {
                exp.push_back(address.getAddress());
            }
            else if (format.validLabel(getOperand()))
            {
                exp.push_back(getOperand());
            }
            else
            {
                exp = format.expression(getOperand());

            }
            if (exp.size() == 0)
            {
                setErrorMessage(getErrorMessage() + "\t\\\t" + "Undefined Operand ..");
                return;
            }
            SymbolTable* symbolTable = SymbolTable::getInstance();
            string s = symbolTable->addequ(getLabel(),exp);
            if (s == "")
            {
                setErrorMessage(getErrorMessage() + "\t\\\t" + s);
            }
            return;
        }
        else
        {
            setErrorFound(true);
            setErrorMessage(getErrorMessage() + "\t\\\t" + "Invalid EQU with no label ..");
        }
        return;
    }
}
void Line::write()
{
    if (this->errorFound())
    {
        ReportError* reportError = ReportError::getInstance();
        string errorLine = this->getInput() + "\n" + this->getErrorMessage();
        reportError->addError(errorLine);
        program->setPass1_errorFlag(true);
    }
    string s = getOpCode();
    std::transform(s.begin(), s.end(), s.begin(), ::toupper);
    if (s == "LTORG" || s == "END")
    {
        return;
    }
    output.makeLine(*this);
}
