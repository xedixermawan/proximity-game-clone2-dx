/*
*  (c) 2013-2014 XediXermawan < edi.ermawan@gmail.com >
*/
#ifndef __STATE_MENU_H__
#define __STATE_MENU_H__

#include "XDGameState.h"
#include "XDFiveTeenBox.h"
#include "XDButton.h"
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::steady_clock;
class XDHexagonTile;

enum PlayerState {
    INIT,
    WAITING,
    EXECUTING,
    THINKING
};

enum PlayState  {
    NONE,
    INITPLAYER,
    WAITPLAYER,
    EXECUTE,
    GAMEOVER
};

enum PlayerType  {
    A,
    B
};

namespace HEXACOLOR {
const unsigned int NETRAL   = 3;
const unsigned int HOVER    = 4;
const unsigned int PLAYER_A = 5;
const unsigned int PLAYER_B = 6;
};

enum PLAYMODE {
    H_VS_H,		// human vs human in 1 computer
    H_VS_C,		// human vs computer AI
    C_VS_C,		// computer AI vs computer AI
    H_VS_N		// human vs the-thing from network (LAN/bluetooth/internet)
};

class Player {
  public:
    Player() {
        Reset();
    };
    void Reset() {
        State = PlayerState::INIT;
        NumberIndex = 0;
        NumberCollected = 0;
        Color = HEXACOLOR::NETRAL;
        Numbers.clear();
    };
    unsigned int GetNextNumber() {
        unsigned int i = Numbers[ NumberIndex ];
        return i;
    }
    std::vector< unsigned int > Numbers;
    PlayerState State;
    unsigned int NumberCollected;
    unsigned int Color;
    unsigned int NumberIndex;
};

class StateMenu : public GameState {
  public:
    StateMenu(XDSystem* xdsystem);
    void Initialize();
    int Update(const double delta_time);
    void Render();
    void RenderDebug();
    void OnSuspending();
    void OnResuming(XDSystem* xdsystem);
    void OnHandleInput(  const SINPUT& input );
    GameState::STATE Next();
    ~StateMenu();
  private:
    XDSprite*  m_SprGames;
    XDSprite*  m_SprBg;
    XDAnimObject*  m_AnimBg;
    XDAnimObject*  m_AnimClick;
    XDAnimObject*  m_AnimGreenPop;
    XDAnimObject*  m_AnimBluePop;
    XDAnimObject*  m_AnimGreenGear;
    XDAnimObject*  m_AnimBlueGear;
    std::shared_ptr< XDAnimObject >  m_AnimTextOption;
    std::shared_ptr< XDAnimObject >  m_AnimTextExit;
    std::unique_ptr<SpriteFont> m_SprFont;
    wchar_t* m_TempNumberDraw;
    XMFLOAT2 m_StartPoint;
    XMFLOAT2 m_WHCell;
    XMFLOAT2 m_WHBoard;

    PhysicTest* m_World;
    Player m_Player_A;
    Player m_Player_B;
    std::shared_ptr < gui::Button > m_ButtonPlay;
    std::shared_ptr < gui::Button > m_ButtonOpt;
    std::shared_ptr < gui::Button > m_ButtonExit;
    std::shared_ptr < gui::Button > m_ButtonInfo;
    std::shared_ptr < gui::Button > m_ButtonSound;

    std::vector < XDHexagonTile* > m_HexTiles;
    int m_Swidth;
    int m_Sheight;
    int m_HexaWidth;
    int m_HexaHeight;
    int m_HexaRowCount;
    int m_HexaColCount;
    int m_StartPointX;
    int m_StartPointY;
    int m_PointerX;
    int m_PointerY;
    PlayState m_SubState_Play;
    PlayerType m_PlayerTurn;
    PLAYMODE m_PlayMode;
    unsigned int m_AI_Answer;
    steady_clock::time_point m_TimePoint01;
  private:
    void ButtonPlayCB(void* sender);
    void ButtonOptionCB(void* sender);
    void ButtonExitCB(void* sender);
    void ButtonInfoCB(void* sender);
    void ButtonSoundCB(void* sender);
    void InitHexagonTile();
    void GetNeighbor(unsigned int indexPointed,int neighbor[] );
    void GenerateNumbers();
    void RenderPlay();
    void UpdatePlay(const double delta_time);
    void RenderTiles();
    void UpdateTiles( const double delta_time, int ret[] );
    void ChangeOnHover(unsigned int index, unsigned int color = HEXACOLOR::HOVER  );
    unsigned int SolveTargetTile(unsigned int newNumb);


};
#endif  // __STATE_MENU_H__