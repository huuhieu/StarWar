/**************************************************************
 *
 * ___HelloWorld.cpp___
 * ___SpaceGame___
 *
 * Create by ___DuongHuuHieu___
 *
 * Copyright Vinova Pte. Ltd. All right reserved.
 **************************************************************
 */

#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene* scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld* layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
  mLives = 3;
  double curTime = getTimeTick();
  mGameOverTime = curTime + 30000;
  
  //////////////////////////////
  // 1. super init first
  if ( !CCLayer::init() )
  {
    return false;
  }

  //add spaceFlier
  mBatchNote = CCSpriteBatchNode::create("Sprites.pvr.ccz");
  this->addChild(mBatchNote);
  CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(
                                              "Sprites.plist");
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
  
  //add shining stars
  HelloWorld::addChild(CCParticleSystemQuad::create("Stars1.plist"));
  HelloWorld::addChild(CCParticleSystemQuad::create("Stars2.plist"));
  HelloWorld::addChild(CCParticleSystemQuad::create("Stars3.plist"));
  
  //create an array of asteroids
  #define KNUMASTEROIDS 30
  mAsteroids = new CCArray();
  for (int i=0; i<KNUMASTEROIDS; ++i)
  {
    CCSprite* asteroid = CCSprite::createWithSpriteFrameName(
                                   "asteroid.png");
    asteroid->setVisible(false);
    mBatchNote->addChild(asteroid);
    mAsteroids->addObject(asteroid);
  }
  
  //create an array of laserbeam
  #define KNUMLASERS 30
  mShipLasers = new CCArray();
  for(int i=0; i<KNUMLASERS; ++i) {
    CCSprite* shipLaser = CCSprite::createWithSpriteFrameName(
                                    "laserbeam_blue.png");
    shipLaser->setVisible(false);
    mBatchNote->addChild(shipLaser);
    mShipLasers->addObject(shipLaser);
  }
  
  //add sound
  SimpleAudioEngine::sharedEngine()->playBackgroundMusic("SpaceGame.wav",true);
  SimpleAudioEngine::sharedEngine()->preloadEffect("explosion_large.wav");
  SimpleAudioEngine::sharedEngine()->preloadEffect("laser_ship.wav");
  
  this->setTouchEnabled(true);
  this->scheduleUpdate();
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
  
  //spawn and move asteroids
  float curTimeMillis = getTimeTick();
  if (curTimeMillis > mNextAsteroidSpawn)
  {
  float randMillisecs = randomValueBetween(0.20,1.0) * 1000;
  mNextAsteroidSpawn = randMillisecs + curTimeMillis;
    
  float randY = randomValueBetween(0.0,winSize.height);
  float randDuration = randomValueBetween(2.0,10.0);
    
  CCSprite* asteroid = (CCSprite *)mAsteroids->objectAtIndex(mNextASteroids);
  mNextASteroids++;
    
  if (mNextASteroids >= mAsteroids->count())
  {
  mNextASteroids = 0;
  }
    
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
  
  //Auto fire laserbeam
  SimpleAudioEngine::sharedEngine()->playEffect("laser_ship.wav");
	CCSprite* shipLaser = (CCSprite*)mShipLasers->objectAtIndex(mNextShipLasers);
  mNextShipLasers++;
	if( mNextShipLasers >= mShipLasers->count())
  {
  mNextShipLasers = 0;
  }
  shipLaser->stopAllActions();
	shipLaser->setPosition( ccpAdd(mShip->getPosition(),
                                 ccp(shipLaser->getContentSize().width/2, 0)));
  shipLaser->setVisible(true);
  shipLaser->runAction(CCSequence::create(
                                   CCMoveBy::create(0.5,ccp(winSize.width, 0)),
                                   CCCallFuncN::create(this,
                                   callfuncN_selector(HelloWorld::setInvisible)),
                                   NULL));
  
  //destroy asteroids
  CCObject* asteroid;
  CCObject* mshipLaser;
  CCARRAY_FOREACH(mAsteroids, asteroid){
    if (!((CCSprite *) asteroid)->isVisible() )
    {
      continue;
    }
    CCARRAY_FOREACH(mShipLasers, mshipLaser){
      if (!((CCSprite *) mshipLaser)->isVisible())
      {
        continue;
      }
      if (((CCSprite *) mshipLaser)->boundingBox().intersectsRect(((
                                    CCSprite *)asteroid)->boundingBox()))
      {
        ((CCSprite *)mshipLaser)->setVisible(false);
        ((CCSprite *)asteroid)->setVisible(false);
        SimpleAudioEngine::sharedEngine()->playEffect("explosion_large.wav");
        continue;
      }
    }
    if (mShip->boundingBox().intersectsRect(((
                             CCSprite *)asteroid)->boundingBox()) )
    {
      ((CCSprite *)asteroid)->setVisible(false);
      mShip->runAction(CCBlink::create(1.0, 9));
      mLives--;
    }
  }
  
  if (mLives <= 0)
  {
    mShip->stopAllActions();
    mShip->setVisible(false);
    this->endScene(KENDREASONLOSE);
  } else if (curTimeMillis >= mGameOverTime)
  {
    this->endScene(KENDREASONWIN);
  }
}

