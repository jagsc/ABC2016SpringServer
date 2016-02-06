
#include <app_delegate.hpp>
#include <c2xa/scene/main_scene.hpp>
//#include <c2xa/scene/title_scene.hpp>
#include <c2xa/config.hpp>
//#include <c2xa/utility.hpp>

using namespace cocos2d;

app_delegete::app_delegete() {
}

app_delegete::~app_delegete()
{
}

void app_delegete::initGLContextAttrs()
{
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};
    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool app_delegete::applicationDidFinishLaunching() {
    auto director = Director::getInstance(); // require initialize
    auto glview = director->getOpenGLView();
    if(!glview) {
        //glview = GLViewImpl::create("My Game");
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
        glview = GLViewImpl::createWithRect( c2xa::app_name, c2xa::app_rectangle, 1 );
        auto h = GetSystemMetrics( SM_CYFULLSCREEN ) - 50;
        glview->setFrameSize( c2xa::app_width * h / c2xa::app_rectangle.size.height, h );
#else
        glview = GLViewImpl::create( c2xa::app_name );
#endif
        director->setOpenGLView(glview);
    }

#ifdef COCOS2D_DEBUG
    // 当面はFPS表示します
    director->setDisplayStats(true);
#endif

    // 60FPSでまずは進めます
    director->setAnimationInterval(1.0f / 60.f);

    // Set the design resolution
    glview->setDesignResolutionSize( c2xa::app_size.width, c2xa::app_size.height, ResolutionPolicy::SHOW_ALL);
    //director->setContentScaleFactor( 0.5 );

    register_all_packages();

    //c2xa::lua::initialize_engine( c2xa::lua::get_engine() );
    //c2xa::lua::initialize_state( c2xa::lua::get_state() );

    // create a scene. it's an autorelease object
    auto scene = c2xa::scene::main_scene::create();

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void app_delegete::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void app_delegete::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
