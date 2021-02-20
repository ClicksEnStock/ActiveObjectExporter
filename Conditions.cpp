
#include "Common.h"


bool Extension::OnAnimationExported()
{
	bool res = exportFinished;
	exportFinished = false;
	return res;
}

