
#include "Common.h"

void Extension::ExportAllAnimations(LPRO pObj)
{
	if (!exporting)// avoid reentrant as long as exporting
	{
		printf("%d: ExportAllAnimations function entry\n", GetTickCount());
		try{
			if (pObj != NULL)
			{
				LPOC ocPtr = pObj->roHo.hoCommon;
				LPAH ahPtr = (LPAH)((LPBYTE)ocPtr + ocPtr->ocAnimations);
				if (!CreateMetadata()){ return; }
				exporting = true;
				exportFinished = false;
				numberOfImageExported = 0;
				for (int nAnim = 0; nAnim < ahPtr->ahAnimMax; nAnim++)
				{
					ExportOneAnimation(pObj, nAnim);
				}
				CloseMetadata();
				exportFinished = true;
			}
		}
		catch (exception ex)
		{
			printf("%d: ExportAllAnimations: %s\n", GetTickCount(), ex.what());
		}
		exporting = false;
	}
}

bool Extension::CreateMetadata()
{
	string eventFilename = appPath + exportPath + "load_" + fileName + ".eve";
	if (isDebugLoggingEnabled)
	{
		printf("%d: create metadata file in %s\n", GetTickCount(), eventFilename.c_str());
	}
	metadataFile = File_Create(eventFilename.c_str());
	if (metadataFile == HFILE_ERROR)
	{
		_snprintf_s(lastError, _countof(lastError), ERRORSIZE, ErrorS[NoEventFile]);
		return false;
	}
	else
	{
		string header = "CreateObject;object_name;" + fileName;
		header += ";id;1000;owner_id;-1;xpix;-300;ypix;500;dir;0;layer;3;start_animation;0\r\n";
		File_Write(metadataFile, (void*)header.c_str(), header.length());
		return true;
	}
}


void Extension::CloseMetadata()
{
	if (metadataFile != HFILE_ERROR)
	{
		string foot = "DestroyObject;object_id;1000";
		File_Write(metadataFile, (void*)foot.c_str(), foot.length());
		// Close file
		File_Close(metadataFile);
		metadataFile == HFILE_ERROR;
	}
}


void Extension::ExportAnimation(LPRO pObj, int nAnim)
{
	if (!exporting)
	{
		if (pObj != NULL)
		{
			printf("%d: ExportAnimation function entry\n", GetTickCount());
			if (!CreateMetadata()){ return; }
			exporting = true;
			exportFinished = false;
			numberOfImageExported = 0;
			ExportOneAnimation(pObj, nAnim);
			CloseMetadata();
			exportFinished = true;
			exporting = false;
		}
	}
}

void Extension::ExportOneAnimation(LPRO pObj, int nAnim)
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
			printf("%d: ExportAnimation failed, AnimationNumberOutOfBound.\n", GetTickCount());
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
					for (int nFrame = 0; nFrame < adPtr->adNumberOfFrame; nFrame++)
					{
						WORD imageNumber = (WORD)adPtr->adFrame[nFrame];
						if (LockImageSurface(pObj->roHo.hoAdRunHeader->rhIdAppli, imageNumber, sprite))
						{
							string name = fileName;
							name += "_anim" + std::to_string(nAnim);
							name += "_dir" + std::to_string(nDir);
							name += "_img" + std::to_string(nFrame);
							string fullName = appPath + exportPath + name;
							CImageFilterMgr* pImgMgr = rhPtr->rh4.rh4Mv->mvImgFilterMgr;
							bool res = ExportImage(pImgMgr, (fullName+".png").c_str(), &sprite, FILTERID_PNG);
							if (!res)
							{
								printf("%d: export image %s failed.\n", GetTickCount(), (fullName + ".png").c_str());
							}
							Img ifo;
							GetImageInfos(pObj->roHo.hoAdRunHeader->rhIdAppli, imageNumber, &ifo);
							string sX = std::to_string(ifo.imgXSpot);
							string sY = std::to_string(ifo.imgYSpot);
							string sXact = std::to_string(ifo.imgXAction);
							string sYact = std::to_string(ifo.imgYAction);
							// Write metadata to event file
							string loadingCommand = "LoadSprite;object_id;1000;path;" + exportPath + name + ".png";
							loadingCommand += ";animation;" + std::to_string(nAnim) + ";direction;" + std::to_string(nDir) + ";image;" + std::to_string(nFrame);
							loadingCommand += ";hot_spot_x;" + sX + ";hot_spot_y;" + sY + ";action_point_x;" + sXact + ";action_point_y;" + sYact + "\r\n";
							if(isDebugLoggingEnabled)
							{
								printf("%d: loading command = %s\n", GetTickCount(), loadingCommand.c_str());
							}
							File_Write(metadataFile, (void*)loadingCommand.c_str(), loadingCommand.length());
							UnlockImageSurface(sprite);
						}
						numberOfImageExported++;
						if (isDebugLoggingEnabled)
						{
							printf("%d: export progress = %s\n", GetTickCount(), std::to_string(numberOfImageExported).c_str());
						}
					}
				}
			}
		}
		else
		{
			// animation does not exist
			printf("%d: Animation %d does not exist\n", GetTickCount(), nAnim);
		}
	}
	else
	{
		_snprintf_s(lastError, _countof(lastError), ERRORSIZE, ErrorS[ActiveUnknown]);
		printf("%d: ExportAnimation, object null or unkown\n", GetTickCount());
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
