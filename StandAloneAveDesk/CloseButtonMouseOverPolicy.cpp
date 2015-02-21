// CloseButtonMouseOverPolicy.cpp: implementation of the CCloseButtonMouseOverPolicy class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CloseButtonMouseOverPolicy.h"
#include "Docklet.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCloseButtonMouseOverPolicy::CCloseButtonMouseOverPolicy()
{

}

CCloseButtonMouseOverPolicy::~CCloseButtonMouseOverPolicy()
{

}

bool CCloseButtonMouseOverPolicy::shouldShow(Docklet* d)
{
	return d->GetMouseIsOver();
}

bool CCloseButtonMouseOverPolicy::shouldHide(Docklet* d)
{
	return !d->GetMouseIsOver();
}