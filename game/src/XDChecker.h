/*
*  (c) 2013-2014 XediXermawan < edi.ermawan@gmail.com >
*/
#ifndef _XD_CHECKER_H_
#define _XD_CHECKER_H_

// #define ENABLE_ASSERT
#define ENABLE_EXCEPTION
#define ENABLE_LOG_CONSOLE

class XDChecker {
  public:
    static void Assert( bool checkedvalue );
    static void Assert( bool checkedvalue, const std::string& msg );
    static void Console(const char* msg, ... );
};

#endif // _XD_CHECKER_H_