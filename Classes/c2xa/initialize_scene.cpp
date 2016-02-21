
/**

    @file   initialize_scene.cpp
    @brief  initialize_scene

    @author 新ゝ月かりな(NewNotMoon)
    @date   created on 2016/02/17

*/

#include <c2xa/initialize_scene.hpp>

#include <cocos2d.h>

#include <c2xa/scene/title_scene.hpp>
#include <c2xa/communication/node.hpp>

cocos2d::Scene* c2xa::initialize_scene()
{
    auto com_node_ = com_node::create();
    com_node_->setName( "com_node" );
    auto scene_ = c2xa::scene::title_scene::create( com_node_ );
    return scene_;
}