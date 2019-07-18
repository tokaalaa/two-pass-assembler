#include "Output.h"
#include<fstream>
#include<iostream>
#include<map>
#include<ReportError.h>

using namespace std;

Output::Output()
{
    //ctor
    this->prevAddress = "";
	this->outline = "Address     label    OPCode  Operand             Comment";
//	this->writeLine(outline);
	ofstream myfile;
    myfile.open ("Output.txt");
    myfile << outline;
    myfile << "\n";
    myfile.close();
	this->expsize = 0;
}

Output::~Output()
{
    //dtor
}

void Output::makeLine(Line l)
{
    this -> outline = "";
    this -> address = l.getLocationCounter();
    this -> opCode = l.getOpCode();
    this -> label = l.getLabel();
    this -> operand = l.getOperand();
    this -> comment = l.getComment();
    this -> iscomment= l.isComment();
    this->isErrors = l.errorFound();
    if (iscomment)
    {
        outline = this ->prevAddress;
        expsize = 7;
        while (outline.size() < expsize)
        {
            outline = outline + " ";
        }
        outline = outline + comment;
    }
    else if(isErrors)
    {
        writeLine(l.getInput());
        writeLine(l.getErrorMessage());
    }
    else
    {
       	this->prevAddress = address;
		expsize = 7 + 5;
		outline = outline + address;
		while (outline.size() < expsize) {
			outline = outline + " ";
		}
		expsize = 7 + 5 + 9;
		outline = outline + label;
		while (outline.size() < expsize) {
			outline = outline + " ";
		}
		expsize = 7 + 5 + 17;
		outline = outline + opCode;
		while (outline.size() < expsize) {
			outline = outline + " ";
		}
		expsize = 7 + 5 + 35;
		outline = outline + operand;
		while (outline.size() < expsize) {
			outline = outline + " ";
		}
		outline = outline + comment;
    }
    writeLine(outline);
    /*if (Warning is true)
    {
        writeLine(l)
    }
    */
}

void Output::printSymbolt(std::map<string, string*> table) {
	ofstream myfile;
	myfile.open("Output.txt", ios::app);
	myfile << "******************************************************** \n";
	std::string line = "Symbol    Location    type    ";
	myfile << line;
	myfile << "\n";
//	expsize = 10;
	for (std::map<string, string*>::iterator it = table.begin(); it != table.end();
			++it) {
		line = it->first;
		expsize = 10;
		while (line.size() < expsize) {
			line = line + " ";
		}
		line = line + it->second[0];
		expsize = 10 + 12;
		while (line.size() < expsize) {
			line = line + " ";
		}
		line = line + it->second[1];
		expsize = 10 + 12+ 9;
		while (line.size() < expsize) {
			line = line + " ";
		}
		//std::cout << line;
		myfile << line;
		myfile << "\n";
	}
	myfile.close();
}
void Output::writeLine(std::string line)
{
    ofstream myfile;
    myfile.open ("Output.txt",std::ios::app);
    myfile << line;
    myfile << "\n";
    myfile.close();
}
void Output::pass2(std::string line)
{
    ofstream o;
    o.open("Pass2.txt",std::ios::app);
    o << line << "\n";
    o.close();
}

