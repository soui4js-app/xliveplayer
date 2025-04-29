#pragma once
#include "exctrls/SScrollSubtitles.h"
#include <sguid.h>

using namespace SOUI;
using namespace qjsbind;

template<typename T1, typename T2>
T1* QueryICtrl(IWindow* pWnd) {
	if (pWnd->IsClass(T2::GetClassName()))
	{
		IObjRef* pCtrl = NULL;
		pWnd->QueryInterface(TGuidOf<T1>(), &pCtrl);
		if (!pCtrl) return NULL;
		return (T1*)pCtrl;
	}
	return NULL;
}

#ifndef DEF_QICTRL
#define DEF_QICTRL(module,itype,stype) module->ExportFunc("Qi"#itype,&QueryICtrl<itype,stype>)
#endif
#ifndef DEF_CAST_IOBJ
#define DEF_CAST_IOBJ(module,x) module->ExportFunc("to"#x,&sobj_cast<x>)
#endif // !DEF_CAST_IOBJ


void Exp_Ctrls(qjsbind::Module* module)
{
	{
		JsClass<IObjRef> jsCls = module->ExportClass<IObjRef>("IObjRef");
		jsCls.Init();
		jsCls.AddFunc("AddRef", &IObjRef::AddRef);
		jsCls.AddFunc("Release", &IObjRef::Release);
		jsCls.AddFunc("OnFinalRelease", &IObjRef::OnFinalRelease);
	}
	{
		JsClass<ICtrl> jsCls = module->ExportClass<ICtrl>("ICtrl");
		jsCls.Init(JsClass<IObjRef>::class_id());
		jsCls.AddFunc("ToIWindow", &ICtrl::ToIWindow);
	}
	{
		JsClass<IScrollSubtitles> jsCls = module->ExportClass<IScrollSubtitles>("IScrollSubtitles");
		jsCls.Init(JsClass<ICtrl>::class_id());
		jsCls.AddFunc("AddSubtitles", &IScrollSubtitles::AddSubtitles);
		DEF_QICTRL(module, IScrollSubtitles, SScrollSubtitles);
	}

	{
		JsClass<IIECtrl> jsCls = module->ExportClass<IIECtrl>("IIECtrl");
		jsCls.Init(JsClass<ICtrl>::class_id());
		jsCls.AddFunc("Navigate", &IIECtrl::Navigate);
		jsCls.AddFunc("GoBack", &IIECtrl::GoBack);
		jsCls.AddFunc("Refresh", &IIECtrl::Refresh);
		DEF_QICTRL(module, IIECtrl, SIECtrl);
	}

	{JsClass<WebEvtBeforeNav> jsCls = module->ExportClass<WebEvtBeforeNav>("WebEvtBeforeNav");
	jsCls.Init(JsClass<IEvtArgs>::class_id());
	jsCls.AddGetSet("url", &WebEvtBeforeNav::pszUrl);
	jsCls.AddGetSet("bCancel", &WebEvtBeforeNav::bCancel);
	DEF_CAST_IOBJ(module, WebEvtBeforeNav); }

	{JsClass<WebEvtNewWindow3> jsCls = module->ExportClass<WebEvtNewWindow3>("WebEvtNewWindow3");
	jsCls.Init(JsClass<IEvtArgs>::class_id());
	jsCls.AddGetSet("url", &WebEvtNewWindow3::pszUrl);
	jsCls.AddGetSet("bCancel", &WebEvtNewWindow3::bCancel);
	DEF_CAST_IOBJ(module, WebEvtNewWindow3); }

	{JsClass<WebEvtNavProg> jsCls = module->ExportClass<WebEvtNavProg>("WebEvtNavProg");
	jsCls.Init(JsClass<IEvtArgs>::class_id());
	jsCls.AddGetSet("progValue", &WebEvtNavProg::progValue);
	jsCls.AddGetSet("progMax", &WebEvtNavProg::progMax);
	DEF_CAST_IOBJ(module, WebEvtNavProg); }

}
