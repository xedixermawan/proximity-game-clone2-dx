/*
*  (c) 2013-2014 XediXermawan < edi.ermawan@gmail.com >
*/
#include "pch.h"
#include "XDHexagonTile.h"

XDHexagonTile::XDHexagonTile(XDSprite* sprite,int posx,int posy)
    :XDAnimObject(sprite,posx,posy),
     Index(0),
     Number(0),
     Color(0) {
}

XDHexagonTile::~XDHexagonTile() {
}
void XDHexagonTile::Update(const double delta_time) {
    XDAnimObject::Update( delta_time );
}

void XDHexagonTile::Render() {
    XDAnimObject::Render();
}