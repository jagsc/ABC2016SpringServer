
/**

    @file   node.cpp
    @brief  node

    @author 新ゝ月かりな(NewNotMoon)
    @date   created on 2016/02/13

*/

#include <c2xa/exception.hpp>
#include <c2xa/communication/node.hpp>

using namespace c2xa;

bool communication_node::init()
{
    using namespace cocos2d;
    if( !Node::init() )
    {
        return false;
    }
    setName( "communication_node" );
    scheduleUpdate();

    listener_ = std::make_shared<bluetooth::listener>();
    address_1p = 0;
    address_2p = 0;

    return true;
}

void communication_node::update( float )
{
    using namespace cocos2d;
    if( listener_ )
    {
        auto accepted_ = listener_->accept();
        if( accepted_ )
        {
            do
            {
                if( !connection_1p )
                {
                    if( address_1p == 0 )
                    {
                        // 初接続
                        connection_1p = std::move( *accepted_ );
                        address_1p = connection_1p->get_client_address();
                        subject_1p.notify( connection_state::connect );
                        break;
                    }
                    else if( address_1p == ( *accepted_ )->get_client_address() )
                    {
                        // 1p 再接続
                        connection_1p = std::move( *accepted_ );
                        subject_1p.notify( connection_state::reconnect );
                        break;
                    }
                }
                if( !connection_2p )
                {
                    if( address_2p == 0 )
                    {
                        // 初接続
                        connection_2p = std::move( *accepted_ );
                        address_2p = connection_2p->get_client_address();
                        subject_2p.notify( connection_state::connect );
                        break;
                    }
                    else if( address_2p == ( *accepted_ )->get_client_address() )
                    {
                        // 2p 再接続
                        connection_2p = std::move( *accepted_ );
                        subject_2p.notify( connection_state::reconnect );
                        break;
                    }
                }
            }
            while( false );
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
        catch( bluetooth_disconnect_exception const& )
        {
            connection_1p.reset();
            if( !listener_ )
            {
                listener_ = std::make_shared<bluetooth::listener>();
            }
            subject_1p.notify( connection_state::disconnect );
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
        catch( bluetooth_disconnect_exception const& )
        {
            connection_2p.reset();
            if( !listener_ )
            {
                listener_ = std::make_shared<bluetooth::listener>();
            }
            subject_2p.notify( connection_state::disconnect );
        }
    }
}