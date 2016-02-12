

#pragma once
#ifndef C2XA_EXCEPTION_HPP
#define C2XA_EXCEPTION_HPP

#include <string>

namespace c2xa
{
#ifdef _DEBUG
    struct error
    {
        std::string file;
        decltype( __LINE__ ) line;
        std::string function;
        std::string message;

        error( error const& e )
            : file{ e.file }
            , line{ e.line }
            , function{ e.function }
            , message{ e.message }
        {
        }
        template< typename File, typename Line, typename Function, typename Message >
        error( File file, Line line, Function function, Message message )
            : file{ std::forward<File>( file ) }
            , line{ std::forward<Line>( line ) }
            , function{ std::forward<Function>( function ) }
            , message{ std::forward<Message>( message ) }
        {
        }
    };
    struct exception
        : virtual std::exception
    {
        error error_;

        exception( error e )
            : error_{ e }
        {
        }
    };
#define _C2XA_EXCEPTION( name )\
    struct name##_exception\
        : public exception\
    {\
        name##_exception( name##_exception const& e )\
            : exception{ e.error_ }\
        {\
        }\
        name##_exception( error e )\
            : exception{ e }\
        {\
        }\
    };
#else
    struct exception
        : virtual std::exception
    {
    };
#define _C2XA_EXCEPTION( name )\
    struct name##_exception\
        : public exception\
    {\
    };
#endif

    _C2XA_EXCEPTION( bluetooth );
    _C2XA_EXCEPTION( bluetooth_disconnect );
}

#undef _C2XA_EXCEPTION
#ifdef _DEBUG
#define C2XA_THROW( exc, msg ) throw exc{ error{ __FILE__, __LINE__, __FUNCTION__, msg } }
#else
#define C2XA_THROW( exc, msg ) throw exc{}
#endif

#endif//C2XA_EXCEPTION_HPP