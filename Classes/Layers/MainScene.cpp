//
//  MainScene.cpp
//  GalacticMice
//
//  Created by Lucky
//
//

#include "MainScene.h"
#include "GameSettings.h"
#include "GameScene.h"
#include "MainScene.h"
#include "SettingScene.h"
Scene* MainScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MainScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

MainScene::~MainScene(){
    removeAllChildren();
    stopAllActions();
    unscheduleAllCallbacks();
}

bool MainScene::init(){
    if ( !Layer::init() )
    {
        return false;
    }
    m_nCnt = 0;
    createBg();
    createLabel();
    createBtn();
    schedule(schedule_selector(MainScene::onTimer), 1.0f/30.0f);
    
    if(banner_view == NULL)
        createBanner();
    
    //SimpleAudioEngine::getInstance()->playBackgroundMusic("res/Audio/bgm.mp3", true);
    return true;
}

void MainScene::createBanner(){
#if defined(__ANDROID__)
    // Android ad unit IDs.
    const char* kBannerAdUnit = "ca-app-pub-1187817622415050/4944942135";
#else
    // iOS ad unit IDs.
    const char* kBannerAdUnit = "ca-app-pub-1187817622415050/9152261986";
#endif
    
    // Create and initialize banner view.
    banner_view = new firebase::admob::BannerView();
    firebase::admob::AdSize ad_size;
    ad_size.ad_size_type = firebase::admob::kAdSizeStandard;
    ad_size.width = 320;
    ad_size.height = 50;
    banner_view->Initialize(getAdParent(), kBannerAdUnit, ad_size);
    
    scheduleUpdate();
}
void MainScene::update(float delta) {
    // Check that the banner has been initialized.
    if (banner_view->InitializeLastResult().status() ==
        firebase::kFutureStatusComplete) {
        // Check that the banner hasn't started loading.
        if (banner_view->LoadAdLastResult().status() ==
            firebase::kFutureStatusInvalid) {
            // Make the banner visible and load an ad.
            CCLOG("Loading a banner.");
            banner_view->MoveTo(firebase::admob::BannerView::Position::kPositionBottom);
            banner_view->Show();
            firebase::admob::AdRequest my_ad_request = {};
            
            static const char* kTestDeviceIDs[] = {"770eeb5de5a867c868131c3cf2dac805",};
            my_ad_request.test_device_id_count = sizeof(kTestDeviceIDs) / sizeof(kTestDeviceIDs[0]);
            my_ad_request.test_device_ids = kTestDeviceIDs;

            banner_view->LoadAd(my_ad_request);
        }
    }
}
void MainScene::createBg()
{
    newSprite("title", G_SWIDTH/2, G_SHEIGHT*3/4, this, 1, RATIO_O);
    newSprite("player", G_SWIDTH/2, G_SHEIGHT/2, this, 1, RATIO_O);
    for(int i = 0; i < 2; i++){
        m_sStars[i] = newSprite("stars", 0, G_SHEIGHT / 2, this, -1, RATIO_O);
        m_sStars[i]->setScale(2 * G_SCALEX);
        m_sStars[i]->setPosition(m_sStars[0]->getBoundingBox().size.width * i, G_SHEIGHT / 2);
    }
}

void MainScene::createLabel()
{
    newLabel("HI SCORE " + toString(g_nHighScore), DEFAULT_FONT, DISO(18), G_SWIDTH / 2, getY(30), this, 1, RATIO_1)->setTextColor(COLOR_YELLOW);
}

void MainScene::createBtn()
{
    MenuItemLabel *iStart = newLabelButton("PRESS TO START", DEFAULT_FONT, DISO(28), G_SWIDTH / 2, G_SHEIGHT/4, CC_CALLBACK_1(MainScene::onMenu, this), RATIO_1);
    iStart->setColor(COLOR_RED);
    iStart->setTag(1000);
    
    MenuItemImage *iSetting = newButton("btn_setting", getX(760), getY(320), CC_CALLBACK_1(MainScene::onMenu, this), RATIO_O);
    iSetting->setTag(1001);
    
    m_mMain = Menu::create(iStart, iSetting, NULL);
    m_mMain->setPosition(0, 0);
    addChild(m_mMain, 1);
}

void MainScene::onMenu(Ref *sender)
{
    MenuItem *item = (MenuItem *)sender;
    if(item->getTag() == 1000) //Start
    {
        Director::getInstance()->replaceScene(TransitionCrossFade::create(0.5f, GameScene::createScene()));
    }else if(item->getTag() == 1001) //Setting
    {
        Director::getInstance()->replaceScene(TransitionCrossFade::create(0.5f, SettingScene::createScene()));
    }
}

void MainScene::onTimer(float dt)
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
    if(m_nCnt % 180 - 40== 0){
        float fYPos = Y_RAND;
        Sprite *sUFO = newSprite("ani/ufo_0", G_SWIDTH + OBJ_SIZE, fYPos, this, 0, RATIO_O);
        sUFO->runAction(RepeatForever::create(Animate::create(g_aniUFO)));
        sUFO->runAction(Sequence::create(MoveTo::create(3, Vec2(getX(450), fYPos)), DelayTime::create(3), MoveTo::create(2, Vec2(-OBJ_SIZE - 10, fYPos)), RemoveSelf::create(), NULL));
    }
    
    
    for(int i = 0; i < 2; i++){
        m_sStars[i]->setPositionX(m_sStars[i]->getPositionX() - DISX(2));
        if(m_sStars[i]->getPositionX() < -m_sStars[i]->getBoundingBox().size.width){
            m_sStars[i]->setPositionX(m_sStars[i]->getBoundingBox().size.width);
        }
    }
}
