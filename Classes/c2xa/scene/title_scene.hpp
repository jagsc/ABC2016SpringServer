
/**

    @file   title_scene.hpp
    @brief  title_scene

    @author 新ゝ月かりな(NewNotMoon)
    @date   created on 2016/02/09


*/
#pragma once
#ifndef C2XA_SCENE_TITLE_SCENE_HPP
#define C2XA_SCENE_TITLE_SCENE_HPP

#include <cocos2d.h>

#include <c2xa/communication/bluetooth/server.hpp>

namespace c2xa
{
    namespace scene
    {
        class title_scene
            : public cocos2d::Scene
        {
        private:
            char buffer_[ 2048 ];
            std::shared_ptr<bluetooth::listener> listener_;
            std::shared_ptr<bluetooth::connection> connection_1p;
            std::shared_ptr<bluetooth::connection> connection_2p;
            BTH_ADDR address_1p;
            BTH_ADDR address_2p;

        public:
            CREATE_FUNC( title_scene );
            virtual bool init() override;
            virtual void update( float ) override;
        };
    }
}

#endif//C2XA_SCENE_TITLE_SCENE_HPP