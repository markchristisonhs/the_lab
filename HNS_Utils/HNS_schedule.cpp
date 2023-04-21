#include "HNS_schedule.h"
#include <time.h>
#include "utilities.h"

HNS_Schedule::HNS_Schedule():
    f_week(7,0),
    f_active(false),
    f_suspend(false),
    f_message_type(HNS_MESSAGE_TYPE_FACTORY),
    f_message_no(0),
    f_title(""),
    f_startNow(true),
    f_neverEnd(true),
    f_override_radar(false),
    f_override_strobe(false)
{
    std::time_t t = std::time(0);
    f_start_date = *std::localtime(&t);
    f_end_date = *std::localtime(&t);
    f_start = *std::localtime(&t);
    f_stop = *std::localtime(&t);
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
        f_week[static_cast<size_t>(day)] = 1;
    }
}

void HNS_Schedule::fClearDay(const int &day)
{
    if(day >= HNS_SCHED_SUNDAY && day <= HNS_SCHED_SATURDAY)
    {
        f_week[static_cast<size_t>(day)] = 0;
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
bool HNS_Schedule::fStartNow() const
{
    return f_startNow;
}
bool HNS_Schedule::fNeverEnd() const
{
    return f_neverEnd;
}

void HNS_Schedule::fSetOverrideRadar(const bool &override)
{
    f_override_radar = override;
}

void HNS_Schedule::fSetOverrideStrobe(const bool &override)
{
    f_override_strobe = override;
}

void HNS_Schedule::fSetStrobeSettings(const HNS_Radar_Strobe &strobe_settings)
{
    f_strobe_settings = strobe_settings;
}

bool HNS_Schedule::fsIsScheduleValid(const HNS_Schedule &schedule, const std::tm &compare_date, type_sched_error &error)
{
    error = SCHED_ERROR_NONE;
    int itemp;

    std::tm start_date = schedule.fGetStartDate();
    std::tm end_date = schedule.fGetEndDate();

    start_date.tm_hour = schedule.fGetStartTime().tm_hour;
    start_date.tm_min = schedule.fGetStartTime().tm_min;
    start_date.tm_sec = schedule.fGetStartTime().tm_sec;

    end_date.tm_hour = schedule.fGetStopTime().tm_hour;
    end_date.tm_min = schedule.fGetStopTime().tm_min;
    end_date.tm_sec = schedule.fGetStopTime().tm_sec;

    if(CompareDates(end_date,start_date) < 0)
    {
        itemp = error;
        itemp = itemp | SCHED_ERROR_END_BEFORE_START;
        error = static_cast<type_sched_error>(itemp);
    }

    if(CompareDates(start_date,compare_date) < 0)
    {
        itemp = error;
        itemp = itemp | SCHED_ERROR_BEGINS_IN_PAST;
        error = static_cast<type_sched_error>(itemp);
    }
    else if(CompareDates(start_date,compare_date) == 0)
    {
        if(CompareTimes(start_date,compare_date))
        {
            itemp = error;
            itemp = itemp | SCHED_ERROR_BEGINS_IN_PAST;
        }
    }

    if(CompareDates(end_date,compare_date) < 0)
    {
        itemp = error;
        itemp = itemp | SCHED_ERROR_ENDS_IN_PAST;
        error = static_cast<type_sched_error>(itemp);
    }
    else if(CompareDates(end_date,compare_date) == 0)
    {
        if(CompareTimes(end_date,compare_date))
        {
            itemp = error;
            itemp = itemp | SCHED_ERROR_ENDS_IN_PAST;
            error = static_cast<type_sched_error>(itemp);
        }
    }

    return error == SCHED_ERROR_NONE;
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

        //is in time range
        if((CompareTimes(*current_tm,start_tm) >= 0)
        && (CompareTimes(end_tm,*current_tm) >= 0))
        {
            return schedule.fGetDay(current_tm->tm_wday);
        }
    }

    return false;
}
