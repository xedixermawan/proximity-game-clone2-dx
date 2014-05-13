/*
*  (c) 2013-2014 XediXermawan < edi.ermawan@gmail.com >
*/
#include "pch.h"
#include "XDResProxy.h"
#include "XDResSpriteLoader.h"
#include "XDResAnimObjLoader.h"
#include "XDResTextureSRVLoader.h"

#include "XDResSprite.h"
#include "XDResAnimObject.h"

ResProxy::ResProxy(XDFileSystem* xdfs)  {
    m_Device = nullptr;
    m_XDFS = xdfs;
    ResLoader* loadersprite  = new ResSpriteLoader();
    ResLoader* loaderanim    = new ResAnimObjLoader();
    ResLoader* loadertexture = new ResTextureSRVLoader();
    m_ResLoaders.insert( std::pair < Resource::Type , ResLoader* > ( Resource::Type::SPRITE,  loadersprite ) );
    m_ResLoaders.insert( std::pair < Resource::Type , ResLoader* > ( Resource::Type::ANIMOBJECT,  loaderanim ) );
    m_ResLoaders.insert( std::pair < Resource::Type , ResLoader* > ( Resource::Type::TEXTURESRV,  loadertexture ) );
}

ResProxy::~ResProxy()  {
}

Resource* ResProxy::GetResource(Resource::Type type, const std::string& name) {
    std::map< std::string, Resource* >::iterator it = m_Resources.find( name );
    if( it != m_Resources.end() ) {
        return (*it).second;
    }
    ResLoader* loader = nullptr;
    std::map< Resource::Type , ResLoader* >::iterator loader_it = m_ResLoaders.find( type ) ;
    if( loader_it != m_ResLoaders.end() ) {
        loader = (*loader_it).second;
    }
    if( !loader ) {
        throw std::exception(" loader for this resource not found ");
    }
    Resource* res = loader->Load( name, this );
    if( res ) {
        m_Resources.insert( std::pair <std::string, Resource* > ( name,  res ) );
        return res;
    }
    return nullptr;
}

XDAnimObject* ResProxy::GetAnim(const std::string& name) {
    ResAnimObject* ressrv  = static_cast< ResAnimObject* > ( GetResource( Resource::ANIMOBJECT, name ) );
    return ressrv->GetAnim();
}

XDSprite* ResProxy::GetSprite(const std::string& name) {
    ResSprite* ressrv  = static_cast< ResSprite* > ( GetResource( Resource::SPRITE, name ) );
    return ressrv->GetSprite();
}

ID3D11Device* ResProxy::GetDevice() {
    assert( m_Device != nullptr ) ;
    return m_Device;
}

void ResProxy::SetDevice(ID3D11Device* device) {
    m_Device = device;
}

XDFileSystem* ResProxy::GetFS() {
    return m_XDFS;
}