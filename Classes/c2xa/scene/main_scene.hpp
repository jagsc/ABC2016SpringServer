/************************************************************************************//**
    @file   c2xa/scene/main_scene.hpp
    @author 新ゝ月かりな
    @date   created on 2015/12/24
****************************************************************************************/
#ifndef C2XA_SCENE_MAIN_SCENE_HPP
#define C2XA_SCENE_MAIN_SCENE_HPP

#include <cocos2d.h>
#include <c2xa/communication/bluetooth/server.hpp>
#include <c2xa/communication/data.hpp>
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
            using sample = math::sample<data, 16, 16>;
            bluetooth::listener listen_server_;
            std::shared_ptr<bluetooth::connection> connection_server_;
            fft fft_;
            sample sample_;
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