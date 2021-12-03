#include "HNS_NTCIP_Scheduler.h"
#include "HNS_NTCIP_MIB_Strings.h"
#include "HNS_NTCIP_Enums.h"
#include <sstream>
#include <time.h>

using namespace std;
using namespace NTCIP_MESSAGE;

//is the current date allowed by the current timebase schedule?  timebase may specify more than one month, day of week, and day of month.
//See NTCIP 1201 section 2.4.3.2
bool CompareMonths(struct tm *time_in, const int &month);
bool CompareDOW(struct tm *time_in, const int &dow);
bool CompareDOM(struct tm *time_in, const int &dom);
bool CompareDate(struct tm *time_in, const int &month, const int &dow, const int &dom);

enum
{
    SCHED_JANUARY = 0x2,
    SCHED_FEBRUARY = 0x4,
    SCHED_MARCH = 0x8,
    SCHED_APRIL = 0x10,
    SCHED_MAY = 0x20,
    SCHED_JUNE = 0x40,
    SCHED_JULY = 0x80,
    SCHED_AUGUST = 0x100,
    SCHED_SEPTEMBER = 0x200,
    SCHED_OCTOBER = 0x400,
    SCHED_NOVEMBER = 0x800,
    SCHED_DECEMBER = 0x1000,
    SCHED_ALL_MONTHS = 0x1FFE
};

enum
{
    SCHED_SUNDAY = 0x2,
    SCHED_MONDAY = 0x4,
    SCHED_TUESDAY = 0x8,
    SCHED_WEDNESDAY = 0x10,
    SCHED_THURSDAY = 0x20,
    SCHED_FRIDAY = 0x40,
    SCHED_SATURDAY = 0x80,
    SCHED_ALL_DOW = 0xFE,
    SCHED_ALL_WEEKDAYS = 0x76,
    SHCED_ALL_WEEKEND = 0x82
};

enum
{
    SCHED_DOM_1 = 0x2,
    SCHED_DOM_2 = 0x4,
    SCHED_DOM_3 = 0x8,
    SCHED_DOM_4 = 0x10,
    SCHED_DOM_5 = 0x20,
    SCHED_DOM_6 = 0x40,
    SCHED_DOM_7 = 0x80,
    SCHED_DOM_8 = 0x100,
    SCHED_DOM_9 = 0x200,
    SCHED_DOM_10 = 0x400,
    SCHED_DOM_11 = 0x800,
    SCHED_DOM_12 = 0x1000,
    SCHED_DOM_13 = 0x2000,
    SCHED_DOM_14 = 0x4000,
    SCHED_DOM_15 = 0x8000,
    SCHED_DOM_16 = 0x10000,
    SCHED_DOM_17 = 0x20000,
    SCHED_DOM_18 = 0x40000,
    SCHED_DOM_19 = 0x80000,
    SCHED_DOM_20 = 0x100000,
    SCHED_DOM_21 = 0x200000,
    SCHED_DOM_22 = 0x400000,
    SCHED_DOM_23 = 0x800000,
    SCHED_DOM_24 = 0x1000000,
    SCHED_DOM_25 = 0x2000000,
    SCHED_DOM_26 = 0x4000000,
    SCHED_DOM_27 = 0x8000000,
    SCHED_DOM_28 = 0x10000000,
    SCHED_DOM_29 = 0x20000000,
    SCHED_DOM_30 = 0x40000000,
    SCHED_DOM_31 = 0x80000000,
    SCHED_ALL_DOM = 0xFFFFFFFE
};

