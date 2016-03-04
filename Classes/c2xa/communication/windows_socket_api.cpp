// ============================================================================
//
//    Author: 新ゝ月かりな(NewNotMoon)
//
//    E-mail: new.not.moon@gmail.com
//    Web: http://new.not-moon.net/
//
// Copyright(c) 2016- 新ゝ月かりな(NewNotMoon). All rights reserved.
// ============================================================================
/**

@file   winsock.cpp
@brief  winsock

@author 新ゝ月かりな(NewNotMoon)
@date   created on 2016/00/00

詳細説明

*/

#include <c2xa/communication/windows_socket_api.hpp>

unsigned int c2xa::windows_socket_api::count_ = 0;
WSADATA c2xa::windows_socket_api::winsock_api_ = {};
WSADATA* c2xa::windows_socket_api::winsock_api_ptr_ = nullptr;
