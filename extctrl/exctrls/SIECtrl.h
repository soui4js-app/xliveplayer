#ifndef _SIECTRL_H_
#define _SIECTRL_H_

#include <control/SActiveX.h>
#include <ExDisp.h>
#include <ExDispid.h>
#include <interface/SCtrl-i.h>
#include <atl.mini/SComCli.h>

SNSBEGIN

struct IWebEvent
{
    virtual void NewWindow3(IDispatch* pDisp,
        VARIANT*& url,
        VARIANT_BOOL*& Cancel) = 0;

    virtual void BeforeNavigate2(IDispatch* pDisp,
        VARIANT*& url,
        VARIANT*& Flags,
        VARIANT*& TargetFrameName,
        VARIANT*& PostData,
        VARIANT*& Headers,
        VARIANT_BOOL*& Cancel) = 0;
    virtual void NavigateError(IDispatch* pDisp,
        VARIANT*& url,
        VARIANT*& TargetFrameName,
        VARIANT*& StatusCode,
        VARIANT_BOOL*& Cancel) = 0;
    virtual void NavigateComplete2(IDispatch* pDisp, VARIANT*& url) = 0;
    virtual void ProgressChange(LONG nProgress, LONG nProgressMax) = 0;
    virtual void CommandStateChange(long Command, VARIANT_BOOL Enable) = 0;
    virtual void DocumentComplete(IDispatch* pDisp, VARIANT*& url) = 0;
    virtual void StatusTextChange(IDispatch* pDisp, VARIANT*& url) = 0;
};


#define EVT_WEB_START (EVT_EXTERNAL_BEGIN + 2000)
DEF_EVT_EXT(WebEvtBeforeNav, EVT_WEB_START + 0, {
    LPCSTR pszUrl;
    BOOL bCancel;
    });

DEF_EVT_EXT(WebEvtNavErr, EVT_WEB_START + 1, {
    LPCSTR pszUrl;
    long code;
    BOOL bCancel;
    });

DEF_EVT_EXT(WebEvtNavProg, EVT_WEB_START + 2, {
    long progValue;
    long progMax;
    });

DEF_EVT_EXT(WebEvtCmdStateChange, EVT_WEB_START + 3, {
    long cmd;
    BOOL bEnable;
    });

DEF_EVT_EXT(WebEvtNavComplete, EVT_WEB_START + 4, {
    LPCSTR pszUrl;
    });

DEF_EVT_EXT(WebEvtNavDocComplete, EVT_WEB_START + 5, {
    LPCSTR pszUrl;
    });

DEF_EVT_EXT(WebEvtStatusTextChange, EVT_WEB_START + 6, {
    LPCSTR pszUrl;
    });

DEF_EVT_EXT(WebEvtNewWindow3, EVT_WEB_START + 7, {
    LPCSTR pszUrl;
    BOOL bCancel;
    });

class SIECtrlEvtDispatch : public IDispatch {
  public:
    SIECtrlEvtDispatch(IWebEvent *pEventHandler)
        : m_pEventHandler(pEventHandler)
        , m_cRef(1)
    {
    }

    void SetEventHandler(IWebEvent *pEventHandler)
    {
        m_pEventHandler = pEventHandler;
    }

  public: // IUnknown
    STDMETHOD_(ULONG, AddRef)()
    {
        return ++m_cRef;
    }
    STDMETHOD_(ULONG, Release)()
    {
        return --m_cRef;
    }

    STDMETHOD(QueryInterface)(REFIID riid, LPVOID *ppvObject)
    {
        if (IsEqualGUID(riid, __uuidof(IUnknown)) || IsEqualGUID(riid, __uuidof(IDispatch)))
        {
            *ppvObject = this;
            AddRef();
            return S_OK;
        }
        return E_NOINTERFACE;
    }

  public: // IDispatch
    virtual HRESULT STDMETHODCALLTYPE GetTypeInfoCount(
        /* [out] */ UINT *pctinfo)
    {
        return E_NOTIMPL;
    }

    virtual HRESULT STDMETHODCALLTYPE GetTypeInfo(
        /* [in] */ UINT iTInfo,
        /* [in] */ LCID lcid,
        /* [out] */ ITypeInfo **ppTInfo)
    {
        return E_NOTIMPL;
    }

    virtual HRESULT STDMETHODCALLTYPE GetIDsOfNames(
        /* [in] */ REFIID riid,
        /* [size_is][in] */ LPOLESTR *rgszNames,
        /* [range][in] */ UINT cNames,
        /* [in] */ LCID lcid,
        /* [size_is][out] */ DISPID *rgDispId)
    {
        return E_NOTIMPL;
    }

    virtual /* [local] */ HRESULT STDMETHODCALLTYPE Invoke(
        /* [in] */ DISPID dispIdMember,
        /* [in] */ REFIID riid,
        /* [in] */ LCID lcid,
        /* [in] */ WORD wFlags,
        /* [out][in] */ DISPPARAMS *pDispParams,
        /* [out] */ VARIANT *pVarResult,
        /* [out] */ EXCEPINFO *pExcepInfo,
        /* [out] */ UINT *puArgErr);

  protected:
    IWebEvent *m_pEventHandler;
    ULONG m_cRef;
};



