#ifndef OUTPUT_H
#define OUTPUT_H
#include<string>
#include<Line.h>
#include<map>


class Output
{
public:
    Output();
    virtual ~Output();
    void makeLine(Line l);
    void printSymbolt(std::map<string, string*> table);
    void writeLine(std::string line);
    void pass2(std::string line);
protected:

private:
    string outline, label, opCode, operand, comment, address, prevAddress;
    string errorMessage;
    int expsize;
    bool iscomment,isErrors;
};

#endif // OUTPUT_H