HNS_NTCIP_MessageActivationCode CheckForScheduleLast24hrs(const int64_t &time, NTCIP_Node *tree, bool &active_schedule)
{
    HNS_NTCIP_MessageActivationCode act_code;
    HNS_NTCIP_OID target_oid;
    int num_timebase_entries/*, num_dayplan_entries*/, num_dayplan_event_entries;
    int month,dow,dom,dayplan;
    int hour, minute;
    struct tm local, local_yesterday;
    int64_t time_seconds = time / 1000;
    time_t time_seconds2 = time_seconds;
    stringstream ss;
    bool found = false;
    active_schedule = false;
    type_ntcip_error ntcip_error;

    bool found_today = false, found_yesterday = false;

    localtime_r(&time_seconds2,&local);
    time_seconds = time_seconds - 86400;
    localtime_r(&time_seconds2,&local_yesterday);

    num_timebase_entries = tree->fGetIntData(gk_maxTimeBaseScheduleEntries);

    for(int i=0;i<num_timebase_entries;i++)
    {
        dayplan = 0;
        dom = 0;
        dow = 0;
        month = 0;

        //get timebase schedule
        ss.str(string());
        ss << gk_timeBaseScheduleMonth << "." << i+1;
        month = tree->fGetIntData(ss.str());

        ss.str(string());
        ss << gk_timeBaseScheduleDay << "." << i+1;
        dow = tree->fGetIntData(ss.str());

        ss.str(string());
        ss << gk_timeBaseScheduleDate << "." << i+1;
        dom = tree->fGetIntData(ss.str());

        ss.str(string());
        ss << gk_timeBaseScheduleDayPlan << "." << i+1;
        dayplan = tree->fGetIntData(ss.str());

        if((month == 0)
        || (dow == 0)
        || (dom == 0)
        || (dayplan == 0))
        {
            //timebase schedule is disabled
            continue;
        }

        if((found_today = CompareDate(&local,month,dow,dom)) || (found_yesterday = CompareDate(&local_yesterday,month,dow,dom)))
        {
            //A plan today.  Is it earlier than now?
            num_dayplan_event_entries = tree->fGetIntData(gk_maxDayPlanEvents);

            for(int j=0;j<num_dayplan_event_entries;j++)
            {
                ss.str(string());
                ss << gk_dayPlanHour << "." << dayplan << "." << j+1;
                hour = tree->fGetIntData(ss.str());

                ss.str(string());
                ss << gk_dayPlanMinute << "." << dayplan << "." << j+1;
                minute = tree->fGetIntData(ss.str());

                ss.str(string());
                ss << gk_dayPlanActionNumberOID << "." << dayplan << "." << j+1;
                target_oid.fSetData(tree->fGetData(ss.str()));

                if(found_today)
                {
                    if((hour < local.tm_hour)
                    || ((hour == local.tm_hour) && (minute <= local.tm_min)))
                    {
                        //found an event to fire
                        act_code.fSetOctetString(tree->fGetData(target_oid.fGetOIDString(),vector<unsigned char> (),&ntcip_error));
                        if(ntcip_error == HNS_NTCIP_NOERROR)
                        {
                            active_schedule = found = true;
                        }
                    }
                }
                else if (found_yesterday)
                {
                    if((hour > local_yesterday.tm_hour)
                    || ((hour == local_yesterday.tm_hour) && (minute >= local_yesterday.tm_min)))
                    {
                        //found an event to fire
                        act_code.fSetOctetString(tree->fGetData(target_oid.fGetOIDString(),vector<unsigned char> (),&ntcip_error));
                        if(ntcip_error == HNS_NTCIP_NOERROR)
                        {
                            active_schedule = found = true;
                        }
                    }
                }
            }
        }

        if(found)
        {
            break;
        }
    }

    return act_code;
}

