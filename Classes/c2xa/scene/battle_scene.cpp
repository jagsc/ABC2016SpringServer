
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
    template< size_t sampling_number_ >
    using buffer_for_fft = std::array<double, sampling_number_ * 2>;

    template< typename SAMPLE, typename BUFFER, typename FUNC >
    void apply_fft(
        SAMPLE&& sample_,
        BUFFER&& buffer_,
        fft& fft_,
        FUNC func )
    {
        using sample_type = typename std::remove_reference<SAMPLE>::type;

        if( sample_.size() == sample_type::sampling_number )
        {
            auto it_ = sample_.cbegin();

            for( int i = 0; i < sample_type::sampling_number; ++i )
            {
                CCASSERT( it_ != sample_.cend(), "" );
                buffer_[ i * 2 ] = static_cast<double>( func( std::get<1>( *it_ ) ) )
                    * ( 0.54 - 0.46 * std::cos( 2 * M_PI * i / ( sample_type::sampling_number * 2 ) ) ); // hamming window
                buffer_[ i * 2 + 1 ] = 0; // imaginary part
                ++it_;
            }

            fft_.cdft( 1, buffer_.data() ); // FFT 結果はdata_に代入される
        }
    }
    template< typename SAMPLE, typename SPECTRUMS >
    void update_spectrums(
        SAMPLE&& sample_,
        SPECTRUMS&& spectrums_,
        fft& fft_ )
    {
        using sample_type = typename std::remove_reference<SAMPLE>::type;
        using spectrums_type = typename std::remove_reference<SPECTRUMS>::type;

        buffer_for_fft< sample_type::sampling_number > buffer_;

        // result assign
        {
            apply_fft( sample_, buffer_, fft_, []( data const& d )
            {
                return d.acceleration.x;
            } );
            for( int i = 0; i < spectrums_type::number; ++i )
            {
                spectrums_.acceleration_x[ i ] = std::pow( buffer_[ i * 2 ], 2 ) + std::pow( buffer_[ i * 2 + 1 ], 2 ) / sample_type::sampling_number;
            }
        }
        {
            apply_fft( sample_, buffer_, fft_, []( data const& d )
            {
                return d.acceleration.y;
            } );
            for( int i = 0; i < spectrums_type::number; ++i )
            {
                spectrums_.acceleration_y[ i ] = std::pow( buffer_[ i * 2 ], 2 ) + std::pow( buffer_[ i * 2 + 1 ], 2 ) / sample_type::sampling_number;
            }
        }
        {
            apply_fft( sample_, buffer_, fft_, []( data const& d )
            {
                return d.acceleration.z;
            } );
            for( int i = 0; i < spectrums_type::number; ++i )
            {
                spectrums_.acceleration_z[ i ] = std::pow( buffer_[ i * 2 ], 2 ) + std::pow( buffer_[ i * 2 + 1 ], 2 ) / sample_type::sampling_number;
            }
        }
    }
    template< typename SAMPLE >
    void update_sample(
        SAMPLE&& sample_,
        communication_node::buffer_type& data_ )
    {
        using namespace cocos2d;

        // parse and push
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
    }
    template< typename SPECTRUMS >
    void update_action( SPECTRUMS&& spectrums_, action& action_ )
    {
        if( spectrums_.acceleration_y[ 1 ] > 1500 )
        {
            action_ = action::slash;
        }
        else if( spectrums_.acceleration_x[ 1 ] > 1000 )
        {
            action_ = action::thrust;
        }
        else if( spectrums_.acceleration_z[ 1 ] > 1000 )
        {
            action_ = action::guard;
        }
        else if( spectrums_.acceleration_x[ 1 ] < 100
            && spectrums_.acceleration_y[ 1 ] < 100
            && spectrums_.acceleration_z[ 1 ] < 100 )
        {
            action_ = action::idol;
        }
    }
    
    void debug_view( battle_scene* that, battle_scene::spectrums_type& spectrums_, action& action_, cocos2d::Vec2 origin_ )
    {
        using namespace cocos2d;
        auto draw_ = static_cast<cocos2d::DrawNode*>( that->getChildByName( "draw_node1" ) );
        if( draw_ == nullptr )
        {
            draw_ = DrawNode::create();
            draw_->setName( "draw_node1" );
            that->addChild( draw_, 5 );
        }
        draw_->clear();
        draw_->drawSegment(
            origin_ + cocos2d::Vec2{ 50, 0 },
            origin_ + cocos2d::Vec2{
                50,
                ( static_cast<float>( spectrums_.acceleration_x[ 1 ] ) )
            },
            30,
            cocos2d::Color4F{ 1, 0, 0, 0.5f } );
        draw_->drawSegment(
            origin_ + cocos2d::Vec2{ 110, 0 },
            origin_ + cocos2d::Vec2{
                110,
                ( static_cast<float>( spectrums_.acceleration_y[ 1 ] ) )
            },
            30,
            cocos2d::Color4F{ 0, 1, 0, 0.5f } );
        draw_->drawSegment(
            origin_ + cocos2d::Vec2{ 170, 0 },
            origin_ + cocos2d::Vec2{
                170,
                ( static_cast<float>( spectrums_.acceleration_z[ 1 ] ) )
            },
            30,
            cocos2d::Color4F{ 0, 0, 1, 0.5f } );

        auto text_ = static_cast<cocos2d::Label*>( that->getChildByName( "text" ) );
        if( text_ == nullptr )
        {
            text_ = cocos2d::Label::createWithTTF( "", "font/Stroke.ttf", 40 );
            text_->setName( "text" );
            text_->setColor( Color3B::BLACK );
            text_->setPosition( origin_ + cocos2d::Vec2{ 100, app_height - 60 } );
            that->addChild( text_, 20 );
        }
        auto text_x = static_cast<cocos2d::Label*>( that->getChildByName( "textx" ) );
        if( text_x == nullptr )
        {
            text_x = cocos2d::Label::createWithTTF( "", "font/Stroke.ttf", 40 );
            text_x->setName( "textx" );
            text_x->setColor( Color3B::BLACK );
            text_x->setAnchorPoint( Vec2::ANCHOR_TOP_RIGHT );
            text_x->setPosition( origin_ + cocos2d::Vec2{ 200, app_height - 100 } );
            that->addChild( text_x, 20 );
        }
        auto text_y = static_cast<cocos2d::Label*>( that->getChildByName( "texty" ) );
        if( text_y == nullptr )
        {
            text_y = cocos2d::Label::createWithTTF( "", "font/Stroke.ttf", 40 );
            text_y->setName( "texty" );
            text_y->setColor( Color3B::BLACK );
            text_y->setAnchorPoint( Vec2::ANCHOR_TOP_RIGHT );
            text_y->setPosition( origin_ + cocos2d::Vec2{ 200, app_height - 140 } );
            that->addChild( text_y, 20 );
        }
        auto text_z = static_cast<cocos2d::Label*>( that->getChildByName( "textz" ) );
        if( text_z == nullptr )
        {
            text_z = cocos2d::Label::createWithTTF( "", "font/Stroke.ttf", 40 );
            text_z->setName( "textz" );
            text_z->setColor( Color3B::BLACK );
            text_z->setAnchorPoint( Vec2::ANCHOR_TOP_RIGHT );
            text_z->setPosition( origin_ + cocos2d::Vec2{ 200, app_height - 180 } );
            that->addChild( text_z, 20 );
        }
        text_x->setString( std::to_string( (int)spectrums_.acceleration_x[ 1 ] ) );
        text_y->setString( std::to_string( (int)spectrums_.acceleration_y[ 1 ] ) );
        text_z->setString( std::to_string( (int)spectrums_.acceleration_z[ 1 ] ) );

        switch( action_ )
        {
            case action::slash:
            {
                text_->setString( "slash" ); break;
            }
            case action::thrust:
            {
                text_->setString( "thrust" ); break;
            }
            case action::guard:
            {
                text_->setString( "guard" ); break;
            }
            case action::idol:
            {
                text_->setString( "idol" ); break;
            }
        }
    }
}

