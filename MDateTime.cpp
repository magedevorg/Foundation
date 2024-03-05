#include "MDateTime.h"



//------------------------------------------------------------
// MDateTime
//------------------------------------------------------------
MDateTime::MDateTime(MINT32 inYear, MINT32 inMonth, MINT32 inDay, MINT32 inHour, MINT32 inMinute, MINT32 inSecond)
{
    // tm값으로 변경
    struct tm src = { 0, };
    {
        src.tm_year = inYear - 1900;
        src.tm_mon = inMonth - 1;
        src.tm_mday = inDay;
        src.tm_hour = inHour;
        src.tm_min = inMinute;
        src.tm_sec = inSecond;
    }

    // mktime 쓰레드 안전성 확인이 필요
    time_t tempTime = mktime(&src);

    // 시간 설정
    TimePoint = std::chrono::system_clock::from_time_t(tempTime);
}

void MDateTime::GetDate(MINT32& outYear, MINT32& outMonth, MINT32& outDay)
{
    tm temp;
    GMTimeLogic(temp, std::chrono::system_clock::to_time_t(TimePoint));

    outYear = temp.tm_year + 1900;
    outMonth = temp.tm_mon + 1;
    outDay = temp.tm_mday;
}

void MDateTime::GetDateTime(MINT32& outYear, MINT32& outMonth, MINT32& outDay, MINT32& outHour, MINT32& outMinute, MINT32& outSecond)
{
    tm temp;
    GMTimeLogic(temp, std::chrono::system_clock::to_time_t(TimePoint));

    outYear = temp.tm_year + 1900;
    outMonth = temp.tm_mon + 1;
    outDay = temp.tm_mday;
    outHour = temp.tm_hour;
    outMinute = temp.tm_min;
    outSecond = temp.tm_sec;
}

void MDateTime::GMTimeLogic(tm& outTM, const time_t& inTime)
{
#if(MPLATFORM == MPLATFORM_WINDOWS)
    {
        ::gmtime_s(&outTM, &inTime);
    }
#else
    {
        ::gmtime_r(&inTime, &outTM);
    }
#endif
}


