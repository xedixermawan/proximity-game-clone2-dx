/*
*  (c) 2013-2014 XediXermawan < edi.ermawan@gmail.com >
*/
#ifndef __XDSPRITE_H__
#define __XDSPRITE_H__

#include "SpriteBatch.h"

#include "XDFrameAnimDescription.h"
#include "XDSpriteLoader.h"
#include "DDSTextureLoader.h"

class XDFileSystem;

using namespace DirectX;

class XDSprite {
  public:
    XDSprite(const std::string& filename, XDFileSystem* xdfs = nullptr);
    ~XDSprite();
    void LoadTexture(ID3D11Device* dxdevice);
    void SetSRV( ID3D11ShaderResourceView* srv);
    void PaintModuleNth(unsigned int index, int offsetx=0, int offsety=0 );
    void PaintFrameNth(unsigned int index, int offsetx=0, int offsety=0 );
    void PaintFrameAnim(unsigned int animIndex, unsigned int frameIndex, int offsetx=0, int offsety=0 );
    void SetSpriteBatch(SpriteBatch* spritebatch);
    int GetNFrameAnim(int animIndex);
    void FlipHorizontally();
    void SetScale(float scale);
  private:
    int m_Nmodule;
    int** m_Modules;
    int m_FlipH;
    float m_Scale;
    std::vector< FrameDesc<int>* > m_FrameDesc;
    std::vector< AnimDesc<int>* > m_AnimDesc;
    std::string m_SpriteName;
    std::string m_TextureName;
    SpriteLoader* m_Loader;
    ID3D11ShaderResourceView* m_ShaderResView;
    SpriteBatch* m_SpriteBatch;


    void Load();
    int GetModuleIndexByID(int moduid) const ;
    int GetFrameIndexByID(int frameuid) const ;
};

#endif // __XDSPRITE_H__