/*
*  (c) 2013-2014 XediXermawan < edi.ermawan@gmail.com >
*/
#ifndef _RESOURCE_H__
#define _RESOURCE_H__

class Resource {
  public:
    enum Type {
        SPRITE,
        ANIMOBJECT,
        TEXTURESRV
    };
    Resource();
    std::string Name;
    Type ResType;
  protected:
    virtual ~Resource();
};

#endif // _RESOURCE_H__