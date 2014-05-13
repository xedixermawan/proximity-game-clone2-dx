/*
*  (c) 2013-2014 XediXermawan < edi.ermawan@gmail.com >
*/
#include "pch.h"
#include "XDResSprite.h"

ResSprite::ResSprite() {
}

ResSprite::~ResSprite() {
    delete m_Sprite;
}

void ResSprite::Create( XDSprite* sprite ) {
    m_Sprite = sprite;
}

XDSprite* ResSprite::GetSprite() const {
    return m_Sprite;
}