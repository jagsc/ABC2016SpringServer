
/**

    @file   sample.hpp
    @brief  sample

    @author 新ゝ月かりな(NewNotMoon)
    @date   created on 2016/02/05

    サンプルデータの集合

*/
#pragma once
#ifndef C2XA_SAMPLE_HPP
#define C2XA_SAMPLE_HPP

#include <list>

#include <c2xa/communication/data.hpp>
#include <c2xa/communication/parse.hpp>

namespace c2xa
{
    template< unsigned int sample_size_ >
    class sample
    {
    public:
        static constexpr unsigned int sample_size = sample_size_;

    private:
        std::list<data> list_;

    public:
        void push( data data_ )
        {
            list_.push_back( data_ );
            if( list_.size() > sample_size )
            {
                list_.pop_front(); // サンプル数を超えたら破棄
            }
        }
        auto cbegin() const
        {
            return list_.cbegin();
        }
        auto cend() const
        {
            return list_.cend();
        }
        auto size() const
        {
            return list_.size();
        }
    };
}

#endif//_sample_HPP