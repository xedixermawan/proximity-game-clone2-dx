/*
*  (c) 2013-2014 XediXermawan < edi.ermawan@gmail.com >
*/
#ifndef _RES_SPRITE_H__
#define _RES_SPRITE_H__

#include "XDResource.h"
#include "XDSprite.h"

class ResSprite : public Resource {
  public:
    ResSprite();
    ~ResSprite();
    void Create( XDSprite* sprite );
    XDSprite* GetSprite() const;
  private:
    XDSprite* m_Sprite;
};

#endif // _RES_SPRITE_H__