/*
*  (c) 2013-2014 XediXermawan < edi.ermawan@gmail.com >
*/
#include "pch.h"
#include "XDResTextureSRV.h"

ResTextureSRV::ResTextureSRV() {
}

ResTextureSRV::~ResTextureSRV() {
    delete m_SRV;
}

void ResTextureSRV::SetSRV(ID3D11ShaderResourceView* srv) {
    m_SRV = srv;
}

ID3D11ShaderResourceView* ResTextureSRV::GetSRV() const {
    return m_SRV;
}