#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    std::string model_ = "model/tortoise.c3b";
    auto sprite_ = Sprite3D::create( model_ );
    sprite_->setScale( 0.1f );
    auto size_ = Director::getInstance()->getWinSize();
    sprite_->setPosition( Vec2( size_.width * 4.f / 5.f, size_.height / 2.f ) );
    addChild( sprite_ );

    auto animation_ = Animation3D::create( model_ );
    if( animation_ )
    {
        auto animate_ = Animate3D::create( animation_, 0.f, 1.933f );
        auto swim_ = RepeatForever::create( animate_ );
        sprite_->runAction( swim_ );
    }

    auto touch_listener_ = EventListenerTouchOneByOne::create();
    touch_listener_->onTouchBegan = [ = ]( Touch* t_, Event* )
    {
        sprite_->stopActionByTag( 1 );
        auto move_ = MoveTo::create( 2.f, t_->getLocation() );
        move_->setTag( 1 );
        sprite_->runAction( move_ );
        return true;
    };

    auto dispatcher_ = Director::getInstance()->getEventDispatcher();
    dispatcher_->addEventListenerWithSceneGraphPriority( touch_listener_, this );
    
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
