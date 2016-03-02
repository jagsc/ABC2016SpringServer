
/**

    @file   result_scene.cpp
    @brief  result_scene

    @author 新ゝ月かりな(NewNotMoon)
    @date   created on 2016/02/09

*/

#include <c2xa/scene/result_scene.hpp>
#include <c2xa/scene/title_scene.hpp>
#include <c2xa/config.hpp>
#include <c2xa/communication/node.hpp>

using namespace c2xa::scene;

bool result_scene::init( c2xa::communication_node* com_node_ )
{
    using namespace cocos2d;
    if( !Scene::init() )
    {
        return false;
    }
    setName( "result_scene" );

    com_node_->retain();
    com_node_->removeFromParent();
    addChild( com_node_ );
    com_node_->release();

    auto bg_ = Sprite::create( "img/result_bg.png" );
    bg_->setAnchorPoint( Vec2::ANCHOR_BOTTOM_LEFT );
    addChild( bg_, 1 );

    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto keyboard_listener_ = EventListenerKeyboard::create();
    keyboard_listener_->onKeyPressed = [ = ]( EventKeyboard::KeyCode key_, Event* event_ )
    {
        com_node_->send_1p( "scene:title", 11 );
        com_node_->send_2p( "scene:title", 11 );
        Director::getInstance()
            ->replaceScene(
                TransitionFade::create(
                    1.0f,
                    title_scene::create( com_node_ )
                )
            );
        dispatcher->removeEventListenersForTarget( this );
    };
    dispatcher->addEventListenerWithSceneGraphPriority( keyboard_listener_, this );

    return true;
}