
/**

    @file   title_scene.cpp
    @brief  title_scene

    @author 新ゝ月かりな(NewNotMoon)
    @date   created on 2016/02/09

*/

#include <c2xa/config.hpp>
#include <c2xa/exception.hpp>
#include <c2xa/player.hpp>
#include <c2xa/scene/title_scene.hpp>
#include <c2xa/scene/battle_scene.hpp>
#include <c2xa/communication/node.hpp>

#include <AudioEngine.h>

using namespace c2xa;
using namespace c2xa::scene;

namespace
{
    static const cocos2d::Vec2 message_text_{ c2xa::app_width - 470, c2xa::app_height - 580 };
    static const cocos2d::Vec2 condition_1p_text_    { c2xa::app_width - 550, c2xa::app_height - 730 };
    static const cocos2d::Vec2 condition_1p_particle_{ c2xa::app_width - 500, c2xa::app_height - 750 };
    static const cocos2d::Vec2 condition_2p_text_    { c2xa::app_width - 550, c2xa::app_height - 770 };
    static const cocos2d::Vec2 condition_2p_particle_{ c2xa::app_width - 500, c2xa::app_height - 790 };
}

bool title_scene::init( communication_node* com_node_ )
{
    using namespace cocos2d;
    if( !Scene::init() )
    {
        return false;
    }
    setName( "title_scene" );
    scheduleUpdate();

    com_node_->retain();
    com_node_->removeFromParent();
    addChild( com_node_ );
    com_node_->release();

    auto bgm_id = cocos2d::experimental::AudioEngine::play2d( "snd/title_bgm.mp3", true, 0.3f, nullptr );

    auto bg_ = Sprite::create( "img/title_bg.png" );
    bg_->setAnchorPoint( Vec2::ANCHOR_BOTTOM_LEFT );
    addChild( bg_, 1 );

    auto logo_ = Sprite::create( "img/logo.png" );
    logo_->setAnchorPoint( Vec2::ANCHOR_TOP_RIGHT );
    logo_->setPosition( Vec2{ app_width - 100, app_height - 100 } );
    addChild( logo_, 10 );

    auto particle_ = ParticleSystemQuad::create( "particle/spark.plist" );
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

    auto text_1p_ = Label::createWithTTF( com_node_->is_connection_1p() ? "1P: CONNECT" : "", "font/Stroke.ttf", 32 );
    text_1p_->setPosition( condition_1p_text_ + Vec2{ 7, 0 } );
    text_1p_->setColor( Color3B{ 255, 230, 99 } );
    text_1p_->setAnchorPoint( Vec2::ANCHOR_TOP_LEFT );
    text_1p_->setName( "text_1p_condition" );
    addChild( text_1p_, 10 );

    auto text_2p_ = Label::createWithTTF( com_node_->is_connection_2p() ? "2P: CONNECT" : "", "font/Stroke.ttf", 32 );
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
                ready1p_ = true;

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
                ready1p_ = false;
                text_1p->setString( "1P: DISCONNECT" );
                break;
            }
            case connection_state::reconnect:
            {
                ready1p_ = true;
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
                ready2p_ = true;
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
                ready2p_ = false;
                text_2p->setString( "2P: DISCONNECT" );
                break;
            }
            case connection_state::reconnect:
            {
                ready2p_ = true;
                text_2p->setString( "2P: CONNECT" );
                break;
            }
        }
    } );
    addChild( observer_2p );

    com_node_->get_subject_1p()->registry_observer( observer_1p );
    com_node_->get_subject_2p()->registry_observer( observer_2p );

    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto keyboard_listener_ = EventListenerKeyboard::create();
    keyboard_listener_->onKeyPressed = [ = ]( EventKeyboard::KeyCode key_, Event* event_ )
    {
        if( com_node_->is_connection_1p() && com_node_->is_connection_2p() || key_ == EventKeyboard::KeyCode::KEY_D )
        {
            auto message_ = static_cast<cocos2d::Label*>( getChildByName( "text_message" ) );
            message_->setString( "now loading..." );
            Texture2D** tex_dft_ = new Texture2D*[ 12 ];
            Texture2D** tex_sls_ = new Texture2D*[ 45 ];
            Texture2D** tex_thr_ = new Texture2D*[ 53 ];
            std::thread t( [ this, tex_dft_, tex_sls_, tex_thr_, com_node_, bgm_id ]
            {
                {
                    std::lock_guard<std::mutex> lock( mtx );
                    std::atomic_int cnt_dft_ = 0;
                    std::atomic_int cnt_sls_ = 0;
                    std::atomic_int cnt_thr_ = 0;
                    auto load = []( Texture2D** buf_, std::atomic_int& cnt_, char const* format_, int num_ )
                    {
                        auto cacher_ = Director::getInstance()->getTextureCache();
                        for( int i = 0; i <= num_; ++i )
                        {
                            char path_[ 128 ] ={};
                            sprintf( path_, format_, i );
                            cacher_->addImageAsync( path_, [ &cnt_, i, buf_ ]( Texture2D* texture_ )
                            {
                                buf_[ i ] = texture_;
                                ++cnt_;
                            } );
                        }
                    };
                    load( tex_dft_, cnt_dft_, "droid/idle/%d.png", 11 );
                    load( tex_sls_, cnt_sls_, "droid/slash/%d.png", 44 );
                    load( tex_thr_, cnt_thr_, "droid/thrust/%d.png", 52 );

                    while( cnt_dft_ < 12 || cnt_sls_ < 45 || cnt_thr_ < 53 )
                    {
                        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
                    }
                }
                auto scheduler = Director::getInstance()->getScheduler();
                scheduler->performFunctionInCocosThread( [ this, tex_dft_, tex_sls_, tex_thr_, bgm_id ]
                {
                    auto cache = []( Texture2D** tex_, int num, const char *cache_name_, float duration_ )
                    {
                        auto animation_cache_ = AnimationCache::getInstance();
                        auto animation_ = Animation::create();
                        for( int i = 0; i <= num; ++i )
                        {
                            auto size_ = tex_[ i ]->getContentSize();
                            animation_->addSpriteFrameWithTexture( tex_[ i ], Rect{ 0, 0, size_.width, size_.height } );
                        }
                        animation_->setRestoreOriginalFrame( true ); // 再生時間とフレーム数がずれた時は最初からリピート
                        animation_->setDelayPerUnit( duration_ );
                        animation_cache_->addAnimation( animation_, cache_name_ );
                    };

                    cache( tex_dft_, 11, "default", 1.f / 6 );
                    cache( tex_sls_, 44, "slash", 0.9f / 44 );
                    cache( tex_thr_, 52, "thrust", 1.2f / 52 );

                    delete[] tex_dft_;
                    delete[] tex_sls_;
                    delete[] tex_thr_;

                    cocos2d::experimental::AudioEngine::stop( bgm_id );
                    auto com_node_ = static_cast<communication_node*>( getChildByName( "com_node" ) );
                    com_node_->send_1p( "scene:battle", 12 );
                    com_node_->send_2p( "scene:battle", 12 );
                    Director::getInstance()
                        ->replaceScene(
                            TransitionFade::create(
                                1.0f,
                                battle_scene::create( com_node_ )
                                )
                            );
                    Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget( this );
                } );
            } );
            t.detach();
        }
    };
    dispatcher->addEventListenerWithSceneGraphPriority( keyboard_listener_, this );

    return true;
}

void title_scene::update( float )
{
    using namespace cocos2d;

    auto com_node_ = static_cast<communication_node*>( getChildByName( "com_node" ) );
    if( com_node_ )
    {
        com_node_->receive_1p( []( auto&& com_, auto&& buffer_ )
        {
            com_->send_1p();
        } );
        com_node_->receive_2p( []( auto&& com_, auto&& buffer_ )
        {
            com_->send_2p();
        } );
    }
}