/*
*  (c) 2013-2014 XediXermawan < edi.ermawan@gmail.com >
*/
#include "pch.h"
#include "XDResTextureSRVLoader.h"
#include "XDFileSystem.h"
#include "XDFuncUtils.h"
#include "DDSTextureLoader.h"
#include "XDResProxy.h"
#include "XDResTextureSRV.h"

ResTextureSRVLoader::ResTextureSRVLoader() {
}

ResTextureSRVLoader::~ResTextureSRVLoader() {
}

Resource* ResTextureSRVLoader::Load(const std::string& name, ResProxy* reproxy) {
    std::string corrected_path="";
    char temp[64];
    GetFilenameNoExt(temp,name.c_str());
    strcat_s(temp,".dds");
    reproxy->GetFS()->ResolveToROFolder(corrected_path, temp );
    assert ( ( reproxy->GetFS()->FileExist ( corrected_path.c_str() ) == true ) );
    ID3D11ShaderResourceView* shader_res_view = nullptr;
    DirectX::CreateDDSTextureFromFile( reproxy->GetDevice() , UTF8ToWChar (corrected_path.c_str() ), nullptr, &shader_res_view, MAXSIZE_T);
    ResTextureSRV* res = new ResTextureSRV();
    res->SetSRV( shader_res_view );
    return res;
}