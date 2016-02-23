
/**

    @file   battle_scene.cpp
    @brief  battle_scene

    @author 新ゝ月かりな(NewNotMoon)
    @date   created on 2016/02/09

*/

#include <c2xa/scene/battle_scene.hpp>
#include <c2xa/config.hpp>
#include <c2xa/exception.hpp>
#include <c2xa/player.hpp>
#include <c2xa/communication/node.hpp>
#include <c2xa/communication/parse.hpp>

using namespace c2xa;
using namespace c2xa::scene;

namespace
{
    void debug_view( cocos2d::Layer* that, player::spectrums_type const& spectrums_, action const& action_, cocos2d::Vec2 origin_ )
    {
        using namespace cocos2d;
        auto draw_ = static_cast<cocos2d::DrawNode*>( that->getChildByName( "draw_node" ) );
        if( draw_ == nullptr )
        {
            draw_ = DrawNode::create();
            draw_->setName( "draw_node" );
            that->addChild( draw_, 20 );
        }
        draw_->clear();
        draw_->drawSegment(
            origin_ + cocos2d::Vec2{ 50, 0 },
            origin_ + cocos2d::Vec2{
                50,
                ( static_cast<float>( spectrums_.acceleration_x[ 1 ] ) )
            },
            30,
            cocos2d::Color4F{ 1, 0, 0, 1 } );
        draw_->drawSegment(
            origin_ + cocos2d::Vec2{ 110, 0 },
            origin_ + cocos2d::Vec2{
                110,
                ( static_cast<float>( spectrums_.acceleration_y[ 1 ] ) )
            },
            30,
            cocos2d::Color4F{ 0, 1, 0, 1 } );
        draw_->drawSegment(
            origin_ + cocos2d::Vec2{ 170, 0 },
            origin_ + cocos2d::Vec2{
                170,
                ( static_cast<float>( spectrums_.acceleration_z[ 1 ] ) )
            },
            30,
            cocos2d::Color4F{ 0, 0, 1, 1 } );

        auto text_ = static_cast<cocos2d::Label*>( that->getChildByName( "text" ) );
        if( text_ == nullptr )
        {
            text_ = cocos2d::Label::createWithTTF( "", "font/Stroke.ttf", 40 );
            text_->setName( "text" );
            text_->setColor( Color3B::WHITE );
            text_->setPosition( origin_ + cocos2d::Vec2{ 100, app_height - 60 } );
            that->addChild( text_, 20 );
        }
        auto text_x = static_cast<cocos2d::Label*>( that->getChildByName( "textx" ) );
        if( text_x == nullptr )
        {
            text_x = cocos2d::Label::createWithTTF( "", "font/Stroke.ttf", 40 );
            text_x->setName( "textx" );
            text_x->setColor( Color3B::WHITE );
            text_x->setAnchorPoint( Vec2::ANCHOR_TOP_RIGHT );
            text_x->setPosition( origin_ + cocos2d::Vec2{ 200, app_height - 100 } );
            that->addChild( text_x, 20 );
        }
        auto text_y = static_cast<cocos2d::Label*>( that->getChildByName( "texty" ) );
        if( text_y == nullptr )
        {
            text_y = cocos2d::Label::createWithTTF( "", "font/Stroke.ttf", 40 );
            text_y->setName( "texty" );
            text_y->setColor( Color3B::WHITE );
            text_y->setAnchorPoint( Vec2::ANCHOR_TOP_RIGHT );
            text_y->setPosition( origin_ + cocos2d::Vec2{ 200, app_height - 140 } );
            that->addChild( text_y, 20 );
        }
        auto text_z = static_cast<cocos2d::Label*>( that->getChildByName( "textz" ) );
        if( text_z == nullptr )
        {
            text_z = cocos2d::Label::createWithTTF( "", "font/Stroke.ttf", 40 );
            text_z->setName( "textz" );
            text_z->setColor( Color3B::WHITE );
            text_z->setAnchorPoint( Vec2::ANCHOR_TOP_RIGHT );
            text_z->setPosition( origin_ + cocos2d::Vec2{ 200, app_height - 180 } );
            that->addChild( text_z, 20 );
        }
        text_x->setString( std::to_string( (int)spectrums_.acceleration_x[ 1 ] ) );
        text_y->setString( std::to_string( (int)spectrums_.acceleration_y[ 1 ] ) );
        text_z->setString( std::to_string( (int)spectrums_.acceleration_z[ 1 ] ) );

        switch( action_ )
        {
            case action::slash:
            {
                text_->setString( "slash" ); break;
            }
            case action::thrust:
            {
                text_->setString( "thrust" ); break;
            }
            case action::guard:
            {
                text_->setString( "guard" ); break;
            }
            case action::idle:
            {
                text_->setString( "idle" ); break;
            }
        }
    }
}

