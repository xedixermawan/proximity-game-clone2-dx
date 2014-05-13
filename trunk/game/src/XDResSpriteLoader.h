/*
*  (c) 2013-2014 XediXermawan < edi.ermawan@gmail.com >
*/
#ifndef _RESSPRITE_LOADER_H__
#define _RESSPRITE_LOADER_H__

#include "XDResLoader.h"

class ResSpriteLoader : public ResLoader {
  public:
    ResSpriteLoader();
    ~ResSpriteLoader();
    virtual Resource* Load(const std::string& name, ResProxy* reproxy);
};

#endif // _RESSPRITE_LOADER_H__