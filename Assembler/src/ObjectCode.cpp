#include "ObjectCode.h"
#include <map>
#include <string>
#include "OperationCodes.h"
#include<algorithm>//to use transform
#include <sstream>
#include <bitset>
#include "SymbolTable.h"
#include "LiteralTable.h"
#include"Program.h"
#include "expressions.h"
#include "FormatChecker.h"

using namespace std;

ObjectCode::ObjectCode()
{
    //ctor
    this->objectOfOPCode["and"] = "40";
    this->objectOfOPCode["or"] = "44";
    this->objectOfOPCode["div"] = "24";
    this->objectOfOPCode["divr"] = "9c";
    this->objectOfOPCode["rmo"] = "ac";
    this->objectOfOPCode["clear"] = "b4";
    this->objectOfOPCode["lda"] = "00";
    this->objectOfOPCode["ldx"] = "04";
    this->objectOfOPCode["lds"] = "6c";
    this->objectOfOPCode["ldt"] = "74";
    this->objectOfOPCode["ldb"] = "68";
    this->objectOfOPCode["ldl"] = "08";
    this->objectOfOPCode["sta"] = "0c";
    this->objectOfOPCode["stx"] = "10";
    this->objectOfOPCode["stb"] = "78";
    this->objectOfOPCode["stl"] = "14";
    this->objectOfOPCode["sts"] = "7c";
    this->objectOfOPCode["stt"] = "84";
    this->objectOfOPCode["ldch"] = "50";
    this->objectOfOPCode["stch"] = "54";
    this->objectOfOPCode["add"] = "18";
    this->objectOfOPCode["sub"] = "1c";
    this->objectOfOPCode["mul"] = "20";
    this->objectOfOPCode["addr"] = "90";
    this->objectOfOPCode["subr"] = "94";
    this->objectOfOPCode["mulr"] = "98";
    this->objectOfOPCode["compr"] = "a0";
    this->objectOfOPCode["comp"] = "28";
    this->objectOfOPCode["j"] = "3c";
    this->objectOfOPCode["jeq"] = "30";
    this->objectOfOPCode["jlt"] = "38";
    this->objectOfOPCode["jgt"] = "34";
    this->objectOfOPCode["tix"] = "2c";
    this->objectOfOPCode["tixr"] = "b8";
    this->objectOfOPCode["jsub"] = "48";
    this->objectOfOPCode["rsub"] = "4c";
    this->objectOfOPCode["td"] = "e0";
    this->objectOfOPCode["rd"] = "d8";
    this->objectOfOPCode["wd"] = "dc";
    this->objectOfOPCode["a"] = "0";
    this->objectOfOPCode["x"] = "1";
    this->objectOfOPCode["l"] = "2";
    this->objectOfOPCode["b"] = "3";
    this->objectOfOPCode["s"] = "4";
    this->objectOfOPCode["t"] = "5";
}

ObjectCode::~ObjectCode()
{
    //dtor
}

map<string, string> ObjectCode::getObjectMap()
{
    return this->objectOfOPCode;
}

string ObjectCode::getObjectCode()
{
    return this->objectCode;
}
//C:\Users\lenovo\Desktop\Assembler\trial.txt
void ObjectCode::calculateObjectCode(std::string address,std::string opCode, std::string operand)
{
    string orginal = operand;
    std::transform(opCode.begin(), opCode.end(), opCode.begin(), ::tolower);
    std::transform(operand.begin(), operand.end(), operand.begin(), ::tolower);
    opCode.erase(remove_if(opCode.begin(), opCode.end(), ::isspace), opCode.end());
    address.erase(remove_if(address.begin(), address.end(), ::isspace), address.end());
    /*if(address.length() == 0) {
        address = "0006";
    }*/
    this->opCode = opCode;
    this->operand = operand;
    OperationCodes operationCodes;
    if (opCode.compare("start") == 0 || opCode.compare("end") == 0
            || opCode.compare("base") == 0 || opCode.compare("nobase") == 0
            || opCode.compare("resw") == 0 || opCode.compare("resb") == 0
            || opCode.compare("use") == 0 || opCode.compare("equ") == 0
            || opCode.compare("org") == 0)
    {
        this->objectCode = " ";
    }
    else
    {
        string format = operationCodes.getMap()[opCode];
        if (format.compare("2") == 0)
        {

            operand.erase(remove_if(operand.begin(), operand.end(), ::isspace), operand.end());
            istringstream iss(operand);
            std::vector<std::string> tokens;
            std::string token;
            while (std::getline(iss, token, ','))
            {
                if (!token.empty())
                    tokens.push_back(token);
            }
            this->objectCode = ObjectCode::getObjectMap()[opCode] + ObjectCode::getObjectMap()[tokens[0]]
                               + ObjectCode::getObjectMap()[tokens[1]];
        }
        else if (format.compare("1") == 0)
        {
            this->objectCode = ObjectCode::getObjectMap()[opCode] + ObjectCode::getObjectMap()[operand] + "0";
        }
        else if (opCode.compare("rsub") == 0)
        {
            this->objectCode = "4f0000";
        }
        else if (opCode.compare("byte") == 0)
        {
            if (operand.at(0) == 'x')
            {
                orginal = orginal.substr (2,orginal.length()-3);
                this->objectCode = orginal;
            }
            else if (operand.at(0) == 'c')
            {
                orginal = orginal.substr (2,orginal.length()-3);
                string hex = "";
                for (int i = 0; i < orginal.length(); i++)
                {
                    char ch = orginal[i];
                    int in = (int)ch;
                    std::stringstream stream;
                    stream << std::hex << in;
                    std::string result( stream.str() );
                    hex += result;
                }
                this->objectCode = hex;
            }
        }
        else if (opCode.compare("word") == 0)
        {
            unsigned int x1;
            stringstream ss1;
            ss1 << orginal;
            ss1 >> x1;
            stringstream ss3;
            ss3 << std::hex << x1;
            string res = ss3.str();
            int len = res.length();
            for(int i = 0; i < (6-len); i++)
            {
                res = "0" + res;
            }
            this->objectCode = res;
        }
        else
        {
            ObjectCode::objectOrdinary(address,opCode,operand,orginal);
        }
    }
}

