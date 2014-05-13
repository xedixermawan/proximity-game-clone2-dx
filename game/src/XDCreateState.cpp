/*
*  (c) 2013-2014 XediXermawan < edi.ermawan@gmail.com >
*/
#include "pch.h"
#include "XDCreateState.h"
#include "XDStateLoading.h"
#include "XDStateMenu.h"

GameState* CreateState::NewState(GameState::STATE state,  XDSystem* xdsystem ) {
    switch (state) {
    case GameState::STATE::LOADING:
        return ( new StateLoading( xdsystem ) );
        break;
    case GameState::STATE::MENU:
        return ( new StateMenu( xdsystem ) );
        break;
    default:
        return nullptr;
        break;
    }
}
