/*
*  (c) 2013-2014 XediXermawan < edi.ermawan@gmail.com >
*/
#include "pch.h"
#include "XDResSpriteLoader.h"
#include "XDResSprite.h"
#include "XDResProxy.h"
#include "XDResTextureSRV.h"
#include "XDFuncUtils.h"

ResSpriteLoader::ResSpriteLoader() : ResLoader() {
}

ResSpriteLoader::~ResSpriteLoader() {
}

Resource* ResSpriteLoader::Load(const std::string& name, ResProxy* reproxy) {
    XDSprite* spr         = new XDSprite ( name , reproxy->GetFS() );
    char texname[512];
    GetFilenameNoExt(texname,name.c_str());
    strcat_s(texname,".dds");
    Resource* ressrv_     = reproxy->GetResource( Resource::TEXTURESRV, texname );
    ResTextureSRV* ressrv = static_cast< ResTextureSRV* > ( ressrv_ );
    spr->SetSRV( ressrv->GetSRV() );
    ResSprite* res = new ResSprite();
    res->Create( spr );
    return res;
}