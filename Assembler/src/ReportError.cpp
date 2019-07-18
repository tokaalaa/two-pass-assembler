#include "ReportError.h"
#include<vector>
#include<string>
#include<Output.h>
#include"Program.h"

using namespace std;

ReportError *ReportError::instance = 0;

ReportError::ReportError()
{
    //ctor
}

ReportError::~ReportError()
{
    //dtor
}

ReportError *ReportError::getInstance()
{
    if (instance == 0)
    {
        instance = new ReportError();
    }
    return instance;
}
void ReportError::addError(string errorLine)
{
    this->errorLines.push_back(errorLine);
}
void ReportError::printReport()
{
    Program* program = Program::getInstance();
    Output output = program->getOutputManager();
    output.pass2("\n\nAssembler Report:\n");
    for (unsigned int i = 0; i < this->errorLines.size(); i++)
    {
        output.pass2(errorLines[i]);
    }
}
