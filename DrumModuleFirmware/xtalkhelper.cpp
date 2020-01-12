#include "xtalkhelper.h"

unsigned long XTalkHelper::lastHitMillis_ = 0;
int XTalkHelper::lastHitValue_ = 0;

void XTalkHelper::setup()
{
}

bool XTalkHelper::checkNotCrossTalk(unsigned long hitMillis, int hitValue)
{
	if ((hitMillis - lastHitMillis_) > timeWindow_) return true;
	if (lastHitValue_ / hitValue < factor_) return true;

	lastHitMillis_ = hitMillis;
	lastHitValue_ = hitValue;
	return false;
}