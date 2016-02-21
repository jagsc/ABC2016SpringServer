

/**

    @file   math/sample.hpp
    @brief  sample

    @author 新ゝ月かりな(NewNotMoon)
    @date   created on 2016/00/00

    FFT用のサンプルを保持するよ

*/
#pragma once
#ifndef C2XA_MATH_SAMPLE_HPP
#define C2XA_MATH_SAMPLE_HPP

#include <list>
#include <vector>
#include <array>
#include <tuple>
#include <chrono>

namespace c2xa
{
    namespace math
    {
        using integer = unsigned int;
        using hertz = unsigned long long;
        constexpr hertz operator""_Hz( hertz value_ )
        {
            return value_;
        }
        
        template< typename data_, hertz sampling_frequency_, integer sampling_number_ >
        class sample
        {
        public:
            using data = data_;
            using element = std::tuple<std::chrono::nanoseconds, data>;
            static constexpr hertz   sampling_frequency       = sampling_frequency_;
            static constexpr integer sampling_number          = sampling_number_;
            static constexpr hertz   analysis_frequency_range = sampling_frequency * 100 / 256;
            static constexpr integer analysis_line_number     = sampling_number * 100 / 256;
            static constexpr unsigned long double window_size          = sampling_number / sampling_frequency;
            static constexpr std::chrono::nanoseconds::rep sampling_period  = 1000000000 / sampling_frequency; //< nanoseconds
            //static constexpr unsigned long double frequency_resolution = 1 / window_size;
            
        private:
            std::list<element> list_;
            std::chrono::nanoseconds before_time_;
            data before_data_;
            data sum_;
            std::chrono::nanoseconds next_ = std::chrono::nanoseconds{ 0 };

        public:
            template< typename function >
            void push( std::chrono::nanoseconds timestamp_, data data_, function func_ )
            {
                if( list_.size() == 0 )
                {
                    next_ = timestamp_ + std::chrono::nanoseconds{ sampling_period };
                    list_.push_back( std::tie( timestamp_, data_ ) );
                    sum_ = data_;
                }
                else
                {
                    // データが次のサンプリングポイントを超えた
                    int i = 0;
                    while( timestamp_ >= next_ )
                    {
                        auto right_ = timestamp_ - next_;
                        auto left_  = next_ - before_time_;
                        // before_data_ + ( data_ - before_data_ ) * left_ / ( left_ + right_ );
                        // ↓
                        // ( data_ * left_ + before_data_ * right_ ) / ( left_ + right_ )

                        list_.push_back( std::tie( next_, func_( static_cast<unsigned>( left_.count() ), static_cast<unsigned>( right_.count() ), before_data_, data_ ) ) );

                        sum_ += data_;

                        if( list_.size() > sampling_number )
                        {
                            sum_ -= std::get<1>( list_.front() );
                            list_.pop_front(); // サンプル数を超えたら破棄
                        }

                        next_ += std::chrono::nanoseconds{ sampling_period };
                        ++i;
                    }
                }
                before_time_ = timestamp_;
                before_data_ = data_;
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
            auto average() const
            {
                return sum_ / list_.size();
            }
        };
    }
}

#endif//C2XA_MATH_SAMPLE_HPP