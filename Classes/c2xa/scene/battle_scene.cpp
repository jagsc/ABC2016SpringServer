
/**

    @file   battle_scene.cpp
    @brief  battle_scene

    @author 新ゝ月かりな(NewNotMoon)
    @date   created on 2016/02/09

*/

#include <c2xa/scene/battle_scene.hpp>
#include <c2xa/config.hpp>
#include <c2xa/exception.hpp>
#include <c2xa/communication/node.hpp>
#include <c2xa/communication/parse.hpp>

using namespace c2xa;
using namespace c2xa::scene;

namespace
{

    template< typename FUNC >
    void apply_fft( fft& fft_, battle_scene::fft_buffer_type& buffer_, battle_scene::sample const& sample_, FUNC func )
    {
        if( sample_.size() == battle_scene::sample::sampling_number )
        {
            auto it_ = sample_.cbegin();

            for( int i = 0; i < battle_scene::sample::sampling_number; ++i )
            {
                CCASSERT( it_ != sample_.cend(), "" );
                buffer_[ i * 2 ] = static_cast<double>( func( std::get<1>( *it_ ) ) )
                    * ( 0.54 - 0.46 * std::cos( 2 * M_PI * i / ( battle_scene::sample::sampling_number * 2 ) ) ); // hamming window
                buffer_[ i * 2 + 1 ] = 0; // imaginary part
                ++it_;
            }

            fft_.cdft( 1, buffer_.data() ); // FFT 結果はdata_に代入される
        }
    }
    void update_player( fft& fft_, battle_scene::fft_buffer_type& tmp_, battle_scene::sample& sample_, communication_node::buffer_type& buffer_ )
    {
        using namespace cocos2d;
        for( auto&& i : communication::parse( buffer_ ) )
        {
            sample_.push(
                static_cast<std::chrono::nanoseconds>( std::get<0>( i ) ),
                std::get<1>( i ),
                []( unsigned left_, unsigned right_, data before_data_, data data_ )
            {
                return ( data_ * left_ + before_data_ * right_ ) / ( left_ + right_ );
            } );
        }
    }

}

battle_scene::battle_scene()
try
    : fft_{ sample::sampling_number }
{
}
catch( c2xa::bluetooth_exception& e )
{
    throw e;
}

bool battle_scene::init( communication_node* com_node_ )
{
    using namespace cocos2d;
    if( !Scene::init() )
    {
        return false;
    }
    setName( "battle_scene" );
    scheduleUpdate();

    com_node_->retain();
    com_node_->removeFromParent();
    addChild( com_node_ );
    com_node_->release();

    auto draw_ = DrawNode::create();
    draw_->setName( "draw_node1" );
    addChild( draw_, 5 );
    draw_ = DrawNode::create();
    draw_->setName( "draw_node2" );
    addChild( draw_, 5 );
    auto bg_ = LayerColor::create( Color4B::WHITE, app_width, app_height );
    addChild( bg_, 1 );

    return true;
}

void battle_scene::update( float )
{
    using namespace cocos2d;

    auto com_node_ = static_cast<communication_node*>( getChildByName( "com_node" ) );
    com_node_->receive_1p( [ = ]( auto&& com_, auto&& buffer_ )
    {
        auto draw_ = static_cast<cocos2d::DrawNode*>( getChildByName( "draw_node1" ) );
        draw_->clear();

        update_player( fft_, tmp_, sample_1p, buffer_ );

        apply_fft( fft_, tmp_, sample_1p, []( data const& d )
        {
            return d.acceleration.x;
        } );
        for( int i = 1; i < sample::sampling_number / 2; ++i )
        {
            draw_->drawSegment(
                cocos2d::Vec2{ ( i - 0.5f ) * app_width / ( sample::sampling_number + 1 ), 0 },
                cocos2d::Vec2{
                    ( i - 0.5f ) * app_width / ( sample::sampling_number + 1 ),
                    ( static_cast<float>( std::pow( tmp_[ i * 2 ], 2 ) + std::pow( tmp_[ i * 2 + 1 ], 2 ) ) / sample::sampling_number * 10 )
                },
                app_width / sample::sampling_number / 2,
                cocos2d::Color4F{ 1, 0, 0, 0.3f } );
        }
        apply_fft( fft_, tmp_, sample_1p, []( data const& d )
        {
            return d.acceleration.y;
        } );
        for( int i = 1; i < sample::sampling_number / 2; ++i )
        {
            draw_->drawSegment(
                cocos2d::Vec2{ ( i - 0.5f ) * app_width / ( sample::sampling_number + 1 ), 0 },
                cocos2d::Vec2{
                    ( i - 0.5f ) * app_width / ( sample::sampling_number + 1 ),
                    ( static_cast<float>( std::pow( tmp_[ i * 2 ], 2 ) + std::pow( tmp_[ i * 2 + 1 ], 2 ) ) / sample::sampling_number * 10 )
                },
                app_width / sample::sampling_number / 2,
                cocos2d::Color4F{ 0, 1, 0, 0.3f } );
        }
        apply_fft( fft_, tmp_, sample_1p, []( data const& d )
        {
            return d.acceleration.z;
        } );
        for( int i = 1; i < sample::sampling_number / 2; ++i )
        {
            draw_->drawSegment(
                cocos2d::Vec2{ ( i - 0.5f ) * app_width / ( sample::sampling_number + 1 ), 0 },
                cocos2d::Vec2{
                    ( i - 0.5f ) * app_width / ( sample::sampling_number + 1 ),
                    ( static_cast<float>( std::pow( tmp_[ i * 2 ], 2 ) + std::pow( tmp_[ i * 2 + 1 ], 2 ) ) / sample::sampling_number * 10 )
                },
                app_width / sample::sampling_number / 2,
                cocos2d::Color4F{ 0, 0, 1, 0.3f } );
        }
        com_->send_1p();
    } );
    com_node_->receive_2p( [ = ]( auto&& com_, auto&& buffer_ )
    {
        update_player( fft_, tmp_, sample_2p, buffer_ );
        auto draw_ = static_cast<cocos2d::DrawNode*>( getChildByName( "draw_node2" ) );
        if( draw_ )
        {
            draw_->clear();
            // 直流スキップ
            for( int i = 1; i < sample::sampling_number / 2; ++i )
            {
                draw_->drawSegment(
                    cocos2d::Vec2{ ( i - 0.5f ) * app_width / ( sample::sampling_number + 1 ), 0 },
                    cocos2d::Vec2{
                    ( i - 0.5f ) * app_width / ( sample::sampling_number + 1 ),
                    ( static_cast<float>( std::pow( tmp_[ i * 2 ], 2 ) + std::pow( tmp_[ i * 2 + 1 ], 2 ) ) / sample::sampling_number )
                },
                    app_width / sample::sampling_number / 2,
                    cocos2d::Color4F{ 0, 1, 0, 0.5f } );
            }
        }
        com_->send_2p();
    } );
}