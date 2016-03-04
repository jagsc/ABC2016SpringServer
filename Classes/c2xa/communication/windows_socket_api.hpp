
/**

    @file   windows_socket_api.hpp
    @brief  windows_socket_api wrap

    @author 新ゝ月かりな(NewNotMoon)
    @date   created on 2016/00/00

*/
#pragma once
#ifndef C2XA_COMMUNICATION_WINDOWS_SOCKET_API_HPP
#define C2XA_COMMUNICATION_WINDOWS_SOCKET_API_HPP

#include <exception>

#include <WinSock2.h>

#pragma comment(lib, "Ws2_32")

namespace c2xa
{
    //! マルチスレッド非対応 排他制御なし
    class windows_socket_api
    {
    private:
        static unsigned int count_;
        static WSADATA winsock_api_;
        static WSADATA* winsock_api_ptr_;

    public:
        windows_socket_api()
        {
            ++count_;
            if( winsock_api_ptr_ == nullptr )
            {
                winsock_api_ = {};
                if( ::WSAStartup( MAKEWORD( 2, 2 ), &winsock_api_ ) != 0 )
                {
                    throw std::exception( "WSAStartup error." );
                }
                winsock_api_ptr_ = &winsock_api_;
            }
        }
        ~windows_socket_api()
        {
            --count_;
            if( count_ == 0 )
            {
                winsock_api_ ={};
                winsock_api_ptr_ = nullptr;
                ::WSACleanup();
            }
        }
    };
    using wsa = windows_socket_api;
}

#endif//C2XA_COMMUNICATION_WINDOWS_SOCKET_API_HPP