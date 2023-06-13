#include "L3_FSMevent.h"
#include "L3_msg.h"
#include "L3_timer.h"
#include "L3_LLinterface.h"
#include "protocol_parameters.h"
#include "mbed.h"
#include <iostream>


//FSM state -------------------------------------------------
#define L3STATE_IDLE                0
#define L3STATE_CALL_ON             1
#define L3STATE_ESTABLISHED         2
#define L3STATE_CALL_OFF            3

//state variables
static uint8_t main_state = L3STATE_IDLE; //protocol state
static uint8_t prev_state = main_state;

//L3 PDU context/size
static uint8_t pduSize;

//SDU (input)
static uint8_t originalWord[1030];
static uint8_t wordLen=0;

static uint8_t sdu[1030];

//serial port interface
static Serial pc(USBTX, USBRX);
static uint8_t myDestId;
static uint8_t call_cause = 0; //0 - origination, 1 - termination

//application event handler : generating SDU from keyboard input
static void L3service_processInputWord(void)
{
    char c = pc.getc();
    if (main_state == L3STATE_IDLE || main_state == L3STATE_ESTABLISHED)
    {
        if (c == '\n' || c == '\r')
        {
            originalWord[wordLen++] = '\0';
            L3_event_setEventFlag(L3_event_KEYBOARD_INPUT);
            debug_if(DBGMSG_L3,"word is ready! ::: %s\n", originalWord);
        }
        else
        {
            originalWord[wordLen++] = c;
            #if 0
            if (wordLen >= L3_MAXDATASIZE-1)
            {
                originalWord[wordLen++] = '\0';
                L3_event_setEventFlag(L3_event_CALLON_REQ);
                pc.printf("\n max reached! word forced to be ready :::: %s\n", originalWord);
            }
            #endif
        }
    }
}


void L3_initFSM(void)
{
    #if 0
    myDestId = destId;
    #endif
    //initialize service layer
    pc.attach(&L3service_processInputWord, Serial::RxIrq);

    pc.printf("Let's start, Input a destination ID : ");
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
            
            if (L3_event_checkEventFlag(L3_event_KEYBOARD_INPUT)) //if user requests to connect
            {
                #if 0
                uint8_t* dataPtr = L3_LLI_getMsgPtr();
                uint8_t size = L3_LLI_getSize();
                #endif

                //convert input string to ID
                myDestId = (uint8_t)atoi((char const*)originalWord); //string to integer

                pduSize = L3_CONREQ_encodeData(sdu);
                L3_LLI_dataReqFunc(sdu, pduSize, myDestId);
                
                wordLen = 0;
                // debug("\n -------------------------------------------------\nRCVD MSG : %s (length:%i)\n -------------------------------------------------\n", 
                        //    dataPtr, size);

                pc.printf("keyboard input in L3STATE_IDLE\n");
                call_cause = 0; //this node is call origination
                main_state = L3STATE_CALL_ON;
                L3_event_clearEventFlag(L3_event_KEYBOARD_INPUT);
            }
            else if(L3_event_checkEventFlag(L3_event_CALLON_REQ)) //if the other uesr sends connect request
            {
                myDestId = L3_LLI_getSrcId(); //string to integer

                pc.printf("rcvd callon_req from %i in L3STATE_IDLE\n", myDestId);

                pduSize = L3_CONCNF_encodeData(sdu);
                L3_LLI_dataReqFunc(sdu, pduSize, myDestId);
                call_cause = 1; //this node is call termination
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
            #if 0
            else if(L3_event_checkEventFlag(L3_event_KEYBOARD_INPUT))
            {
                //clear
                L3_event_clearEventFlag(L3_event_KEYBOARD_INPUT);
            }
            #endif
            else if(L3_event_checkEventFlag(L3_event_TIMEOUT))
            {
                //clear
                L3_event_clearEventFlag(L3_event_TIMEOUT);
            }                                        
            break;

        case L3STATE_CALL_ON: //CallOn state description
            if (L3_event_checkEventFlag(L3_event_CALLON_CNF))
            {
                pc.printf("received callon cnf in L3STATE_CALL_ON\n");

                //action for transition from CALL_ON state to ESTABLISHED state
                //timer?
                //user interface printing
                #if 0
                pduSize = L3_CONCNF_encodeData(sdu);
                L3_LLI_dataReqFunc(sdu, pduSize, myDestId);
                #endif
                
                L3_timer_startTimer();
                pc.printf("Give a word to send : ");

                main_state = L3STATE_ESTABLISHED;
                L3_event_clearEventFlag(L3_event_CALLON_CNF);
            }
            else if (L3_event_checkEventFlag(L3_event_dataSendCnf))
            {
                if (call_cause == 1) //handle DATA_CNF only in call termination case
                {
                    //action for transition from CALL_ON state to ESTABLISHED state
                    //timer?
                    //user interface printing
                    L3_timer_startTimer();
                    pc.printf("Give a word to send : ");

                    main_state = L3STATE_ESTABLISHED;
                }
                L3_event_clearEventFlag(L3_event_dataSendCnf);
            }
            else if(L3_event_checkEventFlag(L3_event_CALLON_REQ))
            {
                pc.printf("dddddL3STATE_CALL_ON");
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
                L3_timer_stopTimer();
                L3_timer_startTimer();

                //pc.printf("L3STATE_ESTABLISHED");

                // keyboard input 받아서 채팅
                //strcpy((char*)sdu, (char*)originalWord);
                wordLen = L3_CHATTXT_encodeData(sdu, originalWord);
                debug("[L3] msg length : %i\n", wordLen);
                L3_LLI_dataReqFunc(sdu, wordLen, myDestId);

                debug_if(DBGMSG_L3, "[L3] sending msg....\n");
                wordLen = 0;

                pc.printf("Give a word to send : ");

                using namespace std;
                std::string program_exit = "exit";

                if (std::string(reinterpret_cast<char*>(sdu)).compare(program_exit)==0){
                    pduSize = L3_DISREQ_encodeData(sdu);
                    L3_LLI_dataReqFunc(sdu, pduSize, myDestId);

                    L3_event_clearEventFlag(L3_event_CALLOFF_REQ);
                    main_state = L3STATE_CALL_OFF;
                }



                L3_event_clearEventFlag(L3_event_KEYBOARD_INPUT);
            }
            else if (L3_event_checkEventFlag(L3_event_CHATTXT))
            {
                uint8_t* msg = L3_LLI_getMsgPtr();
                uint8_t* txt = Msg_getWord(msg);

                pc.printf("\n\n CHAT msg : %s\n\n", txt);

                L3_event_clearEventFlag(L3_event_CHATTXT);
            }
            else if (L3_event_checkEventFlag(L3_event_TIMEOUT))
            {
                    //timeout
                    pc.printf("Timeout: ");
                    pduSize = L3_DISREQ_encodeData(sdu);
                    L3_LLI_dataReqFunc(sdu, pduSize, myDestId);

                    L3_event_clearEventFlag(L3_event_TIMEOUT);
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
                pc.printf("L3STATE_CALL_OFF");
                pduSize = L3_DISCNF_encodeData(sdu);
                L3_LLI_dataReqFunc(sdu, pduSize, myDestId);                
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
