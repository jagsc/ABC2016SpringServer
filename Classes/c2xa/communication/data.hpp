
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

    using data = std::tuple<acceleration, gyro>;
}

#endif//C2XA_COMMUNICATION_DATA_HPP