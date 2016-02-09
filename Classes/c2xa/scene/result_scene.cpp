
/**

    @file   result_scene.cpp
    @brief  result_scene

    @author 新ゝ月かりな(NewNotMoon)
    @date   created on 2016/02/09

*/

#include <c2xa/scene/result_scene.hpp>

using namespace c2xa::scene;

bool result_scene::init()
{
    using namespace cocos2d;
    if( !Scene::init() )
    {
        return false;
    }
    setName( "result_scene" );
    scheduleUpdate();

    return true;
}

void result_scene::update( float )
{
}