//
//  MainScene.h
//  GalacticMice
//
//  Created by Lucky
//
//

#ifndef MainScene_h
#define MainScene_h

#include "cocos2d.h"

using namespace cocos2d;
class MainScene : public Layer{
    
public:
    virtual bool init();
    CREATE_FUNC(MainScene);
    static Scene* createScene();
    ~MainScene();

private:    
    void createBg();
    void createLabel();
    void createBtn();
    void onMenu(Ref *sender);
    void createBanner();
    void update(float delta);
private:
    Menu *m_mMain;
    Sprite *m_sStars[2];
    void onTimer(float dt);
    int m_nCnt;
};

#endif /* MainScene_h */