string ObjectCode::convHexToBinary(std::string address)
{
    stringstream ss;
    ss << hex << address;
    unsigned n;
    ss >> n;
    bitset<8> b(n);
    cout << b.to_string() << endl;
    return b.to_string();
}

void ObjectCode::objectOrdinary(std::string address, std::string opCode, std::string operand, std::string orginal)
{
    int flag = 0;
    string arr[6];
    if (opCode.find('+') != std::string::npos)
    {
        flag = 1;
        opCode = opCode.substr (1,opCode.length()-1);
    }
    string str1 = ObjectCode::convHexToBinary(ObjectCode::getObjectMap()[opCode]);
    string out = str1.substr(0,6);

    if (operand.find('@') != std::string::npos)
    {
        operand = operand.substr (1,operand.length()-1);
        orginal = orginal.substr (1,orginal.length()-1);
        arr[0] = "1";
        arr[1] = "0";
    }
    else if (operand.find('#') != std::string::npos)
    {
        operand = operand.substr (1,operand.length()-1);
        orginal = orginal.substr (1,orginal.length()-1);
        arr[0] = "0";
        arr[1] = "1";
    }
    else
    {
        arr[0] = "1";
        arr[1] = "1";
    }
    if (operand.find(',') != std::string::npos)
    {
        operand = operand.substr (0,operand.length()-2);
        orginal = orginal.substr (0,orginal.length()-2);
        arr[2] = "1";
    }
    else
    {
        arr[2] = "0";
    }
    if (flag == 1)
    {
        arr[3] = "0";
        arr[4] = "0";
        arr[5] = "1";
    }
    else
    {
        if(ObjectCode::is_number(operand))
        {
            arr[3] = "0";
            arr[4] = "0";
        }
        else
        {
            if (checkexp.isExpression(operand))
            {
                exp.evaluate(operand);
                cout << "operand" << operand<<"\n";
                cout << "exp" << exp.getAddress()<<"\n";
            }
            string check = ObjectCode::checkPCorB(address, operand);
            if(check.at(0) == 'p')
            {
                arr[3] = "0";
                arr[4] = "1";
            }
            else if (check.at(0) == 'b')
            {
                arr[3] = "1";
                arr[4] = "0";
            }
            else
            {
                arr[3] = "0";
                arr[4] = "0";
            }
        }
        arr[5] = "0";
    }
    for (int i = 0; i <6 ; i++)
    {
        out = out + arr[i];
    }
    string dis;
    if (checkexp.isExpression(operand))
    {
        exp.evaluate(operand);
        cout << "operand" << operand<<"\n";
        cout << "exp" << exp.getAddress()<<"\n";
    }
    if(ObjectCode::is_number(operand))
    {
        int number;
        std::istringstream iss (operand);
        iss >> number;
        string binary = std::bitset<20>(number).to_string(); //to binary
        /*int n = binary.length();
        cout << "binary" << binary <<"\n" ;
        if (n < 20 && flag == 1)
        {
            for(int i = 0; i < (20 - n); i++)
            {
                binary = "0" + binary;
                //out = out + "0";
            }
        }
        else if (n < 12 && flag == 0)
        {
            for(int i = 0; i < (12 - n); i++)
            {
                binary = "0" + binary;
                //out = out + "0";
            }
        }*/
        if(flag == 0) {
            binary = binary.substr (8,binary.length()-8);
        }
        out = out + binary;
        this->objectCode = ObjectCode::convBinaryToHex(out);
    }
    else if(flag == 0 && !error)
    {
        dis = ObjectCode::checkPCorB(address, operand);
        dis = dis.substr(1,dis.length()-1);
        if (flag == 0)
        {
            dis = dis.substr (2,3);
        }
        if (flag==1 && dis.length() < 5)
        {
            int n = dis.length();
            for (int i = 0 ; i < (5-n); i++)
                dis = "0" + dis ;
        }
        else if (flag==0 && dis.length() < 3)
        {
            int n = dis.length();
            for (int i = 0 ; i < (3-n); i++)
                dis = "0" + dis ;
        }
        this->objectCode = ObjectCode::convBinaryToHex(out);
        this->objectCode = this->objectCode + dis;
    }
    else if (!error)
    {
        SymbolTable* symbolTable = SymbolTable::getInstance();
        string TA;
        map<string, string*> table = symbolTable->GetSymTab();
        for (std::map<string, string*>::iterator it = table.begin(); it != table.end();
                ++it)
        {
            if (operand.compare(it->first) == 0)
            {
                this->error = false;
                this->err = "";
                TA = it->second[0];
                break;
            }
            this->error = true;
            this->err = "this symbol isn't define in pass1";
        }
        dis = TA;
        if (flag==1 && dis.length() < 5)
        {
            int n = dis.length();
            for (int i = 0 ; i < (5-n); i++)
                dis = "0" + dis ;
        }
        else
        {
            dis = dis.substr (dis.length()-5,5);
        }
        this->objectCode = ObjectCode::convBinaryToHex(out);
        this->objectCode = this->objectCode + dis;
    }
    if (flag == 1 && objectCode.length() <8)
    {
        int n =objectCode.length();
        for (int i = 0 ; i < (8-n); i++)
        {
            this->objectCode = "0" + objectCode;
        }
    }
    else if (flag == 0 && objectCode.length()< 6)
    {
        int n =objectCode.length();
        for (int i = 0 ; i < (6-n); i++)
        {
            this->objectCode = "0" + objectCode;
        }
    }
}

