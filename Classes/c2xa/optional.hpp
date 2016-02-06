
/**

    @file   optional.hpp
    @brief  optional

    @author 新ゝ月かりな(NewNotMoon)
    @date   created on 2016/02/05

    boostが使えないので超簡易オプショナル
    実用性クソなのでこのプロジェクト以外で使わないで下さい

    ･デフォルトコンストラクト可能なもののみの対応
    ･メタチェック無し
    ･カスタムでりーたもなーんもなし
    ･例外安全でもない
*/
#pragma once
#ifndef C2XA_OPTIONAL_HPP
#define C2XA_OPTIONAL_HPP

namespace c2xa
{
    struct none_type
    {
    };
    static const none_type none = none_type{};

    template< typename Value >
    class optional
    {
    private:
        Value value_;
        bool flag_;

    public:
        template< typename Source >
        optional( Source&& source_ )
            :value_{ std::forward<Source>( source_ ) }
            ,flag_{ true }
        {
        }
        optional( none_type )
            :flag_{ false }
        {
        }
        template< typename Source >
        optional const& operator=( Source&& source_ )
        {
            value_ = std::forward<Source>( source_ );
            flag_ = true;
        }
        optional const& operator=( none_type )
        {
            value_ = Value{};
            flag_ = false;
        }
        Value& value()
        {
            return value_;
        }
        Value& operator*()
        {
            return value_;
        }
        operator bool()
        {
            return flag_;
        }
    };
}

#endif//C2XA_OPTIONAL_HPP