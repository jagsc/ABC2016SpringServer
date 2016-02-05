
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

#include <string>
#include <vector>

#include <c2xa/communication/data.hpp>

namespace c2xa
{
    namespace communication
    {
        static inline std::vector<data> parse( std::string const& string_ )
        {
            auto begin_ = string_.cbegin();
            auto end_ = string_.cbegin();
            auto eof_ = string_.cend();

            std::vector<data> list_;
            data data_;

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
                        "%f,%f,%f,%f,%f,%f",
                        &std::get<0>( data_ ).x,
                        &std::get<0>( data_ ).y,
                        &std::get<0>( data_ ).z,
                        &std::get<1>( data_ ).x,
                        &std::get<1>( data_ ).y,
                        &std::get<1>( data_ ).z
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
                list_.push_back( data_ );
                if( end_ == eof_ )
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