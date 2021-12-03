#ifndef HNS_NTCIP_SCHEDULER_H
#define HNS_NTCIP_SCHEDULER_H

#include <stdint.h>
#include "NTCIP.h"
#include "HNS_NTCIP_Datatypes.h"

HNS_NTCIP_MessageActivationCode CheckForScheduleLast24hrs(const int64_t &time, NTCIP_Node *tree, bool &active_schedule);

//find if an event is active for the given time
HNS_NTCIP_MessageActivationCode FindActiveEvent(const int64_t &time, NTCIP_Node *tree, bool &active_schedule);

void SetupScheduleTest(NTCIP_Node *tree);

#endif // HNS_NTCIP_SCHEDULER_H
