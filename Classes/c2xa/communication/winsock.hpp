
/**

@file   winsock.hpp
@brief  winsock

@author 新ゝ月かりな(NewNotMoon)
@date   created on 2016/00/00

詳細説明

*/
#pragma once
#ifndef C2XA_COMMUNICATION_WINSOCK_HPP
#define C2XA_COMMUNICATION_WINSOCK_HPP

#include <exception>

#include <WinSock2.h>
#include <ws2bth.h>

#pragma comment(lib, "Ws2_32")

namespace c2xa
{
    class windows_socket_api
    {
    private:
        static WSADATA winsock_api_;
        static WSADATA* winsock_api_ptr_;

    public:
        windows_socket_api()
        {
            if( winsock_api_ptr_ == nullptr )
            {
                winsock_api_ ={};
                if( ::WSAStartup( MAKEWORD( 2, 2 ), &winsock_api_ ) != 0 )
                {
                    throw std::exception( "WSAStartup error." );
                }
                winsock_api_ptr_ = &winsock_api_;
            }
        }
        ~windows_socket_api()
        {
            winsock_api_ ={};
            winsock_api_ptr_ = nullptr;
            ::WSACleanup();
        }
    };
}

#endif//C2XA_COMMUNICATION_WINSOCK_HPP