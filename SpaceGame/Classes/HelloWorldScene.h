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
enum eEndReason
{
    KENDREASONWIN,
    KENDREASONLOSE
};
  
public:
    // Method 'init' in cocos2d-x returns bool,
    //instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();

    // there's no 'id' in cpp, so we recommend
    // o return the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
  
    float randomValueBetween(float pLow, float pHigh);
    void setInvisible(CCNode* node);
    float getTimeTick();
    void ccTouchesMoved(cocos2d::CCSet* pTouches, cocos2d::CCEvent* pEvent);

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
    int mLives;
    float mNextAsteroidSpawn;
    float mShipPointsPerSecY;
    double mGameOverTime;
    bool isGameOver;
    void endScene(eEndReason pEndReason);
    void restartTapped();
    void update(float pDt);
    
};

#endif // __HELLOWORLD_SCENE_H__
