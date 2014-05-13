/*
*  (c) 2013-2014 XediXermawan < edi.ermawan@gmail.com >
*/
#ifndef __CREATE_STATE_H__
#define __CREATE_STATE_H__

#include "XDGameState.h"

class XDSystem;

class CreateState {
  public:
    static GameState* NewState(GameState::STATE state, XDSystem* xdsystem);
    ~CreateState();
    CreateState() {};
};

#endif // __CREATE_STATE_H__