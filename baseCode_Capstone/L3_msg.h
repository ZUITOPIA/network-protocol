#include "mbed.h"

#define L3_MSG_OFFSET_TYPE              0
#define L3_MSG_OFFSET_SYN               1
#define L3_OFFSET_DATA                  2

#define L3_MSG_TYPE_CONREQ              0
#define L3_MSG_TYPE_CONCNF              1
#define L3_MSG_TYPE_DISREQ              2
#define L3_MSG_TYPE_DISCNF              3

int L3_checkIfCallOnReq(uint8_t* msg);
int L3_checkIfCallOnCnf(uint8_t* msg);
int L3_checkIfCallOffReq(uint8_t* msg);
int L3_checkIfCallOffCnf(uint8_t* msg);

uint8_t L3_CONREQ_encodeData(uint8_t* msg);
uint8_t L3_CONCNF_encodeData(uint8_t* msg);
uint8_t L3_DISREQ_encodeData(uint8_t* msg);
uint8_t L3_DISCNF_encodeData(uint8_t* msg);

uint8_t Msg_getSeq(uint8_t* msg);
uint8_t* Msg_getWord(uint8_t* msg);