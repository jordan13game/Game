#include "Box.h"

USING_NS_CC;


// on "init" you need to initialize your instance
bool Box::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCNode::init() )
    {
        return false;
    }
	pSprite = NULL;
    combo_type = 0;
	_type = -1;
    return true;
}



