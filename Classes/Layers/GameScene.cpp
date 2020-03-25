//
//  GameScene.cpp
//  GalacticMice
//
//  Created by Lucky
//
//

#include "GameScene.h"
#include "GameSettings.h"
#include "GameOverScene.h"

#define GRAVITY 0.2f
#define VELOCITY 6
#define MAX_SPEED 10

#define LIVES 1

#define BULLET   201
#define LBULLET  202
#define CBULLET  203


#define TAG_METEOR 1000
#define TAG_UFO 1001

#define TAG_MICE_L 1002
#define TAG_MICE_R 1003

#define TAG_GLASS 101
#define TAG_HELMET 102
#define TAG_EXPLODE 103

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

GameScene::~GameScene(){
    removeAllChildren();
    stopAllActions();
    unscheduleAllCallbacks();
    m_aObjects->clear();
    m_aBullets->clear();
    g_layerGame = NULL;
}

bool GameScene::init(){
    if ( !Layer::init() )
    {
        return false;
    }
    srand((uint)time(0));

    m_aObjects = new Vector<Sprite *>();
    m_aBullets = new Vector<Sprite *>();
    
    g_nAccuracy = 0;
    
    m_fSpeedY = 0;
    m_nCnt = 0;
    m_nLives = LIVES;
    m_nScore = 0;
    m_bTouched = false;
    m_bPaused = true;
    m_nMiceCnt = 0;
    m_nLevel = 0;
    m_nTotalMiceCnt = 0;
    m_bGlassBreakingAll = false;
    createBg();
    createLabel();
    createBtn();
    
    m_nTotalHit = 0;
    m_nTotalShot = 0;
    
    //schedule(schedule_selector(GameScene::onTimer), 1.0f/60.0f);
    
    auto touchListener = EventListenerTouchAllAtOnce::create();
    touchListener->onTouchesBegan = CC_CALLBACK_2(GameScene::onTouchesBegan, this);
    touchListener->onTouchesMoved = CC_CALLBACK_2(GameScene::onTouchesMoved, this);
    touchListener->onTouchesEnded = CC_CALLBACK_2(GameScene::onTouchesEnded, this);
    touchListener->onTouchesCancelled = CC_CALLBACK_2(GameScene::onTouchesEnded, this);
    
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    g_layerGame = this;
    
    return true;
}

void GameScene::createBg()
{
    m_sPlayer = newSprite("playerh", getX(230), G_SHEIGHT/2, this, 0, RATIO_O);
    
    m_sPlayerE = newSprite("player", 0, 0, m_sPlayer, 0, RATIO_1);
    m_sPlayerE->setAnchorPoint(Point(0, 0));
    
    m_sGlasses = newSprite("glasses", 27, 27, m_sPlayer, 1, RATIO_1);
    m_sGlasses->setScale(0.5f);
    m_sGlasses->setVisible(false);
    
    //m_sHelmet = newSprite("helmet", 24, 24, m_sPlayer, 1, RATIO_1); //--Normal Helmet
    m_sHelmet = newSprite("helmetp", 22, 36, m_sPlayer, 1, RATIO_1); //--Pumpkin Helmet
    m_sHelmet->setVisible(false);
    
    m_sStun = newSprite("ani/st_0", 27, 55, m_sPlayer, 1, RATIO_1);
    m_sStun->setVisible(false);
    
    m_sTongue = newSprite("ani/t_1", 28, 13, m_sPlayer, 1, RATIO_1);
    m_sTongue->setVisible(false);
    m_sTongue->setAnchorPoint(Vec2(0, 1));
    
    m_sPlanet = newSprite("p" + toString(rand() % 4), G_SWIDTH + DISX(200), getY(rand() % (int)G_OHEIGHT), this, -1, RATIO_O);
    
    for(int i = 0; i < 2; i++){
        //Stars
        m_sStars[i] = newSprite("stars", 0, G_SHEIGHT / 2, this, -2, RATIO_O);
        m_sStars[i]->setScale(2 * G_SCALEX);
        m_sStars[i]->setPosition(m_sStars[0]->getBoundingBox().size.width * i, G_SHEIGHT / 2);
        
        //Lasers 0:Bottom 1:Top
        m_sLaser[i] = newSprite("laser", G_SWIDTH + DISX(10), (i==0?DISY(5):getY(5)), this, 1, RATIO_X);
        m_sLaser[i]->setAnchorPoint(Vec2(0, 0.5f));
        m_sLaser[i]->setVisible(false);
        Sprite *sSpark = newSprite("ani/sp_0", 0, 2.5f, m_sLaser[i], 1, RATIO_1);
        sSpark->runAction(RepeatForever::create(Animate::create(g_aniSpark)));
    }
    
    m_sJoystick = newSprite("joystick", getX(110), getY(300), this, 0, RATIO_O);
    m_sShootPad = newSprite("shootpad", getX(710), getY(300), this, 0, RATIO_O);
    
    Sprite *sLive = newSprite("player", getX(90), getY(50), this, 0, RATIO_O);
    sLive->setScale(0.5 * G_SCALEO);
}

