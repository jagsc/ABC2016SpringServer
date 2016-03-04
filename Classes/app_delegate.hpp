#ifndef C2XA_APP_DELEGATE_HPP
#define C2XA_APP_DELEGATE_HPP

#include "cocos2d.h"
#include <c2xa/communication/windows_socket_api.hpp>

/**
@brief    The cocos2d Application.

The reason for implement as private inheritance is to hide some interface call by Director.
*/
class app_delegete : private cocos2d::Application
{
    c2xa::wsa wsa_;
public:
    app_delegete();
    virtual ~app_delegete();

    virtual void initGLContextAttrs();

    /**
    @brief    Implement Director and Scene init code here.
    @return true    Initialize success, app continue.
    @return false   Initialize failed, app terminate.
    */
    virtual bool applicationDidFinishLaunching();

    /**
    @brief  The function be called when the application enter background
    @param  the pointer of the application
    */
    virtual void applicationDidEnterBackground();

    /**
    @brief  The function be called when the application enter foreground
    @param  the pointer of the application
    */
    virtual void applicationWillEnterForeground();
};

#endif// C2XA_APP_DELEGATE_HPP

