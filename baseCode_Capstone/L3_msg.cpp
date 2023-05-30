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

uint8_t L3_CONREQ_encodeData(uint8_t* msg, uint8_t seq)
{
    msg[L3_MSG_OFFSET_TYPE] = L3_MSG_TYPE_CONREQ;
    msg[L3_MSG_OFFSET_SYN] = seq;
    msg[L3_OFFSET_DATA] = 1;

    return MSG_SIZE;
}

uint8_t L3_CONCNF_encodeData(uint8_t* msg, uint8_t* data, int seq, int len)
{
    msg[L3_MSG_OFFSET_TYPE] = L3_MSG_TYPE_CONCNF;
    msg[L3_MSG_OFFSET_SYN] = seq;
    memcpy(&msg[L3_OFFSET_DATA], data, len*sizeof(uint8_t));

    return len+L3_OFFSET_DATA;
}

uint8_t L3_DISREQ_encodeData(uint8_t* msg, uint8_t seq)
{
    msg[L3_MSG_OFFSET_TYPE] = L3_MSG_TYPE_DISREQ;
    msg[L3_MSG_OFFSET_SYN] = seq;
    msg[L3_OFFSET_DATA] = 1;

    return MSG_SIZE;
}


uint8_t L3_DISCNF_encodeData(uint8_t* msg, uint8_t* data, int seq, int len)
{
    msg[L3_MSG_OFFSET_TYPE] = L3_MSG_TYPE_DISCNF;
    msg[L3_MSG_OFFSET_SYN] = seq;
    memcpy(&msg[L3_OFFSET_DATA], data, len*sizeof(uint8_t));

    return len+L3_OFFSET_DATA;
}