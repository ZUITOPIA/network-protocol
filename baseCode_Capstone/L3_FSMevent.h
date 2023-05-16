typedef enum L3_event
{
    // L3_event_msgRcvd = 2,
    // L3_event_dataToSend = 4,
    // L3_event_dataSendCnf = 5,
    // L3_event_recfgSrcIdCnf = 6,

    L3_event_CALLON_REQ_SEND = 0, // 송신
    L3_event_CALLON_REQ_CNF = 1, // 수신
    L3_event_CALLOFF_REQ_SEND = 2, // 송신
    L3_event_CALLOFF_REQ_CNF = 3, // 수신
    L3_event_TIMER_START = 4,
    L3_event_TIMEOUT = 5,

} L3_event_e;


void L3_event_setEventFlag(L3_event_e event);
void L3_event_clearEventFlag(L3_event_e event);
void L3_event_clearAllEventFlag(void);
int L3_event_checkEventFlag(L3_event_e event);