battle_scene::battle_scene()
{
}

bool battle_scene::init( communication_node* com_node_ )
{
    using namespace cocos2d;
    if( !Scene::init() )
    {
        return false;
    }
    setName( "battle_scene" );
    scheduleUpdate();

    com_node_->retain();
    com_node_->removeFromParent();
    addChild( com_node_ );
    com_node_->release();

    auto player_1 = player::create();
    player_1->setName( "player_1" );
    addChild( player_1 );

    auto player_2 = player::create();
    player_2->setName( "player_2" );
    addChild( player_2 );

    auto droid_1p = Sprite::create( "img/stubdroid.png" );
    droid_1p->setPosition( Vec2{ app_width / 3, app_height *2 / 5 } );
    addChild( droid_1p, 10 );

    auto droid_2p = Sprite::create( "img/stubdroid.png" );
    droid_2p->setPosition( Vec2{ app_width * 2 / 3, app_height * 2 / 5 } );
    addChild( droid_2p, 10 );

    auto bg_ = Sprite::create( "img/title_bg.png" );
    bg_->setAnchorPoint( Vec2::ANCHOR_BOTTOM_LEFT );
    addChild( bg_, 1 );

    auto bg_img_ = Sprite::create( "img/battle_bg.png" );
    bg_img_->setAnchorPoint( Vec2::ANCHOR_BOTTOM_LEFT );
    addChild( bg_img_, 4 );

    auto bg_front_img_ = Sprite::create( "img/battle_front.png" );
    bg_front_img_->setAnchorPoint( Vec2::ANCHOR_BOTTOM_LEFT );
    addChild( bg_front_img_, 7 );

    auto particle_smoke_ = ParticleSystemQuad::create( "particle/smoke.plist" );
    particle_smoke_->setPosition( Vec2{ app_width / 2, 700 } );
    particle_smoke_->resetSystem();
    addChild( particle_smoke_, 3 );

    {
        auto pf_ = ParticleSystemQuad::create( "particle/fire.plist" );
        pf_->setPosition( Vec2{ 100, 450 } );
        pf_->resetSystem();
        addChild( pf_, 5 );
    }
    {
        auto pf_ = ParticleSystemQuad::create( "particle/fire.plist" );
        pf_->setPosition( Vec2{ app_width - 100, 450 } );
        pf_->resetSystem();
        addChild( pf_, 5 );
    }
    {
        auto pf_ = ParticleSystemQuad::create( "particle/fire.plist" );
        pf_->setPosition( Vec2{ app_width - 400, 250 } );
        pf_->resetSystem();
        addChild( pf_, 5 );
    }
    {
        auto pf_ = ParticleSystemQuad::create( "particle/fire.plist" );
        pf_->setPosition( Vec2{ 500, 350 } );
        pf_->resetSystem();
        addChild( pf_, 5 );
    }
    {
        auto pf_ = ParticleSystemQuad::create( "particle/fire_low.plist" );
        pf_->setPosition( Vec2{ 700, 450 } );
        pf_->resetSystem();
        addChild( pf_, 5 );
    }
    {
        auto pf_ = ParticleSystemQuad::create( "particle/fire_low.plist" );
        pf_->setPosition( Vec2{ 1200, 350 } );
        pf_->resetSystem();
        addChild( pf_, 5 );
    }

    return true;
}

void battle_scene::update( float )
{
    using namespace cocos2d;

    auto com_node_ = static_cast<communication_node*>( getChildByName( "com_node" ) );
    com_node_->receive_1p( [ = ]( auto&& com_, auto&& buffer_ )
    {
        auto player_ = static_cast<player*>( getChildByName( "player_1" ) );
        player_->update_state( buffer_ );
        com_->send_1p();

        auto layer_1 = static_cast<cocos2d::Layer*>( getChildByName( "layer_1" ) );
        if( layer_1 == nullptr )
        {
            layer_1 = Layer::create();
            layer_1->setName( "layer_1" );
            addChild( layer_1, 20 );
        }
        debug_view( layer_1, player_->get_spectrums(), player_->get_action(), Vec2{ 0, 0 } );
    } );
    com_node_->receive_2p( [ = ]( auto&& com_, auto&& buffer_ )
    {
        auto player_ = static_cast<player*>( getChildByName( "player_2" ) );
        player_->update_state( buffer_ );
        com_->send_2p();

        auto layer_2 = static_cast<cocos2d::Layer*>( getChildByName( "layer_2" ) );
        if( layer_2 == nullptr )
        {
            layer_2 = Layer::create();
            layer_2->setName( "layer_2" );
            addChild( layer_2, 20 );
        }
        debug_view( layer_2, player_->get_spectrums(), player_->get_action(), Vec2{ 0, 0 } );
    } );
}