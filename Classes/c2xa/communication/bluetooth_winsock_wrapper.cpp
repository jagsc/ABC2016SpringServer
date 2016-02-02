
#include"bluetooth_winsock_wrapper.hpp"


int bthserver::init_wsadata() {
    bts_wsadata_ = { 0 };
    bts_info_ = { 0 };
    bts_sa_ = { 0 };
    bts_spp_uuid_ = { 0x11111111, 0x1111, 0x1111, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x23 };
    if (WSAStartup(MAKEWORD(2, 2), &bts_wsadata_) != 0) {
        return -1;
    }
    return 0;
}

int bthserver::create_socket() {
    if (INVALID_SOCKET == (bts_listen_sock_ = socket(AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM))) {
        return -1;
    }
    return 0;
}

int bthserver::set_bluetooth_sock() {
    bts_sa_.addressFamily = AF_BTH;
    bts_sa_.port = BT_PORT_ANY;
    if (SOCKET_ERROR == bind(bts_listen_sock_, reinterpret_cast<SOCKADDR*>(&bts_sa_), sizeof(bts_sa_))) {
        return -1;
    }
    return 0;
}

int bthserver::get_bluetooth_sockname() {
    int socksize = sizeof(bts_sa_);
    if (-1 == getsockname(bts_listen_sock_, reinterpret_cast<SOCKADDR*>(&bts_sa_), &socksize)) {
        return -1;
    }
    return 0;
}



int bthserver::wrap_wsa_set_service() {
    bts_info_.LocalAddr.lpSockaddr = reinterpret_cast<LPSOCKADDR>(&bts_sa_);
    bts_info_.LocalAddr.iSockaddrLength = sizeof(bts_sa_);
    bts_info_.iSocketType = SOCK_STREAM;
    bts_info_.iProtocol = BTHPROTO_RFCOMM;

    bts_quset_.dwSize = sizeof(WSAQUERYSET);
    bts_quset_.dwOutputFlags = 0;
    bts_quset_.lpszServiceInstanceName = reinterpret_cast<LPWSTR>("Server");
    bts_quset_.lpServiceClassId = static_cast<LPGUID>(&bts_spp_uuid_);
    bts_quset_.lpVersion = 0;
    bts_quset_.lpszComment = 0;
    bts_quset_.dwNameSpace = NS_BTH;
    bts_quset_.lpNSProviderId = 0;
    bts_quset_.lpszContext = 0;
    bts_quset_.dwNumberOfProtocols = 0;
    bts_quset_.lpafpProtocols = 0;
    bts_quset_.lpszQueryString = 0;
    bts_quset_.dwNumberOfCsAddrs = 1;
    bts_quset_.lpcsaBuffer = &bts_info_;
    bts_quset_.lpBlob = 0;
    if (0 != WSASetService(&bts_quset_, RNRSERVICE_REGISTER, 0)) {
        return -1;
    }
    return 0;
}

int bthserver::listen_accept_connect() {
    listen(bts_listen_sock_, 1);
    SOCKADDR_BTH bts_sa2;
    int bts_ilen = sizeof(bts_sa2);
    if (INVALID_SOCKET == (bts_sock_ = accept(bts_listen_sock_, reinterpret_cast<SOCKADDR*>(&bts_sa2), &bts_ilen))) {
        return -1;
    }
    return 0;
}

int bthserver::recv_data(char *buf, int bufsize, int Flag) {
    int bts_ret;
    bts_ret = recv(bts_sock_, buf, bufsize, Flag);
    return bts_ret;
}

int bthserver::send_data(char *buf, int bufsize, int Flag) {
    int bts_ret;
    bts_ret = send(bts_sock_, buf, bufsize, Flag);
    return bts_ret;
}

void bthserver::destroy_sock() {
    closesocket(bts_listen_sock_);
    closesocket(bts_sock_);
    WSACleanup();
}


