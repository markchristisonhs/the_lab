#include <HNS_NTCIP_Database.h>
#include <HNS_NTCIP_MIB_Strings.h>
#include <HNS_NTCIP_Constants.h>

#include <sstream>
#include <string>

using namespace std;

void VerifyNTCIPDatabase_Schedule(NTCIP_Node *tree);
void VerifyNTCIPDatabase_Root(NTCIP_Node *tree);

void VerifyNTCIPDatabase(NTCIP_Node *tree)
{
    VerifyNTCIPDatabase_Root(tree);
    VerifyNTCIPDatabase_Schedule(tree);
}

void VerifyNTCIPDatabase_Root(NTCIP_Node *tree)
{
    if(!tree->fDoesChildExist(gk_iso))
    {
        tree->fAddChild(gk_iso,"iso");
    }
}

void VerifyNTCIPDatabase_Schedule(NTCIP_Node *tree)
{
    stringstream ss;

    //Start with timebase table.
    if(!tree->fDoesChildExist(gk_maxTimeBaseScheduleEntries))
    {
        tree->fAddChild(gk_maxTimeBaseScheduleEntries,"maxTimeBaseScheduleEntries");
    }
    tree->fSetData(gk_maxTimeBaseScheduleEntries,gk_max_timebase_schedules,HNS_NTCIP_READ,HNS_NTCIP_INTEGER);

    if(!tree->fDoesChildExist(gk_timeBaseScheduleTable))
    {
        tree->fAddChild(gk_timeBaseScheduleTable,"timeBaseScheduleTable");
    }

    if(!tree->fDoesChildExist(gk_timeBaseScheduleEntry))
    {
        tree->fAddChild(gk_timeBaseScheduleEntry,"timeBaseScheduleEntry");
    }

    if(!tree->fDoesChildExist(gk_timeBaseScheduleNumber))
    {
        tree->fAddChild(gk_timeBaseScheduleNumber,"timeBaseScheduleNumber");
    }

    if(!tree->fDoesChildExist(gk_timeBaseScheduleMonth))
    {
        tree->fAddChild(gk_timeBaseScheduleMonth,"timeBaseScheduleMonth");
    }

    if(!tree->fDoesChildExist(gk_timeBaseScheduleDay))
    {
        tree->fAddChild(gk_timeBaseScheduleDay,"timeBaseScheduleDay");
    }

    if(!tree->fDoesChildExist(gk_timeBaseScheduleDate))
    {
        tree->fAddChild(gk_timeBaseScheduleDate,"timeBaseScheduleDate");
    }

    if(!tree->fDoesChildExist(gk_timeBaseScheduleDayPlan))
    {
        tree->fAddChild(gk_timeBaseScheduleDayPlan,"timeBaseScheduleDayPlan");
    }

    for(size_t i=0;i<gk_max_timebase_schedules;i++)
    {
        ss.str(string());
        ss << gk_timeBaseScheduleNumber << "." << i+1;
        if(!tree->fDoesChildExist(ss.str()))
        {
            tree->fAddChild(ss.str(),"sequence");
        }
        tree->fSetData(ss.str(),i+1,HNS_NTCIP_READ,HNS_NTCIP_INTEGER);

        ss.str(string());
        ss << gk_timeBaseScheduleMonth << "." << i+1;
        if(!tree->fDoesChildExist(ss.str()))
        {
            tree->fAddChild(ss.str(),"sequence");
            tree->fSetData(ss.str(),0,HNS_NTCIP_READWRITE,HNS_NTCIP_INTEGER);
        }

        ss.str(string());
        ss << gk_timeBaseScheduleDay << "." << i+1;
        if(!tree->fDoesChildExist(ss.str()))
        {
            tree->fAddChild(ss.str(),"sequence");
            tree->fSetData(ss.str(),0,HNS_NTCIP_READWRITE,HNS_NTCIP_INTEGER);
        }

        ss.str(string());
        ss << gk_timeBaseScheduleDate << "." << i+1;
        if(!tree->fDoesChildExist(ss.str()))
        {
            tree->fAddChild(ss.str(),"sequence");
            tree->fSetData(ss.str(),0,HNS_NTCIP_READWRITE,HNS_NTCIP_INTEGER);
        }

        ss.str(string());
        ss << gk_timeBaseScheduleDayPlan << "." << i+1;
        if(!tree->fDoesChildExist(ss.str()))
        {
            tree->fAddChild(ss.str(),"sequence");
            tree->fSetData(ss.str(),0,HNS_NTCIP_READWRITE,HNS_NTCIP_INTEGER);
        }
    }

    //now for dayplan
    if(!tree->fDoesChildExist(gk_maxDayPlans))
    {
        tree->fAddChild(gk_maxDayPlans,"maxDayPlans");
    }
    tree->fSetData(gk_maxDayPlans,gk_max_dayplans,HNS_NTCIP_READ,HNS_NTCIP_INTEGER);

    if(!tree->fDoesChildExist(gk_maxDayPlanEvents))
    {
        tree->fAddChild(gk_maxDayPlanEvents,"maxDayPlanEvents");
    }
    tree->fSetData(gk_maxDayPlanEvents,gk_max_dayplan_events,HNS_NTCIP_READ,HNS_NTCIP_INTEGER);

    if(!tree->fDoesChildExist(gk_timeBaseDayPlanTable))
    {
        tree->fAddChild(gk_timeBaseDayPlanTable,"timeBaseDayPlanTable");
    }

    if(!tree->fDoesChildExist(gk_timeBaseDayPlanEntry))
    {
        tree->fAddChild(gk_timeBaseDayPlanEntry,"timeBaseDayPlanEntry");
    }

    if(!tree->fDoesChildExist(gk_dayPlanNumber))
    {
        tree->fAddChild(gk_dayPlanNumber,"dayPlanNumber");
    }

    if(!tree->fDoesChildExist(gk_dayPlanEventNumber))
    {
        tree->fAddChild(gk_dayPlanEventNumber);
    }

    if(!tree->fDoesChildExist(gk_dayPlanHour))
    {
        tree->fAddChild(gk_dayPlanHour,"dayPlanHour");
    }

    if(!tree->fDoesChildExist(gk_dayPlanMinute))
    {
        tree->fAddChild(gk_dayPlanMinute,"dayPlanMinute");
    }

    if(!tree->fDoesChildExist(gk_dayPlanActionNumberOID))
    {
        tree->fAddChild(gk_dayPlanActionNumberOID,"dayPlanActionNumberOID");
    }

    for(size_t i=0;i<gk_max_dayplans;i++)
    {
        ss.str(string());
        ss << gk_dayPlanNumber << "." << i+1;
        if(!tree->fDoesChildExist(ss.str()))
        {
            tree->fAddChild(ss.str(),"sequence");
        }

        ss.str(string());
        ss << gk_dayPlanEventNumber << "." << i+1;
        if(!tree->fDoesChildExist(ss.str()))
        {
            tree->fAddChild(ss.str(),"sequence");
        }

        ss.str(string());
        ss << gk_dayPlanHour << "." << i+1;
        if(!tree->fDoesChildExist(ss.str()))
        {
            tree->fAddChild(ss.str(),"sequence");
        }

        ss.str(string());
        ss << gk_dayPlanMinute << "." << i+1;
        if(!tree->fDoesChildExist(ss.str()))
        {
            tree->fAddChild(ss.str(),"sequence");
        }

        ss.str(string());
        ss << gk_dayPlanActionNumberOID << "." << i+1;
        if(!tree->fDoesChildExist(ss.str()))
        {
            tree->fAddChild(ss.str(),"sequence");
        }

        for(size_t j=0;j<gk_max_dayplan_events;j++)
        {
            ss.str(string());
            ss << gk_dayPlanNumber << "." << i+1 << "." << j+1;
            if(!tree->fDoesChildExist(ss.str()))
            {
                tree->fAddChild(ss.str(),"sequence");
            }
            tree->fSetData(ss.str(),i+1,HNS_NTCIP_READ,HNS_NTCIP_INTEGER);

            ss.str(string());
            ss << gk_dayPlanEventNumber << "." << i+1 << "." << j+1;
            if(!tree->fDoesChildExist(ss.str()))
            {
                tree->fAddChild(ss.str(),"seuqnece");
            }
            tree->fSetData(ss.str(),j+1,HNS_NTCIP_READ,HNS_NTCIP_INTEGER);

            ss.str(string());
            ss << gk_dayPlanHour << "." << i+1 << "." << j+1;
            if(!tree->fDoesChildExist(ss.str()))
            {
                tree->fAddChild(ss.str(),"sequence");
                tree->fSetData(ss.str(),0,HNS_NTCIP_READWRITE,HNS_NTCIP_INTEGER);
            }

            ss.str(string());
            ss << gk_dayPlanMinute << "." << i+1 << "." << j+1;
            if(!tree->fDoesChildExist(ss.str()))
            {
                tree->fAddChild(ss.str(),"sequence");
                tree->fSetData(ss.str(),0,HNS_NTCIP_READWRITE,HNS_NTCIP_INTEGER);
            }

            ss.str(string());
            ss << gk_dayPlanActionNumberOID << "." << i+1 << "." << j+1;
            if(!tree->fDoesChildExist(ss.str()))
            {
                tree->fAddChild(ss.str(),"sequence");
                tree->fSetData(ss.str(),vector<unsigned char>(1,0x00),HNS_NTCIP_READWRITE,HNS_NTCIP_OCTETSTRING,true);
            }
        }
    }

    if(!tree->fDoesChildExist(gk_dayPlanStatus))
    {
        tree->fAddChild(gk_dayPlanStatus,"dayPlanStatus");
        tree->fSetData(gk_dayPlanStatus,0,HNS_NTCIP_READ,HNS_NTCIP_INTEGER);
    }

    if(!tree->fDoesChildExist(gk_timeBaseScheduleTableStatus))
    {
        tree->fAddChild(gk_timeBaseScheduleTableStatus,"timeBaseScheduleTableStatus");
        tree->fSetData(gk_timeBaseScheduleTableStatus,0,HNS_NTCIP_READ,HNS_NTCIP_INTEGER);
    }

    //Set 1203 section 5.9 portion
    if(!tree->fDoesChildExist(gk_numActionTableEntries))
    {
        tree->fAddChild(gk_numActionTableEntries,"numActionTableEntries");
    }
    tree->fSetData(gk_numActionTableEntries,gk_max_actiontable_entries,HNS_NTCIP_READ,HNS_NTCIP_INTEGER);

    for(int i=0;i<gk_max_actiontable_entries;i++)
    {
        ss.str(string());
        ss << gk_dmsActionIndex << "." << i+1;
        if(!tree->fDoesChildExist(ss.str()))
        {
            tree->fAddChild(ss.str(),"sequence");
        }
        tree->fSetData(ss.str(),i+1,HNS_NTCIP_READ,HNS_NTCIP_INTEGER);

        ss.str(string());
        ss << gk_dmsActionMsgCode << "." << i+1;
        if(!tree->fDoesChildExist(ss.str()))
        {
            tree->fAddChild(ss.str(),"sequence");
            tree->fSetData(ss.str(),vector<unsigned char>(12,0),HNS_NTCIP_READWRITE,HNS_NTCIP_OCTETSTRING,true);
        }
    }
}
