//
//  CCParallaxNodeExtras.h
//  SpaceGame
//
//  Created by MAC on 7/26/13.
//
//

#ifndef __SpaceGame__CCParallaxNodeExtras__
#define __SpaceGame__CCParallaxNodeExtras__

#include "cocos2d.h"

USING_NS_CC;

class CCParallaxNodeExtras : public CCParallaxNode
{
public:
    //Need to provide a constructor
  CCParallaxNodeExtras();
  
  static CCParallaxNodeExtras* node();
  
  void incrementOffset(CCPoint pOffset, CCNode* pNode);
};

#endif /* defined(__SpaceGame__CCParallaxNodeExtras__) */
