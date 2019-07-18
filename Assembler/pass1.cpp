#include <iostream>
#include<fstream>
#include"Program.h"

using namespace std;

int main()
{
    Program* program= Program::getInstance();
    string path,path2;
    cout << "Enter \"assemble \" then source File Path:\n";
    string x;
    cin >> x >> path;
    ifstream input;
    input.open(path);
    while (input.fail() || x != "pass1")
    {
        cout << "Enter \"assemble \" then source File Path:\n";
        cin >> x >> path;
        input.open(path);
    }
    program->pass1(path);
    program->assemble();

    return 0;
}
