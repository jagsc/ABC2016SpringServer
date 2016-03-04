#pragma once 
#ifndef C2XA_COMMUNICATION_BLUETOOTH_SERVER_HPP
#define C2XA_COMMUNICATION_BLUETOOTH_SERVER_HPP

#include <tuple>
#include <memory>

#include <c2xa/communication/windows_socket_api.hpp>
// include 順序注意
#include <ws2bth.h>

#include <c2xa/optional.hpp>

namespace c2xa
{
    namespace bluetooth
    {
        enum class io_mode
        {
            blocking,
            non_blocking,
        };
        static void change_io_mode( io_mode mode_, SOCKET socket_ )
        {
            u_long value_ = 0;
            switch( mode_ )
            {
            case io_mode::blocking:
                break;
            case io_mode::non_blocking:
                value_ = 1;
                break;
            }
            ::ioctlsocket( socket_, FIONBIO, &value_ );
        }

        class connection
        {
        private:
            SOCKET socket_;
            SOCKADDR_BTH client_;

        public:
            enum class socket_state
            {
                empty,   // socket is empty
                failure, // timeout or error
                success, // receive successed
            };

        public:
            connection( SOCKET, SOCKADDR_BTH );
            connection( connection const& ) = delete;
            connection( connection && );
            ~connection();

            void operator=( connection const& ) = delete;
            void operator=( connection && );

        public:
            socket_state receive( char*, int, int );
            int send( char*, int, int );

            BTH_ADDR get_client_address() const
            {
                return client_.btAddr;
            }
        };

        using shared_connection = std::shared_ptr<bluetooth::connection>;

        class listener
        {
        private:
            wsa wsa_;
            SOCKADDR_BTH address_;
            CSADDR_INFO  info_;
            WSAQUERYSET  quset_;
            SOCKET       listen_socket_;
            //GUID uuid_;

        public:
            listener();
            ~listener();

        public:
            SOCKET get_listen_socket() const
            {
                return listen_socket_;
            }
            optional<std::unique_ptr<connection>> accept();
        };
    }
}

#endif