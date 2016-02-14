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
    : fft_{ sample::sampling_number }
{
}
catch( c2xa::bluetooth_exception& e )
{
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
            connection_server_ = std::move( *is_accept_ );
        }
    }
    else
    {
        auto receive = [ this ]
        {
            std::memset( buffer_, '\0', sizeof( buffer_ ) );
            return connection_server_->receive( buffer_, sizeof( buffer_ ), 0 );
        };
        try
        {
            auto state_ = receive();
            while( state_ == bluetooth::connection::socket_state::success )
            {
                char data_[ sizeof( buffer_ ) * 2 + 1 ];
                unsigned char tmp = 0;
                unsigned char map_[] = { -1, '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-', '.', ',' };
                int i;
                for( i = 0; i < sizeof( buffer_ ); ++i )
                {
                    tmp = static_cast<unsigned char>( buffer_[ i ] ) >> 4;
                    if( tmp == 0b00000000 )
                    {
                        data_[ i * 2 ] = '\0';
                        break;
                    }
                    CCASSERT( tmp <= 13 && tmp >= 1, "" );
                    data_[ i * 2 ] =  map_[ tmp ];
                    tmp = static_cast<unsigned char>( buffer_[ i ] ) & 0b00001111;
                    if( tmp == 0b00000000 )
                    {
                        data_[ i * 2 + 1 ] = '\0';
                        break;
                    }
                    CCASSERT( tmp <= 13 && tmp >= 1, "" );
                    data_[ i * 2 + 1 ] =  map_[ tmp ];
                }

                for( auto&& i : communication::parse( data_ ) )
                {
                    sample_.push(
                        static_cast<std::chrono::nanoseconds>( std::get<0>( i ) ),
                        std::get<1>( i ),
                        []( unsigned left_, unsigned right_, data before_data_, data data_ )
                    {
                        return ( data_ * left_ + before_data_ * right_ ) / ( left_ + right_ );
                    } );
                }

                auto viewer_ = static_cast<debug::sample_viewer*>( getChildByName( "sample_viewer" ) );
                if( viewer_ )
                {
                    viewer_->clear();
                    viewer_->update( sample_ );
                }
                state_ = receive();
            }
        }
        catch( bluetooth_disconnect_exception const& e )
        {
            //TODO: リトライ処理
        }
        // FFT
        //TODO: サンプル数不足の場合幅調整する
        if( sample_.size() == sample::sampling_number )
        {
            double data_[ sample::sampling_number * 2 ];

            auto it_ = sample_.cbegin();

            for( int i = 0; i < sample::sampling_number; ++i )
            {
                CCASSERT( it_ != sample_.cend(), "" );
                data_[ i * 2 ] = static_cast<double>( std::get<1>( *it_ ).acceleration.x )
                    * ( 0.54 - 0.46 * std::cos( 2 * M_PI * i / ( sample::sampling_number * 2 ) ) ); // hamming window
                data_[ i * 2 + 1 ] = 0; // imaginary part
                ++it_;
            }

            fft_.cdft( 1, data_ ); // FFT 結果はdata_に代入される

            auto draw_ = static_cast<cocos2d::DrawNode*>( getChildByName( "draw_node" ) );
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
                        ( static_cast<float>( std::pow( data_[ i * 2 ], 2 ) + std::pow( data_[ i * 2 + 1 ], 2 ) ) / sample::sampling_number )
                    },
                        app_width / sample::sampling_number / 2,
                        cocos2d::Color4F{ 0, 0, 1, 0.5f } );
                }
            }
        }
        connection_server_->send( "", sizeof( "" ), 0 );
    }
}