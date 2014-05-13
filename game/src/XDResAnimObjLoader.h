/*
*  (c) 2013-2014 XediXermawan < edi.ermawan@gmail.com >
*/
#ifndef _RESANIMOBJECT_LOADER_H__
#define _RESANIMOBJECT_LOADER_H__

#include "XDResLoader.h"

class ResAnimObjLoader : public ResLoader {
  public:
    ResAnimObjLoader();
    ~ResAnimObjLoader();
    virtual Resource* Load(const std::string& name, ResProxy* reproxy);
};

#endif // _RESANIMOBJECT_LOADER_H__

