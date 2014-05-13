/*
*  (c) 2013-2014 XediXermawan < edi.ermawan@gmail.com >
*/
#include "pch.h"
#include "XDStateMenu.h"
#include "XDGUIManager.h"
#include <iostream>
#include "XDHexagonTile.h"
#include "XDFuncUtils.h"

StateMenu::StateMenu(XDSystem* xdsystem) : GameState(xdsystem)  {
    m_NextState = STATE::EXIT;
}

void StateMenu::Initialize() {
    m_SubState = SUBSTATE::MAIN;
    m_SubState_Play = PlayState::INITPLAYER;
    std::function<void(void* sender)> _buttonPlayCallback  = std::bind(&StateMenu::ButtonPlayCB, this, std::placeholders::_1);
    std::function<void(void* sender)> _buttonOptCallback   = std::bind(&StateMenu::ButtonOptionCB, this, std::placeholders::_1);
    std::function<void(void* sender)> _buttonExitCallback  = std::bind(&StateMenu::ButtonExitCB, this, std::placeholders::_1);
    std::function<void(void* sender)> _buttonInfoCallback  = std::bind(&StateMenu::ButtonInfoCB, this, std::placeholders::_1);
    std::function<void(void* sender)> _buttonSoundCallback = std::bind(&StateMenu::ButtonSoundCB, this, std::placeholders::_1);
// std::string tmp_str;
// this->m_XDSystem->FileSystem->ResolveToROFolder( tmp_str,"test_gui_01.ep");
// GUI.Load(tmp_str);
// GUI.RegisterCallback( "Play", _buttonPlayCallback );
// GUI.RegisterCallback( "Option", _buttonOptCallback );
// GUI.RegisterCallback( "Exit", _buttonExitCallback );
    std::string corrected_path="";
    m_XDSystem->FileSystem->ResolveToROFolder(corrected_path, "fonts\\AB_18.spritefont" );
    assert ( ( m_XDSystem->FileSystem->FileExist ( corrected_path.c_str() ) == true ) );
    m_SprFont = std::unique_ptr < SpriteFont >(new SpriteFont( m_XDSystem->Renderer->GetDevice(), UTF8ToWChar (corrected_path.c_str() ) ));
    //
    m_SprBg        =  m_XDSystem->Resource->GetSprite( "backgmain.xml" );
    m_SprBg->SetSpriteBatch ( m_XDSystem->Renderer->SpriteBatchs[ XDGameRenderer::EBATCH::L0 ] );
    //
    m_SprGames     =  m_XDSystem->Resource->GetSprite( "proximity.xml" );
    m_SprGames->SetSpriteBatch ( m_XDSystem->Renderer->SpriteBatchs[ XDGameRenderer::EBATCH::L0 ] );
    //
    m_AnimBg           = m_XDSystem->Resource->GetAnim( "background.anim" );
    m_AnimBg->SetSprite( m_SprBg );
    m_AnimClick = m_XDSystem->Resource->GetAnim( "m_AnimClick.anim" );
    m_AnimClick->SetSprite( m_SprGames );
    m_AnimGreenPop = m_XDSystem->Resource->GetAnim( "m_AnimGreenPop.anim" );
    m_AnimGreenPop->SetSprite( m_SprGames );
    m_AnimBluePop = m_XDSystem->Resource->GetAnim( "m_AnimBluePop.anim" );
    m_AnimBluePop->SetSprite( m_SprGames );
    m_AnimGreenGear = m_XDSystem->Resource->GetAnim( "m_AnimGreenGear.anim" );
    m_AnimGreenGear->SetSprite( m_SprGames );
    m_AnimBlueGear = m_XDSystem->Resource->GetAnim( "m_AnimBlueGear.anim" );
    m_AnimBlueGear->SetSprite( m_SprGames );
    m_AnimClick->SetPos( 100, 100 );
    m_AnimClick->SetAnim( 9 );
    m_AnimClick->SetAnimLoop(XDAnimObject::ANIMLOOP::ONCE);
    m_AnimClick->ShowAfterFinish( false );
    m_AnimGreenPop->SetPos( 720, 50 );
    m_AnimGreenPop->SetAnim( 7 );
    m_AnimGreenPop->SetAnimLoop(XDAnimObject::ANIMLOOP::ONCE);
    m_AnimGreenPop->ShowAfterFinish( true );
    m_AnimGreenPop->SetFinishFrame( 4 );
    m_AnimBluePop->SetPos( 720, 380 );
    m_AnimBluePop->SetAnim( 8 );
    m_AnimBluePop->SetAnimLoop(XDAnimObject::ANIMLOOP::ONCE);
    m_AnimBluePop->ShowAfterFinish( true );
    m_AnimBluePop->SetFinishFrame( 5 );
    //
    m_Swidth       = 480;
    m_Sheight      = 800;
    m_HexaWidth    = 64;
    m_HexaHeight   = 64;
    m_HexaRowCount = 7;
    m_HexaColCount = 14;
    m_StartPointX  = 5;
    m_StartPointY  = 4;//( m_Swidth - ( m_HexaHeight*m_HexaRowCount ) ) / 2 ;
    m_PlayerTurn   = PlayerType::A;
}

