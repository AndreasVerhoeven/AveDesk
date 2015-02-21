// BehaviourFactory.cpp: implementation of the CBehaviourFactory class.
//
//////////////////////////////////////////////////////////////////////

#include "BehaviourFactory.h"
#include "pidl.h"

#include "ShellItemBehaviour.h"
#include "MainRootItemBehaviour.h"
#include "RecycleBinBehaviour.h"
#include "FolderBehaviour.h"
#include "NetworkConnectionBehaviour.h"
#include "DriveBehaviour.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBehaviourFactory::CBehaviourFactory()
{

}

CBehaviourFactory::~CBehaviourFactory()
{

}

#define TEST_BEHAVIOUR_FOR_PIDL(n)							\
	if( n::CanHandlePidl(pidl,fileName) )	\
	return apBehaviour(new n(desklet));

std::auto_ptr<CBehaviour> CBehaviourFactory::CreateBehaviourForPidl(CPidl& pidl, std::wstring fileName, IShortcutDesklet* desklet)
{
	typedef std::auto_ptr<CBehaviour> apBehaviour;

	TEST_BEHAVIOUR_FOR_PIDL(CNetworkConnectionBehaviour);
	TEST_BEHAVIOUR_FOR_PIDL(CMainRootItemBehaviour);
	TEST_BEHAVIOUR_FOR_PIDL(CRecycleBinBehaviour);
	TEST_BEHAVIOUR_FOR_PIDL(CFolderBehaviour);
	TEST_BEHAVIOUR_FOR_PIDL(CDriveBehaviour);
	TEST_BEHAVIOUR_FOR_PIDL(CShellItemBehaviour);

	return apBehaviour(NULL);
}
