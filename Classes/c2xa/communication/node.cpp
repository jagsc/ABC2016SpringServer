
/**

    @file   node.cpp
    @brief  node

    @author 新ゝ月かりな(NewNotMoon)
    @date   created on 2016/02/13

*/

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
}