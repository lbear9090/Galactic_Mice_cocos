//
//  GameOverScene.h
//  GalacticMice
//
//  Created by Lucky
//
//

#ifndef GameOverScene_h
#define GameOverScene_h

#include "cocos2d.h"

using namespace cocos2d;
class GameOverScene : public Layer{
    
public:
    virtual bool init();
    CREATE_FUNC(GameOverScene);
    static Scene* createScene();
    ~GameOverScene();

private:    
    void createBg();
    void createLabel();
    void createBtn();
    void onMenu(Ref *sender);
    void gotoMain(Ref *sender);
private:
    Menu *m_mMain;
    Sprite *m_sStars[2];
    void onTimer(float dt);
    int m_nCnt;
};

#endif /* GameOverScene_h */
