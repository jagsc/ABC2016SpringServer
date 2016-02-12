
/**

    @file   node.hpp
    @brief  node

    @author 新ゝ月かりな(NewNotMoon)
    @date   created on 2016/02/13

*/
#pragma once
#ifndef C2XA_COMMUNICATION_NODE_HPP
#define C2XA_COMMUNICATION_NODE_HPP

#include <cocos2d.h>

#include <c2xa/observer.hpp>
#include <c2xa/communication/bluetooth/server.hpp>

namespace c2xa
{
    enum class connection_state
    {
        wait,
        connect,
        disconnect,
        reconnect,
    };
    class communication_node
        : public cocos2d::Node
    {
    private:
        using subject = subject<connection_state>;
    private:
        char buffer_[ 2048 ];
        std::shared_ptr<bluetooth::listener> listener_;
        std::shared_ptr<bluetooth::connection> connection_1p;
        std::shared_ptr<bluetooth::connection> connection_2p;
        BTH_ADDR address_1p;
        BTH_ADDR address_2p;
        subject subject_1p;
        subject subject_2p;

    public:
        CREATE_FUNC( communication_node );
        virtual bool init() override;
        virtual void update( float ) override;
        subject::interface_type *const get_subject_1p()
        {
            return &subject_1p;
        }
        subject::interface_type *const get_subject_2p()
        {
            return &subject_2p;
        }
    };
    using com_node = communication_node;
}

#endif//C2XA_COMMUNICATION_NODE_HPP