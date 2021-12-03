#ifndef HNS_NTCIP_DATABASE_H
#define HNS_NTCIP_DATABASE_H

#include "NTCIP.h"

//this function checks an NTCIP tree and makes sure it has all the nodes needed for an H&S Connect Controller and set any relavent defaults
void VerifyNTCIPDatabase(NTCIP_Node *tree);

#endif // HNS_NTCIP_DATABASE_H
