//
//  GameSettings.cpp
//  GalacticMice
//
//  Created by Lucky
//
//

#include "GameSettings.h"

float getX(int x)
{
    float fx=0;
    
    fx = G_SWIDTH * x / G_OWIDTH;
    return fx;
}

float getY(int y)
{
    float fy=0;
    fy = G_SHEIGHT - G_SHEIGHT * y / G_OHEIGHT;
    return fy;
}

Sprite *newSprite(string sName, float x, float y,Node *target, int zOrder, Ratio nRatio)
{
    Sprite *sprite = Sprite::create("res/Images/" + sName + ".png");
    setScale(sprite, nRatio);
    
    sprite->setPosition(Point(x, y));
    target->addChild(sprite ,zOrder);
    return sprite;
}



MenuItemImage *newButtonGrow(string fileName, float x, float y, ccMenuCallback target, Ratio nRatio)
{
    MenuItemImage *item;
    
    item =MenuItemImage::create("res/Images/" + fileName + ".png", "res/Images/" + fileName + ".png", target);
    
    setScale(item, nRatio);
    item->setPosition(Point(x, y));
    
    return item;
}

MenuItemImage *newButton(string fileName, float x, float y, ccMenuCallback target, Ratio nRatio)
{
    MenuItemImage *item;
    
    item = MenuItemImage::create("res/Images/" + fileName + ".png", "res/Images/" + fileName + ".png", target);
    
    item->getSelectedImage()->setOpacity(200);
    //item->getDisabledImage()->setColor(Color3B(item->getNormalImage()->getColor().r-100,item->getNormalImage()->getColor().g-100,item->getNormalImage()->getColor().b-100));
    
    setScale(item, nRatio);
    item->setPosition(Point(x, y));
    
    return item;
}

MenuItemLabel *newLabelButton(string sText, string sFont, Color4B color, float size, float x, float y, ccMenuCallback target, Ratio nRatio){
    Label* label=Label::createWithTTF(sText, "fonts/" + sFont, size);
    label->setTextColor(color);
    MenuItemLabel *item = MenuItemLabel::create(label, target);
    item->setPosition(x, y);
    setScale(item, nRatio);
    return item;
}

MenuItemImage *newButtonOn(string fileName, float x, float y, ccMenuCallback target, Ratio nRatio)
{
    MenuItemImage *item;
    
    item =MenuItemImage::create("res/Images/" + fileName + ".png", "res/Images/" + fileName + "_on.png", "res/Images/" + fileName + "_on.png", target);
    
    setScale(item, nRatio);
    item->setPosition(Point(x, y));
    
    return item;
}

MenuItemToggle *newToggleButton(string btnNameNormal, string btnNameSelect, float x, float y, ccMenuCallback target, Ratio nRatio)
{
    MenuItemToggle *item;
    MenuItemImage *itemOn, *itemOff;
    itemOn = MenuItemImage::create("res/Images/" + btnNameNormal + ".png", "res/Images/" + btnNameNormal + ".png");
    
    //itemOn->getSelectedImage()->setColor(Color3B(itemOn->getNormalImage()->getColor().r-100,itemOn->getNormalImage()->getColor().g-100,itemOn->getNormalImage()->getColor().b-100));
    
    itemOff = MenuItemImage::create("res/Images/" + btnNameSelect + ".png", "res/Images/" + btnNameSelect + ".png");
    //itemOff->getSelectedImage()->setColor(Color3B(itemOff->getNormalImage()->getColor().r-100,itemOff->getNormalImage()->getColor().g-100,itemOff->getNormalImage()->getColor().b-100));
    
    item = MenuItemToggle::createWithCallback(target,itemOn, itemOff, NULL);
    setScale(item, nRatio);
    item->setPosition(Point(x, y));
    return item;
}

MenuItemLabel *newLabelButton(string sText, string fontName, float size, float x, float y, ccMenuCallback target, Ratio nRatio)
{
    MenuItemLabel *item;
    Label *label = Label::createWithTTF(sText, "fonts/" + fontName, size);
    item = MenuItemLabel::create(label, target);
    
    setScale(item, nRatio);
    item->setPosition(Point(x, y));
    return item;
}

MenuItemToggle *newToggleLabelButton(string sOn, string sOff, string fontName, float size, float x, float y, ccMenuCallback target, Ratio nRatio)
{
    MenuItemLabel *itemOn = MenuItemLabel::create(Label::createWithTTF(sOn, "fonts/" + fontName, size));
    MenuItemLabel *itemOff = MenuItemLabel::create(Label::createWithTTF(sOff, "fonts/" + fontName, size));
    
    MenuItemToggle *item = MenuItemToggle::createWithCallback(target, itemOn, itemOff, NULL);
    
    setScale(item, nRatio);
    item->setPosition(Point(x, y));
    return item;
}
Label* newLabel(string txt, string fontName, float size, float x, float y, Node* target, int zOrder,Ratio nRatio){
    //auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    Label* label=Label::createWithTTF(txt, "fonts/" + fontName, size);
    label->setPosition(Point(x, y));
    target->addChild(label, zOrder);
    setScale(label, nRatio);
    
    return label;
}

Animation *newAnimation(string sName, int nCnt, float delayUnit){
    Animation *ani = Animation::create();
    ani->retain();
    for(int i = 0; i < nCnt; i++){
        string name = "res/Images/ani/" + sName + toString(i) + ".png";
        ani->addSpriteFrameWithFile(name);
    }
    ani->setDelayPerUnit(delayUnit);
    return ani;
}

void setScale(Node *node, Ratio nRatio)
{    
    if(nRatio == RATIO_XY)
    {
        node->setScaleX(G_SCALEX);
        node->setScaleY(G_SCALEY);
    }
    else if(nRatio == RATIO_X)
        node->setScale (G_SCALEX);
    else if(nRatio == RATIO_Y)
        node->setScale(G_SCALEY);
    else if(nRatio == RATIO_O)
        node->setScale(G_SCALEO);
    else if(nRatio == RATIO_1)
        node->setScale(1);
}

ParticleSystemQuad * showParticle(string filename, Node *target, Point pos, int zOrder, Ratio nRatio)
{
    ParticleSystemQuad *particle =ParticleSystemQuad::create(filename);
    particle->setPosition(pos);
    setScale(particle,nRatio);
    target->addChild(particle,zOrder);
    return particle;
}

void playEffect(string sName){
    if(g_bSoundOn){
        SimpleAudioEngine::getInstance()->playEffect(("res/Audio/" + sName).c_str());
    }
}
