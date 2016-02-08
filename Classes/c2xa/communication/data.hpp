
/**

    @file   data.hpp
    @brief  data

    @author 新ゝ月かりな(NewNotMoon)
    @date   created on 2016/02/05

*/
#pragma once
#ifndef C2XA_COMMUNICATION_DATA_HPP
#define C2XA_COMMUNICATION_DATA_HPP

#include <tuple>

namespace c2xa
{
    using data_type = float;
    struct acceleration
    {
        data_type x;
        data_type y;
        data_type z;
        
        acceleration operator+( acceleration const& a ) const
        {
            return { x + a.x, y + a.y, z + a.z };
        }
        acceleration& operator+=( acceleration const& a )
        {
            x += a.x;
            y += a.y;
            z += a.z;
            return *this;
        }
        acceleration operator*( acceleration const& a ) const
        {
            return { x * a.x, y * a.y, z * a.z };
        }
        acceleration& operator*=( acceleration const& a )
        {
            x *= a.x;
            y *= a.y;
            z *= a.z;
            return *this;
        }
        template< typename Type >
        acceleration operator*( Type a ) const
        {
            return { x * a, y * a, z * a };
        }
        template< typename Type >
        acceleration& operator*=( Type a )
        {
            x *= a;
            y *= a;
            z *= a;
            return *this;
        }
        template< typename Type >
        acceleration operator/( Type a ) const
        {
            return { x / a, y / a, z / a };
        }
        template< typename Type >
        acceleration& operator/=( Type a )
        {
            x /= a;
            y /= a;
            z /= a;
            return *this;
        }
    };
    struct gyro
    {
        data_type x;
        data_type y;
        data_type z;

        gyro operator+( gyro const& a ) const
        {
            return { x + a.x, y + a.y, z + a.z };
        }
        gyro& operator+=( gyro const& a )
        {
            x += a.x;
            y += a.y;
            z += a.z;
            return *this;
        }
        gyro operator*( gyro const& a ) const
        {
            return { x * a.x, y * a.y, z * a.z };
        }
        gyro& operator*=( gyro const& a )
        {
            x *= a.x;
            y *= a.y;
            z *= a.z;
            return *this;
        }
        template< typename Type >
        gyro operator*( Type a ) const
        {
            return { x * a, y * a, z * a };
        }
        template< typename Type >
        gyro& operator*=( Type a )
        {
            x *= a;
            y *= a;
            z *= a;
            return *this;
        }
        template< typename Type >
        gyro operator/( Type a ) const
        {
            return { x / a, y / a, z / a };
        }
        template< typename Type >
        gyro& operator/=( Type a )
        {
            x /= a;
            y /= a;
            z /= a;
            return *this;
        }
    };
    struct data
    {
        acceleration acceleration;
        gyro gyro;

        data operator+( data const& a ) const
        {
            return { acceleration + a.acceleration, gyro + a.gyro };
        }
        template< typename Type >
        data operator*( Type a ) const
        {
            return { acceleration * a, gyro * a };
        }
        template< typename Type >
        data operator/( Type a ) const
        {
            return { acceleration / a, gyro / a };
        }
    };
}

#endif//C2XA_COMMUNICATION_DATA_HPP