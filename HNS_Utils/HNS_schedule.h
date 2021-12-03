#ifndef HNS_SCHEDULE_H
#define HNS_SCHEDULE_H

#include <vector>
#include <ctime>
#include "typedefs.h"
#include "message.h"

enum
{
    HNS_SCHED_SUNDAY = 0,
    HNS_SCHED_MONDAY,
    HNS_SCHED_TUESDAY,
    HNS_SCHED_WEDNESDAY,
    HNS_SCHED_THURSDAY,
    HNS_SCHED_FRIDAY,
    HNS_SCHED_SATURDAY,
};

class HNS_Schedule
{
public:
    HNS_Schedule();

    void fSetSuspend();
    void fClearSuspend();
    bool fGetSuspend() const;

    char fGetDayChar(const int &day) const;
    bool fGetDay(const int &day) const;
    void fSetDay(const int &day);
    void fClearDay(const int &day);
    void fToggleDay(const int &day);
    void fSetWeekdays();
    void fSetWeekends();
    void fSetAll();
    void fClearAll();

    void fSetActive(const bool &active);
    void fSetActive();
    void fClearActive();
    void fToggleActive();

    void fSetStartDate(const std::tm &date);
    void fSetEndDate(const std::tm &date);
    void fSetStartTime(const std::tm &start);
    void fSetStopTime(const std::tm &stop);

    std::tm fGetStartDate() const;
    std::tm fGetEndDate() const;
    std::tm fGetStartTime() const;
    std::tm fGetStopTime() const;
    bool fIsActive() const;

    void fSetMessage(const type_message_type &message_type, const int &message_no);
    void fGetMessage(type_message_type &message_type, int &message_no) const;

    void fSetMessage(const HNS_Message2 &message);
    void fGetMessage(HNS_Message2 &message) const;
private:
    bool f_week[7];

    //std::tm f_date;
    std::tm f_start_date;
    std::tm f_end_date;
    std::tm f_start;
    std::tm f_stop;

    bool f_active;
    bool f_suspend;

    type_message_type f_message_type;
    int f_message_no;

    HNS_Message2 f_scheduled_message;
};

bool IsDateInSchedule(const HNS_Schedule &schedule, const int64_t &current_date);

#endif // SCHEDULE_H
