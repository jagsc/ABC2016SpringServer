/************************************************************************************//**
    @file   c2xa/scene/experimental/data_analyzer.hpp
    @author 新ゝ月かりな
    @date   created on 2015/12/24
****************************************************************************************/
#ifndef C2XA_SCENE_EXPERIMENTAL_DATA_ANALYZER_HPP
#define C2XA_SCENE_EXPERIMENTAL_DATA_ANALYZER_HPP

#include <cocos2d.h>

namespace c2xa
{
    namespace experimental
    {
        struct acceleration
        {
            float x;
            float y;
            float z;
        };
        struct gyro
        {
            float x;
            float y;
            float z;
        };

        class data_analyzer
            : public cocos2d::Node
        {
        public:
            CREATE_FUNC( data_analyzer );
            virtual bool init() override
            {
                using namespace cocos2d;
                if( !Node::init() )
                {
                    return false;
                }

                std::string model_ = "model/tortoise.c3b";
                auto sprite_ = Sprite3D::create( model_ );
                sprite_->setScale( 0.1f );
                auto size_ = Director::getInstance()->getWinSize();
                sprite_->setPosition( Vec2( size_.width * 4.f / 5.f, size_.height / 2.f ) );
                addChild( sprite_ );

                //auto animation_ = Animation3D::create( model_ );
                //if( animation_ )
                //{
                //    auto animate_ = Animate3D::create( animation_, 0.f, 1.933f );
                //    auto swim_ = RepeatForever::create( animate_ );
                //    sprite_->runAction( swim_ );
                //}

                //auto touch_listener_ = EventListenerTouchOneByOne::create();
                //touch_listener_->onTouchBegan = [ = ]( Touch* t_, Event* )
                //{
                //    sprite_->stopActionByTag( 1 );
                //    auto move_ = MoveTo::create( 2.f, t_->getLocation() );
                //    move_->setTag( 1 );
                //    sprite_->runAction( move_ );
                //    return true;
                //};

                //auto dispatcher_ = Director::getInstance()->getEventDispatcher();
                //dispatcher_->addEventListenerWithSceneGraphPriority( touch_listener_, this );

                std::string csv_ = FileUtils::getInstance()->getStringFromFile( "data/sensor.csv" );
                auto begin_ = csv_.cbegin();
                auto end_ = csv_.cbegin();
                auto eof_ = csv_.cend();
                acceleration a_;
                gyro g_;

                auto skip = [ & ]
                {
                    while( end_ != eof_ && *end_ != '\n' )
                    {
                        std::advance( end_, 1 );
                    }
                    return true;
                };
                auto scan = [ & ]
                {
                    auto result_ = std::sscanf
                    (
                        std::string{ begin_, end_ }.c_str(),
                        "%f,%f,%f,%f,%f,%f",
                        &a_.x, &a_.y, &a_.z, &g_.x, &g_.y, &g_.z
                    );
                    if( result_ == EOF || result_ < 6 )
                    {
                        return false;
                    }
                    return true;
                };

                while( skip() && scan() )
                {
                    //
                    cocos2d::log(
                        "x:%f,y:%f,z:%f\nx:%f,y:%f,z:%f\n",
                        a_.x, a_.y, a_.z, g_.x, g_.y, g_.z );
                    if( end_ == eof_ )
                    {
                        break;
                    }
                    std::advance( end_, 1 );
                    begin_ = end_;
                }

                return true;
            }
        };
    }
}

#endif//C2XA_SCENE_EXPERIMENTAL_DATA_ANALYZER_HPP