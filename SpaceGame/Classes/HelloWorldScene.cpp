/**************************************************************
 *
 * ___HelloWorld.cpp___
 * ___SpaceGame___
 *
 * Create by ___DuongHuuHieu___
 *
 * Copyright Vinova Pte. Ltd. All right reserved.
 *************************************************************
 */

#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }

  //add spaceFilier
  mBatchNote = CCSpriteBatchNode::create("Sprites.pvr.ccz");
  this->addChild(mBatchNote);
  CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Sprites.plist");
  mShip = CCSprite::createWithSpriteFrameName("SpaceFlier_sm_1.png");
  CCSize winSize = CCDirector::sharedDirector()->getWinSize();
  mShip->setPosition(ccp(winSize.width*0.1, winSize.height*0.5));
  mBatchNote->addChild(mShip,1);
  
  //Create the parallaxNode
  mBackgroundNode = CCParallaxNodeExtras::node();
  this->addChild(mBackgroundNode,-1);
  
  //Create the sprites
  mSpaceDust1 = CCSprite::create("bg_front_spacedust.png");
  mSpaceDust2 = CCSprite::create("bg_front_spacedust.png");
  mPlanetSunrise = CCSprite::create("bg_planetsunrise.png");
  mGalaxy = CCSprite::create("bg_galaxy.png");
  mSpacialAnomaly = CCSprite::create("bg_spacialanomaly.png");
  mSpacialAnomaly2 = CCSprite::create("bg_spacialanomaly.png");
  
  //set movement speeds for dust and background
  CCPoint dustSpeed = ccp(0.1,0.1);
  CCPoint bgSpeed = ccp(0.05,0.05);
  
  //add children to parallaxNode
  mBackgroundNode->addChild(mSpaceDust1, 0, dustSpeed,
                            ccp(0,winSize.height/2));
  mBackgroundNode->addChild(mSpaceDust2, 0, dustSpeed,
                            ccp(mSpaceDust1->
                                getContentSize().width,winSize.height/2));
  mBackgroundNode->addChild(mGalaxy, -1, bgSpeed,
                            ccp(0, winSize.height*0.7));
  mBackgroundNode->addChild(mPlanetSunrise, -1, bgSpeed,
                            ccp(600, winSize.height*0));
  mBackgroundNode->addChild(mSpacialAnomaly, -1, bgSpeed,
                            ccp(900, winSize.height*0.3));
  mBackgroundNode->addChild(mSpacialAnomaly2, -1, bgSpeed,
                            ccp(1500, winSize.height*0.9));
  //call schedule
  this->scheduleUpdate();
  
  //add shining stars
  HelloWorld::addChild(CCParticleSystemQuad::create("Stars1.plist"));
  HelloWorld::addChild(CCParticleSystemQuad::create("Stars2.plist"));
  HelloWorld::addChild(CCParticleSystemQuad::create("Stars3.plist"));
  
  //create an array of asteroids
  #define KNUMASTEROIDS 15
  mAsteroids = new CCArray();
  for (int i=0; i<KNUMASTEROIDS; ++i)
  {
    CCSprite* asteroid = CCSprite::createWithSpriteFrameName("asteroid.png");
    asteroid->setVisible(false);
    mBatchNote->addChild(asteroid);
    mAsteroids->addObject(asteroid);
  }
  
  //enable Accelerometer
  this->setAccelerometerEnabled(true);
  return true;
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

