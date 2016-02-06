
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
    struct acceleration
    {
        float x;
        float y;
        float z;
    };
    struct gyro
    {
        float x;
        float y;
        float z;
    };
    using data = std::tuple<unsigned long,acceleration, gyro>;
    static constexpr unsigned data_timestamp = 0;
    static constexpr unsigned data_acceleration = 1;
    static constexpr unsigned data_gyro = 2;
}

#endif//C2XA_COMMUNICATION_DATA_HPP