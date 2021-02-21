
#include "Common.h"

TCHAR* Extension::LastError()
{

	return lastError;
}

TCHAR* Extension::ExportPath()
{
	TCHAR res[_MAX_PATH];
	_tcscpy_s(res, CA2T(exportPath.c_str()));
	return res;
}

TCHAR* Extension::FileName()
{
	TCHAR res[_MAX_PATH];
	_tcscpy_s(res, CA2T(fileName.c_str()));
	return res;
}

int Extension::NumberOfImageExported()
{

	return numberOfImageExported;
}

