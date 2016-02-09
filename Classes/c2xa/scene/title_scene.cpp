
/**

    @file   title_scene.cpp
    @brief  title_scene

    @author 新ゝ月かりな(NewNotMoon)
    @date   created on 2016/02/09

*/

#include <c2xa/config.hpp>
#include <c2xa/exception.hpp>
#include <c2xa/scene/title_scene.hpp>

using namespace c2xa::scene;

bool title_scene::init()
{
    using namespace cocos2d;
    if( !Scene::init() )
    {
        return false;
    }
    setName( "title_scene" );
    scheduleUpdate();

    Vec2 relative_original_{ 100, app_height - 100 };

    addChild( LayerColor::create( Color4B::WHITE, app_width, app_height ), 1 );

    auto text_1p_ = Label::createWithSystemFont( "1P: disconnect", "Arial", 32 );
    text_1p_->setPosition( relative_original_ );
    text_1p_->setColor( Color3B{ 128, 128, 128 } );
    text_1p_->setAnchorPoint( Vec2::ANCHOR_TOP_LEFT );
    text_1p_->setName( "1p_text" );
    addChild( text_1p_, 10 );

    auto text_2p_ = Label::createWithSystemFont( "2P: disconnect", "Arial", 32 );
    text_2p_->setPosition( relative_original_ + Vec2{ 0, -40 } );
    text_2p_->setColor( Color3B{ 128, 128, 128 } );
    text_2p_->setAnchorPoint( Vec2::ANCHOR_TOP_LEFT );
    text_2p_->setName( "2p_text" );
    addChild( text_2p_, 11 );

    listener_ = std::make_shared<bluetooth::listener>();
    address_1p = 0;
    address_2p = 0;

    return true;
}

void title_scene::update( float )
{
    auto text_1p = static_cast<cocos2d::Label*>( getChildByName( "1p_text" ) );
    auto text_2p = static_cast<cocos2d::Label*>( getChildByName( "2p_text" ) );

    if( listener_ )
    {
        auto accepted_ = listener_->accept();
        if( accepted_ )
        {
            [ this, text_1p, text_2p, &accepted_ ]
            {
                if( !connection_1p )
                {
                    if( address_1p == 0 )
                    {
                        // 初接続
                        connection_1p = std::move( *accepted_ );
                        address_1p = connection_1p->get_client_address();
                        text_1p->setString( "1P: first connecting" );
                        return;
                    }
                    else if( address_1p == ( *accepted_ )->get_client_address() )
                    {
                        // 1p 再接続
                        connection_1p = std::move( *accepted_ );
                        text_1p->setString( "1P: re-connecting" );
                        return;
                    }
                }
                if( !connection_2p )
                {
                    if( address_2p == 0 )
                    {
                        // 初接続
                        connection_2p = std::move( *accepted_ );
                        address_2p = connection_2p->get_client_address();
                        text_2p->setString( "2P: first connecting" );
                        return;
                    }
                    else if( address_2p == ( *accepted_ )->get_client_address() )
                    {
                        // 2p 再接続
                        connection_2p = std::move( *accepted_ );
                        text_2p->setString( "2P: re-connecting" );
                        return;
                    }
                }
            }();
            if( connection_1p && connection_2p )
            {
                listener_.reset();
            }
        }
    }

    if( connection_1p )
    {
        auto receive = [ this ]()
        {
            std::memset( buffer_, '\0', sizeof( buffer_ ) );
            return connection_1p->receive( buffer_, sizeof( buffer_ ), 0 );
        };
        try
        {
            auto state_ = receive();
            while( state_ == bluetooth::connection::socket_state::success )
            {
                state_ = receive();
            }
        }
        catch( bluetooth_disconnect_exception const& e )
        {
            connection_1p.reset();
            text_1p->setString( "1P: dissconnect" );
            if( !listener_ )
            {
                listener_ = std::make_shared<bluetooth::listener>();
            }
        }
    }
    if( connection_2p )
    {
        auto receive = [ this ]
        {
            std::memset( buffer_, '\0', sizeof( buffer_ ) );
            return connection_2p->receive( buffer_, sizeof( buffer_ ), 0 );
        };
        try
        {
            auto state_ = receive();
            while( state_ == bluetooth::connection::socket_state::success )
            {
                state_ = receive();
            }
        }
        catch( bluetooth_disconnect_exception const& e )
        {
            connection_2p.reset();
            text_2p->setString( "2P: dissconnect" );
            if( !listener_ )
            {
                listener_ = std::make_shared<bluetooth::listener>();
            }
        }
    }
}