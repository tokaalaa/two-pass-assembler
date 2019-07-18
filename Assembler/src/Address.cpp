#include <iostream>
//to use stringstream
#include <sstream>
//convert lowercase
#include <algorithm>
#include <string>
#include "Address.h"

using namespace std;

Address::Address()
{
    //ctor
    /*this->opCode = opCode;
    this->operand = operand;*/
    this->address = "";
    this->counAdd = 0;
}

Address::~Address()
{
    //dtor
}

string Address::getAddress()
{
    string out = address;
    if(this->address.length() < 6)
    {
        for(unsigned int i = 0; i < (6 - this->address.length()); i++)
        {
            out = "0" + out;
        }
    }
    return out;
}
void Address::startCounter(string operand)
{
    std::stringstream str;
    str << operand;
    str >> std::hex >> counAdd;
    std::stringstream ss;
    ss << std::hex << counAdd;
    address = ss.str();
}

void Address::updateCounter(string opCode, string operand)
{
    std::transform(opCode.begin(), opCode.end(), opCode.begin(), ::tolower);
    std::transform(operand.begin(), operand.end(), operand.begin(), ::tolower);
    std::stringstream str;
    str << operand;
    if (opCode.find('+') != std::string::npos)
    {
        counAdd = counAdd + 4;
    }
    else if (opCode.compare("resb") == 0 )
    {
        int n = 0;
        str >> n;
        counAdd = counAdd + n;
    }
    else if (opCode.compare("resw") == 0 )
    {
        int n = 0;
        str >> n;
        counAdd = counAdd + n * 3;
    }
    else if (opCode.compare("byte") == 0 )
    {
        if (operand.find('x') != std::string::npos)
        {
            counAdd = counAdd + (operand.length() - 3) / 2;
        }
        else
        {
            counAdd = counAdd + operand.length() - 3;
        }
    }
    else if (opCode.compare("rmo") == 0)
    {
        counAdd = counAdd + 2;
    }
    else if (opCode.compare("clear") == 0)
    {
        counAdd = counAdd + 2;
    }

    else if ((opCode.find("add") != std::string::npos|opCode.find("sub") != std::string::npos|
              opCode.find("mul") != std::string::npos|opCode.find("add") != std::string::npos) &&
             opCode.length() == 4)
    {
        counAdd = counAdd + 2;
    }
    else if (opCode.find("tix") != std::string::npos &&
             opCode.length() == 4)
    {
        counAdd = counAdd + 2;
    }
    else if (opCode.find("comp") != std::string::npos &&
             opCode.length() == 5)
    {
        counAdd = counAdd + 2;
    }
    else if(opCode.compare("equ") != 0 && opCode.compare("end") != 0)
    {
        counAdd = counAdd + 3;
    }
    std::stringstream ss;
    ss << std::hex << counAdd;
    address = ss.str();
    //std::cout << counAdd << " ==> " << address << std::endl;
}
