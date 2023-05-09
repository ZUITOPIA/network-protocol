네트워크 프로토콜 설계 과제 제출을 위한 레포지토리입니다.

해야 할 일

1. state 생성
2. event 생성
3. transition 생성

case : state / if else : transition(action) / if else에 해당돼서 실행될 함수 event (header에 정의, timer도 이벤트)

L2 timer
=> 기존 타이머

L3 timer : 기능 구현해야 할 타이머
=> established state로 변경되자마자 timer start, 1분 경과 시 timeout 되면서 calloff state로 case 변경

L2, L3 interface 수정 X

```
# case : state 조정하는 방법 예시

switch (main_state)
    {
        case L3STATE_IDLE: //IDLE state description ... 생략
```

```
# transition(action)과 event 설정 예시

if (!L3_event_checkEventFlag(L3_event_dataToSend)) ... 생략

# L3_event_checkEventFlag는 transition
# L3_event_dataToSend는 event
```

-   L2_FSMmain.cpp 파일의 198, 372 라인 추후 수정 (양측 다SeqNum == 0으로 셋팅하도록)
