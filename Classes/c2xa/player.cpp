
/**

    @file   player.cpp
    @brief  player

    @author 新ゝ月かりな(NewNotMoon)
    @date   created on 2016/02/23

    詳細説明

*/

#include <c2xa/player.hpp>

#include <c2xa/config.hpp>
#include <c2xa/communication/parse.hpp>

#include <AudioEngine.h>

using namespace c2xa;

#ifdef COCOS2D_DEBUG
void player::debug( cocos2d::Layer* that )
{
    using namespace cocos2d;


    auto draw_rect = [ = ]( char const* name_, spectrums_type::spectrum const& spectrum_, Color3B color_, Vec2 pos_, float norm_ = 1.f, float width = app_width / 6 )
    {
        auto sprites_ = static_cast<Node*>( that->getChildByName( name_ ) );
        if( sprites_ == nullptr )
        {
            sprites_ = Node::create();
            sprites_->setName( name_ );
            that->addChild( sprites_, 20 );
        }
        for( int i = 0; i < spectrums_type::number; ++i )
        {
            auto sprite_ = static_cast<Sprite*>( sprites_->getChildByTag( i ) );
            if( sprite_ == nullptr )
            {
                sprite_ = Sprite::create();
                sprite_->setTag( i );
                sprite_->setColor( color_ );
                sprite_->setOpacity( 255 / 3 );
                sprite_->setAnchorPoint( Vec2::ANCHOR_BOTTOM_LEFT );
                sprite_->setPosition( Vec2{ ( number_ == number::_1p ? 0 : app_width / 2 ) + width * i / 2 / spectrums_type::number, 0 } + pos_ );
                sprites_->addChild( sprite_, 20 );
            }
            sprite_->setTextureRect( { 0, 0, width / 2 / spectrums_type::number, static_cast< float >( spectrum_[i] ) * norm_ } );
        }
    };

    draw_rect( "spc_acx", spectrums_.acceleration_x, { 255, 0, 0 }, { 0, app_height / 2 } );
    draw_rect( "spc_acy", spectrums_.acceleration_y, { 0, 255, 0 }, { 0, app_height / 2 } );
    draw_rect( "spc_acz", spectrums_.acceleration_z, { 0, 0, 255 }, { 0, app_height / 2 } );
    draw_rect( "spc_pax", spectrums_.phase_.acceleration_x, { 255, 0, 0 }, { app_width / 6, app_height / 2 }, 200.f );
    draw_rect( "spc_pay", spectrums_.phase_.acceleration_y, { 0, 255, 0 }, { app_width / 6, app_height / 2 }, 200.f );
    draw_rect( "spc_paz", spectrums_.phase_.acceleration_z, { 0, 0, 255 }, { app_width / 6, app_height / 2 }, 200.f );
    draw_rect( "spc_gyx", spectrums_.gyro_x, { 255, 0, 0 }, { app_width * 2 / 6, app_height / 2 } );
    draw_rect( "spc_gyy", spectrums_.gyro_y, { 0, 255, 0 }, { app_width * 2 / 6, app_height / 2 } );
    draw_rect( "spc_gyz", spectrums_.gyro_z, { 0, 0, 255 }, { app_width * 2 / 6, app_height / 2 } );

    auto get_text = [ & ]( char const* name_, float y )
    {
        auto t_ = static_cast<cocos2d::Label*>( that->getChildByName( name_ ) );
        if( t_ == nullptr )
        {
            auto origin_ = Vec2{ number_ == number::_1p ? 100 : app_width - 100, 0 };
            t_ = Label::createWithTTF( "", "font/Stroke.ttf", 40 );
            t_->setName( name_ );
            t_->setColor( Color3B::WHITE );
            t_->setAnchorPoint( number_ == number::_1p ? Vec2::ANCHOR_TOP_LEFT : Vec2::ANCHOR_TOP_RIGHT );
            t_->setPosition( origin_ + Vec2{ 0, app_height - y } );
            that->addChild( t_, 20 );
        }
        return t_;
    };

    auto text_act = get_text( "text_act", 30 );
    switch( action_ )
    {
        case action::slash:
        {
            text_act->setString( "action: slash" ); break;
        }
        case action::thrust:
        {
            text_act->setString( "action: thrust" ); break;
        }
        case action::guard:
        {
            text_act->setString( "action: guard" ); break;
        }
        case action::messy:
        {
            text_act->setString( "action: messy" ); break;
        }
        case action::idle:
        {
            text_act->setString( "action: idle" ); break;
        }
    }
    get_text( "text_hp", 70 )->setString( "hp: " + std::to_string( hp_ ) );
    get_text( "text_st", 110 )->setString( state_ == state::invincible ? "state: invincible" : "state: defenceless" );
    get_text( "text_acx", 150 )->setString( std::to_string( (int)spectrums_.acceleration_x[ 1 ] ) );
    get_text( "text_acy", 190 )->setString( std::to_string( (int)spectrums_.acceleration_y[ 1 ] ) );
    get_text( "text_acz", 230 )->setString( std::to_string( (int)spectrums_.acceleration_z[ 1 ] ) );
    get_text( "text_gyx", 270 )->setString( std::to_string( (int)spectrums_.gyro_x[ 1 ] ) );
    get_text( "text_gyy", 310 )->setString( std::to_string( (int)spectrums_.gyro_y[ 1 ] ) );
    get_text( "text_gyz", 350 )->setString( std::to_string( (int)spectrums_.gyro_z[ 1 ] ) );
    get_text( "text_pax", 390 )->setString( std::to_string( (int)spectrums_.phase_.acceleration_x[ 1 ] ) );
    get_text( "text_pay", 430 )->setString( std::to_string( (int)spectrums_.phase_.acceleration_y[ 1 ] ) );
    get_text( "text_paz", 470 )->setString( std::to_string( (int)spectrums_.phase_.acceleration_z[ 1 ] ) );
}
#endif//COCOS2D_DEBUG

