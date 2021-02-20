
#include "Common.h"


void Extension::ExportAnimation(LPRO pObj, int nAnim)
{
	if (pObj != NULL)
	{
		cSurface sprite;
		//to get animations
		LPOC ocPtr = pObj->roHo.hoCommon;
		LPAH ahPtr = (LPAH)((LPBYTE)ocPtr + ocPtr->ocAnimations);
		//if given n of animation bigger than number of animation, return with error
		if (nAnim > ahPtr->ahAnimMax)
		{
			// out of bound animation number
			_snprintf_s(lastError, _countof(lastError), ERRORSIZE, ErrorS[AnimationNumberOutOfBound]);
			return;
		}
		int nAnimOffset = ahPtr->ahOffsetToAnim[nAnim];
		if (nAnimOffset > 0) // if animation exists
		{
			LPAN anPtr = (LPAN)((LPBYTE)ahPtr + nAnimOffset);
			// anPtr contains a table of 32 offsets, one per direction.
			// At startup, the runtime fills in unknown values with the offsets 
			// of the closest directions (in editor only some of the directions are defined),
			// therefore all 32 offsets have valid values, greater than 0 (to be checked...). 
			for (int nDir = 0; nDir < 32; nDir++)
			{
				int nDirOffset = anPtr->anOffsetToDir[nDir];
				if (nDirOffset > 0)
				{
					LPAD adPtr = (LPAD)((LPBYTE)anPtr + nDirOffset);
					exportFinished = false;
					exportProgress = 0;
					for (int nFrame = 0; nFrame < adPtr->adNumberOfFrame; nFrame++)
					{
						WORD imageNumber = (WORD)adPtr->adFrame[nFrame];
						if (LockImageSurface(pObj->roHo.hoAdRunHeader->rhIdAppli, imageNumber, sprite))
						{
							string name = fileName;
							name += "_anim" + std::to_string(nAnim);
							name += "_dir" + std::to_string(nDir);
							name += "_img" + std::to_string(nFrame);
							string fullName = exportPath + "\\" + name;
							int currentSystemTime = GetTickCount();
							printf("%d: export anim to %s\n", currentSystemTime, (fullName + ".png").c_str());
							CImageFilterMgr* pImgMgr = rhPtr->rh4.rh4Mv->mvImgFilterMgr;
							bool res = ExportImage(pImgMgr, (fullName+".png").c_str(), &sprite, FILTERID_PNG);
							UnlockImageSurface(sprite);
						}
					}
				}
			}
			exportFinished = true;
		}
		else
		{
			// animation does not exist
		}
	}
	else
	{
		_snprintf_s(lastError, _countof(lastError), ERRORSIZE, ErrorS[ActiveUnknown]);
	}
}

void Extension::SetExportPath(TCHAR* path)
{
	string ws = string(path);
	exportPath = ws;
}

void Extension::SetFilename(TCHAR* path)
{
	string ws = string(path);
	fileName = ws;
}
