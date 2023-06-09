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

int L3_checkIfChatTxt(uint8_t* msg)
{
    return (msg[L3_MSG_OFFSET_TYPE]==L3_MSG_TYPE_CHATTXT);
}



#if 0
int L3_checkIfKeyboardInput(uint8_t* msg)
{
    return (msg[L3_MSG_OFFSET_TYPE]==L3_MSG_TYPE_KEYIINPUT);
}
#endif

int L3_checkIfTimeout(uint8_t* msg)
{
    return (msg[L3_MSG_OFFSET_TYPE]==L3_MSG_TYPE_TIMEOUT);
}

uint8_t L3_CONREQ_encodeData(uint8_t* msg)
{
    msg[L3_MSG_OFFSET_TYPE] = L3_MSG_TYPE_CONREQ;
    return 1;
}

uint8_t L3_CONCNF_encodeData(uint8_t* msg)
{
    msg[L3_MSG_OFFSET_TYPE] = L3_MSG_TYPE_CONCNF;
    return 1;
}

uint8_t L3_DISREQ_encodeData(uint8_t* msg)
{
    msg[L3_MSG_OFFSET_TYPE] = L3_MSG_TYPE_DISREQ;
    return 1;
}


uint8_t L3_DISCNF_encodeData(uint8_t* msg)
{
    msg[L3_MSG_OFFSET_TYPE] = L3_MSG_TYPE_DISCNF;
    return 1;
}

uint8_t L3_CHATTXT_encodeData(uint8_t* msg, uint8_t* txt)
{
    msg[L3_MSG_OFFSET_TYPE] = L3_MSG_TYPE_CHATTXT;
    strcpy((char*)(msg+L3_OFFSET_DATA), (const char*)txt);

    return (strlen((const char*)txt) + 1);
}

#if 0
uint8_t Msg_getSeq(uint8_t* msg)
{
    return msg[L3_MSG_OFFSET_SYN];
}
#endif

uint8_t* Msg_getWord(uint8_t* msg)
{
    return &msg[L3_OFFSET_DATA];
}