HNS_NTCIP_MessageActivationCode FindActiveEvent(const int64_t &time, NTCIP_Node *tree, bool &active_schedule)
{
    HNS_NTCIP_MessageActivationCode act_code;
    HNS_NTCIP_OID target_oid;
    type_ntcip_error ntcip_error;
    int num_timebase_entries, num_dayplan_entries, num_dayplan_event_entries;
    int month,dow,dom,dayplan;
    int hour, minute;
    struct tm *local;
    int64_t time_seconds = time / 1000;
    time_t time_seconds2 = time_seconds;
    stringstream ss;
    bool found = false;
    active_schedule = false;
    static int last_dayplan_fired = 0, last_dayplan_event_fired = 0;

    local = localtime(&time_seconds2);

    num_timebase_entries = tree->fGetIntData(gk_maxTimeBaseScheduleEntries);

    for(int i=0;i<num_timebase_entries;i++)
    {
        dayplan = 0;
        dom = 0;
        dow = 0;
        month = 0;

        //get timebase schedule
        ss.str(string());
        ss << gk_timeBaseScheduleMonth << "." << i+1;
        month = tree->fGetIntData(ss.str());

        ss.str(string());
        ss << gk_timeBaseScheduleDay << "." << i+1;
        dow = tree->fGetIntData(ss.str());

        ss.str(string());
        ss << gk_timeBaseScheduleDate << "." << i+1;
        dom = tree->fGetIntData(ss.str());

        ss.str(string());
        ss << gk_timeBaseScheduleDayPlan << "." << i+1;
        dayplan = tree->fGetIntData(ss.str());

        if((month == 0)
        || (dow == 0)
        || (dom == 0)
        || (dayplan == 0))
        {
            //timebase schedule is disabled
            continue;
        }

        if(CompareDate(local,month,dow,dom))
        {
            //matches timebase, now check dayplan
            num_dayplan_entries = tree->fGetIntData(gk_maxDayPlans);

            if(dayplan <= num_dayplan_entries)
            {
                num_dayplan_event_entries = tree->fGetIntData(gk_maxDayPlanEvents);

                for(int j=0;j<num_dayplan_event_entries;j++)
                {
                    ss.str(string());
                    ss << gk_dayPlanHour << "." << dayplan << "." << j+1;
                    hour = tree->fGetIntData(ss.str());

                    ss.str(string());
                    ss << gk_dayPlanMinute << "." << dayplan << "." << j+1;
                    minute = tree->fGetIntData(ss.str());

                    ss.str(string());
                    ss << gk_dayPlanActionNumberOID << "." << dayplan << "." << j+1;
                    target_oid.fSetData(tree->fGetData(ss.str()));

                    if((local->tm_hour == hour)
                    && (local->tm_min == minute))
                    {
                        if((last_dayplan_fired != dayplan) || ((last_dayplan_fired == dayplan) && (last_dayplan_event_fired != j+1)))
                        {
                            //found an event to fire
                            act_code.fSetOctetString(tree->fGetData(target_oid.fGetOIDString(),vector<unsigned char> (), &ntcip_error));
                            if(ntcip_error == HNS_NTCIP_NOERROR)
                            {
                                active_schedule = found = true;

                                last_dayplan_fired = dayplan;
                                last_dayplan_event_fired = j+1;
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                    else
                    {
                        if(last_dayplan_fired != 0)
                        {
                            last_dayplan_fired = 0;
                        }
                        if(last_dayplan_event_fired != 0)
                        {
                            last_dayplan_event_fired = 0;
                        }
                    }
                }
            }
        }

        if(found)
        {
            break;
        }
    }

    return act_code;
}

void SetupScheduleTest(NTCIP_Node *tree)
{
    stringstream ss;
    HNS_NTCIP_MessageActivationCode act_code(0xFFFF,255,MEM_TYPE_PERMANENT,1,vector<uint8_t>(2,0),"127.0.0.1");
    HNS_NTCIP_OID target_oid;

    ss << gk_dmsActionMsgCode << ".1";
    tree->fSetData(ss.str(),act_code.fGetOctetString(),HNS_NTCIP_READWRITE,HNS_NTCIP_OCTETSTRING,true);
    target_oid.fSetData(ss.str());

    ss.str(string());
    ss << gk_dayPlanHour << ".1.1";
    tree->fSetData(ss.str(),12,HNS_NTCIP_READWRITE,HNS_NTCIP_INTEGER);

    ss.str(string());
    ss << gk_dayPlanMinute << ".1.1";
    tree->fSetData(ss.str(),58,HNS_NTCIP_READWRITE,HNS_NTCIP_INTEGER);

    ss.str(string());
    ss << gk_dayPlanActionNumberOID << ".1.1";
    tree->fSetData(ss.str(),target_oid.fGetOIDOctetString(),HNS_NTCIP_READWRITE,HNS_NTCIP_OCTETSTRING,true);

    ss.str(string());
    ss << gk_timeBaseScheduleMonth << ".1";
    tree->fSetData(ss.str(),0,HNS_NTCIP_READWRITE,HNS_NTCIP_INTEGER);

    ss.str(string());
    ss << gk_timeBaseScheduleDay << ".1";
    tree->fSetData(ss.str(),0,HNS_NTCIP_READWRITE,HNS_NTCIP_INTEGER);

    ss.str(string());
    ss << gk_timeBaseScheduleDate << ".1";
    tree->fSetData(ss.str(),0,HNS_NTCIP_READWRITE,HNS_NTCIP_INTEGER);

    ss.str(string());
    ss << gk_timeBaseScheduleDayPlan << ".1";
    tree->fSetData(ss.str(),0,HNS_NTCIP_READWRITE,HNS_NTCIP_INTEGER);
}


bool CompareMonths(struct tm *time_in, const int &month)
{
    int month_byte = 0x2;
    month_byte = month_byte << time_in->tm_mon;

    return month_byte & month;
}

bool CompareDOW(struct tm *time_in, const int &dow)
{
    int dow_byte = 0x2;
    dow_byte = dow_byte << time_in->tm_wday;

    return dow_byte & dow;
}

bool CompareDOM(struct tm *time_in, const int &dom)
{
    int dom_byte = 0x1;
    dom_byte = dom_byte << time_in->tm_mday;

    return dom_byte & dom;
}

bool CompareDate(struct tm *time_in, const int &month, const int &dow, const int &dom)
{
    bool result = true;

    result = result && CompareMonths(time_in,month);
    result = result && CompareDOW(time_in,dow);
    result = result && CompareDOM(time_in,dom);

    return result;
}
