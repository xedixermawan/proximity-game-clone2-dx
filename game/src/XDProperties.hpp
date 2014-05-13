/*
*  (c) 2013-2014 XediXermawan < edi.ermawan@gmail.com >
*/
#define WINDOWS_NAME "_unknown_game"
#define LWINDOWS_NAME L"_unknown_game"


class Property {
  public:
    enum class Windows {
        Width  = 800 ,
        Height = 480,
    };
    static std::string WindowTitle() {
        return WINDOWS_NAME;
    }
    static const wchar_t* WindowTitleWCHAR() {
        return LWINDOWS_NAME;
    }
};