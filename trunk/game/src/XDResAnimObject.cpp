/*
*  (c) 2013-2014 XediXermawan < edi.ermawan@gmail.com >
*/
#include "pch.h"
#include "XDResAnimObject.h"

ResAnimObject::ResAnimObject() {
}

ResAnimObject::~ResAnimObject() {
    delete m_AnimObject;
}

XDAnimObject* ResAnimObject::GetAnim() const {
    return m_AnimObject;
}

void ResAnimObject::Create(XDAnimObject* anim) {
    m_AnimObject = anim;
}