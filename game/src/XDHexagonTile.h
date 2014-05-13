/*
*  (c) 2013-2014 XediXermawan < edi.ermawan@gmail.com >
*/

#ifndef _HEXAGON_TILE_H_
#define _HEXAGON_TILE_H_

#include "XDAnimObject.h"

class XDAnimObject;

class XDHexagonTile : public XDAnimObject {
  public:
    XDHexagonTile(XDSprite* sprite,int posx=0,int posy=0);
    ~XDHexagonTile();
    void Update(const double delta_time);
    void Render();
    unsigned int Index;
    unsigned int Number;
    unsigned int Color;
};

#endif // _HEXAGON_TILE_H_