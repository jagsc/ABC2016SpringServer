/************************************************************************************//**
    @file   c2xa/scene/main_scene.cpp
    @author 新ゝ月かりな
    @date   created on 2015/09/24
****************************************************************************************/
#include <c2xa/scene/main_scene.hpp>

using namespace cocos2d;
using namespace c2xa::scene;

bool main_scene::init()
{
    if( !Scene::init() )
    {
        return false;
    }

    setName( "main_scene" );

    std::string model_ = "model/tortoise.c3b";
    auto sprite_ = Sprite3D::create( model_ );
    sprite_->setScale( 0.1f );
    auto size_ = Director::getInstance()->getWinSize();
    sprite_->setPosition( Vec2( size_.width * 4.f / 5.f, size_.height / 2.f ) );
    addChild( sprite_ );

    auto animation_ = Animation3D::create( model_ );
    if( animation_ )
    {
        auto animate_ = Animate3D::create( animation_, 0.f, 1.933f );
        auto swim_ = RepeatForever::create( animate_ );
        sprite_->runAction( swim_ );
    }

    auto touch_listener_ = EventListenerTouchOneByOne::create();
    touch_listener_->onTouchBegan = [ = ]( Touch* t_, Event* )
    {
        sprite_->stopActionByTag( 1 );
        auto move_ = MoveTo::create( 2.f, t_->getLocation() );
        move_->setTag( 1 );
        sprite_->runAction( move_ );
        return true;
    };

    auto dispatcher_ = Director::getInstance()->getEventDispatcher();
    dispatcher_->addEventListenerWithSceneGraphPriority( touch_listener_, this );

    return true;
}