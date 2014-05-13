/*
*  (c) 2013-2014 XediXermawan < edi.ermawan@gmail.com >
*/
#include "pch.h"
#include <ctime>

#include "XDGame.h"
#include "XDFileSystem.h"
#include "XDCreateState.h"
#include "XDFuncUtils.h"
#include "XDChecker.h"

extern double g_fps_measure;

#if WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP
extern int g_exit_app;
#endif

//#define TEST_MODE
XDSystem* debugSystemPtr;

XDGame::XDGame()	{
    m_CurrentState = nullptr;
    m_System = new XDSystem();
    debugSystemPtr = m_System;
}

XDGame::~XDGame()	{
}
#if WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
void XDGame::Initialize(ID3D11Device1* device, ID3D11DeviceContext1* context)	 {
    m_Renderer = std::shared_ptr< XDGameRenderer > ( new XDGameRenderer( device,
                 context,
                 ( unsigned int ) Property::Windows::Width,
                 ( unsigned int ) Property::Windows::Height
                                                                       ) );
    m_Renderer->InitResources();
    if( m_CurrentState == nullptr ) {
        extern std::string g_rw_local_folder;
        extern std::string g_ro_local_folder;
        FileSystem::RWFolder.append(g_rw_local_folder);
        FileSystem::ROFolder.append(g_ro_local_folder);
        m_CurrentState = std::unique_ptr < GameState > ( CreateState::NewState ( GameState::STATE::LOADING , m_Renderer.get() ) );
        m_CurrentState->Initialize();
    }
    srand( time (NULL) );
}
#elif WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP
void XDGame::Initialize(HWND& hwindow) {
    // init system
    m_System->FileSystem->SetROFolder(".\\data");
    m_System->FileSystem->SetRWFolder(".\\data");
    m_System->Renderer->Init( hwindow , ( unsigned int ) Property::Windows::Width, ( unsigned int ) Property::Windows::Height );
    // after renderer init, device d3d is ready
    m_System->Resource->SetDevice( m_System->Renderer->GetDevice() );
    m_System->Renderer->InitResources();
    // init game state
    m_CurrentState = std::unique_ptr < GameState > ( CreateState::NewState ( GameState::STATE::LOADING , m_System ) );
    m_CurrentState->Initialize();
    // load test font
    std::string corrected_path="";
    m_System->FileSystem->ResolveToROFolder(corrected_path, "fonts\\vani_16b.spritefont" );
    XDChecker::Assert( ( m_System->FileSystem->FileExist ( corrected_path.c_str() ) == true ) );
    m_TestFont = std::unique_ptr < SpriteFont >(new SpriteFont( m_System->Renderer->GetDevice(), UTF8ToWChar (corrected_path.c_str() ) ));
    srand( time (NULL) );
}
#endif

#if WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
void XDGame::Update(const double delta_time, ID3D11Device1* device, _In_ ID3D11DeviceContext1* context, ID3D11RenderTargetView* renderTargetView) {
    m_System->Renderer->Update(device,context,renderTargetView);
    Update(delta_time);
    Render();
}
#endif

void XDGame::Update(const double delta_time) {
#ifndef TEST_MODE
    GameState::SUBSTATE ret = ( GameState::SUBSTATE ) m_CurrentState->Update(delta_time);
    if( GameState::SUBSTATE::STATEOUT == ret ) {
        GameState::STATE newstate = m_CurrentState->Next();
        if( newstate == GameState::STATE::EXIT ) {
#if WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP
            //clean something here
            g_exit_app = 1;
#endif
        } else {
            m_CurrentState = std::unique_ptr < GameState > ( CreateState::NewState ( newstate ,  m_System ) );
            m_CurrentState->Initialize();
        }
    }
    m_System->Renderer->Update(delta_time);
#else
    m_System->Renderer->UpdateTestMode(delta_time);
#endif
}

void XDGame::Render() {
#ifndef TEST_MODE
    m_System->Renderer->ClearScene();
    m_System->Renderer->Begin();
    m_CurrentState->Render();
    //	char cg_fps_measure[32];
    //sprintf(cg_fps_measure,"fps: %2.f",g_fps_measure);
    //wchar_t*  fp = UTF8ToWChar(cg_fps_measure);
    //m_TestFont->DrawString( m_Renderer->SpriteBatchs[ XDGameRenderer::EBATCH::L1 ] , fp , XMFLOAT2(10, 10), Colors::Red );
    //delete  fp;
    ////ParticleSystem::Instance()->Render();
    m_System->Renderer->End();
    m_CurrentState->RenderDebug();
#if WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP
    m_System->Renderer->Present();
#endif
#else
    m_Renderer->RenderTestMode();
#endif
}

void XDGame::ShutDown()	{
    // ParticleSystem::Destroy();
}

void XDGame::OnSuspending()	{
    m_CurrentState->OnSuspending();
}

void XDGame::OnResuming()	{
    m_CurrentState->OnResuming( m_System );
}

void XDGame::OnHandleInput( const SINPUT& input )	{
    m_CurrentState->OnHandleInput( input );
}