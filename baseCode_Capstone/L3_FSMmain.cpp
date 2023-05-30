#include "L3_FSMevent.h"
#include "L3_msg.h"
#include "L3_timer.h"
#include "L3_LLinterface.h"
#include "protocol_parameters.h"
#include "mbed.h"


//FSM state -------------------------------------------------
#define L3STATE_IDLE                0
#define L3STATE_CALL_ON             1
#define L3STATE_ESTABLISHED         2
#define L3STATE_CALL_OFF            3

//state variables
static uint8_t main_state = L3STATE_IDLE; //protocol state
static uint8_t prev_state = main_state;

//source/destination ID
static uint8_t myL2ID=1;
static uint8_t destL2ID=0;

//L3 PDU context/size
static uint8_t connectPdu[200];
static uint8_t pduSize;

//SDU (input)
static uint8_t originalWord[1030];
static uint8_t wordLen=0;

static uint8_t sdu[1030];

//serial port interface
static Serial pc(USBTX, USBRX);
static uint8_t myDestId;

//application event handler : generating SDU from keyboard input
static void L3service_processInputWord(void)
{
    char c = pc.getc();
    if (!L3_event_checkEventFlag(L3_event_CALLON_REQ))
    {
        if (c == '\n' || c == '\r')
        {
            originalWord[wordLen++] = '\0';
            L3_event_setEventFlag(L3_event_CALLON_REQ);
            debug_if(DBGMSG_L3,"word is ready! ::: %s\n", originalWord);
        }
        else
        {
            originalWord[wordLen++] = c;
            if (wordLen >= L3_MAXDATASIZE-1)
            {
                originalWord[wordLen++] = '\0';
                L3_event_setEventFlag(L3_event_CALLON_REQ);
                pc.printf("\n max reached! word forced to be ready :::: %s\n", originalWord);
            }
        }
    }
}



void L3_initFSM(uint8_t destId)
{

    myDestId = destId;
    //initialize service layer
    pc.attach(&L3service_processInputWord, Serial::RxIrq);

    pc.printf("Give a word to send : ");
}

