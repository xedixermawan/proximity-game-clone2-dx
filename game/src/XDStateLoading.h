/*
*  (c) 2013-2014 XediXermawan < edi.ermawan@gmail.com >
*/

#ifndef __STATE_LOADING_H__
#define __STATE_LOADING_H__

#include "XDGameState.h"
#include <chrono>

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::steady_clock;

class StateLoading : public GameState {
  public:
    StateLoading(XDSystem* xdsystem);
    void Initialize();
    int Update(const double delta_time);
    void Render();
    void RenderDebug();
    void OnSuspending();
    void OnResuming(XDSystem* xdsystem);
    void OnHandleInput(  const SINPUT& input );
    STATE Next();
    ~StateLoading();
  private:
    XDSprite*  m_SprLoading;
    XDAnimObject*  m_AnimLoading;
    steady_clock::time_point m_StateStartTime;
};
#endif  // __STATE_LOADING_H__