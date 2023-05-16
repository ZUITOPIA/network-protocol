#include "mbed.h"
#include "L3_msg.h"

int L3_checkIfConnectReq(uint8_t* msg)
{
    return (msg[L3_MSG_OFFSET_TYPE]==L3_MSG_TYPE_CONREQ)
}