#ifndef FORMATCHECKER_H
#define FORMATCHECKER_H
#include<string>
#include<regex>

using namespace std;
using namespace std::regex_constants;


class FormatChecker
{
public:
    FormatChecker();
    virtual ~FormatChecker();
    bool formatTwo(std::string operand);
    bool formatThree(std::string operand);
    bool validLabel(std::string label);
    vector<string> expression(string operand);
    bool isExpression(string operand);
    bool noLabelCode(string opCode);
    bool storageDirectives(string opCode, string operand);

protected:

private:
};

#endif // FORMATCHECKER_H
