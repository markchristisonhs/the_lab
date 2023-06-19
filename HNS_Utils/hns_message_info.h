#ifndef HNS_MESSAGE_INFO_H
#define HNS_MESSAGE_INFO_H

#include <stdint.h>
#include <vector>

enum
{
    HNS_MESSAGE_SORT_NONE = 0x0,
    HNS_MESSAGE_SORT_DATE_NEWEST_FIRST = 0x1,
    HNS_MESSAGE_SORT_DATE_OLDEST_FIRST = 0x2
};

//An almost pointless class for storing information about messages that isn't covered by the NTCIP database.
//In this instance, just the time the message was created/edited.  But this may expand later, which is why
//I am making this class.
class HNS_Message_Info
{
public:
    HNS_Message_Info();
    HNS_Message_Info(const int64_t &time);

    void fSetTime(const int64_t &time) {f_time=time;}
    int64_t fGetTime() const {return f_time;}
private:
    int64_t f_time;
};

std::vector<size_t> SortMessageLog(const std::vector<HNS_Message_Info> &message_log, const int &sort_mode = HNS_MESSAGE_SORT_DATE_NEWEST_FIRST);

#endif // HNS_MESSAGE_INFO_H
