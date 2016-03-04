
/**

    @file   demo_scene.cpp
    @brief  demo_scene

    @author 新ゝ月かりな(NewNotMoon)
    @date   created on 2016/02/09

*/

#include <c2xa/scene/demo_scene.hpp>

using namespace c2xa::scene;

bool demo_scene::init()
{
    using namespace cocos2d;
    if( !Scene::init() )
    {
        return false;
    }
    setName( "demo_scene" );
    scheduleUpdate();

    return true;
}

void demo_scene::update( float )
{
}