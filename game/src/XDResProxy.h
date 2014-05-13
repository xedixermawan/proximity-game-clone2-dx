/*
*  (c) 2013-2014 XediXermawan < edi.ermawan@gmail.com >
*/
#ifndef _RES_PROXY_H__
#define _RES_PROXY_H__

#include <d3d11.h>
#include <map>
#include "XDResLoader.h"

class Resource;
class ResAnimObject;
class ResSprite;
class ResTextureSRV;
class XDFileSystem;

class XDAnimObject;
class XDSprite;

class ResProxy {
  public:
    ResProxy(XDFileSystem* xdfs);
    ~ResProxy();
    Resource* GetResource(Resource::Type type, const std::string& name);
    XDAnimObject* GetAnim(const std::string& name);
    XDSprite* GetSprite(const std::string& name);
    ID3D11Device* GetDevice();
    void SetDevice(ID3D11Device* device);
    XDFileSystem* GetFS();
  private:
    std::map< std::string, Resource* > m_Resources;
    std::map< Resource::Type, ResLoader* > m_ResLoaders;
    ID3D11Device* m_Device;
    XDFileSystem* m_XDFS;
};

#endif // _RES_PROXY_H__