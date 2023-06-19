#include "hns_message_info.h"
#include <numeric>
#include <algorithm>

using namespace std;

HNS_Message_Info::HNS_Message_Info() :
    f_time(0)
{

}

HNS_Message_Info::HNS_Message_Info(const int64_t &time) :
    f_time(time)
{

}

vector<size_t> SortMessageLog(const vector<HNS_Message_Info> &message_log, const int &sort_mode)
{
    vector<size_t> idx(message_log.size());
    iota(idx.begin(),idx.end(),0);

    if(sort_mode == HNS_MESSAGE_SORT_DATE_NEWEST_FIRST)
    {
        stable_sort(idx.begin(),idx.end(),
                    [&message_log](size_t i1, size_t i2) {return message_log[i1].fGetTime() > message_log[i2].fGetTime();});
    }
    else if(sort_mode == HNS_MESSAGE_SORT_DATE_OLDEST_FIRST)
    {
        stable_sort(idx.begin(),idx.end(),
                    [&message_log](size_t i1, size_t i2) {return message_log[i1].fGetTime() < message_log[i2].fGetTime();});
    }

    return idx;
}
