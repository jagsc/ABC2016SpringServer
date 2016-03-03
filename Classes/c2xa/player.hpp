
/**

    @file   player.hpp
    @brief  player

    @author 新ゝ月かりな(NewNotMoon)
    @date   created on 2016/02/23

*/
#pragma once
#ifndef C2XA_PLAYER_SCENE_HPP
#define C2XA_PLAYER_SCENE_HPP

#include <array>

#include <cocos2d.h>

#include <c2xa/communication/data.hpp>
#include <c2xa/communication/node.hpp>
#include <c2xa/math/sample.hpp>
#include <c2xa/math/fast_fourier_transform.hpp>

namespace c2xa
{
    template< size_t number_ >
    struct spectrums
    {
    public:
        static constexpr auto number = number_;
        using spectrum = std::array<double, number>;
        spectrum acceleration_x;
        spectrum acceleration_y;
        spectrum acceleration_z;
        spectrum gyro_x;
        spectrum gyro_y;
        spectrum gyro_z;
        struct phase
        {
            spectrum acceleration_x;
            spectrum acceleration_y;
            spectrum acceleration_z;
            spectrum gyro_x;
            spectrum gyro_y;
            spectrum gyro_z;
        } phase_;
    };
    enum class action
    {
        slash,
        thrust,
        guard,
        messy,
        idle,
    };
    static constexpr int player_max_hp = 800;
    class player
        : public cocos2d::Node
    {
    public:
        using sample = math::sample<data, 16, 16>;
        using spectrums_type = spectrums< sample::analysis_line_number >;

        enum class number
        {
            _1p,
            _2p,
        };
        enum class state
        {
            defenseless,
            invincible,
        };

    private:
        sample sample_;
        spectrums_type spectrums_;
        action action_;
        number number_;
        state state_;
        bool is_doing_;
        bool is_updated_;
        bool is_attacking_;
        int hp_;

    public:
        static player* create( number c )
        {
            return create_template<player>( c );
        }
        player();
        ~player();
        virtual bool init( number );
        virtual void update( float ) override;

        void update_state( communication_node::buffer_type& );
        void judge();
        void damage( int );

        spectrums_type const& get_spectrums() const
        {
            return spectrums_;
        }
        action const& get_action() const
        {
            return action_;
        }
        bool check_attacking()
        {
            if( is_attacking_ )
            {
                is_attacking_ = false;
                return true;
            }
            return false;
        }
        state const& get_state() const
        {
            return state_;
        }
        int const& get_hp() const
        {
            return hp_;
        }
        bool is_dead() const
        {
            return hp_ <= 0;
        }
#ifdef COCOS2D_DEBUG
        void debug( cocos2d::Layer* );
#endif
    };
}

#endif//C2XA_PLAYER_SCENE_HPP