#include "HNS_schedule.h"
#include <time.h>
#include "utilities.h"

HNS_Schedule::HNS_Schedule():
    f_active(false),
    f_suspend(false),
    f_message_type(HNS_MESSAGE_TYPE_FACTORY),
    f_message_no(0),
    f_title("")
{
    int i;
    for(i=0;i<7;i++)
    {
        f_week[i] = false;
    }
}

void HNS_Schedule::fSetSuspend()
{
    f_suspend = true;
}

void HNS_Schedule::fClearSuspend()
{
    f_suspend = false;
}

bool HNS_Schedule::fGetSuspend() const
{
    return f_suspend;
}

char HNS_Schedule::fGetDayChar(const int &day) const
{
    char result = 'X';
    if(day >= HNS_SCHED_SUNDAY && day <= HNS_SCHED_SATURDAY)
    {
        if(f_week[day])
        {
            switch(day)
            {
            case HNS_SCHED_SUNDAY:
                result = 'U';
                break;
            case HNS_SCHED_MONDAY:
                result = 'M';
                break;
            case HNS_SCHED_TUESDAY:
                result = 'T';
                break;
            case HNS_SCHED_WEDNESDAY:
                result = 'W';
                break;
            case HNS_SCHED_THURSDAY:
                result = 'R';
                break;
            case HNS_SCHED_FRIDAY:
                result = 'F';
                break;
            case HNS_SCHED_SATURDAY:
                result = 'S';
                break;
            }
        }
    }
    return result;
}

bool HNS_Schedule::fGetDay(const int &day) const
{
    if(day >= HNS_SCHED_SUNDAY && day <= HNS_SCHED_SATURDAY)
    {
        return f_week[day];
    }
    return false;
}

void HNS_Schedule::fSetDay(const int &day)
{
    if(day >= HNS_SCHED_SUNDAY && day <= HNS_SCHED_SATURDAY)
    {
        f_week[static_cast<size_t>(day)] = true;
    }
}

void HNS_Schedule::fClearDay(const int &day)
{
    if(day >= HNS_SCHED_SUNDAY && day <= HNS_SCHED_SATURDAY)
    {
        f_week[static_cast<size_t>(day)] = false;
    }
}

void HNS_Schedule::fToggleDay(const int &day)
{
    if(day >= HNS_SCHED_SUNDAY && day <= HNS_SCHED_SATURDAY)
    {
        f_week[static_cast<size_t>(day)] = !f_week[static_cast<size_t>(day)];
    }
}

void HNS_Schedule::fSetWeekdays()
{
    f_week[HNS_SCHED_SUNDAY] = false;
    f_week[HNS_SCHED_SATURDAY] = false;

    f_week[HNS_SCHED_MONDAY] = true;
    f_week[HNS_SCHED_TUESDAY] = true;
    f_week[HNS_SCHED_WEDNESDAY] = true;
    f_week[HNS_SCHED_THURSDAY] = true;
    f_week[HNS_SCHED_FRIDAY] = true;
}

void HNS_Schedule::fSetWeekends()
{
    f_week[HNS_SCHED_SUNDAY] = true;
    f_week[HNS_SCHED_SATURDAY] = true;

    f_week[HNS_SCHED_MONDAY] = false;
    f_week[HNS_SCHED_TUESDAY] = false;
    f_week[HNS_SCHED_WEDNESDAY] = false;
    f_week[HNS_SCHED_THURSDAY] = false;
    f_week[HNS_SCHED_FRIDAY] = false;
}

void HNS_Schedule::fSetAll()
{
    f_week[HNS_SCHED_SUNDAY] = true;
    f_week[HNS_SCHED_SATURDAY] = true;

    f_week[HNS_SCHED_MONDAY] = true;
    f_week[HNS_SCHED_TUESDAY] = true;
    f_week[HNS_SCHED_WEDNESDAY] = true;
    f_week[HNS_SCHED_THURSDAY] = true;
    f_week[HNS_SCHED_FRIDAY] = true;
}

void HNS_Schedule::fClearAll()
{
    f_week[HNS_SCHED_SUNDAY] = false;
    f_week[HNS_SCHED_SATURDAY] = false;

    f_week[HNS_SCHED_MONDAY] = false;
    f_week[HNS_SCHED_TUESDAY] = false;
    f_week[HNS_SCHED_WEDNESDAY] = false;
    f_week[HNS_SCHED_THURSDAY] = false;
    f_week[HNS_SCHED_FRIDAY] = false;
}

void HNS_Schedule::fSetStartDate(const std::tm &date)
{
    f_start_date = date;
}

void HNS_Schedule::fSetEndDate(const std::tm &date)
{
    f_end_date = date;
}

void HNS_Schedule::fSetStartTime(const std::tm &start)
{
    f_start = start;
}

void HNS_Schedule::fSetStopTime(const std::tm &stop)
{
    f_stop = stop;
}

void HNS_Schedule::fSetTitle(const std::string &title)
{
    f_title = title;
}

void HNS_Schedule::fSetActive(const bool &active)
{
    f_active = active;
}

void HNS_Schedule::fSetActive()
{
    f_active = true;
}

void HNS_Schedule::fClearActive()
{
    f_active = false;
}

void HNS_Schedule::fToggleActive()
{
    f_active = !f_active;
}

std::tm HNS_Schedule::fGetStartDate() const
{
    return f_start_date;
}

std::tm HNS_Schedule::fGetEndDate() const
{
    return f_end_date;
}

std::tm HNS_Schedule::fGetStartTime() const
{
    return f_start;
}

std::tm HNS_Schedule::fGetStopTime() const
{
    return f_stop;
}

std::string HNS_Schedule::fGetTitle() const
{
    return f_title;
}

bool HNS_Schedule::fIsActive() const
{
    return f_active;
}

void HNS_Schedule::fSetMessage(const type_message_type &message_type, const int &message_no)
{
    f_message_type = message_type;
    f_message_no = message_no;
}

void HNS_Schedule::fGetMessage(type_message_type &message_type, int &message_no) const
{
    message_type = f_message_type;
    message_no = f_message_no;
}

void HNS_Schedule::fSetMessage(const HNS_Message2 &message)
{
    f_scheduled_message = message;
}

void HNS_Schedule::fGetMessage(HNS_Message2 &message) const
{
    message = f_scheduled_message;
}

void HNS_Schedule::fSetStartNow(const bool &startNow)
{
    f_startNow = startNow;
}
void HNS_Schedule::fSetNeverEnd(const bool &neverEnd)
{
    f_neverEnd = neverEnd;
}
bool HNS_Schedule::fStartNow()
{
    return f_startNow;
}
bool HNS_Schedule::fNeverEnd()
{
    return f_neverEnd;
}

bool IsDateInSchedule(const HNS_Schedule &schedule, const int64_t &current_date)
{
    time_t temp_time = current_date/1000;
    tm *current_tm = localtime(&temp_time), start_tm, end_tm;

    start_tm = schedule.fGetStartDate();
    end_tm = schedule.fGetEndDate();

    //is in date range
    if((CompareDates(*current_tm,start_tm) >= 0)
    && (CompareDates(end_tm,*current_tm) >= 0))
    {
        start_tm = schedule.fGetStartTime();
        end_tm = schedule.fGetStopTime();

        int temp1 = CompareTimes(*current_tm,start_tm);
        int temp2 = CompareTimes(end_tm,*current_tm);
        //is in time range
        if((CompareTimes(*current_tm,start_tm) >= 0)
        && (CompareTimes(end_tm,*current_tm) >= 0))
        {
            return schedule.fGetDay(current_tm->tm_wday);
        }
    }

    return false;
}
