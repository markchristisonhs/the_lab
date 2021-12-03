#ifndef LOGALINE_H
#define LOGALINE_H

#include <fstream>
#include <vector>

class HNS_LogALine
{
public:
    HNS_LogALine();
    HNS_LogALine(const char* filename, std::ios_base::openmode mode = std::ios_base::out);
    HNS_LogALine(const std::string& filename, std::ios_base::openmode mode = std::ios_base::out);

    void fLogALine(const std::string &line);
    void fLogALine(const std::string &line, const std::string &caller);
private:
    std::ofstream f_out;
};

class HNS_LogALine2
{
public:
    virtual void fLogALine(const std::string &line) = 0;
    virtual void fLogALine(const std::string &line, const std::string &caller) = 0;
    virtual void fLogBuffer(const std::vector <unsigned char> &buffer) = 0;
    virtual void fLogCurrentTime() = 0;
    virtual ~HNS_LogALine2() {}
};

#endif // LOGALINE_H
