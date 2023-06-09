// dui-demo.cpp : main source file
//

#include "stdafx.h"
#include <quickjs.h>
#include <qjsbind.h>
#include "exctrls/SProgressRing.h"
#include "exctrls/SRatingBar.h"
#include "exctrls/SAniWindow.h"
#include "exctrls/SChatEdit.h"
#include "exctrls/SCheckBox2.h"
#include "exctrls/SGroupList.h"
#include "exctrls/SHexEdit.h"
#include "exctrls/SRoundWnd.h"
#include "exctrls/SScrollText.h"
#include "exctrls/propgrid/SPropertyGrid.h"
#include "exctrls/SScrollSubtitles.h"
#include "exctrls/SVText.h"
#include "exctrls/SIECtrl.h"
#include "exp_ctrls.h"

#include <core/SObjectFactory.h>
#pragma comment(lib, "shlwapi.lib")


BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

using namespace SOUI;
void RegisterCtrls(IApplication* pApp) {
	pApp->RegisterObjFactory(&TplSObjectFactory<SProgressRing>());
	pApp->RegisterObjFactory(&TplSObjectFactory<SRatingBar>());
	pApp->RegisterObjFactory(&TplSObjectFactory<SAniWindow>());
	pApp->RegisterObjFactory(&TplSObjectFactory<SChatEdit>());
	pApp->RegisterObjFactory(&TplSObjectFactory<SCheckBox2>());
	pApp->RegisterObjFactory(&TplSObjectFactory<SGroupList>());
	pApp->RegisterObjFactory(&TplSObjectFactory<SHexEdit>());
	pApp->RegisterObjFactory(&TplSObjectFactory<SRoundWnd>());
	pApp->RegisterObjFactory(&TplSObjectFactory<SScrollText>());
	pApp->RegisterObjFactory(&TplSObjectFactory<SPropertyGrid>());
	pApp->RegisterObjFactory(&TplSObjectFactory<SScrollSubtitles>());
	pApp->RegisterObjFactory(&TplSObjectFactory<SVText>());
	pApp->RegisterObjFactory(&TplSObjectFactory<SIECtrl>());
}

void UnregisterCtrls(IApplication* pApp) {
	pApp->UnregisterObjFactory(&TplSObjectFactory<SProgressRing>());
	pApp->UnregisterObjFactory(&TplSObjectFactory<SRatingBar>());
	pApp->UnregisterObjFactory(&TplSObjectFactory<SAniWindow>());
	pApp->UnregisterObjFactory(&TplSObjectFactory<SChatEdit>());
	pApp->UnregisterObjFactory(&TplSObjectFactory<SCheckBox2>());
	pApp->UnregisterObjFactory(&TplSObjectFactory<SGroupList>());
	pApp->UnregisterObjFactory(&TplSObjectFactory<SHexEdit>());
	pApp->UnregisterObjFactory(&TplSObjectFactory<SRoundWnd>());
	pApp->UnregisterObjFactory(&TplSObjectFactory<SScrollText>());
	pApp->UnregisterObjFactory(&TplSObjectFactory<SPropertyGrid>());
	pApp->UnregisterObjFactory(&TplSObjectFactory<SScrollSubtitles>());
	pApp->UnregisterObjFactory(&TplSObjectFactory<SVText>());
	pApp->UnregisterObjFactory(&TplSObjectFactory<SIECtrl>());
}

extern "C"
__declspec(dllexport) JSModuleDef* js_init_module(JSContext* ctx, const char* module_name)
{
	qjsbind::Context* context = qjsbind::Context::get(ctx);
	qjsbind::Module *module = context->NewModule(module_name);
	module->ExportFunc("RegisterCtrls", RegisterCtrls);
	module->ExportFunc("UnregisterCtrls", UnregisterCtrls);
	Exp_Ctrls(module);
	return module->module();
}
