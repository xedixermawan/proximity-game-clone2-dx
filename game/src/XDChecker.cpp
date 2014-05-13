/*
*  (c) 2013-2014 XediXermawan < edi.ermawan@gmail.com >
*/
#include "pch.h"
#include "XDChecker.h"

void XDChecker::Assert( bool checkedvalue ) {
#if defined(ENABLE_ASSERT)
    assert( checkedvalue );
#elif defined(ENABLE_EXCEPTION)
    if( !checkedvalue ) {
        throw std::exception( " exception trown " );
    }
#else
    // none
#endif
}

void XDChecker::Assert( bool checkedvalue, const std::string& msg ) {
#if defined(ENABLE_ASSERT)
    assert( checkedvalue );
#elif defined(ENABLE_EXCEPTION)
    if( !checkedvalue ) {
        throw std::exception( msg.c_str() );
    }
#else
    // none
#endif
}

void XDChecker::Console(const char* msg, ... ) {
#if defined(ENABLE_LOG_CONSOLE)
#define BUFF_S 2048
    char str[BUFF_S];
    va_list arg_list;
    va_start(arg_list, msg);
    vsnprintf_s(str, BUFF_S , msg, arg_list);
    OutputDebugStringA(str);
    printf( str );
#endif
}