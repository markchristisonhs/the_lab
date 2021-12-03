#ifndef LOGALINE_ANDROID_H
#define LOGALINE_ANDROID_H

#include "logaline.h"

class HNS_LogALine_Android : public HNS_LogALine2
{
public:
    HNS_LogALine_Android();
    void fLogALine(const std::string &line);
    void fLogALine(const std::string &line, const std::string &caller);
    void fLogBuffer(const std::vector<unsigned char> &buffer);
    void fLogCurrentTime();
    ~HNS_LogALine_Android();
private:
    void fWriteToLogcat(const char *caller, const char *s);
};

#endif // LOGALINE_ANDROID_H
