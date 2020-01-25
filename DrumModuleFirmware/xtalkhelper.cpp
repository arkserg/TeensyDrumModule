#include "xtalkhelper.h"

unsigned long XTalkHelper::lastHitMillis_ = 0;
int XTalkHelper::lastHitValue_ = 0;

void XTalkHelper::setup()
{
}

bool XTalkHelper::isCrossTalk(unsigned long hitMillis, int hitValue, int xTalkFactor)
{
	if ((hitMillis - lastHitMillis_) > timeWindow_
		|| ((100 * hitValue) / lastHitValue_) >= xTalkFactor)
	{
		lastHitMillis_ = hitMillis;
		lastHitValue_ = hitValue;
		return false;
	}

	return true;
}