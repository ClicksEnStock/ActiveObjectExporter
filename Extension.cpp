
#include "Common.h"


///
/// EXTENSION CONSTRUCTOR/DESTRUCTOR
///

Extension::Extension(LPRDATA _rdPtr, LPEDATA edPtr, fpcob cobPtr)
    : rdPtr(_rdPtr), rhPtr(_rdPtr->rHo.hoAdRunHeader), Runtime(_rdPtr)
{

    /*
        Link all your action/condition/expression functions to their IDs to match the
        IDs in the JSON here
    */

	LinkAction(0, ExportAnimation);
	LinkAction(1, ExportAllAnimations);
	LinkAction(2, SetExportPath);
	LinkAction(3, SetFilename);

	LinkCondition(0, OnAnimationExported);

	LinkExpression(0, LastError);
	LinkExpression(1, NumberOfImageExported);
	LinkExpression(2, ExportPath);
	LinkExpression(3, FileName);

    /*
        This is where you'd do anything you'd do in CreateRunObject in the original SDK

        It's the only place you'll get access to edPtr at runtime, so you should transfer
        anything from edPtr to the extension class here.
    
    */
    isDebugLoggingEnabled = edPtr->isDebugLoggingEnabled!=0;

	exportFinished=false;
	exporting = false;
	numberOfImageExported=0;
	callRunTimeFunction(rdPtr, RFUNCTION_GETFILEINFOS, FILEINFO_PATH, (LPARAM)appPath);
	_snprintf_s(lastError, _countof(lastError), ERRORSIZE, ErrorS[noError]);
	_snprintf_s(errorTemp, _countof(errorTemp), ERRORSIZE, ErrorS[noError]);
	exportPath = ".";
	fileName = "object";
	metadataFile = HFILE_ERROR;
	if (isDebugLoggingEnabled)
	{
		AllocConsole();
		freopen("conin$", "r", stdin);
		freopen("conout$", "w", stdout);
		freopen("conout$", "w", stderr);
		printf("Debug enabled:\n");
	}
}

Extension::~Extension()
{
    /*
        This is where you'd do anything you'd do in DestroyRunObject in the original SDK.
        (except calling destructors and other such atrocities, because that's automatic in Edif)
    */
}


short Extension::Handle()
{
	return 0;
}


short Extension::Display()
{
    /*
       If you return REFLAG_DISPLAY in Handle() this routine will run.
    */

    // Ok
    return 0;
}

short Extension::Pause()
{

    // Ok
    return 0;
}

short Extension::Continue()
{

    // Ok
    return 0;
}

bool Extension::Save(HANDLE File)
{
	bool OK = false;

    #ifndef VITALIZE

	    // Save the object's data here

	    OK = true;

    #endif

	return OK;
}

bool Extension::Load(HANDLE File)
{
	bool OK = false;

    #ifndef VITALIZE

	    // Load the object's data here

	    OK = true;

    #endif

	return OK;
}


// These are called if there's no function linked to an ID

void Extension::Action(int ID, LPRDATA rdPtr, long param1, long param2)
{

}

long Extension::Condition(int ID, LPRDATA rdPtr, long param1, long param2)
{

    return false;
}

long Extension::Expression(int ID, LPRDATA rdPtr, long param)
{

    return 0;
}

