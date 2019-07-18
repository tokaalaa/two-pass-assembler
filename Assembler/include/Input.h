#ifndef INPUT_H
#define INPUT_H
#include "string"
#include<fstream>
#include<algorithm>//to use transform

class Input
{
public:
    Input();
    virtual ~Input();
    void readTwolines();
    void openFile();
    std::string getOpCode();
    std::string getOperand();
    std::string getLocCount();
    std::string getNextLoc();
    std::string getCurrentLine();
    void fixl();
    void newLine();
    void closeFile();
protected:

private:
    void removeSpaces(char *str);
    std::string currentLine, nextLine,thirdline;
    bool flag;
};

#endif // INPUT_H
