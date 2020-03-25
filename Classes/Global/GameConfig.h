//
//  GameConfig.h
//  GalacticMice
//
//  Created by Lucky
//
//

#ifndef __GameConfig__
#define __GameConfig__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "GameBridge.h"
#include "FirebaseHelper.h"
#include "firebase/admob.h"
#include "firebase/admob/types.h"
#include "firebase/app.h"
#include "firebase/future.h"
#include "firebase/admob/banner_view.h"

using namespace cocos2d;
using namespace CocosDenshion;
using namespace std;
using namespace StringUtils;

#define TROPHY_CNT      4

#define MAX_LEVEL       8
#define OBJ_SIZE        DISX(50)
#define Y_RAND          getY(rand()%200+70)
#define COLOR_YELLOW    Color4B(237, 255, 14, 255)
#define COLOR_RED       Color3B(217,0,0)
#define DEFAULT_FONT        "SpaceMarine-Nominal.ttf"
#define MICE_CNT        4
#define SP_1  1
#define SP_2  1.1f
#define SP_3  1.2f
#define SP_4  1.3f
#define SP_5  1.4f

typedef struct LevelInfo{
    int miceSpawnCnt;
    float meteorSpawnRate;
    float ufoSpawnRate;
    
    float miceSpeed;
    float meteorSpeed;
    float ufoSpeed;
    
    bool showPerk;
}LEVEL_INFO;

extern bool g_bSoundOn;
extern int g_nScore;
extern int g_nHighScore;
extern int g_nAccuracy;
extern Animation *g_aniExplode;
extern Animation *g_aniMeteor;
extern Animation *g_aniTongue;
extern Animation *g_aniUFO;
extern Animation *g_aniStun;
extern Animation *g_aniSpark;
extern Animation *g_aniMice[MICE_CNT];
extern LevelInfo g_levelInfo[MAX_LEVEL];
extern bool g_bTrophy[TROPHY_CNT];

extern Layer *g_layerGame;
extern firebase::admob::BannerView* banner_view;

void loadGameInfo();
void saveGameInfo();

int getCurrentTime();

void rateApp();
#endif
