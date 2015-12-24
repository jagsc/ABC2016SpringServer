/************************************************************************************//**
    @file   c2xa/scene/main_scene.cpp
    @author êVÅTåéÇ©ÇËÇ»
    @date   created on 2015/09/24
****************************************************************************************/
#include <c2xa/scene/main_scene.hpp>
#include <c2xa/experimental/data_analyzer.hpp>

using namespace c2xa::scene;

bool main_scene::init()
{
    using namespace cocos2d;
    if( !Scene::init() )
    {
        return false;
    }

    setName( "main_scene" );

    addChild( experimental::data_analyzer::create() );
    
    return true;
}