//
//  GameOverScene.cpp
//  GalacticMice
//
//  Created by Lucky
//
//

#include "GameOverScene.h"
#include "GameSettings.h"
#include "GameScene.h"
#include "MainScene.h"
#include "SettingScene.h"

Scene* GameOverScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameOverScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

GameOverScene::~GameOverScene(){
    removeAllChildren();
    stopAllActions();
    unscheduleAllCallbacks();
}

bool GameOverScene::init(){
    if ( !Layer::init() )
    {
        return false;
    }
    m_nCnt = 0;

    createBg();
    createLabel();
    createBtn();
    schedule(schedule_selector(GameOverScene::onTimer), 1.0f/30.0f);
    runAction(Sequence::create(DelayTime::create(10.0f), CallFuncN::create(CC_CALLBACK_1(GameOverScene::gotoMain, this)), NULL));
    return true;
}

void GameOverScene::createBg()
{
    newSprite("gameover", G_SWIDTH/2, G_SHEIGHT*3/4, this, 1, RATIO_XY);
    Sprite *sPlayer = newSprite("player", G_SWIDTH/2, getY(160), this, 2, RATIO_O);
    Sprite *sStun = newSprite("ani/st_0", 27, 55, sPlayer, 1, RATIO_1);
    
    sStun->runAction(RepeatForever::create(Animate::create(g_aniStun)));
    //sPlayer->runAction(RepeatForever::create(Sequence::create(FadeTo::create(0.5f, 128), FadeIn::create(0.5f), NULL)));
    
    for(int i = 0; i < 2; i++){
        m_sStars[i] = newSprite("stars", 0, G_SHEIGHT / 2, this, -1, RATIO_O);
        m_sStars[i]->setScale(2 * G_SCALEX);
        m_sStars[i]->setPosition(m_sStars[0]->getBoundingBox().size.width * i, G_SHEIGHT / 2);
    }
}

void GameOverScene::createLabel()
{
    string sScore = "SCORE: " + toString(g_nScore);
    if(g_nHighScore < g_nScore){
        g_nHighScore = g_nScore;
        saveGameInfo();
        sScore = "NEW HIGH SCORE: " + toString(g_nScore);
    }
    
    newLabel(sScore, DEFAULT_FONT, DISO(32), G_SWIDTH / 2, getY(220), this, 1, RATIO_1)->setTextColor(COLOR_YELLOW);
    
    Label *lAcc = newLabel("ACCURACY: " + toString(g_nAccuracy), DEFAULT_FONT, DISO(32), G_SWIDTH / 2, getY(250), this, 1, RATIO_1);
    lAcc->setTextColor(COLOR_YELLOW);
    
    Label *lPercent = newLabel(" %", "spaceranger.ttf", DISO(32), lAcc->getBoundingBox().getMaxX(), getY(250), this, 1, RATIO_1);
    lPercent->setAnchorPoint(Vec2(0, 0.5f));
    lPercent->setTextColor(COLOR_YELLOW);
}

void GameOverScene::createBtn()
{
    MenuItemLabel *iTryAgain = newLabelButton("TRY AGAIN", DEFAULT_FONT, DISO(28), G_SWIDTH / 2, getY(290), CC_CALLBACK_1(GameOverScene::onMenu, this), RATIO_1);
    iTryAgain->setColor(COLOR_RED);
    iTryAgain->setTag(1000);
    
    MenuItemImage *iSetting = newButton("btn_setting", getX(760), getY(320), CC_CALLBACK_1(GameOverScene::onMenu, this), RATIO_O);
    iSetting->setTag(1001);
    
    m_mMain = Menu::create(iTryAgain, iSetting, NULL);
    m_mMain->setPosition(0, 0);
    addChild(m_mMain, 1);
}

void GameOverScene::onMenu(Ref *sender)
{
    MenuItem *item = (MenuItem *)sender;
    
    if(item->getTag() == 1000) //Start
    {
        //Director::getInstance()->replaceScene(TransitionCrossFade::create(0.5f, GameScene::createScene()));
        Director::getInstance()->replaceScene(TransitionCrossFade::create(0.5f, MainScene::createScene()));
    }else if(item->getTag() == 1001) //Setting
    {
        Director::getInstance()->replaceScene(TransitionCrossFade::create(0.5f, SettingScene::createScene()));
    }
}

void GameOverScene::gotoMain(Ref *sender){
    Director::getInstance()->replaceScene(TransitionCrossFade::create(0.5f, MainScene::createScene()));
}

void GameOverScene::onTimer(float dt)
{
    m_nCnt++;
    //Create Mouse
    if(m_nCnt % 60 == 0){
        float fYPos = Y_RAND;
        int nMice = rand() % 4;
        Sprite *sMice = newSprite("ani/mice" + toString(nMice) + "_0", G_SWIDTH  + OBJ_SIZE, fYPos, this, 0, RATIO_O);
        sMice->runAction(RepeatForever::create(Animate::create(g_aniMice[nMice])));
        sMice->runAction(Sequence::create(MoveTo::create(5, Vec2(-OBJ_SIZE, fYPos)), RemoveSelf::create(), NULL));
    }
    
    //Create Meteor
    if(m_nCnt % 120 - 20 == 0){
        float fYPos = Y_RAND;
        Sprite *sMeteor = newSprite("ani/m_0", G_SWIDTH + OBJ_SIZE, fYPos, this, 0, RATIO_O);
        sMeteor->runAction(Sequence::create(MoveTo::create(2.5f, Vec2(-OBJ_SIZE, fYPos)), RemoveSelf::create(), NULL));
        sMeteor->runAction(RepeatForever::create(Animate::create(g_aniMeteor)));
    }
    
    //Create UFO
    if(m_nCnt % 180 - 40 == 0){
        float fYPos = Y_RAND;
        Sprite *sUFO = newSprite("ani/ufo_0", G_SWIDTH + OBJ_SIZE, fYPos, this, 0, RATIO_O);
        sUFO->runAction(RepeatForever::create(Animate::create(g_aniUFO)));
        sUFO->runAction(Sequence::create(MoveTo::create(3, Vec2(getX(450), fYPos)), DelayTime::create(3), MoveTo::create(2, Vec2(-OBJ_SIZE - 10, fYPos)), RemoveSelf::create(), NULL));
    }
    
    for(int i = 0; i < 2; i++){
        m_sStars[i]->setPositionX(m_sStars[i]->getPositionX() -  DISX(2));
        if(m_sStars[i]->getPositionX() < -m_sStars[i]->getBoundingBox().size.width){
            m_sStars[i]->setPositionX(m_sStars[i]->getBoundingBox().size.width);
        }
    }
}
