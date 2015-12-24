/************************************************************************************//**
    @file    c2xa/c2xa_config.hpp
    @author  新ゝ月かりな(NewNotMoon)
    @date    created on 2015/12/24
****************************************************************************************/
#ifndef C2XA_C2XA_CONFIG_HPP
#define C2XA_C2XA_CONFIG_HPP

#include <cocos2d.h>

namespace c2xa
{
    static const char* app_name = u8"仕様なき戦い ウェアラブル編";
    static constexpr float app_width  = 1920.f;
    static constexpr float app_height = 1080.f;
    static cocos2d::Rect const app_rectangle { 0, 0, app_width, app_height };
    static cocos2d::Size const app_size      { app_width, app_height };
}

#endif//C2XA_C2XA_CONFIG_HPP