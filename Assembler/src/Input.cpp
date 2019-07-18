#include "Input.h"
#include<fstream>
#include <sstream>
#include<algorithm>//to use transform
#include<iostream>
#include<regex>

using namespace std;
ifstream input;
Input::Input()
{
    //ctor
}

Input::~Input()
{
    //dtor
}
void Input::openFile()
{
    string path = "Output.txt";
    input.open(path);
}
void Input::readTwolines()
{
    getline(input, currentLine);
    getline(input, currentLine);
    while(currentLine.at(7) == '.')
    {
        getline(input, currentLine);
    }
    getline(input, nextLine);
    while(nextLine.at(7) == '.')
    {
        getline(input, nextLine);
    }
    return ;
}
string Input::getOpCode()
{
    string temp = currentLine.substr(21, 8);
    //temp.erase(remove_if(temp.begin(), temp.end(), ::isdigit), temp.end());
    std::smatch m;
    std::regex e("\\s+");
    if(std::regex_search(temp,m,e))
    {
        temp = temp.substr(0,m.position());
    }
//   removeSpaces(temp);
    return temp;
}
string Input::getOperand()
{
    string temp = currentLine.substr(29, 18);
    //temp.erase(remove_if(temp.begin(), temp.end(), ::isdigit), temp.end());
    std::smatch m;
    std::regex e("\\s+");
    if(std::regex_search(temp,m,e))
    {
        temp = temp.substr(0,m.position());
    }
    return temp;
}
string Input::getLocCount()
{
    string temp = currentLine.substr(0, 7);
    //temp.erase(remove_if(temp.begin(), temp.end(), ::isdigit), temp.end());
    std::smatch m;
    std::regex e("\\s+");
    if(std::regex_search(temp,m,e))
    {
        temp = temp.substr(0,m.position());
    }
    return temp;
}
string Input::getNextLoc()
{
    string s = getOpCode();
    std::transform(s.begin(), s.end(), s.begin(), ::toupper);
    if (s == "END")
    {
        return "";
    }
    string temp;
    if (flag)
    {
        temp = thirdline.substr(0,7);
    }
    else
    {
        temp = nextLine.substr(0, 7);
    }
    //temp.erase(remove_if(temp.begin(), temp.end(), ::isdigit), temp.end());
    std::smatch m;
    std::regex e("\\s+");
    if(std::regex_search(temp,m,e))
    {
        temp = temp.substr(0,m.position());
    }
    return temp;
}
string Input::getCurrentLine()
{
    string line = currentLine.substr(0,47);
    return line;
}
void Input::newLine()
{
    currentLine = nextLine;
    string s = getOpCode();
    std::transform(s.begin(), s.end(), s.begin(), ::toupper);
    if (s == "END")
    {
        return;
    }
    if (!flag)
    {
        getline(input, nextLine);
        while(nextLine.at(7) == '.')
        {
            getline(input, nextLine);
        }
    }
    else
    {
        nextLine = thirdline;
        flag = false;
    }
    fixl();
    return;

}
void Input::fixl()
{
    if (nextLine.substr(0, 7) == "       ")
    {
        getline(input, thirdline);
        while(thirdline.at(7) == '.')
        {
            getline(input, thirdline);
        }
        flag = true;
    }
    else
    {
        flag = false;
    }
}
void Input::closeFile()
{
    input.close();
}
