
#include "main.h"
#include <app_delegate.hpp>
#include "cocos2d.h"

USING_NS_CC;

int APIENTRY _tWinMain( HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPTSTR    lpCmdLine,
    int       nCmdShow )
{
#if	defined( _MSC_VER ) && defined( _DEBUG )
    //::_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif
    UNREFERENCED_PARAMETER( hPrevInstance );
    UNREFERENCED_PARAMETER( lpCmdLine );

    // create the application instance
    app_delegete app;
    return Application::getInstance()->run();
}