void GameScene::createLabel()
{
    m_lScore = newLabel("0", DEFAULT_FONT, DISO(30), getX(750), getY(50), this, 0, RATIO_1);
    m_lScore->setTextColor(COLOR_YELLOW);
    
    m_lPause = newLabel("PAUSE", DEFAULT_FONT, DISO(40), G_SWIDTH / 2, G_SHEIGHT / 2, this, 3, RATIO_1);
    m_lPause->setTextColor(COLOR_YELLOW);
    m_lPause->setVisible(false);
    
    m_lLevel = newLabel("LEVEL 1", DEFAULT_FONT, DISO(32), G_SWIDTH / 2, G_SHEIGHT / 2, this, 3, RATIO_1);
    m_lLevel->setTextColor(COLOR_YELLOW);
    
    m_lLives = newLabel("X " + toString(m_nLives), DEFAULT_FONT, DISO(20), getX(110), getY(50), this, 0, RATIO_1);
    m_lLives->setAnchorPoint(Vec2(0, 0.5f));
    m_lLives->setTextColor(COLOR_YELLOW);
    
    //m_lStart = newLabel("CATCH ALL OF\nTHE MICE", DEFAULT_FONT, DISO(32), G_SWIDTH / 2, G_SHEIGHT * 3/4, this, 1, RATIO_1);
    //m_lStart->setAlignment(TextHAlignment::CENTER);
    //m_lStart->setTextColor(COLOR_YELLOW);
    m_lStart = newSprite("gametitle", G_SWIDTH / 2, G_SHEIGHT * 3 / 4, this, 1, RATIO_O);
}

void GameScene::createBtn()
{
    /*MenuItemImage *iStart = newButton("btn_start", G_SWIDTH / 2, G_SHEIGHT/4, CC_CALLBACK_1(GameScene::onMenu, this), RATIO_XY);
    iStart->setTag(1000);
    
    m_mMain = Menu::create(iStart, NULL);
    m_mMain->setPosition(Point(0, 0));
    addChild(m_mMain, 1);*/
}

void GameScene::onMenu(Ref *sender)
{
    MenuItemImage *item = (MenuItemImage *)sender;
    if(item->getTag() == 1000) //Play Again
    {
        //Director::getInstance()->replaceScene(TransitionCrossFade::create(0.5f, GameScene::createScene()));
    }
    else if(item->getTag() == 1001) //Main
    {
        //Director::getInstance()->replaceScene(TransitionCrossFade::create(0.5f, GameScene::createScene()));
    }
}

void GameScene::showGlass(){
    m_bGlassBreakingAll = true;
    m_sGlasses->stopAllActions();
    m_sGlasses->setOpacity(0);
    m_sGlasses->setVisible(true);
    m_sGlasses->runAction(Sequence::create(FadeIn::create(0.2f), DelayTime::create(10), FadeOut::create(0.2f), Hide::create(), NULL));
}

