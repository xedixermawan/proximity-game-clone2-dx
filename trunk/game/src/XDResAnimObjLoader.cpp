/*
*  (c) 2013-2014 XediXermawan < edi.ermawan@gmail.com >
*/
#include "pch.h"
#include "XDResAnimObjLoader.h"
#include "XDResAnimObject.h"
#include "XDResSprite.h"
#include "XDResProxy.h"

ResAnimObjLoader::ResAnimObjLoader() {
}

ResAnimObjLoader::~ResAnimObjLoader() {
}

Resource* ResAnimObjLoader::Load(const std::string& name, ResProxy* reproxy) {
    ResAnimObject* resAnim = new ResAnimObject();
    resAnim->Create( new XDAnimObject ( nullptr ) );
    return resAnim;
}