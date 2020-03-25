//
//  GameScene.h
//  GalacticMice
//
//  Created by Lucky
//
//

#ifndef GameScene_h
#define GameScene_h

#include "cocos2d.h"


using namespace cocos2d;
using namespace std;
class GameScene : public Layer{
    
public:
    virtual bool init();
    CREATE_FUNC(GameScene);
    static Scene* createScene();
    ~GameScene();

public:
    void setPause(bool isPause);
private:    
    void createBg();
    void createLabel();
    void createBtn();
    void onMenu(Ref *sender);
    
private:
    Menu *m_mMain;
    Sprite *m_sStars[2];
    Sprite *m_sPlayer;
    Sprite *m_sPlayerE;
    Sprite *m_sStun;
    Sprite *m_sTongue;
    Sprite *m_sGlasses;
    Sprite *m_sHelmet;
    Sprite *m_sJoystick, *m_sShootPad;
    
    Sprite *m_sLaser[2];
    
    Label *m_lScore;
    Label *m_lLives;
    Sprite *m_lStart;
    Label *m_lPause;
    Label *m_lLevel;
    Sprite *m_sPlanet;
    
    bool m_bTouched;
    bool m_bPaused;
    int m_nCnt;
    int m_nLives;
    int m_nScore;
    int m_nMiceCnt;
    int m_nTotalMiceCnt;
    int m_nLevel;
    
    int m_nTotalShot;
    int m_nTotalHit;
    
    bool m_bGlassBreakingAll;
    Vector<Sprite *> *m_aObjects;
    Vector<Sprite *> *m_aBullets;
    float m_fSpeedY;
    
    void onTimer(float dt);
    
    void setLife(int nLife);
    void stunPlayer();
    bool checkCollide(Sprite *s1, Sprite *s2);
    void removeObject(Sprite *obj);
    void spawnBullet(int nType);
    void showPoint(string sText, string sFont, Vec2 pos, float delay);
    void showGlass();
    void showTrophy(int nIndex);
    void checkLaser();
    
    void gotoGameOver(Ref *sender);
    virtual void onTouchesBegan(const std::vector<Touch*>& touches, Event *pEvent);
    virtual void onTouchesMoved(const std::vector<Touch*>& touches, Event *pEvent);
    virtual void onTouchesEnded(const std::vector<Touch*>& touches, Event *pEvent);
};

#endif /* GameScene_h */
