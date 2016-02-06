/************************************************************************************//**
    @file   c2xa/scene/main_scene.cpp
    @author 新ゝ月かりな
    @date   created on 2015/09/24
****************************************************************************************/

#include <c2xa/exception.hpp>
#include <c2xa/scene/main_scene.hpp>
#include <c2xa/communication/parse.hpp>

#include <c2xa/debug/sample_viewer.hpp>

using namespace c2xa::scene;


main_scene::main_scene()
try
    : fft_{ decltype( sample_ )::sample_size }
{
}
catch( c2xa::bluetooth_exception& e )
{
    cocos2d::log( e.error_.message.c_str() );
    throw e;
}

main_scene::~main_scene()
{
}

bool main_scene::init()
{
    using namespace cocos2d;
    if( !Scene::init() )
    {
        return false;
    }
    setName( "main_scene" );
    scheduleUpdate();


    auto viewer_ = debug::sample_viewer::create();
    viewer_->setName( "sample_viewer" );
    addChild( viewer_, 3 );

    auto viewer2_ = debug::sample_viewer2::create();
    viewer2_->setName( "sample_viewer2" );
    addChild( viewer2_, 4 );

    auto draw_ = DrawNode::create();
    draw_->setName( "draw_node" );
    addChild( draw_, 5 );

    auto bg_ = LayerColor::create( Color4B::WHITE, app_width, app_height );
    addChild( bg_, 1 );

    return true;
}

void main_scene::update( float )
{
    if( !connection_server_ )
    {
        auto is_accept_ = listen_server_.accept();
        if( is_accept_ )
        {
            connection_server_.reset( new bluetooth::connection_server( *is_accept_ ) );
        }
    }
    else
    {
        std::memset( buffer_, '\0', sizeof( buffer_ ) );
        auto state_ = connection_server_->receive( buffer_, sizeof( buffer_ ), 0 );
        if( state_ == bluetooth::connection_server::socket_state::success )
        {
            //cocos2d::log( "[BT log] %s", buffer_ );
            for( auto&& i : communication::parse( buffer_ ) )
            {
                auto tmp = std::get<1>( i );
                sample_.push( tmp );
                sampler_.push(
                    static_cast<std::chrono::nanoseconds>( std::get<0>( i ) ),
                    std::get<1>( i ),
                    []( unsigned left_, unsigned right_, data before_data_, data data_ )
                {
                    return ( data_ * left_ + before_data_ * ( right_ - left_ ) ) / right_;
                } );
            }

            auto viewer_ = static_cast<debug::sample_viewer*>( getChildByName( "sample_viewer" ) );
            if( viewer_ )
            {
                viewer_->update( sample_ );
            }
            auto viewer2_ = static_cast<debug::sample_viewer2*>( getChildByName( "sample_viewer2" ) );
            if( viewer2_ )
            {
                viewer2_->update( sampler_ );
            }

            // FFT
            //TODO: サンプル数不足の場合幅調整する
            if( sample_.size() == sample_size )
            {
                double data_[ sample_size * 2 ];

                auto it_ = sample_.cbegin();

                for( int i = 0; i < sample_size; ++i )
                {
                    CCASSERT( it_ != sample_.cend(), "" );
                    data_[ i * 2 ] = static_cast<double>( it_->acceleration.x )
                        * ( 0.54 - 0.46 * std::cos( 2 * M_PI * i / ( sample_size * 2 ) ) ); // hamming window
                    data_[ i * 2 + 1 ] = 0; // imaginary part
                    ++it_;
                }

                fft_.cdft( 1, data_ ); // FFT 結果はdata_に代入される

                auto draw_ = static_cast<cocos2d::DrawNode*>( getChildByName( "draw_node" ) );
                if( draw_ )
                {
                    draw_->clear();
                    // 直流スキップ
                    for( int i = 1; i < sample_size / 2; ++i )
                    {
                        draw_->drawSegment(
                            cocos2d::Vec2{ ( i - 0.5f ) * app_width / ( sample_size + 1 ), 0 },
                            cocos2d::Vec2{
                                ( i - 0.5f ) * app_width / ( sample_size + 1 ),
                                ( static_cast<float>( std::pow( data_[ i * 2 ], 2 ) + std::pow( data_[ i * 2 + 1 ], 2 ) ) / sample_size )
                            },
                            app_width / sample_size / 2,
                            cocos2d::Color4F{ 0, 0, 1, 0.5f } );
                    }
                }
            }
        }
        connection_server_->send( "", sizeof( "" ), 0 );
    }
}