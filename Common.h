#pragma once

// #define TGFEXT       // TGF2, MMF2, MMF2 Dev
 #define MMFEXT       // MMF2, MMF2 Dev
// #define PROEXT       // MMF2 Dev only

#include    "Edif.h"
#include	"Resource.h"
#include	"atlstr.h"

// edPtr : Used at edittime and saved in the MFA/CCN/EXE files

struct EDITDATA
{
	// Header - required
	extHeader		eHeader;

	// Object's data
	short		isDebugLoggingEnabled;

};

#define MAX_STRING          256
#define MAX_ERROR_STRING          2048

enum ErrorN
{
	noError,
	ActiveUnknown,
	AnimationNumberOutOfBound,
	NoEventFile
};

static TCHAR* ErrorS[50] = {
	_T("[00] No Error"),
	_T("[01] Export failed: Active Unknown."),
	_T("[02] Export failed: Animation Number Out Of Bound."),
	_T("[03] Export failed: Cannot create event file."),
	
};

#include "Extension.h"