int StateMenu::Update(const double delta_time) {
    m_AnimBg->Update( delta_time );
    m_AnimClick->Update( delta_time );
    m_AnimGreenPop->Update( delta_time );
    m_AnimBluePop->Update( delta_time );
    gui::GUIManager::GetInstance().Update(delta_time);
    // __asm int 9;
    switch( m_SubState ) {
    case SUBSTATE::IGM :
        // update IGM
        break;
    case SUBSTATE::MAIN :
        UpdatePlay( delta_time );
        break;
    }
    return m_SubState;
}

void StateMenu::UpdatePlay( const double delta_time )  {
    switch( m_SubState_Play ) {
    case PlayState::INITPLAYER : {
        m_PlayMode       = PLAYMODE::H_VS_C;
        InitHexagonTile();
        m_TempNumberDraw = nullptr;
        GenerateNumbers();
        m_Player_A.Color = HEXACOLOR::PLAYER_A;
        m_Player_B.Color = HEXACOLOR::PLAYER_B;
        m_SubState_Play  = PlayState::WAITPLAYER;
    }
    break;
    case PlayState::WAITPLAYER: {
        int ret[7];		// 1 pointed tile, 6 neighbor
        UpdateTiles( delta_time , ret );
        if( m_PlayMode  == PLAYMODE::H_VS_C ) {
            if( m_PlayerTurn == PlayerType::B ) {
                steady_clock::time_point _time_now = steady_clock::now();
                double _delta_time_seconds = ( duration_cast<milliseconds>(_time_now - m_TimePoint01).count() ) ;
                if( 2000 < _delta_time_seconds) {
                    //
                    // get answer from AI
                    Player* playernow;
                    if( m_PlayerTurn == PlayerType::B ) {
                        playernow = &m_Player_B;
                    } else {
                        playernow = &m_Player_A;
                    }
                    playernow->NumberIndex = playernow->NumberIndex + 1;
                    m_AI_Answer = SolveTargetTile( playernow->GetNextNumber() );
                    m_SubState_Play = PlayState::EXECUTE;
                }
            }
            //
        } else if( m_PlayMode  == PLAYMODE::H_VS_N ) {
            // TODO:
            // get answer from the-thing in network
            // if answer is available & valid :
            // --m_SubState_Play = PlayState::EXECUTE;
            //
        }  else if( m_PlayMode  == PLAYMODE::C_VS_C ) {
            // TODO:
            // get answer from AI
            //
        }
    }
    break;
    case PlayState::EXECUTE: {
        int ret[7];		// 1 pointed tile, 6 neighbor
        UpdateTiles( delta_time , ret );
        if( ret[0] != -1 ) {
            if( m_HexTiles[ ret[0] ]->Color == HEXACOLOR::NETRAL || m_HexTiles[ ret[0] ]->Color == HEXACOLOR::HOVER ) {
                Player* playernow;
                if( m_PlayerTurn == PlayerType::B ) {
                    playernow = &m_Player_B;
                } else {
                    m_Player_A.NumberIndex = m_Player_A.NumberIndex+1;
                    playernow = &m_Player_A;
                }
                // pointed tile
                m_HexTiles[ ret[0] ]->Color  = playernow->Color;
                m_HexTiles[ ret[0] ]->Number = playernow->GetNextNumber();
                // neighbor
                for(unsigned int i=1; i<7; i++) {
                    if( ret[i] >=0 && ret[i]<(m_HexaColCount * m_HexaRowCount) ) {
                        if( m_HexTiles[ ret[0] ]->Number > m_HexTiles[ ret[i] ]->Number ) {
                            if( !(m_HexTiles[ ret[i] ]->Color == HEXACOLOR::NETRAL || m_HexTiles[ ret[i] ]->Color == HEXACOLOR::HOVER) )
                                m_HexTiles[ ret[i] ]->Color = playernow->Color;
                        }
                    }
                }
                // check if game finished
                // todo :
                // change turn
                if( m_PlayerTurn == PlayerType::B ) {
                    m_PlayerTurn = PlayerType::A;
                    m_AnimGreenPop->SetAnimLoop(XDAnimObject::ANIMLOOP::ONCE);
                } else {
                    m_PlayerTurn  = PlayerType::B;
                    m_AnimBluePop->SetAnimLoop(XDAnimObject::ANIMLOOP::ONCE);
                    m_TimePoint01 = steady_clock::now();
                }
            }
        }
        m_SubState_Play = PlayState::WAITPLAYER;
    }
    break;
    case PlayState::GAMEOVER: {
    }
    break;
    }
    unsigned int index =0;
    for(int j=0; j<m_HexaColCount; j++) {
        for(int i=0; i<m_HexaRowCount; i++) {
            m_HexTiles[ index++ ]->Update( delta_time );
        }
    }
}

