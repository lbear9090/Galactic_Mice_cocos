#include "AppDelegate.h"
#include "MainScene.h"
#include "GameScene.h"
#include "GameSettings.h"
#include "firebase/app.h"
#include "firebase/admob.h"

//#define USE_AUDIO_ENGINE 1
#define USE_SIMPLE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE && USE_SIMPLE_AUDIO_ENGINE
#error "Don't use AudioEngine and SimpleAudioEngine at the same time. Please just select one in your game!"
#endif

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#elif USE_SIMPLE_AUDIO_ENGINE
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;
#endif

USING_NS_CC;

//static cocos2d::Size designResolutionSize = cocos2d::Size(480, 320);
//static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
//static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
//static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate() 
{
#if USE_AUDIO_ENGINE
    AudioEngine::end();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::end();
#endif
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,  
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    // Initialize Firebase for Android.
    firebase::App* app = firebase::App::Create(firebase::AppOptions(), JniHelper::getEnv(), JniHelper::getActivity());
    // Initialize AdMob.
    firebase::admob::Initialize(*app, "ca-app-pub-1187817622415050~1956758661");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    // Initialize Firebase for iOS.
    firebase::App* app = firebase::App::Create(firebase::AppOptions());
    // Initialize AdMob.
    firebase::admob::Initialize(*app, "ca-app-pub-1187817622415050~6274817202");
#endif
    // Initialize AdMob.
    //firebase::admob::Initialize(*app);

    
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("GalacticMice", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("GalacticMice");
#endif
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);

    // Set the design resolution
    /*glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
    auto frameSize = glview->getFrameSize();
    // if the frame's height is larger than the height of medium size.
    if (frameSize.height > mediumResolutionSize.height)
    {        
        director->setContentScaleFactor(MIN(largeResolutionSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));
    }
    // if the frame's height is larger than the height of small size.
    else if (frameSize.height > smallResolutionSize.height)
    {        
        director->setContentScaleFactor(MIN(mediumResolutionSize.height/designResolutionSize.height, mediumResolutionSize.width/designResolutionSize.width));
    }
    // if the frame's height is smaller than the height of medium size.
    else
    {        
        director->setContentScaleFactor(MIN(smallResolutionSize.height/designResolutionSize.height, smallResolutionSize.width/designResolutionSize.width));
    }*/

    register_all_packages();
    loadGameInfo();
    createAnimation();
    // create a scene. it's an autorelease object
    auto scene = MainScene::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

void AppDelegate::createAnimation(){
    g_aniExplode = newAnimation("ex_", 4, 0.1f);
    g_aniMeteor = newAnimation("m_", 2, 0.3f);
    g_aniUFO = newAnimation("ufo_", 2, 0.3f);
    g_aniTongue = newAnimation("t_", 4, 0.03f);
    g_aniStun = newAnimation("st_", 4, 0.1f);
    g_aniSpark = newAnimation("sp_", 3, 0.1);
    for(int i = 0; i < MICE_CNT; i++){
        g_aniMice[i] = newAnimation("mice" + toString(i) + "_", 2, 0.3f);
    }
}

void AppDelegate::preloadEffects(){
    SimpleAudioEngine::getInstance()->preloadBackgroundMusic("res/Audio/bgm.mp3");
    SimpleAudioEngine::getInstance()->preloadEffect("res/Audio/levelup.wav");
    SimpleAudioEngine::getInstance()->preloadEffect("res/Audio/extralife.wav");
    SimpleAudioEngine::getInstance()->preloadEffect("res/Audio/gameover.wav");
    SimpleAudioEngine::getInstance()->preloadEffect("res/Audio/jump.wav");
    SimpleAudioEngine::getInstance()->preloadEffect("res/Audio/lasertouch.wav");
    SimpleAudioEngine::getInstance()->preloadEffect("res/Audio/micecapture.wav");
    SimpleAudioEngine::getInstance()->preloadEffect("res/Audio/miceescape.wav");
    SimpleAudioEngine::getInstance()->preloadEffect("res/Audio/perkcapture.wav");
    SimpleAudioEngine::getInstance()->preloadEffect("res/Audio/perklaserhit.wav");
    SimpleAudioEngine::getInstance()->preloadEffect("res/Audio/playerhit.wav");
    SimpleAudioEngine::getInstance()->preloadEffect("res/Audio/shootlaser.wav");
    SimpleAudioEngine::getInstance()->preloadEffect("res/Audio/tongue.wav");
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
    
    if(g_layerGame){
        ((GameScene *)g_layerGame)->setPause(true);
    }
#if USE_AUDIO_ENGINE
    AudioEngine::pauseAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    SimpleAudioEngine::getInstance()->pauseAllEffects();
#endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    SimpleAudioEngine::getInstance()->resumeAllEffects();
#endif
}
