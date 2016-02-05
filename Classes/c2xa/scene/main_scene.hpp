/************************************************************************************//**
    @file   c2xa/scene/main_scene.hpp
    @author 新ゝ月かりな
    @date   created on 2015/12/24
****************************************************************************************/
#ifndef C2XA_SCENE_MAIN_SCENE_HPP
#define C2XA_SCENE_MAIN_SCENE_HPP

#include <cocos2d.h>
#include <c2xa/experimental/data_analyzer.hpp>
#include <c2xa/experimental/fast_fourier_transform.hpp>
#include <c2xa/communication/bluetooth_winsock_wrapper.hpp>

namespace c2xa
{
    namespace scene
    {
        class main_scene
            : public cocos2d::Scene
        {
        private:
            static constexpr unsigned int sample_size = 256;
            bthserver bluetooth_server_;
            fft fft_;
            experimental::sample<sample_size> sample_;
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