void StateMenu::UpdateTiles( const double delta_time, int ret[] ) {
    // check pointer
    bool areYouOdd=false;
    unsigned int index =0;
    unsigned int indexPointed = -1;
    for(int j=0; j<m_HexaColCount; j++) {
        for(int i=0; i<m_HexaRowCount; i++) {
            int px = 0;
            int py = 0;
            if( areYouOdd ) {
                px =  m_StartPointX+(i*(m_HexaWidth+32))+48;
                py =  m_StartPointY+(j*(m_HexaHeight/2 -1));
            } else {
                px = m_StartPointX+(i*(m_HexaWidth+32)) ;
                py = m_StartPointY+(j*(m_HexaHeight/2 -1 ));
            }
            ChangeOnHover( index , HEXACOLOR::NETRAL);
            if( ( m_PointerX > px ) && ( m_PointerX < (px + m_HexaWidth) ) &&
                    ( m_PointerY > py ) && ( m_PointerY < (py + m_HexaHeight)) ) {
                if( indexPointed == -1 )
                    indexPointed = index;
                else {
                    ChangeOnHover( index , HEXACOLOR::NETRAL);
                }
            }
            index++;
        }
        areYouOdd = !areYouOdd;
    }
    // if vs computer
    if( m_PlayMode  == PLAYMODE::H_VS_C ) {
        if( m_PlayerTurn == PlayerType::B ) {
            // discard indexPointed calcualtion above, use m_AI_Answer
            indexPointed = m_AI_Answer;
        }
    }
    if( indexPointed < (m_HexaColCount * m_HexaRowCount) ) {
        ChangeOnHover( indexPointed );
        ret[0] = indexPointed;
        // get neighbor
        int neighbor_[6];
        GetNeighbor( indexPointed, neighbor_ );
        for(int i=0; i<6; i++) {
            ret[ i+1 ] = neighbor_ [i];
            if( neighbor_[i]>=0 && neighbor_[i]<(m_HexaColCount * m_HexaRowCount) ) {
                ChangeOnHover( neighbor_[i] , HEXACOLOR::NETRAL);
            }
        }
    } else {
        cout<<"indexPointed FALSE  "<<endl;
        ret[0] = -1;
    }
}

void StateMenu::ChangeOnHover(unsigned int index, unsigned int color)  {
    if( !((m_HexTiles[ index ]->Color == HEXACOLOR::PLAYER_A) || ( m_HexTiles[ index ]->Color == HEXACOLOR::PLAYER_B)) )
        m_HexTiles[ index ]->Color = color;
}

void StateMenu::GetNeighbor(unsigned int indexPointed,int neighbor[] ) {
    int row_ =m_HexTiles[ indexPointed ]->Index / m_HexaRowCount;
    if( row_%2 ) {
        neighbor[2]=indexPointed-m_HexaRowCount;
        neighbor[4]=indexPointed+m_HexaRowCount;
    } else {
        neighbor[2]=indexPointed-m_HexaRowCount-1;
        neighbor[4]=indexPointed+m_HexaRowCount-1;
    }
    neighbor[3]=neighbor[2]+1;
    neighbor[5]=neighbor[4]+1;
    if((row_%2!=0)  && (m_HexTiles[ indexPointed ]->Index % m_HexaRowCount==m_HexaRowCount-1)) {
        neighbor[3]=-1;
        neighbor[5]=-1;
    }
    if((row_%2==0)  && (m_HexTiles[ indexPointed ]->Index % m_HexaRowCount==0)) {
        neighbor[2]=-1;
        neighbor[4]=-1;
    }
    neighbor[0]=indexPointed-(m_HexaColCount);
    neighbor[1]=indexPointed+(m_HexaColCount);
}

