#include "mbed.h"
#include "L3_msg.h"

int L3_checkIfCallOnReq(uint8_t* msg)
{
    return (msg[L3_MSG_OFFSET_TYPE]==L3_MSG_TYPE_CONREQ);
}

int L3_checkIfCallOnCnf(uint8_t* msg)
{
    return (msg[L3_MSG_OFFSET_TYPE]==L3_MSG_TYPE_CONCNF);
}

int L3_checkIfCallOffReq(uint8_t* msg)
{
    return (msg[L3_MSG_OFFSET_TYPE]==L3_MSG_TYPE_DISREQ);
}

int L3_checkIfCallOffCnf(uint8_t* msg)
{
    return (msg[L3_MSG_OFFSET_TYPE]==L3_MSG_TYPE_DISCNF);
}
int L3_checkIfCallOffCnf(uint8_t* msg)
{
    return (msg[L3_MSG_OFFSET_TYPE]==L3_MSG_TYPE_KEYINPUT);
}
int L3_checkIfCallOffCnf(uint8_t* msg)
{
    return (msg[L3_MSG_OFFSET_TYPE]==L3_MSG_TYPE_TIMEOUT);
}