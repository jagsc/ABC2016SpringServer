#pragma once 
#ifndef C2XA_COMMUNICATION_BLUETOOTH_SERVER_HPP
#define C2XA_COMMUNICATION_BLUETOOTH_SERVER_HPP

#include <c2xa/communication/windows_socket_api.hpp>
// include èáèòíçà”
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

        class listen_server
        {
        private:
            wsa wsa_;
            SOCKADDR_BTH address_;
            CSADDR_INFO  info_;
            WSAQUERYSET  quset_;
            SOCKET       listen_socket_;
            //GUID uuid_;

        public:
            listen_server();
            ~listen_server();

        public:
            SOCKET get_listen_socket() const
            {
                return listen_socket_;
            }
            optional<SOCKET> accept();
        };

        class connection_server
        {
        private:
            SOCKET socket_;

        public:
            enum class socket_state
            {
                empty,   // socket is empty
                failure, // timeout or error
                success, // receive successed
            };

        public:
            connection_server( SOCKET );
            ~connection_server();
            socket_state receive( char*, int, int );
            int send( char*, int, int );
        };
    }
}

#endif