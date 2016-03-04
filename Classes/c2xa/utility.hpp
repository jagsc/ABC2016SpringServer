

/**

    @file   utility.hpp
    @brief  utility

    @author 新ゝ月かりな(NewNotMoon)
    @date   created on 2016/00/00

*/
#pragma once
#ifndef C2XA_UTILITY_HPP
#define C2XA_UTILITY_HPP

namespace c2xa
{

    /*!
        Cocos2d-xのCREATE_FUNCの代わりとなるヘルパ関数です。
        init関数が複数の引数を保つ場合でも使えます。
    */
    template< typename TYPE, typename... ARGS >
    inline TYPE* create_template( ARGS&&... a )
    {
        auto p = new TYPE{};

        if( p && p->init( std::forward<ARGS>( a )... ) )
        {
            p->autorelease();
            return p;
        }
        else
        {
            delete p;
            return nullptr;
        }
    };
}

#endif//C2XA_UTILITY_HPP