void GameScene::onTimer(float dt)
{
    if(m_bPaused)
        return;
    m_nCnt++;
    
    m_fSpeedY -= GRAVITY;
    if(m_fSpeedY < -MAX_SPEED){
        m_fSpeedY = -MAX_SPEED;
    }
    
    float fPosY = m_sPlayer->getPositionY() + DISY(m_fSpeedY);
    
    if(fPosY < m_sPlayer->getBoundingBox().size.width/2){ //Floor
        m_fSpeedY = 0;
        fPosY = m_sPlayer->getBoundingBox().size.width/2;
    }else if(fPosY > G_SHEIGHT - m_sPlayer->getBoundingBox().size.width/2){ //Ceiling
        m_fSpeedY = 0;
        fPosY = G_SHEIGHT - m_sPlayer->getBoundingBox().size.width/2;
    }
    
    m_sPlayer->setPositionY(fPosY);
    
    for(int i = 0; i < 2; i++){
        if(m_sLaser[i]->getBoundingBox().intersectsCircle(m_sPlayer->getPosition(), m_sPlayer->getBoundingBox().size.width / 2)){

            stunPlayer();
            playEffect("lasertouch.wav");
            m_fSpeedY = (i == 0 ? VELOCITY : -VELOCITY/2);
            break;
        }
    }
    
    //Create Mouse
    if(m_nCnt % 120 == 0){
        int nMice = rand() % 4;
        Sprite *sMice = newSprite("ani/mice" + toString(nMice) + "_0", G_SWIDTH  + OBJ_SIZE, Y_RAND, this, 0, RATIO_O);
        sMice->runAction(RepeatForever::create(Animate::create(g_aniMice[nMice])));
        sMice->setTag(TAG_MICE_L);
        m_aObjects->pushBack(sMice);
    }
    
    //Create Meteor
    int nRate = g_levelInfo[m_nLevel].meteorSpawnRate;
    if(nRate > 0 && m_nCnt % (120 * nRate) - 20 == 0){
        float fSpeedRate = g_levelInfo[m_nLevel].meteorSpeed;
        
        float fYPos = Y_RAND;
        Sprite *sMeteor = newSprite("ani/m_0", G_SWIDTH + OBJ_SIZE, fYPos, this, 0, RATIO_O);
        sMeteor->runAction(RepeatForever::create(Animate::create(g_aniMeteor)));
        sMeteor->runAction(Sequence::create(MoveTo::create(2.5f / fSpeedRate, Vec2(-OBJ_SIZE - 10, fYPos)), NULL));
        sMeteor->setTag(TAG_METEOR);
        m_aObjects->pushBack(sMeteor);
    }
    
    //Create UFO
    nRate = g_levelInfo[m_nLevel].ufoSpawnRate;
    if(nRate > 0 && m_nCnt % (120 * nRate) - 40 == 0){
        float fSpeedRate = g_levelInfo[m_nLevel].ufoSpeed;
        
        float fYPos = Y_RAND;
        Sprite *sUFO = newSprite("ani/ufo_0", G_SWIDTH + OBJ_SIZE, fYPos, this, 0, RATIO_O);
        sUFO->runAction(RepeatForever::create(Animate::create(g_aniUFO)));
        sUFO->runAction(Sequence::create(MoveTo::create(2.5f / fSpeedRate, Vec2(getX(450), fYPos)), DelayTime::create(2.0f / fSpeedRate), MoveTo::create(1.0f / fSpeedRate, Vec2(-OBJ_SIZE - 10, fYPos)),NULL));
        sUFO->setTag(TAG_UFO);
        m_aObjects->pushBack(sUFO);
    }
    
    //Trophy 2 - 10+ mice in screen
    if(!g_bTrophy[1]){
        int nMice = 0;
        for(int i = 0; i < m_aObjects->size(); i++){
            Sprite *sObject = m_aObjects->at(i);
            int nTag = sObject->getTag();
            if(nTag == TAG_MICE_L || nTag == TAG_MICE_R){
                nMice++;
                if(nMice >= 10){
                    showTrophy(1);
                    break;
                }
            }
        }
    }
    
    for(int i = 0; i < m_aObjects->size(); i++){
        Sprite *sObject = m_aObjects->at(i);
        int nTag = sObject->getTag();
        float fSpeedRate = g_levelInfo[m_nLevel].miceSpeed;
        //Move Objects
        if(nTag == TAG_MICE_L){
            sObject->setPositionX(sObject->getPositionX() - DISX(3 * fSpeedRate));
            if(sObject->getBoundingBox().intersectsRect(m_sPlayer->getBoundingBox())){
                sObject->setTag(TAG_MICE_R);
            }
            //Avoid overlap mice
            for(int j = i + 1; j < m_aObjects->size(); j++){
                Sprite *sObject1 = m_aObjects->at(j);
                if(sObject1->getTag() == TAG_MICE_L){
                    Rect rect = sObject->getBoundingBox();
                    Rect rect1 = sObject1->getBoundingBox();
                    
                    if(rect.intersectsRect(rect1)){
                        if(rect.getMinX() < rect1.getMinX()){
                            sObject1->setPositionX(rect.getMaxX() + rect1.size.width / 2);
                        }else{
                            sObject->setPositionX(rect1.getMaxX() + rect.size.width / 2);
                        }
                    }
                }
            }
        }else if(nTag == TAG_MICE_R){
            sObject->setPositionX(sObject->getPositionX() + DISX(6 * fSpeedRate));
            if(sObject->getPositionX() > G_SWIDTH - OBJ_SIZE){
                sObject->setTag(TAG_MICE_L);
            }
        }else if(nTag == TAG_GLASS || nTag == TAG_HELMET)
            sObject->setPositionX(sObject->getPositionX() - DISX(3.5f));
        
        //UFO, METEOR vs Player
        if(!m_sStun->isVisible() && (nTag == TAG_UFO || nTag == TAG_METEOR)){
            if(checkCollide(m_sPlayer, sObject)){
                sObject->stopAllActions();
                sObject->setTag(TAG_EXPLODE);
                sObject->runAction(Sequence::create(Animate::create(g_aniExplode), Hide::create(), NULL));
                playEffect("playerhit.wav");
                stunPlayer();
            }
        }
        
        //Glass vs Player
        if(nTag == TAG_GLASS){
            if(checkCollide(m_sPlayer, sObject)){
                playEffect("perkcapture.wav");
                sObject->setTag(TAG_EXPLODE);
                sObject->setVisible(false);
                showGlass();
            }
        }else if(nTag == TAG_HELMET){
            if(checkCollide(m_sPlayer, sObject)){
                playEffect("perkcapture.wav");
                sObject->setTag(TAG_EXPLODE);
                sObject->setVisible(false);
                m_sPlayerE->setVisible(false);
                m_sHelmet->setVisible(true);
            }
        }
        
        //Bullet vs Object
        if(nTag >= TAG_METEOR)
        {
            for(int j = 0; j < m_aBullets->size(); j++){
                Sprite *bullet = m_aBullets->at(j);
                int nBTag = bullet->getTag();
                if(nBTag == LBULLET || nBTag == CBULLET || (nBTag == BULLET && nTag >= TAG_MICE_L)){
                    if(bullet->isVisible() && bullet->getBoundingBox().intersectsRect(sObject->getBoundingBox())){
                        bullet->setVisible(false);
                        sObject->setTag(TAG_EXPLODE);
                        
                        m_nTotalHit++;
                        
                        int nScore = 10;
                        if(nTag >= TAG_MICE_L){
                            sObject->runAction(Sequence::create(FadeOut::create(0.3f), Hide::create(), NULL));
                            nScore = 10;
                            m_nMiceCnt++;
                            m_nTotalMiceCnt++;
                            //--Achive 1st Trophy catch 10 mices
                            if(!g_bTrophy[0] && m_nMiceCnt == 10){
                                showTrophy(0);
                            }
                            
                            playEffect("micecapture.wav");
                            //Increase level if catch amount of mice
                            if(m_nMiceCnt == g_levelInfo[m_nLevel].miceSpawnCnt){
                                m_nMiceCnt = 0;
                                if(m_nLevel < MAX_LEVEL - 1)
                                {
                                    m_nLevel++;
                                    checkLaser();
                                }
                            }
                            
                            //Create a perk
                            if(g_levelInfo[m_nLevel].showPerk && m_nMiceCnt == 15){
                                Sprite *sGlass = newSprite("perk", G_SWIDTH + OBJ_SIZE, Y_RAND, this, 0, RATIO_O);
                                sGlass->setTag(TAG_GLASS);
                                m_aObjects->pushBack(sGlass);
                            }
                            
                            //Create Helmet
                            if(m_nTotalMiceCnt % 31 == 0 && !m_sHelmet->isVisible()){
                                Sprite *sHelmet = newSprite("helmetp_b", G_SWIDTH + OBJ_SIZE, Y_RAND, this, 0, RATIO_O);
                                sHelmet->setTag(TAG_HELMET);
                                m_aObjects->pushBack(sHelmet);
                            }
                            
                            if(m_nTotalMiceCnt % 30 == 0){
                                int nLevel = (m_nTotalMiceCnt / 30) + 1;
                                playEffect("levelup.wav");
                                m_lLevel->setString("Level " + toString(nLevel));
                                m_lLevel->runAction(Sequence::create(FadeIn::create(0.3f), DelayTime::create(2), FadeOut::create(1),NULL));
                            }
                        }else{ //Destroy UFO & Meteor
                            playEffect("perklaserhit.wav");
                            sObject->stopAllActions();
                            sObject->runAction(Sequence::create(Animate::create(g_aniExplode), Hide::create(), NULL));
                            nScore = 20;
                        }
                        
                        if((m_nScore / 10) % 10 == 9 &&  m_nLives < 9){
                            setLife(1);
                            showPoint("1UP", DEFAULT_FONT, sObject->getPosition(), 0.3f);
                            playEffect("extralife.wav");
                        }
                        
                        m_nScore += nScore;
                        m_lScore->setString(toString(m_nScore));
                        showPoint("+" + toString(nScore), "spaceranger.ttf", sObject->getPosition(), 0.05f);
                        break;
                    }
                }
            }
        }
        //Remove arrived or marked(hidden) Objects
        if(sObject->getPositionX() < -OBJ_SIZE || !sObject->isVisible())
        {
            //Trophy 3 -- Glass Break All
            if(m_sGlasses->isVisible() && sObject->getPositionX() < -OBJ_SIZE){
                m_bGlassBreakingAll = false;
            }
            
            removeObject(sObject);
            
            if(nTag == TAG_MICE_L){
                playEffect("miceescape.wav");
                stunPlayer();
                //setLife(-1);
            }
            i--;
        }
    }
    //Trophy 3 -- Glass Break All
    if(!g_bTrophy[2] && m_bGlassBreakingAll && !m_sGlasses->isVisible()){
        m_bGlassBreakingAll = false;
        showTrophy(2);
    }
    //Remove unnecessary bullets
    for(int i = 0; i < m_aBullets->size(); i++){
        Sprite *bullet = m_aBullets->at(i);
        if(!bullet->isVisible()){
            bullet->stopAllActions();
            removeChild(bullet);
            m_aBullets->eraseObject(bullet);
            i--;
        }
    }
    
    //Stars Flow
    for(int i = 0; i < 2; i++){
        m_sStars[i]->setPositionX(m_sStars[i]->getPositionX() - DISX(1));
        if(m_sStars[i]->getPositionX() < -m_sStars[i]->getBoundingBox().size.width){
            m_sStars[i]->setPositionX(m_sStars[i]->getBoundingBox().size.width);
        }
    }
    
    m_sPlanet->setPositionX(m_sPlanet->getPositionX() - DISX(1.3f));
    if(m_sPlanet->getPositionX() < -DISX(600)){
        removeChild(m_sPlanet);
        m_sPlanet = newSprite("p" + toString(rand() % 4), G_SWIDTH + DISX(200), getY(rand() % (int)G_OHEIGHT), this, -1, RATIO_O);
    }
}