//update position
void HelloWorld::update(float pDt)
{
  //scroll background
  CCPoint backgroundScrollVert = ccp(-1000, 0);
  mBackgroundNode->setPosition(ccpAdd(mBackgroundNode->getPosition(),
                                      ccpMult(backgroundScrollVert, pDt)));
  
  //repeat scroll backround
  CCArray* spaceDusts = CCArray::createWithCapacity(2);
  spaceDusts->addObject(mSpaceDust1);
  spaceDusts->addObject(mSpaceDust2);
  for (int ii=0; ii<spaceDusts->count(); ii++)
  {
    CCSprite* spaceDust = (CCSprite*)(spaceDusts->objectAtIndex(ii));
    float xPosition = mBackgroundNode->convertToWorldSpace(
                                       spaceDust->getPosition()).x;
    float size = spaceDust->getContentSize().width;
    if (xPosition<-size/2)
    {
      mBackgroundNode->incrementOffset(ccp(
                       spaceDust->getContentSize().width*2,0), spaceDust);
    }
  }
  CCArray* backGrounds = CCArray::createWithCapacity(4);
  backGrounds->addObject(mGalaxy);
  backGrounds->addObject(mPlanetSunrise);
  backGrounds->addObject(mSpacialAnomaly);
  backGrounds->addObject(mSpacialAnomaly2);
  for (int ii=0; ii<backGrounds->count(); ii++)
  {
    CCSprite* background = (CCSprite*)(backGrounds->objectAtIndex(ii));
    float xPosition = mBackgroundNode->convertToWorldSpace(
                                      background->getPosition()).x;
    float size = background->getContentSize().width;
    if (xPosition<-size)
    {
      mBackgroundNode->incrementOffset(ccp(2000,0), background);
    }
  }
  
  CCSize winSize = CCDirector::sharedDirector()->getWinSize();
  float maxY = winSize.height - mShip->getContentSize().height/2;
  float minY = mShip->getContentSize().height/2;
  
  float diff = (mShipPointsPerSecY * pDt);
  float newY = mShip->getPosition().y + diff;
  newY = MIN(MAX(newY, minY), maxY);
  mShip->setPosition(ccp(mShip->getPosition().x, newY));
  
  float curTimeMillis = getTimeTick();
  if (curTimeMillis > mNextAsteroidSpawn);
  {
  float randMillisecs = randomValueBetween(0.20,1.0) * 1000;
  mNextAsteroidSpawn = randMillisecs + curTimeMillis;
    
  float randY = randomValueBetween(0.0,winSize.height);
  float randDuration = randomValueBetween(2.0,10.0);
    
  CCSprite* asteroid = (CCSprite *)mAsteroids->objectAtIndex(mNextASteroids);
  mNextASteroids++;
    
  if (mNextASteroids >= mAsteroids->count())
  mNextASteroids = 0;
    
  asteroid->stopAllActions();
  asteroid->setPosition( ccp(winSize.width+asteroid->getContentSize().width/2,
                               randY));
  asteroid->setVisible(true);
  asteroid->runAction(CCSequence::create(
                      CCMoveBy::create(randDuration,
                      ccp(-winSize.width-asteroid->getContentSize().width, 0)),
                      CCCallFuncN::create(this,
                      callfuncN_selector(HelloWorld::setInvisible)),NULL));
  }
}

void HelloWorld::didAccelerate(CCAcceleration* pAccelerationValue)
{
#define KFILTERINGFACTOR 0.1
#define KRESTACCELX -0.6
#define KSHIPMAXPOINTSPERSEC (winSize.height*0.5)  
#define KMAXDIFFX 0.2
  
  double rollingX;
  
  pAccelerationValue->x = pAccelerationValue->y;
  rollingX = (pAccelerationValue->x * KFILTERINGFACTOR) +
              (rollingX * (1.0 - KFILTERINGFACTOR));
  float accelX = pAccelerationValue->x - rollingX;
  CCSize winSize = CCDirector::sharedDirector()->getWinSize();
  float accelDiff = accelX - KRESTACCELX;
  float accelFraction = accelDiff / KMAXDIFFX;
  mShipPointsPerSecY = KSHIPMAXPOINTSPERSEC * accelFraction;
}

//return random value between low and high
float HelloWorld::randomValueBetween(float pLow, float pHigh)
{
  return (((float)arc4random()/0xFFFFFFFFu)*(pHigh-pLow)*(pHigh-pLow)) + pLow;
}

//get the time in milliseconds
float HelloWorld::getTimeTick()
{
  timeval time;
  gettimeofday(&time, NULL);
  unsigned long millisecs = (time.tv_sec*1000) +
                             (time.tv_usec/1000);
  return (float) millisecs;
}

void HelloWorld::setInvisible(CCNode* node)
{
  node->setVisible(false);
}

