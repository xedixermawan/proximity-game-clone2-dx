/*
*  (c) 2013-2014 XediXermawan < edi.ermawan@gmail.com >
*/
#include "pch.h"
#include "XDGUIManager.h"
#include "XDFuncUtils.h"
#include "XDButton.h"
#include <algorithm>
#include <iostream> // test only

#define GUI_CONSOLE_LOG

namespace gui {

GUIManager* GUIManager::m_Instance = nullptr;

GUIManager& GUIManager::GetInstance() {
    if(nullptr == m_Instance) {
        m_Instance = new GUIManager();
    }
    return *m_Instance;
}

void GUIManager::AddElement(const std::shared_ptr < ElementBase >& element) {
    m_Elements.push_back(element);
}

void GUIManager::RemoveElement(const std::shared_ptr < ElementBase >& element) {
    m_Elements.erase(std::remove(m_Elements.begin(), m_Elements.end(), element), m_Elements.end());
}

void GUIManager::CheckIntersect(const int& posx,const int& posy) {
    std::vector < std::shared_ptr < ElementBase > >::iterator it;
    for( it = m_Elements.begin(); it != m_Elements.end() ; it++ )	{
        if (1 == (*it)->GetVisible() )
            ( (*it)->OnHover(posx,posy) );
    }
}

void GUIManager::CheckClicked(const int& posx,const int& posy) {
}

void GUIManager::Clicked(const int& posx,const int& posy) {
    std::vector < std::shared_ptr < ElementBase > >::iterator it;
    for( it = m_Elements.begin(); it != m_Elements.end() ; it++ )	{
        if (1 == (*it)->GetVisible() )
            ( (*it)->OnClicked(posx,posy) );
    }
}

void GUIManager::Update(const double delta_time) {
    std::vector < std::shared_ptr < ElementBase > >::iterator it;
    for( it = m_Elements.begin(); it != m_Elements.end() ; it++ )	{
        if (1 == (*it)->GetVisible() )
            (*it)->Update(delta_time);
    }
}

void GUIManager::Render() {
    std::vector < std::shared_ptr < ElementBase > >::iterator it;
    for( it = m_Elements.begin(); it != m_Elements.end() ; it++ )	{
        if (1 == (*it)->GetVisible() )
            (*it)->Render();
    }
}

void GUIManager::HandleInput() {
}

void GUIManager::Free() {
}

void GUIManager::Clear()  {
    m_Elements.clear();
}

GUIManager::GUIManager() {
}

GUIManager::~GUIManager() {
}

void GUIManager::Load(const std::string& guifile)  {
    // if guifile is available -> don't load, just show
    std::shared_ptr<gui_file_s> newguifile ( new gui_file_s(guifile) );
    m_GUIFiles.push_back( newguifile  );
    TiXmlDocument doc(guifile.c_str());
    bool loadOkay = doc.LoadFile();
    assert(loadOkay);
    if (loadOkay) {
        TiXmlHandle docHandle( &doc );
        TiXmlNode* node = docHandle.FirstChild("Document").FirstChild("Properties").ToNode()->NextSibling();
        TiXmlNode* page = node->FirstChild("Page");
        while( page!= nullptr ) {
            // properties page
            TiXmlNode* page_props = page->FirstChild("Properties");
            TiXmlElement* page_prop_element = page_props->FirstChildElement("Property");
            std::string page_name  = page_prop_element->GetText();
            std::cout<< page_name.c_str() <<std::endl;
            std::shared_ptr<gui_page_s> newpage ( new gui_page_s( page_name ) );
            std::cout<<"  properties"<<std::endl;
            while( page_prop_element!= nullptr ) {
                std::string page_props_attr  = page_prop_element->Attribute("name");
                const char* cv  = page_prop_element->GetText();
                const char* cvl = page_prop_element->Value() ;
                if( cv!=0 ) {
                    if( !page_props_attr.compare("width") ) {
                        std::string val(cv);
                        StrToNumber( val, newpage->Width   );
                    } else if( !page_props_attr.compare("height") ) {
                        std::string val(cv);
                        StrToNumber( val, newpage->Height   );
                    }
                    std::cout<<"    "<< page_props_attr.c_str() <<" : "<<cv<<std::endl;
                    if( cvl!=0) {
                        std::cout<<"    "<< page_props_attr.c_str() <<" : "<<cvl<<std::endl;
                    }
                }
                page_prop_element = page_prop_element->NextSiblingElement();
            }
            // content
            TiXmlNode* page_content = page->FirstChild("Properties")->NextSibling();
            TiXmlNode* GrafElement = page_content->FirstChild("g");
            std::cout<<"  content"<<std::endl;
            while( GrafElement!=nullptr) {
                std::shared_ptr<ElementBase> newelement ( new Button() );
                newpage->ElementArray.push_back( newelement );
                // graf attribute
                std::string gf_type = GrafElement->ToElement()->Attribute("p:type");
                std::string gf_def  = GrafElement->ToElement()->Attribute("p:def");
                std::string gf_id   = GrafElement->ToElement()->Attribute("id");
                std::string gf_tr   = GrafElement->ToElement()->Attribute("transform");
                std::cout<<"    "<< gf_type.c_str() <<std::endl;
                std::cout<<"    "<< gf_def.c_str() <<std::endl;
                std::cout<<"    "<< gf_id.c_str() <<std::endl;
                std::cout<<"    "<< gf_tr.c_str() <<std::endl;
                std::cout<<"    "<<"--"<<std::endl;
                // parse transform (pos x,y)
                {
                    std::string px="";
                    std::string py="";
                    unsigned int i=0;
                    for(unsigned int n=0; n<gf_tr.size(); n++) {
                        if(i<4 && gf_tr[n]==',') {
                            i++;
                        } else if(i==4) {
                            if(gf_tr[n]!=',') {
                                px.push_back(gf_tr[n]);
                            } else {
                                i=5;
                            }
                        } else if(i==5) {
                            if(gf_tr[n]!=')') {
                                py.push_back(gf_tr[n]);
                            }
                        }
                    }
                    int _px,_py;
                    StrToNumber( px, _px  );
                    StrToNumber( py, _py   );
                    newelement->SetPos(_px,_py);
                }
                // graf childs
                TiXmlNode* GrafElement_metadata = GrafElement->FirstChild("p:metadata")->FirstChild("p:property");
                while( GrafElement_metadata!=nullptr) {
                    std::string gf_element_prop = GrafElement_metadata->ToElement()->Attribute("name");
                    std::cout<<"     "<< gf_element_prop.c_str() <<" : "<<GrafElement_metadata->ToElement()->GetText()<<std::endl;
                    if( !gf_element_prop.compare("box") ) {
                        std::string gf_element_propvalue = GrafElement_metadata->ToElement()->GetText();
                        //parse width,height
                        {
                            std::string pw="";
                            std::string ph="";
                            bool secondval=false;
                            for(unsigned int i=0; i<gf_element_propvalue.size(); i++) {
                                if(gf_element_propvalue[i]==',') {
                                    secondval=true;
                                    continue;
                                }
                                if(secondval)
                                    ph.push_back( gf_element_propvalue[i] );
                                else
                                    pw.push_back( gf_element_propvalue[i] );
                            }
                            int _pw,_ph;
                            StrToNumber( pw, _pw  );
                            StrToNumber( ph, _ph   );
                            newelement->SetWidthHeight(_pw,_ph);
                        }
                    } else if( !gf_element_prop.compare("textContent") ) {
                        // format: ButtonName_SpriteID_AnimIdleID_AnimHoverID_AnimClickedID
                        // Delimiter : '_'	  , endDelimiter = '|'
                        std::string gf_element_propvalue = GrafElement_metadata->ToElement()->GetText();
                        const unsigned int newelementInfosCount = 5;
                        unsigned int iter_ = 0;
                        std::string strTemp="";
                        std::string newelementInfos[newelementInfosCount];
                        for(unsigned int i=0; i<gf_element_propvalue.size(); i++) {
                            if( gf_element_propvalue[i] != '_' ) {
                                if( gf_element_propvalue[i] == '|' ) {
                                    newelementInfos[ iter_ ] = strTemp;
                                    break;
                                } else {
                                    strTemp.push_back( gf_element_propvalue[i] );
                                }
                            } else {
                                newelementInfos[ iter_++ ] = strTemp;
                                strTemp="";
                                assert( iter_ < newelementInfosCount );
                            }
                        }
#pragma message(" TODO: code below --> check if props gui element is correct")
                        if( true ) {
                            unsigned int spriteID;
                            unsigned int animIdleID;
                            unsigned int animHoverID;
                            unsigned int animClickedID;
                            StrToNumber( newelementInfos[ 1] , spriteID  );
                            StrToNumber( newelementInfos[ 2] , animIdleID   );
                            StrToNumber( newelementInfos[ 3] , animHoverID   );
                            StrToNumber( newelementInfos[ 4] , animClickedID   );
                            newelement->SetName( newelementInfos[0] );
                            newelement->SetAnimProps( spriteID, animIdleID, animHoverID, animClickedID );
                        }
                    }
                    GrafElement_metadata = GrafElement_metadata->NextSibling();
                }
                GrafElement = GrafElement->NextSibling();
            }
            // add new page to gui file
            newguifile->PageArray.push_back(  newpage );
            //next page
            page = page->NextSibling();
        } // page
    } // if
    m_Elements = m_GUIFiles[0]->PageArray[0]->ElementArray;
}// load

void GUIManager::RegisterCallback(const std::string& guiname, std::function< void(void* sender) >& function) {
#pragma message(" TODO : batching this callback registration ");
    GUIFileArray::iterator it_guifiles;
    GUIPageArray::iterator it_pages;
    GUIElementArray::iterator it_elements;
    for( it_guifiles = m_GUIFiles.begin(); it_guifiles != m_GUIFiles.end(); it_guifiles++ ) {
        GUIPageArray pagearray = (*it_guifiles)->PageArray;
        for(it_pages = pagearray.begin(); it_pages != pagearray.end(); it_pages++ ) {
            GUIElementArray elementsarray = (*it_pages)->ElementArray;
            for( it_elements = elementsarray.begin(); it_elements != elementsarray.end() ; it_elements++ ) {
                if( (*it_elements)->GetName().compare( guiname ) == 0 ) {
                    (*it_elements)->SetCallBack( function );
                }
            }
        }
    }
}

}; // end gui