void StateMenu::InitHexagonTile() {
    unsigned int index =0;
    for(int j=0; j<m_HexaColCount; j++) {
        for(int i=0; i<m_HexaRowCount; i++) {
            XDHexagonTile* hexa = new XDHexagonTile( m_SprGames );
            hexa->Color = HEXACOLOR::NETRAL;
            hexa->Index = index;
            hexa->Number = index++;
            m_HexTiles.push_back( hexa );
        }
    }
}

void StateMenu::Render() {
    m_AnimBg->Render();
    switch( m_SubState ) {
    case SUBSTATE::IGM :
        RenderPlay();
        // render IGM
        break;
    case SUBSTATE::MAIN :
        RenderPlay();
        break;
    }
    gui::GUIManager::GetInstance().Render();
    m_AnimBluePop->Render();
    m_AnimGreenPop->Render();
    m_AnimClick->Render();
}

void StateMenu::RenderPlay()	{
    switch( m_SubState_Play ) {
    case PlayState::INITPLAYER : {
        return;
    }
    break;
    case PlayState::WAITPLAYER: {
        RenderTiles();
    }
    break;
    case PlayState::EXECUTE: {
        RenderTiles();
    }
    break;
    case PlayState::GAMEOVER: {
    }
    break;
    }
    // draw player next number
    Player* playernow;
    if( m_PlayerTurn == PlayerType::B ) {
        playernow = &m_Player_B; // blue
        unsigned int nnumb = playernow->Numbers[ playernow->NumberIndex+1 ];
        char temp[8];
        itoa( nnumb , temp, 10);
        wchar_t* tempwch = UTF8ToWChar( temp );
        if( nnumb < 10 )
            m_SprFont->DrawString( m_XDSystem->Renderer->SpriteBatchs[ XDGameRenderer::EBATCH::L1 ] , tempwch, XMFLOAT2( 731, 384 ), Colors::OrangeRed, 0 );
        else
            m_SprFont->DrawString( m_XDSystem->Renderer->SpriteBatchs[ XDGameRenderer::EBATCH::L1 ] , tempwch, XMFLOAT2( 724, 384 ), Colors::OrangeRed, 0 );
        delete[] tempwch;
    } else {
        playernow = &m_Player_A;
        unsigned int nnumb = playernow->Numbers[ playernow->NumberIndex+1 ];
        char temp[8];
        itoa( nnumb , temp, 10);
        wchar_t* tempwch = UTF8ToWChar( temp );
        if( nnumb < 10 )
            m_SprFont->DrawString( m_XDSystem->Renderer->SpriteBatchs[ XDGameRenderer::EBATCH::L1 ] , tempwch, XMFLOAT2( 731, 54 ), Colors::OrangeRed, 0 );
        else
            m_SprFont->DrawString( m_XDSystem->Renderer->SpriteBatchs[ XDGameRenderer::EBATCH::L1 ] , tempwch, XMFLOAT2( 724, 54 ), Colors::OrangeRed, 0 );
        delete[] tempwch;
    }
}


void StateMenu::RenderDebug()	{
    //m_World->RenderDebug();
}

void StateMenu::OnSuspending() {
}

void StateMenu::OnResuming(XDSystem* xdsystem) {
    //m_Renderer  = renderer;
    //m_SprMenuText->SetSpriteBatch ( m_Renderer->SpriteBatchs[ XDGameRenderer::EBATCH::L0 ] );
    //m_SprMenuText->LoadTexture( m_Renderer->GetDevice() );
    //m_World->OnResuming ( m_SprMenuText.get() , m_Renderer );
}

