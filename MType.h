#pragma once

#include "MPrerequisites.h"



//-----------------------------------------------------------
// 사이즈 타입
//-----------------------------------------------------------
template <typename T>
class MTSize
{
public:
    MTSize() {}
    MTSize(T inWidth, T inHeight)
        : Width(inWidth), Height(inHeight)
    {}

public:
    void Set(T inWidth, T inHeight)
    {
        Width = inWidth;
        Height = inHeight;
    }

    MTSize<T>& operator/=(T inValue)
    {
        Width /= inValue;
        Height /= inValue;
        return *this;
    }

    MTSize<T> operator/(T inValue)
    {
        MTSize<T> temp(Width, Height);
        temp /= inValue;
        return temp;
    }

public:
    T Width;
    T Height;
};

using MIntSize = MTSize<MINT32>;

//-----------------------------------------------------------
// 포인트 타입
//-----------------------------------------------------------
template <typename T>
class MTPoint
{
public:
    MTPoint() {}
    MTPoint(T inX, T inY)
        : X(inX), Y(inY)
        {}

    MTPoint(const MTPoint<T>& inOther)
        : X(inOther.X), Y(inOther.Y)
    {}

    void Set(T inX, T inY)
    {
        X = inX;
        Y = inY;
    }

public:
    bool operator<(const MTPoint<T>& inOther) const
    {
        return (X < inOther.X) || ((X == inOther.X) && (Y < inOther.Y));
    }

    bool operator==(const MTPoint<T>& inOther) const
    {
        return (X == inOther.X) && (Y == inOther.Y);
    }

    MTPoint<T> operator+(const MTPoint<T>& inOther) const
    {
        return MTPoint<T>(X + inOther.X, Y + inOther.Y);
    }

    MTPoint<T>& operator+=(const MTPoint<T>& inOther)
    {
        X += inOther.X;
        Y += inOther.Y;
        return *this;
    }
    
    MTPoint<T>& operator+(const MTSize<T>& inSize) const
    {
        return MTPoint<T>(X + inSize.X, Y + inSize.Y);
    }
    

public:
    T X;
    T Y;
};


// int type
using MIntPoint = MTPoint<MINT32>;