//return random value between low and high
float HelloWorld::randomValueBetween(float pLow, float pHigh)
{
  return (((float)arc4random()/0xFFFFFFFFu)*(pHigh-pLow)) + pLow;
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

//Moving the ship
void HelloWorld::ccTouchesMoved(cocos2d::CCSet* pTouches,
                                cocos2d::CCEvent* pEvent)
{
	CCTouch* touch = (CCTouch*) pTouches->anyObject();
	CCPoint dt = touch->getLocationInView();
  
  CCSize winSize = CCDirector::sharedDirector()->getWinSize();
  float maxY = winSize.height - mShip->getContentSize().height/2;
  float minY = mShip->getContentSize().height/2;
  
  float diff = (mShipPointsPerSecY * dt.y);
  float newY = mShip->getPosition().y + diff;
  newY = MIN(MAX(newY, minY), maxY);
  
	mShip->setPosition(ccp(mShip->getPosition().x,
                         winSize.height - dt.y));
  
}

void HelloWorld::restartTapped()
{
  CCDirector::sharedDirector()->replaceScene
  (CCTransitionZoomFlipX::create(0.5, this->scene()));
  // reschedule
  this->scheduleUpdate();
}

//End Scene
void HelloWorld::endScene(eEndReason endReason)
{
  if (isGameOver)
  {
  return;
  }
  isGameOver = true;
  
  CCSize winSize = CCDirector::sharedDirector()->getWinSize();
  char message[10] = "You Win";
  if ( endReason == KENDREASONLOSE)
  {
    strcpy(message,"You Lose");
  }
  CCLabelBMFont* label ;
  label = CCLabelBMFont::create(message, "Arial.fnt");
  label->setScale(0.1);
  label->setPosition(ccp(winSize.width/2 , winSize.height*0.6));
  this->addChild(label);
  
  CCLabelBMFont* restartLabel;
  strcpy(message,"Restart");
  restartLabel = CCLabelBMFont::create(message, "Arial.fnt");
  CCMenuItemLabel *restartItem = CCMenuItemLabel::create(restartLabel,
                                                  this, menu_selector(
                                                  HelloWorld::restartTapped));
  restartItem->setScale(0.1);
  restartItem->setPosition(ccp(winSize.width/2, winSize.height*0.4));
  
  CCMenu* menu = CCMenu::create(restartItem, NULL);
  menu->setPosition(CCPointZero);
  this->addChild(menu);
  
  //Clear label and menu
  restartItem->runAction(CCScaleTo::create(0.5, 1.0));
  label ->runAction(CCScaleTo::create(0.5, 1.0));
  this->unscheduleUpdate();
}
