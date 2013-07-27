/**************************************************************
 *
 * ___CCParallaxNodeExtras___
 * ___SpaceGame___
 *
 * Create by ___DuongHuuHieu___
 *
 * Copyright Vinova Pte. Ltd. All right reserved.
 **************************************************************
 */

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
