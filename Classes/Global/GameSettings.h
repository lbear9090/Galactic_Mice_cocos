//
//  GameSettings.h
//  GalacticMice
//
//  Created by Lucky
//
//

#ifndef __GameSettings__
#define __GameSettings__
#include "GameConfig.h"
using namespace std;
using namespace StringUtils;
#define G_OWIDTH (813.0f)
#define G_OHEIGHT (375.0f)

#define G_SWIDTH    (Director::getInstance()->getVisibleSize().width)  //Screen width
#define G_SHEIGHT   (Director::getInstance()->getVisibleSize().height)   //Screen height

#define G_SCALEX    (G_SWIDTH / G_OWIDTH)
#define G_SCALEY    (G_SHEIGHT / G_OHEIGHT)
#define G_SCALEO    (G_SCALEX<G_SCALEY?G_SCALEX:G_SCALEY)

#define DISX(X)  ((X) * (G_SCALEX))
#define DISY(Y)  ((Y) * (G_SCALEY))
#define DISO(O)  ((O) * (G_SCALEO))

typedef enum
{
    RATIO_XY = 0,
    RATIO_X,
    RATIO_Y,
    RATIO_O,
    RATIO_1,
}Ratio;

float getX(int x);
float getY(int y);

Sprite *newSprite(string sName, float x, float y, Node* target, int zOrder, Ratio nRatio);

MenuItemImage *newButtonGrow(string fileName, float x, float y, ccMenuCallback target, Ratio nRatio);
MenuItemImage *newButton(string fileName, float x, float y, ccMenuCallback target, Ratio nRatio);
MenuItemImage *newButtonOn(string fileName, float x, float y, ccMenuCallback target, Ratio nRatio);
MenuItemLabel *newLabelButton(string sText, string fontName, float size, float x, float y, ccMenuCallback target, Ratio nRatio);

MenuItemToggle *newToggleButton(string btnNameNormal,string btnNameSelect, float x, float y, ccMenuCallback target, Ratio nRatio);
MenuItemToggle *newToggleLabelButton(string sOn, string sOff, string fontName, float size, float x, float y, ccMenuCallback target, Ratio nRatio);

Label* newLabel(string txt, string fontName,float size, float x, float y, Node* target, int zOrder,Ratio nRatio);
Animation *newAnimation(string sName, int nCnt, float delayUnit);

void setScale(Node *node, Ratio nRatio);
ParticleSystemQuad* showParticle(string filename, Node *target, Point pos, int zOrder, Ratio nRatio);

void playEffect(string sName);
#endif