void GameScene::showTrophy(int nIndex){
    g_bTrophy[nIndex] = true;
    saveGameInfo();
    Sprite *sTrophy = newSprite("tr_d" + toString(nIndex), G_SWIDTH / 2, G_SHEIGHT / 2, this, 2, RATIO_O);
    sTrophy->setOpacity(0);
    sTrophy->setScale(G_SCALEO);
    sTrophy->runAction(Sequence::create(FadeIn::create(0.5f), DelayTime::create(1.5f), Spawn::create(ScaleTo::create(0.5f, 0), FadeOut::create(0.5f), NULL), RemoveSelf::create(), NULL));
}

void GameScene::checkLaser(){
    if(m_nLevel == 4){ //Level 5 appear bottom laser
        m_sLaser[0]->setVisible(true);
        m_sLaser[0]->runAction(MoveTo::create(10, Vec2(-DISX(10), m_sLaser[0]->getPositionY())));
    }else if(m_nLevel == 6){ //level 7 appear top laser
        m_sLaser[1]->setVisible(true);
        m_sLaser[1]->runAction(MoveTo::create(10, Vec2(-DISX(10), m_sLaser[1]->getPositionY())));
    }
}

void GameScene::showPoint(string sText, string sFont, Vec2 pos, float delay)
{
    Label *label = newLabel(sText, sFont, DISO(18), pos.x, pos.y, this, 1, RATIO_1);
    label->setOpacity(0);
    label->runAction(Sequence::create(DelayTime::create(delay), FadeIn::create(0.05f), Spawn::create(MoveBy::create(0.5f, Vec2(0, DISY(20))), FadeOut::create(0.5f), NULL), RemoveSelf::create(), NULL));
}

