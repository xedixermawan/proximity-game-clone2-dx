/*
*  (c) 2013-2014 XediXermawan < edi.ermawan@gmail.com >
*/
#include "pch.h"
#include "XDStateLoading.h"
#include "XDGameRenderer.h"
#include "XDGUIManager.h"
#include "XDResProxy.h"

#include "XDResSprite.h"
#include "XDResAnimObject.h"

#include <iostream>	//test

StateLoading::StateLoading(XDSystem* xdsystem) : GameState(xdsystem)   {
    m_NextState      = STATE::MENU;
    m_StateStartTime = steady_clock::now();
}

void StateLoading::Initialize() {
    m_SprLoading            =  m_XDSystem->Resource->GetSprite( "xedisoftware.xml" );
    m_SprLoading->SetSpriteBatch ( m_XDSystem->Renderer->SpriteBatchs[ XDGameRenderer::EBATCH::L0 ] );
    m_AnimLoading           = m_XDSystem->Resource->GetAnim( "xedisoftware.anim" );
    m_AnimLoading->SetSprite( m_SprLoading );
    m_SubState = SUBSTATE::MAIN;
}

int StateLoading::Update(const double delta_time) {
    m_AnimLoading->Update(delta_time);
    steady_clock::time_point _time_now = steady_clock::now();
    double _delta_time_seconds = ( duration_cast<milliseconds>(_time_now - m_StateStartTime).count() ) ;
    if( 0 < _delta_time_seconds) {
        m_SubState = SUBSTATE::STATEOUT;
    }
    return m_SubState;
}

void StateLoading::Render() {
    switch (m_SubState) {
    case GameState::INIT:
        break;
    case GameState::MAIN:
        m_AnimLoading->Render();
        break;
    case GameState::DEINIT:
        break;
    case GameState::STATEOUT:
        break;
    default:
        break;
    }
    gui::GUIManager::GetInstance().Render();
}

void StateLoading::RenderDebug()	{
}

void StateLoading::OnSuspending() {
}

void StateLoading::OnResuming(XDSystem* xdsystem) {
    //m_Renderer  = renderer;
    //m_SprLoading  = std::shared_ptr< XDSprite> ( new XDSprite (std::string("xedisoftware.xml") ) ) ;
    //m_AnimLoading = std::shared_ptr< XDAnimObject> (  new XDAnimObject( m_SprLoading.get() , 0 ,0) );
    //m_SprLoading->SetSpriteBatch ( m_Renderer->SpriteBatchs[ XDGameRenderer::EBATCH::L0 ] );
    //m_SprLoading->LoadTexture( m_Renderer->GetDevice() );
}

void StateLoading::OnHandleInput(  const SINPUT& input ) {
}

StateLoading::~StateLoading() {
}

GameState::STATE StateLoading::Next()	{
    return m_NextState;
}
