#include "mbed.h"

#define L3_MSG_OFFSET_TYPE              0
#define L3_MSG_OFFSET_USER_ID           1
#define L3_MSG_OFFSET_SYN               2

#define L3_MSG_TYPE_CONREQ              0
#define L3_MSG_TYPE_CONCNF              1
#define L3_MSG_TYPE_DISREQ              2
#define L3_MSG_TYPE_DISCNF              3
#define L3_MSG_TYPE_KEYINPUT            4
#define L3_MSG_TYPE_TIMEOUT             5

int L3_checkIfCallOnReq(uint8_t* msg);
int L3_checkIfCallOnCnf(uint8_t* msg);
int L3_checkIfCallOffReq(uint8_t* msg);
int L3_checkIfCallOffCnf(uint8_t* msg);