void GameScene::stunPlayer(){
    m_bGlassBreakingAll = false;
    
    if(m_sHelmet->isVisible()){
        m_sPlayerE->setVisible(true);
        m_sHelmet->setVisible(false);
    }else{
        if(!m_sStun->isVisible()){
            m_sStun->setVisible(true);
            m_sStun->runAction(Sequence::create(Repeat::create(Animate::create(g_aniStun), 5), Hide::create(), NULL));
            m_sPlayer->runAction(Repeat::create(Sequence::create(FadeTo::create(0.2f, 128), FadeIn::create(0.2f), NULL) ,5));
            setLife(-1);
            
            m_sGlasses->stopAllActions();
            m_sGlasses->setVisible(false);
        }
    }
}

void GameScene::spawnBullet(int nType){
    Vec2 pos = m_sPlayer->getPosition();
    string sName = "bullet";
    if(nType == LBULLET)
        sName = "bullet_l";
    else if(nType == CBULLET)
        sName = "bullet_c";
    Sprite *bullet = newSprite(sName, pos.x, pos.y - (nType==BULLET?DISY(13):0), this, 0, RATIO_O);
    bullet->runAction(Sequence::create(MoveBy::create(0.12, Vec2(DISX(500), -DISY(80))), Hide::create(), NULL));
    bullet->setTag(nType);
    m_aBullets->pushBack(bullet);
    playEffect((nType==BULLET?"tongue.wav":"shootlaser.wav"));
}