namespace
{
    static const cocos2d::Vec2 pos_1p{ app_width / 3, app_height * 2 / 5 };
    static const cocos2d::Vec2 pos_2p{ app_width * 2 / 3, app_height * 2 / 5 };

    template< size_t sampling_number_ >
    using buffer_for_fft = std::array<double, sampling_number_ * 2>;

    template< typename SAMPLE, typename BUFFER, typename FUNC >
    bool apply_fft(
        SAMPLE&& sample_,
        BUFFER&& buffer_,
        FUNC&& func )
    {
        using sample_type = typename std::remove_reference<SAMPLE>::type;
        auto fft_ = get_fft<sample_type::sampling_number>();

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

            fft_->cdft( 1, buffer_.data() ); // FFT 結果はdata_に代入される
            return true;
        }
        return false;
    }

    template< typename SAMPLE, typename SPECTRUMS >
    void update_spectrums(
        SAMPLE&& sample_,
        SPECTRUMS&& spectrums_ )
    {
        using sample_type = typename std::remove_reference<SAMPLE>::type;
        using spectrums_type = typename std::remove_reference<SPECTRUMS>::type;

        buffer_for_fft< sample_type::sampling_number > buffer_;

        // result assign
        {
            auto is_ = apply_fft( sample_, buffer_, []( data const& d )
            {
                return d.acceleration.x;
            } );
            if( is_ )
            {
                for( int i = 0; i < spectrums_type::number; ++i )
                {
                    spectrums_.acceleration_x[ i ] = std::pow( buffer_[ i * 2 ], 2 ) + std::pow( buffer_[ i * 2 + 1 ], 2 ) / sample_type::sampling_number;
                    spectrums_.phase_.acceleration_x[ i ] = std::atan( buffer_[ i * 2 + 1 ] / buffer_[ i * 2 ] );
                }
            }
        }
        {
            auto is_ = apply_fft( sample_, buffer_, []( data const& d )
            {
                return d.acceleration.y;
            } );
            if( is_ )
            {
                for( int i = 0; i < spectrums_type::number; ++i )
                {
                    spectrums_.acceleration_y[ i ] = std::pow( buffer_[ i * 2 ], 2 ) + std::pow( buffer_[ i * 2 + 1 ], 2 ) / sample_type::sampling_number;
                    spectrums_.phase_.acceleration_y[ i ] = std::atan( buffer_[ i * 2 + 1 ] / buffer_[ i * 2 ] );
                }
            }
        }
        {
            auto is_ = apply_fft( sample_, buffer_, []( data const& d )
            {
                return d.acceleration.z;
            } );
            if( is_ )
            {
                for( int i = 0; i < spectrums_type::number; ++i )
                {
                    spectrums_.acceleration_z[ i ] = std::pow( buffer_[ i * 2 ], 2 ) + std::pow( buffer_[ i * 2 + 1 ], 2 ) / sample_type::sampling_number;
                    spectrums_.phase_.acceleration_z[ i ] = std::atan( buffer_[ i * 2 + 1 ] / buffer_[ i * 2 ] );
                }
            }
        }
        {
            auto is_ = apply_fft( sample_, buffer_, []( data const& d )
            {
                return d.gyro.x;
            } );
            if( is_ )
            {
                for( int i = 0; i < spectrums_type::number; ++i )
                {
                    spectrums_.gyro_x[ i ] = std::pow( buffer_[ i * 2 ], 2 ) + std::pow( buffer_[ i * 2 + 1 ], 2 ) / sample_type::sampling_number;
                }
            }
        }
        {
            auto is_ = apply_fft( sample_, buffer_, []( data const& d )
            {
                return d.gyro.y;
            } );
            if( is_ )
            {
                for( int i = 0; i < spectrums_type::number; ++i )
                {
                    spectrums_.gyro_y[ i ] = std::pow( buffer_[ i * 2 ], 2 ) + std::pow( buffer_[ i * 2 + 1 ], 2 ) / sample_type::sampling_number;
                }
            }
        }
        {
            auto is_ = apply_fft( sample_, buffer_, []( data const& d )
            {
                return d.gyro.z;
            } );
            if( is_ )
            {
                for( int i = 0; i < spectrums_type::number; ++i )
                {
                    spectrums_.gyro_z[ i ] = std::pow( buffer_[ i * 2 ], 2 ) + std::pow( buffer_[ i * 2 + 1 ], 2 ) / sample_type::sampling_number;
                }
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
        if( spectrums_.gyro_x[ 1 ] > 400 )
        {
            action_ = action::guard;
        }
        else if( spectrums_.acceleration_y[ 1 ] > 1500 && spectrums_.acceleration_x[ 1 ] < spectrums_.acceleration_y[ 1 ] / 6 && spectrums_.acceleration_z[ 1 ] < spectrums_.acceleration_y[ 1 ] / 3 )
        {
            action_ = action::slash;
        }
        else if( spectrums_.acceleration_x[ 1 ] > 1000 && spectrums_.acceleration_z[ 1 ] < spectrums_.acceleration_x[ 1 ] / 3 )
        {
            action_ = action::thrust;
        }
        else if( spectrums_.acceleration_x[ 1 ] < 100
            && spectrums_.acceleration_y[ 1 ] < 100
            && spectrums_.acceleration_z[ 1 ] < 100 )
        {
            action_ = action::idle;
        }
        else
        {
            action_ = action::messy;
        }
    }
}

player::player()
{
}

player::~player()
{
}

bool player::init( number n )
{
    using namespace cocos2d;
    if( !Node::init() )
    {
        return false;
    }
    is_doing_ = false;
    is_updated_ = false;
    is_attacking_ = false;
    number_ = n;
    state_ = state::defenseless;
    hp_ = player_max_hp;
    action_ = action::idle;
    spectrums_ = {};
    scheduleUpdate();

    auto droid_ = Sprite::create( "img/stubdroid.png" );
    switch( number_ )
    {
        case number::_1p: droid_->setPosition( pos_1p ); droid_->setFlippedX( true ); break;
        case number::_2p: droid_->setPosition( pos_2p ); break;
    }
    droid_->setName( "droid" );
    addChild( droid_, 10 );

    return true;
}
#ifdef COCOS2D_DEBUG
#include <c2xa/debug/sample_viewer.hpp>
#endif

void player::update( float )
{
#ifdef COCOS2D_DEBUG
    auto viewer_ = static_cast<debug::sample_viewer*>( getChildByName( "sample_viewer" ) );
    if( viewer_ )
    {
        viewer_->clear();
        viewer_->update( sample_ );
    }
    else
    {
        viewer_ = debug::sample_viewer::create();
        viewer_->setName( "sample_viewer" );
        addChild( viewer_ );
    }
#endif
}

void player::judge()
{
    using namespace cocos2d;
    if( state_ == state::defenseless )
    {
        if( !is_doing_ )
        {
            if( is_updated_ )
            {
                update_spectrums( sample_, spectrums_ );
                update_action( spectrums_, action_ );
                is_updated_ = false;
                if( action_ != action::idle && action_ != action::messy )
                {
                    auto droid_ = static_cast<cocos2d::Sprite*>( getChildByName( "droid" ) );
                    is_doing_ = true;
                    switch( action_ )
                    {
                        case action::thrust:
                        {
                            droid_->runAction(
                                Spawn::createWithTwoActions(
                                    Sequence::create(
                                        MoveBy::create( 0.3f, Vec2{ number_ == number::_1p ? -100.f : 100.f, 0 } ),
                                        MoveBy::create( 0.1f, Vec2{ number_ == number::_1p ? 600.f : -600.f, 0 } ),
                                        MoveBy::create( 0.7f, Vec2{ number_ == number::_1p ? -500.f : 500.f, 0 } ),
                                        CallFunc::create( [ & ]{ is_doing_ = false; } ),
                                        nullptr ),
                                    Sequence::create(
                                        DelayTime::create( 0.35f ),
                                        CallFunc::create( [ & ]{ cocos2d::experimental::AudioEngine::play2d( "snd/taa.mp3", false, 0.3f, nullptr ); is_attacking_ = true; } ),
                                        nullptr ) ) );
                            break;
                        }
                        case action::slash:
                        {
                            ccBezierConfig bc_;
                            bc_.controlPoint_1 ={ 0, 0 };
                            bc_.controlPoint_2 ={ number_ == number::_1p ? 100.f : -100.f, 200 };
                            bc_.endPosition ={ number_ == number::_1p ? 200.f : -200.f, 0 };
                            droid_->runAction(
                                Spawn::createWithTwoActions(
                                    Sequence::create(
                                        BezierBy::create( 0.5f, bc_ ),
                                        CallFunc::create( [ & ]{
                                            auto eff  = ParticleSystemQuad::create( "particle/slash.plist" );
                                            eff->setPosition( pos_1p + Vec2{ number_ == number::_1p ? 300.f : -300.f, 0.f } );
                                            eff->resetSystem();
                                            eff->setScale( 2.3f );
                                            if( number_ == number::_1p )
                                            {
                                                eff->setStartSpin( 194 );
                                                eff->setEndSpin( 235 );
                                            }
                                            getParent()->addChild( eff, 30 );
                                        } ),
                                        MoveTo::create( 0.4f, pos_1p ),
                                        CallFunc::create( [ & ]{ is_doing_ = false; } ),
                                        nullptr ),
                                    Sequence::create(
                                        DelayTime::create( 0.45f ),
                                        CallFunc::create( [ & ]{ cocos2d::experimental::AudioEngine::play2d( "snd/sokoda.mp3", false, 0.3f, nullptr ); is_attacking_ = true; } ),
                                        nullptr ) ) );
                            break;
                        }
                        case action::guard:
                        {
                            auto adf = ParticleSystemQuad::create( "particle/adfield.plist" );
                            adf->setPosition( droid_->getPosition() + Vec2{ number_ == number::_1p ? 200.f : -200.f, 0.f } );
                            adf->resetSystem();
                            addChild( adf, 40 );
                            droid_->runAction( Sequence::create(
                                JumpBy::create( 1.f, { 0, 0 }, 50, 10 ),
                                CallFunc::create( [ this ]{ is_doing_ = false; } ),
                                DelayTime::create( 0.5f ),
                                CallFunc::create( [ adf ]{ adf->removeFromParent(); } ),
                                nullptr ) );
                            break;
                        }
                    }
                }
            }
        }
    }
}

void player::update_state( communication_node::buffer_type& data_ )
{
    update_sample( sample_, data_ );
    is_updated_ = true;
}

void player::damage( int dm_ )
{
    using namespace cocos2d;

    is_doing_ = false;
    is_attacking_ = false;

    auto droid_ = static_cast<cocos2d::Sprite*>( getChildByName( "droid" ) );
    droid_->stopAllActions();

    state_ = state::invincible;

    auto p = droid_->getPosition();
    auto n  = "";
    switch( ( random() % 4 ) )
    {
        case 1:
            n = "img/ga.png";
            break;
        case 2:
            n = "img/go.png";
            break;
        case 3:
            n = "img/boki.png";
            break;
        default:
            n = "img/ga.png";
            break;
    }
    auto e = Sprite::create( n );
    e->setPosition( p + Vec2{ (float)(random() % 400) - 200, (float)( random() % 200 ) + 100.f } );
    e->setOpacity( 0 );
    e->runAction(
        Sequence::create(
            Spawn::create(
                MoveBy::create( 0.1f, Vec2{ 0, 50.f } ),
                FadeIn::create( 0.1f ),
                nullptr ),
            DelayTime::create( 0.1f ),
            Spawn::create(
                MoveBy::create( 0.4f, Vec2{ 0, 50.f } ),
                FadeOut::create( 0.4f ),
                nullptr ),
            RemoveSelf::create( true ),
            nullptr ) );
    getParent()->addChild( e, 40 );
    cocos2d::experimental::AudioEngine::play2d( "snd/boki.mp3", false, 0.3f, nullptr );
    
    droid_->retain();
    droid_->runAction(
        Sequence::create(
            Spawn::create(
                Sequence::create(
                    MoveTo::create( 0.4f, ( number_ == number::_1p ? pos_1p - Vec2{ 100, 0 } : pos_2p + Vec2{ 100, 0 } ) ),
                    MoveTo::create( 0.8f, ( number_ == number::_1p ? pos_1p : pos_2p ) ),
                    nullptr ),
                RotateTo::create( 0.1f, 0 ),
                Blink::create( 1.2f, 6 ),
                nullptr ),
            CallFunc::create( [ this, droid_ ]{ state_ = state::defenseless; droid_->setVisible( true ); droid_->release(); } ),
        nullptr ) );
    hp_ -= dm_;
}