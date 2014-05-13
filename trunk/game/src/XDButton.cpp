/*
*  (c) 2013-2014 XediXermawan < edi.ermawan@gmail.com >
*/
#include "pch.h"
#include "XDButton.h"

#include "XDGameState.h"
extern XDSystem* debugSystemPtr;
#include "XDChecker.h"
#include "XDFuncUtils.h"

namespace gui {

Button::Button(const std::shared_ptr< XDSprite >& sharedptrSpr) :ElementBase(sharedptrSpr)	{
    m_Anim->SetAnimLoop(XDAnimObject::ANIMLOOP::ONCE);
    m_Anim->ShowAfterFinish(true);
    m_FrameHover =  1;
    m_HoverState =  0;
    m_ClickedState= 0;
}

Button::Button() : ElementBase(nullptr)	{
    m_HoverState =  0;
    m_ClickedState= 0;
}

Button::~Button()	{
}

bool Button::IsIntersect(const int& posx, const int& posy)	{
    if( ( posx > m_PosX ) && ( posx < (m_PosX + m_Width) ) &&
            ( posy > m_PosY ) && ( posy < (m_PosY + m_Height)) ) {
        XDChecker::Console(" inside ");
        return true;
    }
    return false;
}

void Button::OnClicked(const int& posx, const int& posy)	{
    if( IsIntersect(posx,posy) ) {
        if( m_CallBackFunction )
            m_CallBackFunction(this);
        m_ClickedState=1;
    }
}

void Button::OnClickReleased(const int& posx, const int& posy)	{
    if( IsIntersect(posx,posy) ) {
        m_ClickedState=1;
    }
}

void Button::OnHover(const int& posx, const int& posy)	{
    if( IsIntersect(posx,posy) ) {
        m_Anim->SetFinishFrame(m_FrameHover);
        m_HoverState=1;
    } else {
        m_Anim->SetFinishFrame(0);
        m_HoverState=0;
    }
}

void Button::Render()	{
    //m_Anim->Render();
    if( m_HoverState ) {
        debugSystemPtr->Renderer->DrawRect( XMFLOAT2(m_PosX-4,m_PosY-4),XMFLOAT2(m_Width+8,m_Height+8) );
    } else {
        debugSystemPtr->Renderer->DrawRect( XMFLOAT2(m_PosX,m_PosY),XMFLOAT2(m_Width,m_Height) );
    }
    if(m_ClickedState>0 && m_ClickedState < 20) {
        int r_v = RandomRange(2,20);
        debugSystemPtr->Renderer->DrawRect( XMFLOAT2(m_PosX-r_v,m_PosY-r_v),XMFLOAT2(m_Width+2*r_v,m_Height+2*r_v) );
        m_ClickedState++;
    }
}

void Button::Update(const double delta_time)	{
    //m_Anim->Update(delta_time);
}

int Button::GetUID()	{
    return 0; // todo
}

void Button::SetPos(int vposx,int vposy)	{
    m_PosX = vposx;
    m_PosY = vposy;
    m_Anim->SetPos(vposx,vposy);
}

void Button::GetWidthHeight(int& gwidth, int& gheight )	{
    gwidth  = m_Width;
    gheight = m_Height;
}

void Button::SetWidthHeight(int gwidth, int gheight )	{
    m_Width  = gwidth;
    m_Height = gheight;
}

void Button::SetCallBack(std::function< void(void* sender) >& function) {
    m_CallBackFunction = function;
}

}