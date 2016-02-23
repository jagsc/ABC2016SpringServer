
/**

    @file   battle_scene.hpp
    @brief  battle_scene

    @author 新ゝ月かりな(NewNotMoon)
    @date   created on 2016/02/09

*/
#pragma once
#ifndef C2XA_SCENE_BATTLE_SCENE_HPP
#define C2XA_SCENE_BATTLE_SCENE_HPP

#include <array>

#include <cocos2d.h>

#include <c2xa/utility.hpp>
#include <c2xa/communication/data.hpp>
#include <c2xa/math/sample.hpp>
#include <c2xa/math/fast_fourier_transform.hpp>

namespace c2xa
{
    class communication_node;

    namespace scene
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
            idol,
        };
        class battle_scene
            : public cocos2d::Scene
        {
        public:
            using sample = math::sample<data, 16, 16>;
            using spectrums_type = spectrums< battle_scene::sample::analysis_line_number >;

        private:
            fft fft_;
            sample sample_1p;
            sample sample_2p;
            spectrums_type spectrums_1p;
            spectrums_type spectrums_2p;
            action action_1p;
            action action_2p;

        public:
            battle_scene();
            static battle_scene* create( communication_node* c )
            {
                return create_template<battle_scene>( c );
            }
            virtual bool init( communication_node* );
            virtual void update( float ) override;
        };
    }
}

#endif//C2XA_SCENE_BATTLE_SCENE_HPP