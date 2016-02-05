///************************************************************************************//**
//    @file   c2xa/scene/experimental/data_analyzer.hpp
//    @author 新ゝ月かりな
//    @date   created on 2015/12/24
//****************************************************************************************/
//#ifndef C2XA_SCENE_EXPERIMENTAL_DATA_ANALYZER_HPP
//#define C2XA_SCENE_EXPERIMENTAL_DATA_ANALYZER_HPP
//
//#include <cocos2d.h>
//#include <list>
//#include <tuple>
//#include <c2xa/c2xa_config.hpp>
//#include <c2xa/experimental/fast_fourier_transform.hpp>
//
//namespace c2xa
//{
//    namespace experimental
//    {
//        struct acceleration
//        {
//            float x;
//            float y;
//            float z;
//        };
//        struct gyro
//        {
//            float x;
//            float y;
//            float z;
//        };
//
//
//        class data_analyzer
//            : public cocos2d::Node
//        {
//        public:
//            static const int sample = 128;
//
//        private:
//            using data = std::tuple<acceleration, gyro>;
//            std::list<data> list_;
//            std::string csv_;
//            std::string::const_iterator begin_;
//            std::string::const_iterator end_;
//            std::string::const_iterator eof_;
//
//        public:
//            CREATE_FUNC( data_analyzer );
//            virtual bool init() override
//            {
//                using namespace cocos2d;
//                if( !Node::init() )
//                {
//                    return false;
//                }
//
//                csv_ = FileUtils::getInstance()->getStringFromFile( "data/sensor.csv" );
//
//                begin_ = csv_.cbegin();
//                end_ = csv_.cbegin();
//                eof_ = csv_.cend();
//
//                //std::string model_ = "model/tortoise.c3b";
//                //auto sprite_ = Sprite3D::create( model_ );
//                //sprite_->setScale( 0.1f );
//                //auto size_ = Director::getInstance()->getWinSize();
//                //sprite_->setPosition( Vec2( size_.width * 4.f / 5.f, size_.height / 2.f ) );
//                //addChild( sprite_ );
//
//                //auto animation_ = Animation3D::create( model_ );
//                //if( animation_ )
//                //{
//                //    auto animate_ = Animate3D::create( animation_, 0.f, 1.933f );
//                //    auto swim_ = RepeatForever::create( animate_ );
//                //    sprite_->runAction( swim_ );
//                //}
//
//                //auto touch_listener_ = EventListenerTouchOneByOne::create();
//                //touch_listener_->onTouchBegan = [ = ]( Touch* t_, Event* )
//                //{
//                //    sprite_->stopActionByTag( 1 );
//                //    auto move_ = MoveTo::create( 2.f, t_->getLocation() );
//                //    move_->setTag( 1 );
//                //    sprite_->runAction( move_ );
//                //    return true;
//                //};
//
//                //auto dispatcher_ = Director::getInstance()->getEventDispatcher();
//                //dispatcher_->addEventListenerWithSceneGraphPriority( touch_listener_, this );
//
//                return true;
//            }
//
//            bool analyze()
//            {
//                data data_;
//                
//                auto skip = [ & ]
//                {
//                    while( end_ != eof_ && *end_ != '\n' )
//                    {
//                        std::advance( end_, 1 );
//                    }
//                    return true;
//                };
//                auto scan = [ & ]
//                {
//                    auto result_ = std::sscanf
//                        (
//                            std::string{ begin_, end_ }.c_str(),
//                            "%f,%f,%f,%f,%f,%f",
//                            &std::get<0>( data_ ).x,
//                            &std::get<0>( data_ ).y,
//                            &std::get<0>( data_ ).z,
//                            &std::get<1>( data_ ).x,
//                            &std::get<1>( data_ ).y,
//                            &std::get<1>( data_ ).z
//                        );
//                    if( result_ == EOF || result_ < 6 )
//                    {
//                        return false;
//                    }
//                    return true;
//                };
//
//                if( skip() && scan() )
//                {
//                    //
//                    list_.push_back( data_ );
//                    if( list_.size() > sample )
//                    {
//                        list_.pop_front();
//                    }
//                    cocos2d::log(
//                        "x:%f,y:%f,z:%f\nx:%f,y:%f,z:%f\n",
//                        std::get<0>( data_ ).x,
//                        std::get<0>( data_ ).y,
//                        std::get<0>( data_ ).z,
//                        std::get<1>( data_ ).x,
//                        std::get<1>( data_ ).y,
//                        std::get<1>( data_ ).z );
//                    if( end_ == eof_ )
//                    {
//                        return false;
//                    }
//                    std::advance( end_, 1 );
//                    begin_ = end_;
//                }
//                return true;
//            }
//
//            std::list<data> const& referenceList() const
//            {
//                return list_;
//            }
//        };
//
//        template< unsigned int sample_size_ >
//        class sample
//        {
//        public:
//            static constexpr unsigned int sample_size = sample_size_;
//
//        private:
//            using data = std::tuple<acceleration, gyro>;
//            std::list<data> list_;
//
//        public:
//            bool push( std::string string_ )
//            {
//                auto begin_ = string_.cbegin();
//                auto end_ = string_.cbegin();
//                auto eof_ = string_.cend();
//
//                data data_;
//
//                auto skip = [ & ]
//                {
//                    while( end_ != eof_ && *end_ != '\n' )
//                    {
//                        std::advance( end_, 1 );
//                    }
//                    return true;
//                };
//                auto scan = [ & ]
//                {
//                    auto result_ = std::sscanf
//                        (
//                            std::string{ begin_, end_ }.c_str(),
//                            "%f,%f,%f,%f,%f,%f",
//                            &std::get<0>( data_ ).x,
//                            &std::get<0>( data_ ).y,
//                            &std::get<0>( data_ ).z,
//                            &std::get<1>( data_ ).x,
//                            &std::get<1>( data_ ).y,
//                            &std::get<1>( data_ ).z
//                            );
//                    if( result_ == EOF || result_ < 6 )
//                    {
//                        return false;
//                    }
//                    return true;
//                };
//
//                while( skip() && scan() )
//                {
//                    //TODO: サンプルデータの位相を考慮する
//                    list_.push_back( data_ );
//                    if( list_.size() > sample_size )
//                    {
//                        list_.pop_front(); // サンプル数を超えたら破棄
//                    }
//                    //cocos2d::log(
//                    //    "x:%f,y:%f,z:%f\nx:%f,y:%f,z:%f\n",
//                    //    std::get<0>( data_ ).x,
//                    //    std::get<0>( data_ ).y,
//                    //    std::get<0>( data_ ).z,
//                    //    std::get<1>( data_ ).x,
//                    //    std::get<1>( data_ ).y,
//                    //    std::get<1>( data_ ).z );
//                    if( end_ == eof_ )
//                    {
//                        return false;
//                    }
//                    std::advance( end_, 1 );
//                    begin_ = end_;
//                }
//                return true;
//            }
//            auto cbegin() const
//            {
//                return list_.cbegin();
//            }
//            auto cend() const
//            {
//                return list_.cend();
//            }
//            auto size() const
//            {
//                return list_.size();
//            }
//        };
//
//
//        class sample_viewer
//            : public cocos2d::Node
//        {
//        public:
//            CREATE_FUNC( sample_viewer );
//            virtual bool init() override
//            {
//                using namespace cocos2d;
//                if( !Node::init() )
//                {
//                    return false;
//                }
//
//                auto draw_ = DrawNode::create();
//                draw_->setName( "draw_node" );
//                addChild( draw_, 2 );
//
//                auto bg_ = LayerColor::create( Color4B::WHITE, app_width, app_height );
//                addChild( bg_, 1 );
//
//                return true;
//            }
//            template< unsigned int sample_size >
//            void update( sample<sample_size> const& sample_ )
//            {
//                using namespace cocos2d;
//                auto draw_ = static_cast<DrawNode*>( getChildByName( "draw_node" ) );
//
//                static const float unit_height = 300.f;
//                static const float unit = 30.f;
//
//                draw_->clear();
//
//                // 目盛原点
//                Vec2 trans_{ 0, app_height / 2 };
//
//                // 目盛0線
//                draw_->drawSegment(
//                    trans_,
//                    trans_ + Vec2{ app_width, 0 },
//                    1.f,
//                    Color4F{ 0, 0, 0, 0.1f } );
//
//                // 目盛+unit線
//                draw_->drawSegment(
//                    trans_ + Vec2{ 0, unit_height },
//                    trans_ + Vec2{ app_width, unit_height },
//                    0.5f,
//                    Color4F{ 0, 0, 0, 0.1f } );
//
//                // 目盛-unit線
//                draw_->drawSegment(
//                    trans_ + Vec2{ 0, -unit_height },
//                    trans_ + Vec2{ app_width, -unit_height },
//                    0.5f,
//                    Color4F{ 0, 0, 0, 0.1f } );
//
//                if( sample_.size() > 1 )
//                {
//                    auto prev_ = sample_.cbegin();
//                    auto now_ = std::next( prev_ );
//
//                    float prevv_ = std::get<0>( *prev_ ).x;
//                    float nowv_ = 0.5 * prevv_ + 0.5 * std::get<0>( *now_ ).x;
//
//                    for( int i = 0; now_ != sample_.cend(); ++i )
//                    {
//                        // a.x
//                        draw_->drawLine(
//                            trans_ + Vec2{ i * app_width / sample_size, std::get<0>( *prev_ ).x * unit_height / unit },
//                            trans_ + Vec2{ ( i + 1 ) * app_width / sample_size, std::get<0>( *now_ ).x * unit_height / unit },
//                            Color4F{ 1, 0, 0, 0.5f } );
//
//
//                        // LowPass a.x
//                        nowv_ = 0.5 * prevv_ + 0.5 * std::get<0>( *now_ ).x;
//                        draw_->drawLine(
//                            trans_ + Vec2{ i * app_width / sample_size, prevv_ * unit_height / unit },
//                            trans_ + Vec2{ ( i + 1 ) * app_width / sample_size, nowv_ * unit_height / unit },
//                            Color4F{ 0, 0, 1, 0.5f } );
//                        prevv_ = nowv_;
//
//                        prev_ = now_;
//                        now_ = std::next( prev_ );
//                    }
//                }
//            }
//        };
//
//        class data_viewer
//            : public cocos2d::Node
//        {
//        private:
//            fft fft_;
//            unsigned cnt_;
//
//        public:
//            CREATE_FUNC( data_viewer );
//            data_viewer()
//                : fft_{ data_analyzer::sample }
//            {
//            }
//            virtual bool init() override
//            {
//                using namespace cocos2d;
//                if( !Node::init() )
//                {
//                    return false;
//                }
//                scheduleUpdate();
//
//                auto analyzer_ = data_analyzer::create();
//                analyzer_->setName( "analyzer" );
//
//                auto draw_ = DrawNode::create();
//                draw_->setName( "draw_node" );
//
//                auto bg_ = LayerColor::create( Color4B::WHITE, app_width, app_height );
//
//                addChild( analyzer_, 0 );
//                addChild( bg_, 1 );
//                addChild( draw_, 2 );
//
//                return true;
//            }
//            virtual void update( float ) override
//            {
//                using namespace cocos2d;
//
//                if( ( cnt_ %= 10 ) == 0 )
//                {
//                    auto draw_ = static_cast<DrawNode*>( getChildByName( "draw_node" ) );
//                    auto analyzer_ = static_cast<data_analyzer*>( getChildByName( "analyzer" ) );
//                    if( draw_ && analyzer_ )
//                    {
//                        static const float unit_height = 300.f;
//                        static const float unit = 30.f;
//
//                        draw_->clear();
//                        analyzer_->analyze();
//                        auto const& list_ = analyzer_->referenceList();
//                        
//                        // 目盛原点
//                        Vec2 trans_{ 0, app_height / 2 };
//
//                        // 目盛0線
//                        draw_->drawSegment(
//                            trans_,
//                            trans_ + Vec2{ app_width, 0 },
//                            1.f,
//                            Color4F{ 0, 0, 0, 0.1f } );
//
//                        // 目盛+unit線
//                        draw_->drawSegment(
//                            trans_ + Vec2{ 0, unit_height },
//                            trans_ + Vec2{ app_width, unit_height },
//                            0.5f,
//                            Color4F{ 0, 0, 0, 0.1f } );
//
//                        // 目盛-unit線
//                        draw_->drawSegment(
//                            trans_ + Vec2{ 0, -unit_height },
//                            trans_ + Vec2{ app_width, -unit_height },
//                            0.5f,
//                            Color4F{ 0, 0, 0, 0.1f } );
//
//                        if( list_.size() > 1 )
//                        {
//                            auto prev_ = list_.cbegin();
//                            auto now_ = std::next( prev_ );
//
//                            float prevv_ = std::get<0>( *prev_ ).x;
//                            float nowv_ = 0.5 * prevv_ + 0.5 * std::get<0>( *now_ ).x;
//
//                            for( int i = 0; now_ != list_.cend(); ++i )
//                            {
//                                // a.x
//                                draw_->drawLine(
//                                    trans_ + Vec2{ i * app_width / data_analyzer::sample, std::get<0>( *prev_ ).x * unit_height / unit },
//                                    trans_ + Vec2{ ( i + 1 ) * app_width / data_analyzer::sample, std::get<0>( *now_ ).x * unit_height / unit },
//                                    Color4F{ 1, 0, 0, 0.5f } );
//
//
//                                // LowPass a.x
//                                nowv_ = 0.5 * prevv_ + 0.5 * std::get<0>( *now_ ).x;
//                                draw_->drawLine(
//                                    trans_ + Vec2{ i * app_width / data_analyzer::sample, prevv_ * unit_height / unit },
//                                    trans_ + Vec2{ ( i + 1 ) * app_width / data_analyzer::sample, nowv_ * unit_height / unit },
//                                    Color4F{ 0, 0, 1, 0.5f } );
//                                prevv_ = nowv_;
//
//                                //// a.y
//                                //draw_->drawLine(
//                                //    trans_ + Vec2{ i * app_width / data_analyzer::sample, std::get<0>( *prev_ ).y * unit_height / unit },
//                                //    trans_ + Vec2{ ( i + 1 ) * app_width / data_analyzer::sample, std::get<0>( *now_ ).y * unit_height / unit },
//                                //    Color4F{ 0, 0, 1, 0.5f } );
//
//                                //// a.z
//                                //draw_->drawLine(
//                                //    trans_ + Vec2{ i * app_width / data_analyzer::sample, std::get<0>( *prev_ ).z * unit_height / unit },
//                                //    trans_ + Vec2{ ( i + 1 ) * app_width / data_analyzer::sample, std::get<0>( *now_ ).z * unit_height / unit },
//                                //    Color4F{ 0, 1, 0, 0.5f } );
//
//                                prev_ = now_;
//                                now_ = std::next( prev_ );
//                            }
//
//                            if( list_.size() == data_analyzer::sample )
//                            {
//                                double data_[ data_analyzer::sample * 2 ];
//
//                                auto it_ = list_.cbegin();
//
//                                for( int i = 0; i < data_analyzer::sample; ++i )
//                                {
//                                    CCASSERT( it_ != list_.cend(), "" );
//                                    data_[ i * 2 ] = static_cast<double>( std::get<0>( *it_ ).x )
//                                        * ( 0.54 - 0.46 * std::cos( 2 * M_PI * i / ( data_analyzer::sample * 2 ) ) ); // hamming window
//                                    data_[ i * 2 + 1 ] = 0; // imaginary part
//                                    ++it_;
//                                }
//
//                                fft_.cdft( -1, data_ );
//
//                                int c = 0;
//                                // 直流スキップ
//                                for( int i = 1; i < data_analyzer::sample / 2; ++i )
//                                {
//                                    draw_->drawSegment(
//                                        Vec2{ ( c + 0.5f ) * app_width / ( data_analyzer::sample + 1 ), 0 },
//                                        Vec2{
//                                            ( c + 0.5f ) * app_width / ( data_analyzer::sample + 1 ),
//                                            ( static_cast<float>( std::pow( data_[ i * 2 ], 2 ) + std::pow( data_[ i * 2 + 1 ], 2 ) ) / data_analyzer::sample )
//                                        },
//                                        app_width / data_analyzer::sample / 2,
//                                        Color4F{ 0, 0, 1, 0.5f } );
//                                    ++c;
//                                }
//                            }
//                        }
//                    }
//                }
//                ++cnt_;
//            }
//        };
//    }
//}
//
//#endif//C2XA_SCENE_EXPERIMENTAL_DATA_ANALYZER_HPP