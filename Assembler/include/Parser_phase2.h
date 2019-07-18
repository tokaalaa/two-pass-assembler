#ifndef PARSER_PHASE2_H
#define PARSER_PHASE2_H
#include "string"

using namespace std;

class Parser_phase2
{
public:
    Parser_phase2();
    virtual ~Parser_phase2();
    void parse();
    void setLines(std::string line);
protected:

private:
    bool isStart, isEnd;
    string opcode, operand,nextLoc;
    string currentLine, nextLine;
    bool icompare(std::string a, std::string b);
};
#endif // PARSER_PHASE2_H
