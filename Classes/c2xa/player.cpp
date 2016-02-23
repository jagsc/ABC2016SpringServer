
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

using namespace c2xa;

namespace
{
    template< size_t sampling_number_ >
    using buffer_for_fft = std::array<double, sampling_number_ * 2>;

    template< typename SAMPLE, typename BUFFER, typename FUNC >
    void apply_fft(
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
        }
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
            apply_fft( sample_, buffer_, []( data const& d )
            {
                return d.acceleration.x;
            } );
            for( int i = 0; i < spectrums_type::number; ++i )
            {
                spectrums_.acceleration_x[ i ] = std::pow( buffer_[ i * 2 ], 2 ) + std::pow( buffer_[ i * 2 + 1 ], 2 ) / sample_type::sampling_number;
            }
        }
        {
            apply_fft( sample_, buffer_, []( data const& d )
            {
                return d.acceleration.y;
            } );
            for( int i = 0; i < spectrums_type::number; ++i )
            {
                spectrums_.acceleration_y[ i ] = std::pow( buffer_[ i * 2 ], 2 ) + std::pow( buffer_[ i * 2 + 1 ], 2 ) / sample_type::sampling_number;
            }
        }
        {
            apply_fft( sample_, buffer_, []( data const& d )
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
            action_ = action::idle;
        }
    }
}

player::player()
{
}

player::~player()
{
}

bool player::init()
{
    using namespace cocos2d;
    if( !Node::init() )
    {
        return false;
    }
    return true;
}

void player::update_state( communication_node::buffer_type& data_ )
{
    update_sample( sample_, data_ );
    update_spectrums( sample_, spectrums_ );
    update_action( spectrums_, action_ );
}