#undef INTERFACE
#define INTERFACE IIECtrl
DECLARE_INTERFACE_IID_(IIECtrl, ICtrl, "E56E1D4D-DA66-477D-B8AD-73F51134C88D") {
    /**
 * @brief 增加引用计数
 * @return 新引用计数
 */
    STDMETHOD_(long, AddRef)(THIS) PURE;

    /**
     * @brief 减少引用计数
     * @return 新引用计数
     */
    STDMETHOD_(long, Release)(THIS) PURE;

    /**
     * @brief 释放对象
     * @return void
     */
    STDMETHOD_(void, OnFinalRelease)(THIS) PURE;

    /**
     * @brief 转换为IWindow*接口
     * @return IWindow*接口
     */
    STDMETHOD_(IWindow*, ToIWindow)(THIS) PURE;
    //////////////////////////////////////////////////////////////////////////
    STDMETHOD_(BOOL, Navigate)(THIS_ LPCSTR pszUrl) PURE;
    STDMETHOD_(void, GoBack)(THIS) PURE;
    STDMETHOD_(void, Refresh)(THIS) PURE;
    STDMETHOD_(IWebBrowser2*, GetIEObject)() PURE;
};


class SIECtrl
    : public SActiveX
    , public IIECtrl
    , public IMsgFilter
    , IWebEvent
{
    DEF_SOBJECT(SActiveX, L"iectrl")
  public:
    SIECtrl(void);
    ~SIECtrl(void);


    SIECtrlEvtDispatch *GetEventDispatch()
    {
        return &m_eventDispatch;
    }

    BOOL Navigate(LPCWSTR pszUrl);
public:
    STDMETHOD_(long, AddRef)(THIS) OVERRIDE {
       return  __baseCls::AddRef();
    }

    STDMETHOD_(long, Release)(THIS) OVERRIDE {
        return __baseCls::Release();
    }
    STDMETHOD_(void, OnFinalRelease)(THIS) OVERRIDE {
        return __baseCls::OnFinalRelease();
    }
    STDMETHOD_(IWindow*, ToIWindow)(THIS) OVERRIDE {
        return this;
    }
    STDMETHOD_(BOOL, Navigate)(THIS_ LPCSTR pszUrl) OVERRIDE;

    STDMETHOD_(void, GoBack)(THIS) OVERRIDE {
        m_pIE->GoBack();
    }
    STDMETHOD_(void, Refresh)(THIS) OVERRIDE {
        m_pIE->Refresh();
    }
    STDMETHOD_(IWebBrowser2*, GetIEObject)() OVERRIDE {
        return m_pIE;
    }

    STDMETHOD_(HRESULT, QueryInterface)(THIS_ REFGUID id, IObjRef** ppRet) OVERRIDE {
        if (IsEqualGUID(id, __uuidof(IIECtrl))) {
            *ppRet = (IIECtrl*)this;
            AddRef();
            return S_OK;
        }
        return __baseCls::QueryInterface(id, ppRet);
    }

protected:
    // 通过 IWebEvent 继承
    virtual void NewWindow3(IDispatch* pDisp,
        VARIANT*& url,
        VARIANT_BOOL*& Cancel) override;
    virtual void BeforeNavigate2(IDispatch* pDisp, VARIANT*& url, VARIANT*& Flags, VARIANT*& TargetFrameName, VARIANT*& PostData, VARIANT*& Headers, VARIANT_BOOL*& Cancel) override;
    virtual void NavigateError(IDispatch* pDisp, VARIANT*& url, VARIANT*& TargetFrameName, VARIANT*& StatusCode, VARIANT_BOOL*& Cancel) override;
    virtual void NavigateComplete2(IDispatch* pDisp, VARIANT*& url) override;
    virtual void ProgressChange(LONG nProgress, LONG nProgressMax) override;
    virtual void CommandStateChange(long Command, VARIANT_BOOL Enable) override;
    virtual void DocumentComplete(IDispatch* pDisp, VARIANT*& url) override;
    virtual void StatusTextChange(IDispatch* pDisp, VARIANT*& url) override;

  protected:
    virtual void OnAxActivate(IUnknown *pUnknwn);
    virtual BOOL WINAPI PreTranslateMessage(MSG *pMsg);

    int OnCreate(LPVOID);
    void OnDestroy();

    SOUI_MSG_MAP_BEGIN()
    MSG_WM_CREATE(OnCreate)
    MSG_WM_DESTROY(OnDestroy)
    SOUI_MSG_MAP_END()
  protected:
    HRESULT RegisterEventHandler(BOOL inAdvise);

    SOUI_ATTRS_BEGIN()
    ATTR_STRINGW(L"url", m_strUrl, FALSE)
    ATTR_INT(L"disableScrollbar", m_bDisableScrollbar, FALSE)
    ATTR_INT(L"disable3DBorder", m_bDisable3DBorder, FALSE)
    ATTR_INT(L"disableContextMenu", m_bDisableContextMenu, FALSE)
    ATTR_INT(L"disableTextSelect", m_bDisableTextSelect, FALSE)
    SOUI_ATTRS_END()

    SStringW m_strUrl;

    DWORD m_dwCookie;

    SIECtrlEvtDispatch m_eventDispatch;
    SComQIPtr<IWebBrowser2> m_pIE;

    BOOL m_bDisableScrollbar;
    BOOL m_bDisable3DBorder;
    BOOL m_bDisableContextMenu;
    BOOL m_bDisableTextSelect;
    HWND m_hIEWnd;
};

SNSEND

#endif//_SIECTRL_H_