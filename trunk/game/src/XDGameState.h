/*
*  (c) 2013-2014 XediXermawan < edi.ermawan@gmail.com >
*/
#ifndef __GAME_STATE_H__
#define __GAME_STATE_H__

#include "XDGameRenderer.h"
#include "XDGlobalConst.h"
#include "XDFilesystem.h"
#include "XDResProxy.h"

class XDSystem {
  public:
    XDSystem() {
        Renderer   = new XDGameRenderer();
        FileSystem = new XDFileSystem();
        Resource   = new ResProxy( FileSystem );
    }
    ~XDSystem() {
        delete Renderer;
        delete FileSystem;
        delete Resource;
    };
    XDGameRenderer* Renderer;
    XDFileSystem* FileSystem;
    ResProxy* Resource;
};

class GameState {
  public:
    enum SUBSTATE {
        INIT = 0,
        MAIN = 1,
        DEINIT = 2,
        STATEOUT = 3,
        IGM = 4,
        STATS = 5
    };

    enum STATE {
        LOADING = 0,
        MENU = 1,
        GAMEPLAY = 2,
        EXIT = 3
    };

    GameState(XDSystem* xdsystem): m_XDSystem(xdsystem)  {
        m_SubState = SUBSTATE::INIT;
    };

    virtual void Initialize() = 0;
    virtual int Update(const double delta_time) = 0;
    virtual void Render() = 0;
    virtual void RenderDebug() = 0;
    virtual void OnSuspending() = 0;
    virtual void OnResuming(XDSystem* xdsystem) = 0;
    virtual void OnHandleInput( const SINPUT& input ) = 0;
    virtual STATE Next() = 0;
    virtual ~GameState() {
    };

  protected:
    SUBSTATE m_SubState;
    STATE m_NextState;
    XDSystem* m_XDSystem;


};

#endif