void GameScene::removeObject(Sprite *obj){
    obj->stopAllActions();
    removeChild(obj);
    m_aObjects->eraseObject(obj);
}

bool GameScene::checkCollide(Sprite *s1, Sprite *s2){
    int nMinDistance = (s1->getBoundingBox().size.height + s2->getBoundingBox().size.height) / 2;
    int nDis = s1->getPosition().getDistance(s2->getPosition());
    if(nDis <= nMinDistance){
        return true;
    }
    return false;
}
void GameScene::setLife(int nLife){
    m_nLives += nLife;
    m_lLives->setString("X " + toString(m_nLives));
    
    m_lLives->stopAllActions();
    m_lLives->setScale(1);
    m_lLives->runAction(Sequence::create(ScaleTo::create(0.3f, 1.5f), ScaleTo::create(0.2f, 1),NULL));
    
    //Game Over
    if(m_nLives <= 0){
        g_nScore = m_nScore;
        unscheduleAllCallbacks();
        playEffect("gameover.wav");
        
        if(m_nTotalMiceCnt < 30)
            g_nAccuracy = 0;
        else{
            g_nAccuracy = m_nTotalHit * 100 / m_nTotalShot;
            //Trophy 4 - 90% Accuracy
            if(!g_bTrophy[3] && g_nAccuracy >= 90){
                showTrophy(3);
            }
        }
        
        //Director::getInstance()->replaceScene(TransitionCrossFade::create(0.5f, GameOverScene::createScene()));
        m_sPlayer->runAction(Sequence::create(DelayTime::create(0.5f), Spawn::create(ScaleTo::create(0.5f, 0), RotateBy::create(0.5f, 540),NULL), CallFuncN::create(CC_CALLBACK_1(GameScene::gotoGameOver, this)) ,NULL));
        return;
    }
}

