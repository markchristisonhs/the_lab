#include "logaline_android.h"

#ifdef HNS_ANDROID
#include <android/log.h>
#endif

#include <string>
#include <sstream>
#include <sys/time.h>
#include <iomanip>

//cook booked from stack overflow:

struct HexCharStruct
{
  unsigned char c;
  HexCharStruct(unsigned char _c) : c(_c) { }
};

inline std::ostream& operator<<(std::ostream& o, const HexCharStruct& hs)
{
  return (o << std::hex << (int)hs.c);
}

inline HexCharStruct hex(unsigned char _c)
{
  return HexCharStruct(_c);
}

using namespace std;

HNS_LogALine_Android::HNS_LogALine_Android()
{

}

void HNS_LogALine_Android::fLogALine(const string &line)
{
    fWriteToLogcat("HNS",line.c_str());
}

void HNS_LogALine_Android::fLogALine(const string &line, const string &caller)
{
    fWriteToLogcat(caller.c_str(),line.c_str());
}

void HNS_LogALine_Android::fLogBuffer(const vector<unsigned char> &buffer)
{
    string tempstring;
    stringstream os;

    for(size_t ui=0;ui<buffer.size();ui++)
    {
        //os << setfill('0') << setw(2) << hex << buffer[ui];
        os << setfill('0') << setw(2) << hex(buffer[ui]);
        if((ui % 16) != 15)
        {
            os << setw(0) << setfill(' ') << " ";
        }
        else
        {
            os << endl;
        }
    }

    fWriteToLogcat("HNS_BUFFER",os.str().c_str());
}

void HNS_LogALine_Android::fLogCurrentTime()
{
    struct timeval current_time;
    time_t t;
    struct tm *info;

    gettimeofday(&current_time,nullptr);
    t = current_time.tv_sec;
    info = gmtime(&t);
    fWriteToLogcat("HNS_Timestamp",asctime(info));

}

HNS_LogALine_Android::~HNS_LogALine_Android()
{

}

void HNS_LogALine_Android::fWriteToLogcat(const char *caller, const char *s)
{
#ifdef HNS_ANDROID
    __android_log_write(ANDROID_LOG_DEBUG,caller,s);
#endif
}
