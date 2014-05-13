/*
*  (c) 2013-2014 XediXermawan < edi.ermawan@gmail.com >
*/
#ifndef _RESANIMOBJECT_H__
#define _RESANIMOBJECT_H__

#include "XDResource.h"
#include "XDAnimObject.h"

class ResAnimObject : public Resource {
  public:
    ResAnimObject();
    ~ResAnimObject();
    XDAnimObject* GetAnim() const;
    void Create(XDAnimObject* anim);
  private:
    XDAnimObject* m_AnimObject;
};

#endif // _RESANIMOBJECT_H__

