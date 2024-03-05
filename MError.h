#pragma once


enum class EMError
{
    None = 0,

    // IO 모델 
    WinsockInitFailed = 100,            // 윈도우 소켓 초기화 에러
    CreateSocketFailed,                 // 소켓 생성 실패
    LackOfSocket,                       // 소켓 부족
    WouldBlock,                         //  
    AlreadyUseAddress,                  // 이미 사용중인 주소
    Timeout,                            // 타임 아웃
    IOPending,                          

    ServerLackOfSocket,                 // 서버 소켓 부족

    CreateIOCPFailed,                   // IOCP 생성실패
    BindFailed,                         // 바인드 실패
    ListenFailed,                       // 리슨 실패

    MError_Test = 1000,
};