battle_scene::battle_scene()
    : fft_{ sample::sampling_number }
{
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

    auto droid_1p = Sprite::create( "img/stubdroid.png" );
    droid_1p->setPosition( Vec2{ app_width / 3, app_height *2 / 5 } );
    addChild( droid_1p, 10 );

    auto droid_2p = Sprite::create( "img/stubdroid.png" );
    droid_2p->setPosition( Vec2{ app_width * 2 / 3, app_height * 2 / 5 } );
    addChild( droid_2p, 10 );

    auto bg_ = Sprite::create( "img/title_bg.png" );
    bg_->setAnchorPoint( Vec2::ANCHOR_BOTTOM_LEFT );
    addChild( bg_, 1 );

    auto bg_img_ = Sprite::create( "img/battle_bg.png" );
    bg_img_->setAnchorPoint( Vec2::ANCHOR_BOTTOM_LEFT );
    addChild( bg_img_, 4 );

    auto bg_front_img_ = Sprite::create( "img/battle_front.png" );
    bg_front_img_->setAnchorPoint( Vec2::ANCHOR_BOTTOM_LEFT );
    addChild( bg_front_img_, 7 );

    auto particle_smoke_ = ParticleSystemQuad::create( "particle/smoke.plist" );
    particle_smoke_->setPosition( Vec2{ app_width / 2, 700 } );
    particle_smoke_->resetSystem();
    addChild( particle_smoke_, 3 );

    {
        auto pf_ = ParticleSystemQuad::create( "particle/fire.plist" );
        pf_->setPosition( Vec2{ 100, 450 } );
        pf_->resetSystem();
        addChild( pf_, 5 );
    }
    {
        auto pf_ = ParticleSystemQuad::create( "particle/fire.plist" );
        pf_->setPosition( Vec2{ app_width - 100, 450 } );
        pf_->resetSystem();
        addChild( pf_, 5 );
    }
    {
        auto pf_ = ParticleSystemQuad::create( "particle/fire.plist" );
        pf_->setPosition( Vec2{ app_width - 400, 250 } );
        pf_->resetSystem();
        addChild( pf_, 5 );
    }
    {
        auto pf_ = ParticleSystemQuad::create( "particle/fire.plist" );
        pf_->setPosition( Vec2{ 500, 350 } );
        pf_->resetSystem();
        addChild( pf_, 5 );
    }
    {
        auto pf_ = ParticleSystemQuad::create( "particle/fire_low.plist" );
        pf_->setPosition( Vec2{ 700, 450 } );
        pf_->resetSystem();
        addChild( pf_, 5 );
    }
    {
        auto pf_ = ParticleSystemQuad::create( "particle/fire_low.plist" );
        pf_->setPosition( Vec2{ 1200, 350 } );
        pf_->resetSystem();
        addChild( pf_, 5 );
    }

    return true;
}

void battle_scene::update( float )
{
    using namespace cocos2d;

    auto com_node_ = static_cast<communication_node*>( getChildByName( "com_node" ) );
    com_node_->receive_1p( [ = ]( auto&& com_, auto&& buffer_ )
    {
        update_sample( sample_1p, buffer_ );
        com_->send_1p();
        update_spectrums( sample_1p, spectrums_1p, fft_ );
        update_action( spectrums_1p, action_1p );
        debug_view( this, spectrums_1p, action_1p, Vec2{ 0, 0 } );
    } );
    com_node_->receive_2p( [ = ]( auto&& com_, auto&& buffer_ )
    {
        update_sample( sample_2p, buffer_ );
        com_->send_2p();
        update_spectrums( sample_2p, spectrums_2p, fft_ );
        update_action( spectrums_2p, action_2p );
        debug_view( this, spectrums_1p, action_1p, Vec2{ 220, 0 } );
    } );
}