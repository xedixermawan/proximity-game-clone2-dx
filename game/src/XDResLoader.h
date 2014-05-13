/*
*  (c) 2013-2014 XediXermawan < edi.ermawan@gmail.com >
*/
#ifndef _RESOURCE_LOADER_H__
#define _RESOURCE_LOADER_H__

#include "XDResource.h"

class ResProxy;

class ResLoader {
  public:
    ResLoader() {};
    virtual Resource* Load(const std::string& name, ResProxy* reproxy)=0;
  protected:
    virtual ~ResLoader() {};
};

#endif	// _RESOURCE_LOADER_H__