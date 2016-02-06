/************************************************************************************//**
    @file   c2xa/scene/main_scene.hpp
    @author 新ゝ月かりな
    @date   created on 2015/12/24
****************************************************************************************/
#ifndef C2XA_SCENE_MAIN_SCENE_HPP
#define C2XA_SCENE_MAIN_SCENE_HPP

#include <cocos2d.h>
#include <c2xa/communication/bluetooth_server.hpp>
#include <c2xa/sample.hpp>
#include <c2xa/math/sample.hpp>
#include <c2xa/math/fast_fourier_transform.hpp>

namespace c2xa
{
    namespace scene
    {
        class main_scene
            : public cocos2d::Scene
        {
        private:
            static constexpr unsigned int sample_size = 256;
            bluetooth::listen_server listen_server_;
            std::unique_ptr<bluetooth::connection_server> connection_server_;
            fft fft_;
            sample<sample_size> sample_;
            math::sample<data,32,256> sampler_;
            char buffer_[ 2024 ];

        public:
            CREATE_FUNC( main_scene );
            main_scene();
            ~main_scene();
            virtual bool init() override;
            virtual void update( float ) override;
        };
    }
}

#endif//C2XA_SCENE_MAIN_SCENE_HPP