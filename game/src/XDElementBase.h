/*
*  (c) 2013-2014 XediXermawan < edi.ermawan@gmail.com >
*/
#ifndef __GUIELEMENT_BASE_H__
#define __GUIELEMENT_BASE_H__

#include "XDSprite.h"
#include "XDAnimObject.h"

namespace gui {

class ElementBase {
  public:
    ElementBase(const std::shared_ptr< XDSprite >& sharedptrSpr)
        : m_Sprite(sharedptrSpr),
          m_FrameClicked(0),
          m_FrameHover(0),
          m_PosX(0),
          m_PosY(0),
          m_Height(150),
          m_Width(150),
          m_UID(0),
          m_Visibility(1),
          m_Name(""),
          m_SpriteID(0),
          m_AnimIdleID(0),
          m_AnimHoverID(0),
          m_AnimClickedID(0) {
        m_Anim = std::shared_ptr< XDAnimObject > ( new XDAnimObject( m_Sprite.get() ) );
    };
    virtual ~ElementBase() {
    };
    virtual bool IsIntersect(const int& posx,const int& posy)=0;
    virtual void OnClicked(const int& posx, const int& posy)=0;
    virtual void OnClickReleased(const int& posx, const int& posy)=0;
    virtual void OnHover(const int& posx, const int& posy)=0;
    virtual void Render()=0;
    virtual void Update(const double delta_time)=0;
    virtual int GetUID()=0;
    virtual void SetPos(int vposx,int vposy)=0;
    virtual void GetWidthHeight(int& gwidth, int& gheight )=0;
    virtual void SetWidthHeight(int gwidth, int gheight )=0;
    virtual void SetCallBack(std::function< void(void* sender) >& function)=0;
    virtual void SetAnim(const int animid) {
        m_Anim->SetAnim(animid);
    };
    virtual void SetVisible(bool val) {
        m_Visibility= val;
    };
    virtual int GetVisible() const {
        return m_Visibility;
    };
    virtual void SetAnimProps(unsigned int spriteID,unsigned int animIdleID,unsigned int animHoverID,unsigned int animClickedID) {
        m_SpriteID      = spriteID;
        m_AnimIdleID    = animIdleID;
        m_AnimHoverID   = animHoverID;
        m_AnimClickedID = animClickedID;
    };
    virtual void SetName(const std::string& name) {
        m_Name = name;
    };
    virtual std::string GetName() const {
        return m_Name;
    };
  protected:
    std::shared_ptr< XDSprite > m_Sprite;
    std::shared_ptr< XDAnimObject > m_Anim;
    int m_FrameClicked;
    int m_FrameHover;
    int m_PosX;
    int m_PosY;
    int m_Height;
    int m_Width;
    int m_UID;
    int m_Visibility;
    std::string m_Name;
    unsigned int m_SpriteID,m_AnimIdleID,m_AnimHoverID,m_AnimClickedID;
};

};

#endif // __GUIELEMENT_BASE_H__