void GameScene::gotoGameOver(Ref *sender){
    Director::getInstance()->replaceScene(TransitionCrossFade::create(0.5f, GameOverScene::createScene()));
}

void GameScene::setPause(bool isPause){
    if(m_bTouched){
        m_bPaused = isPause;
        m_lPause->setVisible(isPause);
        for(int i = 0; i < m_aObjects->size(); i++){
            Sprite *sObject = m_aObjects->at(i);
            int nTag = sObject->getTag();
            if(nTag == TAG_UFO || nTag == TAG_METEOR){
                if(isPause)
                    sObject->pause();
                else
                    sObject->resume();
            }
        }
    }
}

void GameScene::onTouchesBegan(const std::vector<Touch*>& touches, Event *pEvent){
    Point touchLocation=touches[0]->getLocation();
    
    if(!m_bTouched){
        schedule(schedule_selector(GameScene::onTimer), 1.0f/60.0f);
        m_lStart->runAction(FadeOut::create(1));
        m_lLevel->runAction(FadeOut::create(1));
        m_bTouched = true;
        setPause(false);
    }else{
        if(!m_bPaused){
            if(touchLocation.y > G_SHEIGHT / 2){
                setPause(true);
            }
        }else{
            setPause(false);
        }
    }
    
    if(m_sJoystick->getBoundingBox().containsPoint(touchLocation)){ //touch joystick
        m_fSpeedY = VELOCITY;
        playEffect("jump.wav");
    }
    if(m_sShootPad->getBoundingBox().containsPoint(touchLocation)){ //touch shootpad
        m_nTotalShot++;
        if(m_sGlasses->isVisible()){
            if(m_sHelmet->isVisible())
                spawnBullet(CBULLET);
            else
                spawnBullet(LBULLET);
        }
        else if(!m_sTongue->isVisible() && !m_sStun->isVisible()){
            m_sTongue->setVisible(true);
            m_sTongue->runAction(Sequence::create(Animate::create(g_aniTongue), Hide::create(), NULL));
            spawnBullet(BULLET);
        }
    }
}

void GameScene::onTouchesMoved(const std::vector<Touch*>& touches, Event *pEvent){
    
}
void GameScene::onTouchesEnded(const std::vector<Touch*>& touches, Event *pEvent){
    
}

