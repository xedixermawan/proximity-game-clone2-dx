/*
*  (c) 2013-2014 XediXermawan < edi.ermawan@gmail.com >
*/
#ifndef _RESTEXSRV_LOADER_H__
#define _RESTEXSRV_LOADER_H__

#include "XDResLoader.h"

class ResTextureSRVLoader : public ResLoader {
  public:
    ResTextureSRVLoader();
    ~ResTextureSRVLoader();
    virtual Resource* Load(const std::string& name, ResProxy* reproxy);
};

#endif // _RESTEXSRV_LOADER_H__