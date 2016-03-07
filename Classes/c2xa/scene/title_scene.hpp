
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

#include <c2xa/utility.hpp>

namespace c2xa
{
    class communication_node;

    namespace scene
    {
        class title_scene
            : public cocos2d::Scene
        {
        private:
            bool ready1p_ = false;
            bool ready2p_ = false;

        public:
            static title_scene* create( communication_node* c )
            {
                return create_template<title_scene>( c );
            }
            virtual bool init( communication_node* );
            virtual void update( float ) override;
        };
    }
}

#endif//C2XA_SCENE_TITLE_SCENE_HPP