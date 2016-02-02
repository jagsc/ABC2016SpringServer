#pragma once 
#ifndef C2XA_COMMUNICATION_BLUETOOTH_WINSOCK_WRAPPER_HPP
#define C2XA_COMMUNICATION_BLUETOOTH_WINSOCK_WRAPPER_HPP

#include <WinSock2.h>
#include <ws2bth.h>

#pragma comment(lib, "Ws2_32")

class bthserver {
private:
    WSADATA bts_wsadata_;
    SOCKADDR_BTH bts_sa_;
    SOCKET bts_listen_sock_;
    CSADDR_INFO bts_info_;
    WSAQUERYSET bts_quset_;
    SOCKET bts_sock_;
    GUID bts_spp_uuid_;
public:

    int init_wsadata();

    int create_socket();

    int set_bluetooth_sock();

    int get_bluetooth_sockname();

    int wrap_wsa_set_service();

    int listen_accept_connect();

    int recv_data(char*, int , int);

    int send_data(char*,int, int);

    void destroy_sock();

};

#endif