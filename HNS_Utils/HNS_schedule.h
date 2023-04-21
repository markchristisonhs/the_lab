#ifndef HNS_SCHEDULE_H
#define HNS_SCHEDULE_H

#include <vector>
#include <ctime>
#include "typedefs.h"
#include "message.h"
#include "structures.h"

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

typedef enum
{
    SCHED_ERROR_NONE = 0x0,
    SCHED_ERROR_END_BEFORE_START = 0x1,
    SCHED_ERROR_BEGINS_IN_PAST = 0x2,
    SCHED_ERROR_ENDS_IN_PAST = 0x4
} type_sched_error;

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
    void fSetTitle(const std::string &title);

    std::tm fGetStartDate() const;
    std::tm fGetEndDate() const;
    std::tm fGetStartTime() const;
    std::tm fGetStopTime() const;
    std::string fGetTitle() const;
    bool fIsActive() const;

    void fSetMessage(const type_message_type &message_type, const int &message_no);
    void fGetMessage(type_message_type &message_type, int &message_no) const;

    void fSetMessage(const HNS_Message2 &message);
    void fGetMessage(HNS_Message2 &message) const;
    void fSetStartNow(const bool &startNow);
    void fSetNeverEnd(const bool &neverEnd);
    bool fStartNow() const;
    bool fNeverEnd() const;

    void fSetOverrideRadar(const bool &override);
    bool fGetOverrideRadar() const {return f_override_radar;}

    void fSetOverrideStrobe(const bool &override);
    bool fGetOverrideStrobe() const {return f_override_strobe;}

    void fSetStrobeSettings(const HNS_Radar_Strobe &strobe_settings);
    HNS_Radar_Strobe fGetStrobeSettings() const {return f_strobe_settings;}

    void fSetRadarSettings(const HNS_Radar_Settings &settings) {f_radar_settings = settings;}
    HNS_Radar_Settings fGetRadarSettings() const {return f_radar_settings;}

    static bool fsIsScheduleValid(const HNS_Schedule &schedule, const std::tm &compare_date, type_sched_error &error);
private:
    std::vector<unsigned char> f_week;

    std::tm f_start_date;
    std::tm f_end_date;
    std::tm f_start;
    std::tm f_stop;

    bool f_active;
    bool f_suspend;

    type_message_type f_message_type;
    int f_message_no;

    HNS_Message2 f_scheduled_message;
    std::string f_title;
    bool f_startNow;
    bool f_neverEnd;

    //scheduled radar modes; experimental
    //If this schedule overrides the radar settings, or leaves them as is.
    bool f_override_radar;
    //If this schedule overrides the strobe settings or leaves them as is.
    bool f_override_strobe;
    HNS_Radar_Settings f_radar_settings;
    HNS_Radar_Strobe f_strobe_settings;
};

bool IsDateInSchedule(const HNS_Schedule &schedule, const int64_t &current_date);

#endif // SCHEDULE_H
