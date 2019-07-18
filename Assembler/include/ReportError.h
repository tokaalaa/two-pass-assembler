#ifndef REPORTERROR_H
#define REPORTERROR_H
#include<vector>
#include<string>

using namespace std;

class ReportError
{
    public:
        static ReportError *getInstance();
        virtual ~ReportError();
        void addError(string errorLine);
        void printReport();

    protected:

    private:
        ReportError();
        static ReportError *instance;
        vector<string> errorLines;
};

#endif // REPORTERROR_H
