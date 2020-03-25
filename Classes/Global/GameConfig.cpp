//
//  GameConfig.cpp
//  GalacticMice
//
//  Created by Lucky
//
//

#include "GameConfig.h"

bool g_bSoundOn = true;

int g_nScore = 0;
int g_nHighScore = 0;
int g_nAccuracy = 0;

Animation *g_aniExplode;
Animation *g_aniMeteor;
Animation *g_aniTongue;
Animation *g_aniUFO;
Animation *g_aniStun;
Animation *g_aniSpark;
Animation *g_aniMice[MICE_CNT];
bool g_bTrophy[TROPHY_CNT];
Layer *g_layerGame = NULL;
firebase::admob::BannerView* banner_view;
//Mice, Meteor, UFO, S_MICE, S_METEOR, S_UFO, TOP, BOTTOM
LevelInfo g_levelInfo[] = {
    {10, -1, -1, SP_1, 1, 1, false},
    {20, 2, -1, SP_1, SP_1, 1, false},
    {30, 1, -1, SP_2, SP_2, 1, true},
    {30, 1, 3, SP_3, SP_3, SP_1, true},
    {30, 1, 2.8f, SP_3, SP_3, SP_2, true},
    {30, 1, 2.6f, SP_4, SP_4, SP_4, true},
    {40, 1, 2.4f, SP_4, SP_4, SP_5, true},
    {50, 1, 2.2f, SP_5, SP_5, SP_5, true},
};
void loadGameInfo(){
//    SimpleAudioEngine::getInstance()->preloadEffect("res/Musics/shining_time.mp3");
    
//    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1.0);
//    SimpleAudioEngine::getInstance()->setEffectsVolume(1.0);
    g_nHighScore = UserDefault::getInstance()->getIntegerForKey("HIGHSCORE", 0);
    
    for(int i = 0; i < TROPHY_CNT; i++){
        string sKey = "TROPHY" + toString(i);
        g_bTrophy[i] = UserDefault::getInstance()->getBoolForKey(sKey.c_str(), false);
    }
}

void saveGameInfo(){
    UserDefault::getInstance()->setIntegerForKey("HIGHSCORE", g_nHighScore);
    for(int i = 0; i < TROPHY_CNT; i++){
        string sKey = "TROPHY" + toString(i);
        UserDefault::getInstance()->setBoolForKey(sKey.c_str(), g_bTrophy[i]);
    }
}

int getCurrentTime(){
    time_t t = time(0);
    struct tm * now = localtime(&t);
    int crtTime = now->tm_mon*(30*24*3600)+now->tm_mday*(24*3600)+now->tm_hour*3600+now->tm_min*60+now->tm_sec;

    return crtTime;

    //    return now->tm_gmtoff;// crtTime;
}

void rateApp(){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    string sUrl = "market://details?id=com.council_tech.galactic_mice_android";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    string sUrl = "https://itunes.apple.com/app/id1430768557?action=write-review";
#endif
    Application::getInstance()->openURL(sUrl);
}
