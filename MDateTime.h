#pragma once


#include "MPrerequisites.h"
#include <chrono>

using namespace std::chrono;

namespace MTimeConvert
{
    // ����ũ�� ������ -> ƽ
    constexpr MINT64 TicksPerMicrosecond = 10;

};


//------------------------------------------------------------
// ��� �ð�
// ����ũ�� �ð����� ó��
//------------------------------------------------------------
class MTimespan
{
    friend class MDateTime;

public:
    MTimespan() {}
    MTimespan(microseconds inDuration)
        : Duration(inDuration)
    {}

public:
    MBOOL IsZero() const {
        return Duration == microseconds::zero();
    }

    static MTimespan FromMilliseconds(MINT64 inValue) {
        return MTimespan(milliseconds(inValue));
    }

    //----------------------------------------------
    // operator
    //----------------------------------------------
    MTimespan operator-(const MTimespan& inOther) const {
        return MTimespan(Duration - inOther.Duration);
    }

    MTimespan operator+(const MTimespan& inOther) const {
        return MTimespan(Duration + inOther.Duration);
    }

    MBOOL operator<(const MTimespan& inOther) const {
        return Duration < inOther.Duration;
    }

    MBOOL operator<=(const MTimespan& inOther) const {
        return Duration <= inOther.Duration;
    }

    MBOOL operator>(const MTimespan& inOther) const {
        return Duration > inOther.Duration;
    }

    MBOOL operator>=(const MTimespan& inOther) const {
        return Duration >= inOther.Duration;
    }

    MBOOL operator==(const MTimespan& inOther) const {
        return Duration == inOther.Duration;
    }

    MBOOL operator!=(const MTimespan& inOther) const {
        return Duration != inOther.Duration;
    }


    //----------------------------------------------
    // Getter
    //----------------------------------------------
    microseconds GetDuration() const {
        return Duration;
    }

    MINT64 GetMicroseconds() const {
        return Duration.count();
    }

    MINT64 GetMilliseconds() const {
        return duration_cast<milliseconds>(Duration).count();
    }

    MINT64 GetSeconds() const {
        return duration_cast<seconds>(Duration).count();
    }

    MINT64 GetMinutes() const {
        return duration_cast<minutes>(Duration).count();
    }

    MINT64 GetHours() const {
        return duration_cast<hours>(Duration).count();
    }

protected:
    microseconds Duration;
};



//------------------------------------------------------------
// �ð� Ŭ����
// chrono�� ������� ���. �ð��� micro second�� ���
//------------------------------------------------------------
class MDateTime
{
public:
    MDateTime() {}
    MDateTime(std::chrono::system_clock::time_point inTimePoint)
        : TimePoint(inTimePoint)
    {}

    MDateTime(MINT32 inYear, MINT32 inMonth, MINT32 inDay, MINT32 inHour, MINT32 inMinute, MINT32 inSecond);

public:
    // ���� UTC�ð��� ��´�
    static MDateTime UtcNow() {
        return MDateTime(std::chrono::system_clock::now());
    }

    // ������ ���� ���� �⺻��������
    MBOOL IsDefault() const {
        return TimePoint.time_since_epoch() == system_clock::duration::zero();
    }


    // ������ ��¥�� ��´�
    void GetDate(MINT32& outYear, MINT32& outMonth, MINT32& outDay);
    void GetDateTime(MINT32& outYear, MINT32& outMonth, MINT32& outDay, MINT32& outHour, MINT32& outMinute, MINT32& outSecond);

    //---------------------------------------------------
    // ������ �����ε�
    //---------------------------------------------------
    MTimespan operator-(const MDateTime& inOther) const {
        return MTimespan(duration_cast<microseconds>(TimePoint - inOther.TimePoint));
    }

    MDateTime& operator-=(const MTimespan& inTimespan)
    {
        TimePoint -= inTimespan.Duration;
        return *this;
    }

    MDateTime operator-(const MTimespan& inTimespan) const {
        return MDateTime(TimePoint - inTimespan.Duration);
    }

    MDateTime& operator+=(const MTimespan& inTimespan)
    {
        TimePoint += inTimespan.Duration;
        return *this;
    }

    MDateTime operator+(const MTimespan& inTimespan) const {
        return MDateTime(TimePoint + inTimespan.Duration);
    }

protected:
    // thraed safe time_t -> tm
    void GMTimeLogic(tm& outTM, const time_t& inTime);

protected:
    // Ÿ�� ����Ʈ
    std::chrono::system_clock::time_point TimePoint;
};