string ObjectCode::checkPCorB(std::string pc, std::string operand)
{
    string dis;
    SymbolTable* symbolTable = SymbolTable::getInstance();
    string TA;
    map<string, string*> table = symbolTable->GetSymTab();
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
                this->error = false;
                this->err = "";
                TA = it->second[2];
                break;
            }
            this->error = true;
            this->err = "this literal isn't define in pass1";
        }
    }
    else if (checkexp.isExpression(operand))
    {
        TA = exp.getAddress();
        if(TA.length() == 0)
        {
            this->error = true;
            this->err = exp.errorMessage();
        }
    }
    else
    {
        for (std::map<string, string*>::iterator it = table.begin(); it != table.end();
                ++it)
        {
            if (operand.compare(it->first) == 0)
            {
                this->error = false;
                this->err = "";
                TA = it->second[0];
                break;
            }
            this->error = true;
            this->err = "this symbol isn't define in pass1";
        }
    }
    if (!error)
    {
        unsigned int x1;
        unsigned int x2;
        stringstream ss1;
        stringstream ss2;
        ss1 << std::hex << TA;
        ss1 >> x1;
        ss2 << std::hex << pc;
        ss2 >> x2;
        int sub = x1 - x2;
        cout << sub << "\n";
        if (sub >= -2048 && sub <= 2047)
        {
            stringstream ss3;
            ss3 << std::hex << sub;
            string res = ss3.str();
            if (res.length() < 5)
            {
                int n = res.length();
                for(int i = 0; i < (5 - n); i++)
                {
                    res = "0" + res;
                }
            }
            else
            {
                res = res.substr(res.length()-5,5);
            }
            dis = "p" + res;
        }
        else
        {
            Program* program= Program::getInstance();
            bool check = program->isBaseRealative();
            if(check)
            {
                stringstream ss4;
                string add = program->getBaseRegister();
                std::transform(add.begin(), add.end(), add.begin(), ::tolower);
                add.erase(remove_if(add.begin(), add.end(), ::isspace), add.end());
                for (std::map<string, string*>::iterator it = table.begin(); it != table.end();
                        ++it)
                {
                    if (add.compare(it->first) == 0)
                    {
                        add = it->second[0];
                        break;
                    }
                }
                ss4 << std::hex << add;
                ss4 >> x2;
                int sub = x1 - x2;
                if (sub <= 4095 && sub >= 0)
                {
                    stringstream ss5;
                    ss5 << std::hex << sub;
                    string res = ss5.str();
                    if (res.length() < 5)
                    {
                        int n = res.length();
                        for(int i = 0; i < (5 - n); i++)
                        {
                            res = "0" + res;
                        }
                    }
                    else
                    {
                        res = res.substr (res.length()-5,5);
                    }
                    dis = "b" + res;
                }
                else
                {
                    this->error = true;
                    this->err = "disp Outside bounded of PC and B relative";
                }
            }
            else
            {
                this->error = true;
                this->err = "can't calculate B relative because of set nobase";
            }
        }
    }
    else
    {
        dis = " ";
    }
    return dis;
}
bool ObjectCode::existError()
{
    return this->error;
}

string ObjectCode::geterror()
{
    return this->err;
}

string ObjectCode::convBinaryToHex(std::string address)
{
    bitset<32> set(address);
    std::stringstream ss;
    ss << hex << set.to_ulong();
    return ss.str();
}

bool ObjectCode::is_number(const std::string& s)
{
    return !s.empty() && std::find_if(s.begin(),
                                      s.end(), [](char c)
    {
        return !std::isdigit(c);
    }) == s.end();
}
