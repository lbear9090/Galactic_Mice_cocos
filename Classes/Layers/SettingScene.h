//
//  SettingScene.h
//  GalacticMice
//
//  Created by Lucky
//
//

#ifndef SettingScene_h
#define SettingScene_h

#include "cocos2d.h"

using namespace cocos2d;
class SettingScene : public Layer{
    
public:
    virtual bool init();
    CREATE_FUNC(SettingScene);
    static Scene* createScene();
    ~SettingScene();

private:    
    void createBg();
    void createLabel();
    void createBtn();
    void onMenu(Ref *sender);
    void refreshTrophy();
    void gotoMain(Ref *sender);
private:
    Menu *m_mMain;
    Sprite *m_sStars[2];
    Node *m_sTrophy[4];
    void onTimer(float dt);
};

#endif /* SettingScene_h */
