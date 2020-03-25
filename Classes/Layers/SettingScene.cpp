//
//  SettingScene.cpp
//  GalacticMice
//
//  Created by Lucky
//
//

#include "SettingScene.h"
#include "GameSettings.h"
#include "GameScene.h"
#include "MainScene.h"

Scene* SettingScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = SettingScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

SettingScene::~SettingScene(){
    removeAllChildren();
    stopAllActions();
    unscheduleAllCallbacks();
}

bool SettingScene::init(){
    if ( !Layer::init() )
    {
        return false;
    }
    
    createBg();
    createLabel();
    createBtn();
    schedule(schedule_selector(SettingScene::onTimer), 1.0f/30.0f);
    return true;
}

void SettingScene::createBg()
{
    refreshTrophy();
    for(int i = 0; i < 2; i++){
        m_sStars[i] = newSprite("stars", 0, G_SHEIGHT / 2, this, -1, RATIO_O);
        m_sStars[i]->setScale(2 * G_SCALEX);
        m_sStars[i]->setPosition(m_sStars[0]->getBoundingBox().size.width * i, G_SHEIGHT / 2);
    }
}

void SettingScene::refreshTrophy(){
    for(int i = 0; i < TROPHY_CNT; i++)
        removeChildByTag(100);
    for(int i = 0; i < TROPHY_CNT; i++){
        int nPosX = (G_SWIDTH / (TROPHY_CNT + 1)) * (i + 1);
        
        if(g_bTrophy[i]){
            m_sTrophy[i] = newSprite("tr" + toString(i), nPosX, getY(150), this, 1, RATIO_O);
        }else{
            Label *lQuest = newLabel("?", DEFAULT_FONT, DISO(34), nPosX, getY(150), this, 1, RATIO_O);
            lQuest->setTextColor(COLOR_YELLOW);
            m_sTrophy[i] = lQuest;
        }
        
        m_sTrophy[i]->setTag(100);
    }
}
void SettingScene::createLabel()
{
    newLabel("TROPHIES", DEFAULT_FONT, DISO(34), G_SWIDTH / 2, getY(70), this, 1, RATIO_1)->setTextColor(Color4B(COLOR_RED));
}

void SettingScene::createBtn()
{
    MenuItemImage *iBack = newButton("btn_back", getX(50), getY(50), CC_CALLBACK_1(SettingScene::onMenu, this), RATIO_O);
    iBack->setTag(1000);
    
    MenuItemToggle *iSound = newToggleLabelButton("SOUNDS ON", "SOUNDS OFF", DEFAULT_FONT, DISO(8), G_SWIDTH / 2, getY(215), CC_CALLBACK_1(SettingScene::onMenu, this), RATIO_O);
    iSound->setTag(1001);
    iSound->setSelectedIndex(g_bSoundOn?0:1);

    MenuItemLabel *iReset = newLabelButton("RESET ACHIEVEMENTS", DEFAULT_FONT, DISO(8), G_SWIDTH / 2, getY(250), CC_CALLBACK_1(SettingScene::onMenu, this), RATIO_O);
    iReset->setTag(1002);
    MenuItemLabel *iPrivacy = newLabelButton("PRIVACY POLICY", DEFAULT_FONT, DISO(8), G_SWIDTH / 2, getY(285), CC_CALLBACK_1(SettingScene::onMenu, this), RATIO_O);
    iPrivacy->setTag(1003);
    
    MenuItemLabel *iRate = newLabelButton("LEAVE US FEEDBACK", DEFAULT_FONT, DISO(8), G_SWIDTH / 2, getY(320), CC_CALLBACK_1(SettingScene::onMenu, this), RATIO_O);
    iRate->setTag(1004);
    
    m_mMain = Menu::create(iBack, iSound, iReset, iPrivacy, iRate, NULL);
    m_mMain->setPosition(0, 0);
    addChild(m_mMain, 1);
}

void SettingScene::onMenu(Ref *sender)
{
    int nTag = ((MenuItem *)sender)->getTag();
    
    if(nTag == 1000) //Back
    {
        Director::getInstance()->replaceScene(TransitionCrossFade::create(0.5f, MainScene::createScene()));
    }else if(nTag == 1001) //Sound
    {
        MenuItemToggle *item = (MenuItemToggle *)sender;
        g_bSoundOn = (item->getSelectedIndex() == 0);
        
        float fVolume = (g_bSoundOn?1.0f:0);
        SimpleAudioEngine::getInstance()->setEffectsVolume(fVolume);
        SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(fVolume);
        
    }else if(nTag == 1002) //Reset
    {
        g_nHighScore = 0;
        for(int i = 0; i < TROPHY_CNT; i++){
            g_bTrophy[i] = false;
        }
        saveGameInfo();
        refreshTrophy();
    }else if(nTag == 1003) //Privacy
    {
        Application::getInstance()->openURL("https://www.cats-in-space.com/privacy.html");
    }else if(nTag == 1004) //Rate
    {
        rateApp();
    }
}

void SettingScene::gotoMain(Ref *sender){
    Director::getInstance()->replaceScene(TransitionCrossFade::create(0.5f, MainScene::createScene()));
}

void SettingScene::onTimer(float dt)
{
    for(int i = 0; i < 2; i++){
        m_sStars[i]->setPositionX(m_sStars[i]->getPositionX() -  DISX(2));
        if(m_sStars[i]->getPositionX() < -m_sStars[i]->getBoundingBox().size.width){
            m_sStars[i]->setPositionX(m_sStars[i]->getBoundingBox().size.width);
        }
    }
}