void StateMenu::OnHandleInput(  const SINPUT& input ) {
    switch (input.pflag) {
    case FINPUT::P_MOVE :
        gui::GUIManager::GetInstance().CheckIntersect( input.pposx, input.pposy );
        m_PointerX = input.pposx;
        m_PointerY = input.pposy;
        break;
    case FINPUT::P_PRESSED :
        gui::GUIManager::GetInstance().Clicked( input.pposx, input.pposy );
        m_PointerX = input.pposx;
        m_PointerY = input.pposy;
        break;
    case FINPUT::P_RELEASED :
        if( m_PlayMode == PLAYMODE::H_VS_H )	{
            m_SubState_Play = PlayState::EXECUTE;
        } else {
            if( m_PlayerTurn == PlayerType::A) {
                m_SubState_Play = PlayState::EXECUTE;
            }
        }
        m_PointerX = input.pposx;
        m_PointerY = input.pposy;
        m_AnimClick->SetPos( m_PointerX, m_PointerY );
        m_AnimClick->SetAnimLoop(XDAnimObject::ANIMLOOP::ONCE);
        m_AnimClick->ShowAfterFinish( false );
        break;
    case FINPUT::KEY_DOWN :
#define ESC_KEY 27
        //if( input.pid == ESC_KEY ) {
        //	if( m_SubState == SUBSTATE::MAIN ) {
        //		m_ButtonPlay->SetVisible( 1 );
        //		m_ButtonOpt->SetVisible( 1 );
        //		m_ButtonExit->SetVisible( 1 );
        //		m_ButtonInfo->SetVisible( 1 );
        //		m_ButtonSound->SetVisible( 1 );
        //		m_World->OnHideIGM();
        //		m_SubState = SUBSTATE::IGM;
        //	}
        //}
        break;
    default:
        break;
    }
}

StateMenu::~StateMenu() {
}

GameState::STATE StateMenu::Next() {
    return m_NextState;
}

void StateMenu::ButtonPlayCB(void* sender)	{
    ////m_ButtonPlay->SetVisible( 0 );
    //m_ButtonOpt->SetVisible( 0 );
    //m_ButtonExit->SetVisible( 0 );
    //m_ButtonInfo->SetVisible( 0 );
    //m_ButtonSound->SetVisible( 0 );
    //m_SubState = SUBSTATE::MAIN;
    //m_World->OnShowIGM();
}

void StateMenu::ButtonOptionCB(void* sender)	{
    /*m_ButtonPlay->SetVisible( 0 );
    m_ButtonOpt->SetVisible( 0 );
    m_ButtonExit->SetVisible( 0 );
    m_ButtonInfo->SetVisible( 0 );
    m_ButtonSound->SetVisible( 0 );
    m_World->NewGame( m_SprMenuText.get() );
    m_SubState = SUBSTATE::MAIN;*/
}

void StateMenu::ButtonExitCB(void* sender)	{
    m_SubState = SUBSTATE::STATEOUT;
}

void StateMenu::ButtonInfoCB(void* sender)	{
    //m_SubState = SUBSTATE::STATEOUT;
}

void StateMenu::ButtonSoundCB(void* sender)	{
    //m_SubState = SUBSTATE::STATEOUT;
}

void StateMenu::GenerateNumbers() {
    unsigned int total_numb = (m_HexaColCount*m_HexaRowCount)/2 + 5;
    std::vector<int> numberPA;
    std::vector<int> numberPB;
    // generate
    for(int j=0; j<total_numb; j++) {
        unsigned int numbvalue = ( j % 12) + 1;
        m_Player_A.Numbers.push_back( numbvalue );
        m_Player_B.Numbers.push_back( numbvalue );
    }
    // assign
    for(unsigned int a=0; a<1000; a++) {
        for(unsigned int k=0; k<total_numb; k++) {
            // A
            int b = RandomRange(0,total_numb);
            int temp = m_Player_A.Numbers[ b ];
            m_Player_A.Numbers[ b ] = m_Player_A.Numbers[ k ];
            m_Player_A.Numbers[ k ] = temp;
            // B
            int c = RandomRange(0,total_numb);
            int temp2 = m_Player_B.Numbers[ c ];
            m_Player_B.Numbers[ c ] = m_Player_B.Numbers[ k ];
            m_Player_B.Numbers[ k ] = temp2;
        }
    }
#if 1
    for(int j=0; j<total_numb; j++) {
        std::cout<<" Number ["<<j<<"]:  "<< m_Player_A.Numbers[j] <<" --  "<< m_Player_B.Numbers[j] <<std::endl;
    }
#endif
}

