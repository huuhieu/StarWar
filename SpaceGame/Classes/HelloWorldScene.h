/**************************************************************
 *
 * ___HelloWorld.h___
 * ___SpaceGame___
 *
 * Create by ___DuongHuuHieu___
 *
 * Copyright Vinova Pte. Ltd. All right reserved.
 *************************************************************
 */

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "CCParallaxNodeExtras.h"

USING_NS_CC;

class HelloWorld : public cocos2d::CCLayer
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();

    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
  
    virtual void didAccelerate(CCAcceleration* pAccelerationValue);
  
    float randomValueBetween(float pLow, float pHigh);
    void setInvisible(CCNode* node);
    float getTimeTick();
    virtual void ccTouchesBegan(cocos2dx_ccTypeInfo_h)

    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(HelloWorld);
  
private:
    cocos2d::CCSpriteBatchNode* mBatchNote;
    cocos2d::CCSprite* mShip;               
    CCParallaxNodeExtras* mBackgroundNode;  
    CCSprite* mSpaceDust1;                  
    CCSprite* mSpaceDust2;                  
    CCSprite* mPlanetSunrise;               
    CCSprite* mGalaxy;                      
    CCSprite* mSpacialAnomaly;
    CCSprite* mSpacialAnomaly2;
    CCArray* mAsteroids;
    CCArray* mShipLasers;
    int mNextShipLasers;
    int mNextASteroids;
    float mNextAsteroidSpawn;
    float mShipPointsPerSecY;
    void update(float pDt);
    
};

#endif // __HELLOWORLD_SCENE_H__
