
/**

    @file   title_scene.cpp
    @brief  title_scene

    @author 新ゝ月かりな(NewNotMoon)
    @date   created on 2016/02/09

*/

#include <c2xa/config.hpp>
#include <c2xa/exception.hpp>
#include <c2xa/scene/title_scene.hpp>
#include <c2xa/communication/node.hpp>

using namespace c2xa::scene;

namespace
{
    static const cocos2d::Vec2 message_text_{ c2xa::app_width - 470, c2xa::app_height - 580 };
    static const cocos2d::Vec2 condition_1p_text_    { c2xa::app_width - 550, c2xa::app_height - 730 };
    static const cocos2d::Vec2 condition_1p_particle_{ c2xa::app_width - 500, c2xa::app_height - 750 };
    static const cocos2d::Vec2 condition_2p_text_    { c2xa::app_width - 550, c2xa::app_height - 770 };
    static const cocos2d::Vec2 condition_2p_particle_{ c2xa::app_width - 500, c2xa::app_height - 790 };
}

bool title_scene::init()
{
    using namespace cocos2d;
    if( !Scene::init() )
    {
        return false;
    }
    setName( "title_scene" );
    scheduleUpdate();

    auto bg_ = Sprite::create( "img/title_bg.png" );
    bg_->setAnchorPoint( Vec2::ANCHOR_BOTTOM_LEFT );
    addChild( bg_, 1 );

    auto logo_ = Sprite::create( "img/logo.png" );
    logo_->setAnchorPoint( Vec2::ANCHOR_TOP_RIGHT );
    logo_->setPosition( Vec2{ app_width - 100, app_height - 100 } );
    addChild( logo_, 10 );

    auto particle_ = ParticleSystemQuad::create( "particle/fire.plist" );
    particle_->setPosition( Vec2{ app_width / 2, 0 } );
    particle_->resetSystem();
    addChild( particle_, 2 );

    auto bg_add_ = Sprite::create( "img/title_add.png" );
    bg_add_->setAnchorPoint( Vec2::ANCHOR_BOTTOM_LEFT );
    bg_add_->setBlendFunc( { GL_SRC_ALPHA, GL_ONE } );
    bg_add_->setOpacity( 100 );
    addChild( bg_add_, 3 );

    auto text_message_ = Label::createWithTTF( "waiting for players...", "font/Stroke.ttf", 32 );
    text_message_->setPosition( message_text_ );
    text_message_->setColor( Color3B{ 255, 255, 99 } );
    text_message_->setOpacity( 0 );
    text_message_->setName( "text_message" );
    addChild( text_message_, 10 );

    auto blink_ = RepeatForever::create( Sequence::create( FadeTo::create( 0.3f, 255 ), FadeTo::create( 1.2f, 0 ), nullptr ) );
    text_message_->runAction( blink_ );

    auto text_1p_ = Label::createWithTTF( "", "font/Stroke.ttf", 32 );
    text_1p_->setPosition( condition_1p_text_ + Vec2{ 7, 0 } );
    text_1p_->setColor( Color3B{ 255, 230, 99 } );
    text_1p_->setAnchorPoint( Vec2::ANCHOR_TOP_LEFT );
    text_1p_->setName( "text_1p_condition" );
    addChild( text_1p_, 10 );

    auto text_2p_ = Label::createWithTTF( "", "font/Stroke.ttf", 32 );
    text_2p_->setPosition( condition_2p_text_ );
    text_2p_->setColor( Color3B{ 255, 230, 99 } );
    text_2p_->setAnchorPoint( Vec2::ANCHOR_TOP_LEFT );
    text_2p_->setName( "text_2p_condition" );
    addChild( text_2p_, 11 );

    auto observer_1p = observer<connection_state>::create( [ this ]( connection_state state_ )
    {
        auto text_1p = static_cast<cocos2d::Label*>( getChildByName( "text_1p_condition" ) );
        switch( state_ )
        {
        case connection_state::connect:
        {
            text_1p->setString( "1P: CONNECT" );
            text_1p->setOpacity( 0 );
            text_1p->runAction( FadeTo::create( 0.3f, 255 ) );

            auto particle_ = ParticleSystemQuad::create( "particle/entry.plist" );
            particle_->setAnchorPoint( Vec2::ANCHOR_TOP_LEFT );
            particle_->setPosition( condition_1p_particle_ );
            particle_->setAutoRemoveOnFinish( true );
            addChild( particle_, 12 );

            break;
        }
        case connection_state::disconnect:
        {
            text_1p->setString( "1P: DISCONNECT" );
            break;
        }
        case connection_state::reconnect:
        {
            text_1p->setString( "1P: CONNECT" );
            break;
        }
        }
    } );
    addChild( observer_1p );

    auto observer_2p = observer<connection_state>::create( [ this ]( connection_state state_ )
    {
        auto text_2p = static_cast<cocos2d::Label*>( getChildByName( "text_2p_condition" ) );
        switch( state_ )
        {
            case connection_state::connect:
            {
                text_2p->setString( "2P: CONNECT" );
                text_2p->setOpacity( 0 );
                text_2p->runAction( FadeTo::create( 0.3f, 255 ) );

                auto particle_ = ParticleSystemQuad::create( "particle/entry.plist" );
                particle_->setAnchorPoint( Vec2::ANCHOR_TOP_LEFT );
                particle_->setPosition( condition_2p_particle_ );
                particle_->setAutoRemoveOnFinish( true );
                addChild( particle_, 12 );

                break;
            }
            case connection_state::disconnect:
            {
                text_2p->setString( "2P: DISCONNECT" );
                break;
            }
            case connection_state::reconnect:
            {
                text_2p->setString( "2P: CONNECT" );
                break;
            }
        }
    } );
    addChild( observer_2p );

    auto com_ = communication_node::create();
    com_->setName( "com_node" );
    com_->get_subject_1p()->registry_observer( observer_1p );
    com_->get_subject_2p()->registry_observer( observer_2p );
    addChild( com_ );

    return true;
}

void title_scene::update( float )
{
    using namespace cocos2d;

    auto com_node_ = static_cast<communication_node*>( getChildByName( "com_node" ) );
    com_node_->receive_1p( []( auto buffer_ )
    {
    } );
    com_node_->receive_2p( []( auto buffer_ )
    {
    } );
}