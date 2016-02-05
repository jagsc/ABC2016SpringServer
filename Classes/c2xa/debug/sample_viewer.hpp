
/**

@file   sample_viewer.hpp
@brief  sample_viewer

@author 新ゝ月かりな(NewNotMoon)
@date   created on 2016/00/00

詳細説明

*/
#pragma once
#ifndef C2XA_DEBUG_SAMPLE_VIEWER_HPP
#define C2XA_DEBUG_SAMPLE_VIEWER_HPP

#include <cocos2d.h>
#include <c2xa/config.hpp>

namespace c2xa
{
    namespace debug
    {
        class sample_viewer
            : public cocos2d::Node
        {
        public:
            CREATE_FUNC( sample_viewer );
            virtual bool init() override
            {
                using namespace cocos2d;
                if( !Node::init() )
                {
                    return false;
                }

                auto draw_ = DrawNode::create();
                draw_->setName( "draw_node" );
                addChild( draw_, 2 );

                auto bg_ = LayerColor::create( Color4B::WHITE, app_width, app_height );
                addChild( bg_, 1 );

                return true;
            }
            template< unsigned int sample_size >
            void update( sample<sample_size> const& sample_ )
            {
                using namespace cocos2d;
                auto draw_ = static_cast<DrawNode*>( getChildByName( "draw_node" ) );

                static const float unit_height = 300.f;
                static const float unit = 30.f;

                draw_->clear();

                // 目盛原点
                Vec2 trans_{ 0, app_height / 2 };

                // 目盛0線
                draw_->drawSegment(
                    trans_,
                    trans_ + Vec2{ app_width, 0 },
                    1.f,
                    Color4F{ 0, 0, 0, 0.1f } );

                // 目盛+unit線
                draw_->drawSegment(
                    trans_ + Vec2{ 0, unit_height },
                    trans_ + Vec2{ app_width, unit_height },
                    0.5f,
                    Color4F{ 0, 0, 0, 0.1f } );

                // 目盛-unit線
                draw_->drawSegment(
                    trans_ + Vec2{ 0, -unit_height },
                    trans_ + Vec2{ app_width, -unit_height },
                    0.5f,
                    Color4F{ 0, 0, 0, 0.1f } );

                if( sample_.size() > 1 )
                {
                    auto prev_ = sample_.cbegin();
                    auto now_ = std::next( prev_ );

                    float prevv_ = std::get<0>( *prev_ ).x;
                    float nowv_ = 0.5 * prevv_ + 0.5 * std::get<0>( *now_ ).x;

                    for( int i = 0; now_ != sample_.cend(); ++i )
                    {
                        // a.x
                        draw_->drawLine(
                            trans_ + Vec2{ i * app_width / sample_size, std::get<0>( *prev_ ).x * unit_height / unit },
                            trans_ + Vec2{ ( i + 1 ) * app_width / sample_size, std::get<0>( *now_ ).x * unit_height / unit },
                            Color4F{ 1, 0, 0, 0.5f } );


                        // LowPass a.x
                        nowv_ = 0.5 * prevv_ + 0.5 * std::get<0>( *now_ ).x;
                        draw_->drawLine(
                            trans_ + Vec2{ i * app_width / sample_size, prevv_ * unit_height / unit },
                            trans_ + Vec2{ ( i + 1 ) * app_width / sample_size, nowv_ * unit_height / unit },
                            Color4F{ 0, 0, 1, 0.5f } );
                        prevv_ = nowv_;

                        prev_ = now_;
                        now_ = std::next( prev_ );
                    }
                }
            }
        };
    }
}

#endif//C2XA_DEBUG_SAMPLE_VIEWER_HPP