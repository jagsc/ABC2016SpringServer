

/**

    @file   observer.hpp
    @brief  observer

    @author 新ゝ月かりな(NewNotMoon)
    @date   created on 2016/00/00

*/
#pragma once
#ifndef C2XA_OBSERVER_HPP
#define C2XA_OBSERVER_HPP

#include <list>

#include <cocos2d.h>

#include <c2xa/utility.hpp>

namespace c2xa
{
    template< typename DATA >
    class subject;
    template< typename DATA >
    class observer
        : public cocos2d::Node
    {
    private:
        subject<DATA>* bind_ = nullptr;
        std::function<void( DATA )> callback_;

    public:
        static observer<DATA>* create( std::function<void( DATA )> c )
        {
            return create_template<observer<DATA>>( c );
        }
        virtual bool init( std::function<void( DATA )> c )
        {
            using namespace cocos2d;
            if( !Node::init() )
            {
                return false;
            }
            callback_ = c;
            return true;
        }
        ~observer();
        void notified( DATA a )
        {
            callback_( a );
        }
        void bind( subject<DATA>* a, bool pre_registry_ = false );
        void cancel();
    };
    template< typename DATA >
    class subject_interface
    {
    public:
        using observer_type = observer<DATA>;

    public:
        virtual void registry_observer( observer_type* o, bool pre_bind_ = false ) = 0;
        virtual void remove_observer( observer_type* o, bool pre_separate_ = false ) = 0;
    };
    template< typename DATA >
    class subject
        : public subject_interface< DATA >
    {
    public:
        using observer_type  = observer<DATA>;
        using interface_type = subject_interface<DATA>;

    private:
        std::list<observer_type*> observers_;

    public:
        ~subject()
        {
            clear();
        }
        void notify( DATA data_ )
        {
            for( auto i : observers_ )
            {
                i->notified( data_ );
            }
        }
        void registry_observer( observer_type* o, bool pre_bind_ = false ) override
        {
            if( !pre_bind_ )
            {
                o->bind( this, false );
            }
            observers_.push_back( o );
        }
        void remove_observer( observer_type* o, bool pre_separate_ = false ) override
        {
            auto i = std::remove_if( observers_.begin(), observers_.end(),
                [ o, pre_separate_ ]( observer_type* i )
            {
                if( o == i )
                {
                    if( !pre_separate_ )
                    {
                        i->cancel();
                    }
                    return true;
                }
                return false;
            } );
            observers_.erase( i, observers_.end() );
        }
        void clear()
        {
            for( auto i : observers_ )
            {
                i->cancel();
            }
            observers_.clear();
        }
    };
    template< typename DATA >
    void bind_observer( subject< DATA >* sub_, observer< DATA >* obs_ )
    {
        obs_->bind( sub_ );
        sub_->registry_observer( obs_ );
    }
    template< typename DATA >
    observer<DATA>::~observer()
    {
        cancel();
    }
    template< typename DATA >
    void observer<DATA>::bind( subject<DATA>* a, bool pre_registry_ )
    {
        cancel();
        bind_ = a;
        if( !pre_registry_ )
        {
            bind_->registry_observer( this, true );
        }
    }
    template< typename DATA >
    void observer<DATA>::cancel()
    {
        if( bind_ )
        {
            bind_->remove_observer( this, true );
            bind_ = nullptr;
        }
    }
}

#endif//C2XA_OBSERVER_HPP