
#include <c2xa/exception.hpp>
#include <c2xa/communication/bluetooth/server.hpp>

using namespace c2xa::bluetooth;

listener::listener()
{
    info_ = {};
    address_ = {};


    // listen用ソケット作成
    if( INVALID_SOCKET == ( listen_socket_ = ::socket( AF_BTH, SOCK_STREAM, BTHPROTO_RFCOMM ) ) )
    {
        C2XA_THROW( bluetooth_exception, "listen_socket create failed." );
    }

    address_.addressFamily = AF_BTH;
    address_.port = BT_PORT_ANY;
    if( SOCKET_ERROR == ::bind( listen_socket_, reinterpret_cast<SOCKADDR*>( &address_ ), sizeof( address_ ) ) )
    {
        C2XA_THROW( bluetooth_exception, "bind failed." );
    }

    int socksize = sizeof( address_ );
    if( -1 == ::getsockname( listen_socket_, reinterpret_cast<SOCKADDR*>( &address_ ), &socksize ) )
    {
        C2XA_THROW( bluetooth_exception, "getsockname failed." );
    }

    info_.LocalAddr.lpSockaddr = reinterpret_cast<LPSOCKADDR>( &address_ );
    info_.LocalAddr.iSockaddrLength = sizeof( address_ );
    info_.iSocketType = SOCK_STREAM;
    info_.iProtocol = BTHPROTO_RFCOMM;

    quset_.dwSize = sizeof( WSAQUERYSET );
    quset_.dwOutputFlags = 0;
    quset_.lpszServiceInstanceName = reinterpret_cast<LPWSTR>( "Server" );
    //quset_.lpServiceClassId = static_cast<LPGUID>( &bts_spp_uuid_ );
    quset_.lpServiceClassId = const_cast<LPGUID>( &SerialPortServiceClass_UUID );
    quset_.lpVersion = 0;
    quset_.lpszComment = 0;
    quset_.dwNameSpace = NS_BTH;
    quset_.lpNSProviderId = 0;
    quset_.lpszContext = 0;
    quset_.dwNumberOfProtocols = 0;
    quset_.lpafpProtocols = 0;
    quset_.lpszQueryString = 0;
    quset_.dwNumberOfCsAddrs = 1;
    quset_.lpcsaBuffer = &info_;
    quset_.lpBlob = 0;
    if( 0 != ::WSASetService( &quset_, RNRSERVICE_REGISTER, 0 ) )
    {
        C2XA_THROW( bluetooth_exception, "WSASetService error." );
    }

    if( -1 == ::listen( listen_socket_, 0 ) )
    {
        C2XA_THROW( bluetooth_exception, "listen error." );
    }
    change_io_mode( io_mode::non_blocking, listen_socket_ );
}

listener::~listener()
{
    if( 0 != ::WSASetService( &quset_, RNRSERVICE_DELETE, 0 ) )
    {
        C2XA_THROW( bluetooth_exception, "WSASetService error." );
    }
    ::closesocket( listen_socket_ );
}

c2xa::optional<std::unique_ptr<connection>> listener::accept()
{
    SOCKADDR_BTH client_address_ = {};
    int address_size_ = sizeof( client_address_ );
    SOCKET tmp_ = ::accept( listen_socket_, reinterpret_cast<SOCKADDR*>( &client_address_ ), &address_size_ );

    if( INVALID_SOCKET == tmp_ )
    {
        if( ::WSAGetLastError() == WSAEWOULDBLOCK )
        {
            return none;
        }
        else
        {
            C2XA_THROW( bluetooth_disconnect_exception, "accept error" );
        }
    }
    return std::make_unique<connection>( tmp_, client_address_ );
}

connection::connection( SOCKET socket_, SOCKADDR_BTH c )
    : socket_{ socket_ }
    , client_( c )
{
}

connection::connection( connection && a )
{
    socket_ = a.socket_;
    client_ = a.client_;
    a.socket_ = 0;
    a.client_ = {};
}

void connection::operator=( connection && a )
{
    socket_ = a.socket_;
    client_ = a.client_;
    a.socket_ = 0;
    a.client_ = {};
}

connection::~connection()
{
    if( socket_ != 0 )
    {
        ::closesocket( socket_ );
    }
}

connection::socket_state connection::receive( char *buffer_, int buffer_size_, int flag_ )
{
    int return_ = ::recv( socket_, buffer_, buffer_size_, flag_ );
    if( return_ > 0 )
    {
        return socket_state::success;
    }
    else
    {
        if( WSAGetLastError() == WSAEWOULDBLOCK )
        {
            return socket_state::empty;
        }
        else
        {
            C2XA_THROW( bluetooth_disconnect_exception, "receive error" );
        }
    }
}

int connection::send( char *buf, int bufsize, int Flag )
{
    int bts_ret;
    bts_ret = ::send( socket_, buf, bufsize, Flag );
    return bts_ret;
}