
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
    };
    enum class action
    {
        slash,
        thrust,
        guard,
        idle,
    };
    class player
        : public cocos2d::Node
    {
    public:
        using sample = math::sample<data, 16, 16>;
        using spectrums_type = spectrums< sample::analysis_line_number >;

    private:
        sample sample_;
        spectrums_type spectrums_;
        action action_;

    public:
        CREATE_FUNC( player );
        player();
        ~player();
        virtual bool init() override;

        void update_state( communication_node::buffer_type& );

        spectrums_type const& get_spectrums() const
        {
            return spectrums_;
        }
        action const& get_action() const
        {
            return action_;
        }
    };
}

#endif//C2XA_PLAYER_SCENE_HPP