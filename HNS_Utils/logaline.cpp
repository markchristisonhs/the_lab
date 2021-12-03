#include "logaline.h"

using namespace std;

HNS_LogALine::HNS_LogALine()
{

}

HNS_LogALine::HNS_LogALine(const char *filename, ios_base::openmode mode)
    :f_out(filename,mode)
{

}

HNS_LogALine::HNS_LogALine(const string &filename, ios_base::openmode mode)
    :f_out(filename,mode)
{

}

void HNS_LogALine::fLogALine(const string &line)
{
    time_t now = time(0);
    string datestring = string(ctime(&now));
    //strip \n
    datestring.erase(datestring.size()-1);
    if(f_out.is_open())
    {
        f_out << datestring.c_str() << ": " << line.c_str() << endl;
    }
}

void HNS_LogALine::fLogALine(const std::string &line, const std::string &caller)
{
    time_t now = time(0);
    string datestring = string(ctime(&now));
    //strip \n
    datestring.erase(datestring.size()-1);
    if(f_out.is_open())
    {
        f_out << datestring.c_str() << ":" << caller.c_str() << ": " << line.c_str() << endl;
    }
}

//HNS_LogALine2::~HNS_LogALine2()
//{

//}