void StateMenu::RenderTiles() {
    bool isYouOdd=false;
    unsigned int index =0;
    for(int j=0; j<m_HexaColCount; j++) {
        for(int i=0; i<m_HexaRowCount; i++) {
            if( isYouOdd ) {
                m_HexTiles[ index ]->SetPos( m_StartPointX+(i*(m_HexaWidth+32))+48,m_StartPointY+(j*(m_HexaHeight/2 -1 ))  );
            } else {
                m_HexTiles[ index ]->SetPos( m_StartPointX+(i*(m_HexaWidth+32)),m_StartPointY+(j*(m_HexaHeight/2-1) )  );
            }
            m_HexTiles[ index ]->SetAnim( m_HexTiles[ index ]->Color );
            m_HexTiles[ index ]->Render();
            if( !(m_HexTiles[ index ]->Color == HEXACOLOR::NETRAL || m_HexTiles[ index ]->Color == HEXACOLOR::HOVER) ) {
                // little centering trick
                int FONT_OFFSET_X = 13;
                int FONT_OFFSET_Y = 15;
                if( m_HexTiles[ index ]->Number < 10  ) {
                    FONT_OFFSET_X = 19;
                    FONT_OFFSET_Y = 15;
                    if(  m_HexTiles[ index ]->Number ==1 ) {
                        FONT_OFFSET_X = 19;
                        FONT_OFFSET_Y = 15;
                    }
                }
                //
                int _px,_py;
                m_HexTiles[ index ]->GetPos( _px, _py );
                char temp[10];
                itoa( m_HexTiles[ index ]->Number, temp, 10 );
                if( m_TempNumberDraw )
                    delete m_TempNumberDraw;
                m_TempNumberDraw = UTF8ToWChar( temp );
                m_SprFont->DrawString( m_XDSystem->Renderer->SpriteBatchs[ XDGameRenderer::EBATCH::L1 ] , m_TempNumberDraw ,
                                       XMFLOAT2( _px+FONT_OFFSET_X, _py+FONT_OFFSET_Y ), Colors::OrangeRed, 0 );
            }
            index++;
        }
        isYouOdd = !isYouOdd;
    }
}

unsigned int StateMenu::SolveTargetTile(unsigned int newNumb) {
    int neighbor_this_tile[6];
    int defenseWeight=0;
    int offenseWeight=0;
    int maxDecweight=-1;
    int resultTile=0;
    std::vector<XDHexagonTile*>::iterator iter_;
    Player* playernow;
    if( m_PlayerTurn == PlayerType::B ) {
        playernow = &m_Player_B;
    } else {
        playernow = &m_Player_A;
    }
    for(iter_=m_HexTiles.begin(); iter_!=m_HexTiles.end(); iter_++) {
        int index_this_tile=(*iter_)->Index;
        neighbor_this_tile[0]=-1;
        neighbor_this_tile[1]=-1;
        neighbor_this_tile[2]=-1;
        neighbor_this_tile[3]=-1;
        neighbor_this_tile[4]=-1;
        neighbor_this_tile[5]=-1;
        GetNeighbor(index_this_tile,neighbor_this_tile);
        int numbToCompare=newNumb;
        int colToCompare=m_HexTiles.at(index_this_tile)->Color;
        int decweight=0;
        for(int n=0; n<6; n++) {
            if( neighbor_this_tile[n]>=0 && neighbor_this_tile[n]<(m_HexaColCount * m_HexaRowCount)  ) {
                int numb=m_HexTiles.at(neighbor_this_tile[n])->Number;
                int col=m_HexTiles.at(neighbor_this_tile[n])->Color;
                if(numb<numbToCompare) {
                    int divider_=numbToCompare-numb;
                    if( col==HEXACOLOR::NETRAL )
                        decweight+=4;
                    else if(col!=playernow->Color)
                        decweight+=30+30/(divider_);
                    else if(col==playernow->Color)
                        decweight+=2;
                } else if(numb>=numbToCompare) {
                    if(col!=playernow->Color)
                        decweight+=0;
                    else if(col==playernow->Color)
                        decweight+=20;
                }
            }///end for
            if(maxDecweight<decweight) {
                if(colToCompare==HEXACOLOR::NETRAL) {
                    maxDecweight=decweight;
                    resultTile=index_this_tile;
                }
            }
        }
    }
    cout<<"[ computer ] Number Now : "<<newNumb<<endl;
    return resultTile;
}