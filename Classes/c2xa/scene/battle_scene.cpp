
/**

    @file   battle_scene.cpp
    @brief  battle_scene

    @author 新ゝ月かりな(NewNotMoon)
    @date   created on 2016/02/09

*/

#include <c2xa/scene/battle_scene.hpp>

using namespace c2xa::scene;

bool battle_scene::init()
{
    using namespace cocos2d;
    if( !Scene::init() )
    {
        return false;
    }
    setName( "battle_scene" );
    scheduleUpdate();

    return true;
}

void battle_scene::update( float )
{
}