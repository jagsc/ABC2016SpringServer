
/**

    @file   result_scene.hpp
    @brief  result_scene

    @author 新ゝ月かりな(NewNotMoon)
    @date   created on 2016/02/09

*/
#pragma once
#ifndef C2XA_SCENE_RESULT_SCENE_HPP
#define C2XA_SCENE_RESULT_SCENE_HPP

#include <cocos2d.h>

#include <c2xa/utility.hpp>

namespace c2xa
{
    class communication_node;

    namespace scene
    {
        class result_scene
            : public cocos2d::Scene
        {
        public:
            static result_scene* create( communication_node* c )
            {
                return create_template<result_scene>( c );
            }
            virtual bool init( communication_node* );
        };
    }
}


#endif//C2XA_SCENE_RESULT_SCENE_HPP