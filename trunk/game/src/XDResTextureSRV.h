/*
*  (c) 2013-2014 XediXermawan < edi.ermawan@gmail.com >
*/
#ifndef _RES_TEXTURE_SRV_H__
#define _RES_TEXTURE_SRV_H__

#include "XDResource.h"

class ResTextureSRV : public Resource {
  public:
    ResTextureSRV();
    ~ResTextureSRV();
    void SetSRV(ID3D11ShaderResourceView* srv);
    ID3D11ShaderResourceView* GetSRV() const;
  private:
    ID3D11ShaderResourceView* m_SRV;
};

#endif // _RES_TEXTURE_SRV_H__