void L3_FSMrun(void)
{   
    if (prev_state != main_state)
    {
        debug_if(DBGMSG_L3, "[L3] State transition from %i to %i\n", prev_state, main_state);
        prev_state = main_state;
    }

    //FSM should be implemented here! ---->>>>
    switch (main_state)
    {
        case L3STATE_IDLE: //IDLE state description
            
            if (L3_event_checkEventFlag(L3_event_CALLON_REQ)) //if data reception event happens
            {
                uint8_t srcId = L3_LLI_getSrcId();
                uint8_t* dataPtr = L3_LLI_getMsgPtr();
                uint8_t size = L3_LLI_getSize();

                //PDU encoding(connection callonreq)
                L3_CONREQ_encodeData(, (dataPtr));
                L3_LLI_dataReqFunc(sdu, wordLen, myDestId);
                
                debug("\n -------------------------------------------------\nRCVD MSG : %s (length:%i)\n -------------------------------------------------\n", 
                           dataPtr, size);
                
                main_state = L3STATE_CALL_ON;
                L3_event_clearEventFlag(L3_event_CALLON_REQ);
            }
            else if(L3_event_checkEventFlag(L3_event_CALLON_CNF))
            {
                //clear
                L3_event_clearEventFlag(L3_event_CALLON_CNF);
            }
            else if(L3_event_checkEventFlag(L3_event_CALLOFF_REQ))
            {
                //clear
                L3_event_clearEventFlag(L3_event_CALLOFF_REQ);
            }   
            else if(L3_event_checkEventFlag(L3_event_CALLOFF_CNF))
            {
                //clear
                L3_event_clearEventFlag(L3_event_CALLOFF_CNF);
            }
            else if(L3_event_checkEventFlag(L3_event_KEYBOARD_INPUT))
            {
                //clear
                L3_event_clearEventFlag(L3_event_KEYBOARD_INPUT);
            }
            else if(L3_event_checkEventFlag(L3_event_TIMEOUT))
            {
                //clear
                L3_event_clearEventFlag(L3_event_TIMEOUT);
            }                                        
            break;

        case L3STATE_CALL_ON: //CallOn state description
            if (L3_event_checkEventFlag(L3_event_CALLON_CNF)){
                L3_CONCNF_encodeData();
                L3_LLI_dataReqFunc();
                //sending CALLON_REQ to L3STATE_ESTABLISHED
                main_state = L3STATE_ESTABLISHED;
                L3_event_clearEventFlag(L3_event_CALLON_CNF);
            }
            else if(L3_event_checkEventFlag(L3_event_CALLON_REQ))
            {
                //clear
                L3_event_clearEventFlag(L3_event_CALLON_REQ);
            }
            else if(L3_event_checkEventFlag(L3_event_CALLOFF_REQ))
            {
                //clear
                L3_event_clearEventFlag(L3_event_CALLOFF_REQ);
            }   
            else if(L3_event_checkEventFlag(L3_event_CALLOFF_CNF))
            {
                //clear
                L3_event_clearEventFlag(L3_event_CALLOFF_CNF);
            }
            else if(L3_event_checkEventFlag(L3_event_KEYBOARD_INPUT))
            {
                //clear
                L3_event_clearEventFlag(L3_event_KEYBOARD_INPUT);
            }
            else if(L3_event_checkEventFlag(L3_event_TIMEOUT))
            {
                //clear
                L3_event_clearEventFlag(L3_event_TIMEOUT);
            }       
            break;

        case L3STATE_ESTABLISHED: //ESTABLISHED state description
            if (L3_event_checkEventFlag(L3_event_KEYBOARD_INPUT)){
                // keyboard input 받아서 채팅 
                // 함수 실행 함수만들기

                //others (timer start...)
                L3_timer_stopTimer();
                L3_timer_startTimer();
                L3_event_clearEventFlag(L3_event_KEYBOARD_INPUT);


                if (L3_event_checkEventFlag(L3_event_TIMEOUT)){
                //timeout
                L3_DISREQ_encodeData();
                L3_LLI_dataReqFunc();

                L3_event_clearEventFlag(L3_event_TIMEOUT);
                main_state = L3STATE_CALL_OFF;
                }
                else if (L3_event_checkEventFlag(L3_event_CALLOFF_REQ)){
                //exit입력
                L3_DISREQ_encodeData();
                L3_LLI_dataReqFunc();

                L3_event_clearEventFlag(L3_event_CALLOFF_REQ);
                main_state = L3STATE_CALL_OFF;
            }
            else if(L3_event_checkEventFlag(L3_event_CALLON_REQ))
            {
                //clear
                L3_event_clearEventFlag(L3_event_CALLON_REQ);
            }
            else if(L3_event_checkEventFlag(L3_event_CALLON_CNF))
            {
                //clear
                L3_event_clearEventFlag(L3_event_CALLON_CNF);
            }
            else if(L3_event_checkEventFlag(L3_event_CALLOFF_CNF))
            {
                //clear
                L3_event_clearEventFlag(L3_event_CALLOFF_CNF);
            }

            break;

        case L3STATE_CALL_OFF: //CallOff state description
            if (L3_event_checkEventFlag(L3_event_CALLOFF_CNF)){
                L3_DISCNF_encodeData();
                L3_LLI_dataReqFunc();
                main_state = L3STATE_IDLE;
                L3_event_clearEventFlag(L3_event_CALLOFF_CNF);
            }
            else if(L3_event_checkEventFlag(L3_event_CALLON_REQ))
            {
                //clear
                L3_event_clearEventFlag(L3_event_CALLON_REQ);
            }
            else if(L3_event_checkEventFlag(L3_event_CALLON_CNF))
            {
                //clear
                L3_event_clearEventFlag(L3_event_CALLON_CNF);
            }   
            else if(L3_event_checkEventFlag(L3_event_CALLOFF_REQ))
            {
                //clear
                L3_event_clearEventFlag(L3_event_CALLOFF_REQ);
            }
            else if(L3_event_checkEventFlag(L3_event_KEYBOARD_INPUT))
            {
                //clear
                L3_event_clearEventFlag(L3_event_KEYBOARD_INPUT);
            }
            else if(L3_event_checkEventFlag(L3_event_TIMEOUT))
            {
                //clear
                L3_event_clearEventFlag(L3_event_TIMEOUT);
            }
            break;

        default :
            break;
    }
}