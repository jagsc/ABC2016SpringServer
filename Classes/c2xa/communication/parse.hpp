
/**

    @file   parse.hpp
    @brief  parse

    @author 新ゝ月かりな(NewNotMoon)
    @date   created on 2016/00/00

    センサデータパース関数

*/
#pragma once
#ifndef C2XA_COMMUNICATION_PARSE_HPP
#define C2XA_COMMUNICATION_PARSE_HPP

#include <array>
#include <string>
#include <tuple>
#include <vector>

#include <c2xa/communication/data.hpp>

namespace c2xa
{
    namespace communication
    {
        template< size_t size_ >
        static inline std::vector<std::tuple<unsigned long long, data>> parse( std::array<char,size_> const& buffer_ )
        {
            std::array< char, size_ * 2 + 1 > data_;
            unsigned char byte_ = 0;
            unsigned char map_[] ={ (unsigned char)-1, '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-', '.', ',' };
            int i;
            for( i = 0; i < size_; ++i )
            {
                byte_ = static_cast<unsigned char>( buffer_[ i ] ) >> 4;
                if( byte_ == 0b00000000 )
                {
                    data_[ i * 2 ] = '\0';
                    break;
                }
                //CCASSERT( byte_ <= 13 && byte_ >= 1, "" );
                data_[ i * 2 ] =  map_[ byte_ ];
                byte_ = static_cast<unsigned char>( buffer_[ i ] ) & 0b00001111;
                if( byte_ == 0b00000000 )
                {
                    data_[ i * 2 + 1 ] = '\0';
                    break;
                }
                //CCASSERT( byte_ <= 13 && byte_ >= 1, "" );
                data_[ i * 2 + 1 ] =  map_[ byte_ ];
            }

            auto begin_ = data_.cbegin();
            auto end_ = data_.cbegin();
            auto eof_ = data_.cend();

            std::vector<std::tuple<unsigned long long, data>> list_;
            unsigned long long timestamp_;
            data tmp_;

            auto skip = [ & ]
            {
                while( end_ != eof_ && *end_ != '\n' )
                {
                    std::advance( end_, 1 );
                }
                return true;
            };
            auto scan = [ & ]
            {
                auto result_ = std::sscanf
                    (
                        std::string{ begin_, end_ }.c_str(),
                        "%llu,%f,%f,%f,%f,%f,%f",
                        &timestamp_,
                        &tmp_.acceleration.x,
                        &tmp_.acceleration.y,
                        &tmp_.acceleration.z,
                        &tmp_.gyro.x,
                        &tmp_.gyro.y,
                        &tmp_.gyro.z
                    );
                if( result_ == EOF || result_ < 6 )
                {
                    return false;
                }
                return true;
            };

            while( skip() && scan() )
            {
                //TODO: サンプルデータの位相を考慮する
                list_.push_back( std::tie( timestamp_, tmp_ ) );
                if( end_ == eof_ || *end_ != '\0' )
                {
                    break;
                }
                std::advance( end_, 1 );
                begin_ = end_;
            }

            return list_;
        }
    }
}

#endif//C2XA_COMMUNICATION_PARSE_HPP