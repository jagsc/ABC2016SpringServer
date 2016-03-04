
/**

    @file   demo_scene.hpp
    @brief  demo_scene

    @author 新ゝ月かりな(NewNotMoon)
    @date   created on 2016/02/09

*/
#pragma once
#ifndef C2XA_SCENE_DEMO_SCENE_HPP
#define C2XA_SCENE_DEMO_SCENE_HPP

#include <cocos2d.h>

namespace c2xa
{
    namespace scene
    {
        class demo_scene
            : public cocos2d::Scene
        {
        public:
            CREATE_FUNC( demo_scene );
            virtual bool init() override;
            virtual void update( float ) override;
        };
    }
}

#endif//C2XA_SCENE_DEMO_SCENE_HPP