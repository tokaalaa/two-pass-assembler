#ifndef OBJECTFILE_H
#define OBJECTFILE_H
#include<string>
#include<vector>

using namespace std;


class ObjectFile
{
    public:
        static ObjectFile *getInstance();
        virtual ~ObjectFile();
        void appendObjectCode(string objectCode,string locationConter);
        void Header();
        void End(string address);
        void write(string s);

    protected:

    private:
        ObjectFile();
        static ObjectFile *instance;
        string Hrecord,Erecord,currentTrecord,startTrecord;
        int currentLength;
        vector<string> modificationRecords;
};

#endif // OBJECTFILE_H
