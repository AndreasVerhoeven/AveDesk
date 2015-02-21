

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0366 */
/* at Mon Jan 01 21:57:04 2007
 */
/* Compiler settings for .\activdbg.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __activdbg_h__
#define __activdbg_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IActiveScriptDebug_FWD_DEFINED__
#define __IActiveScriptDebug_FWD_DEFINED__
typedef interface IActiveScriptDebug IActiveScriptDebug;
#endif 	/* __IActiveScriptDebug_FWD_DEFINED__ */


#ifndef __IActiveScriptError_FWD_DEFINED__
#define __IActiveScriptError_FWD_DEFINED__
typedef interface IActiveScriptError IActiveScriptError;
#endif 	/* __IActiveScriptError_FWD_DEFINED__ */


#ifndef __IActiveScriptErrorDebug_FWD_DEFINED__
#define __IActiveScriptErrorDebug_FWD_DEFINED__
typedef interface IActiveScriptErrorDebug IActiveScriptErrorDebug;
#endif 	/* __IActiveScriptErrorDebug_FWD_DEFINED__ */


#ifndef __IActiveScriptSiteDebug_FWD_DEFINED__
#define __IActiveScriptSiteDebug_FWD_DEFINED__
typedef interface IActiveScriptSiteDebug IActiveScriptSiteDebug;
#endif 	/* __IActiveScriptSiteDebug_FWD_DEFINED__ */


#ifndef __IDebugCodeContext_FWD_DEFINED__
#define __IDebugCodeContext_FWD_DEFINED__
typedef interface IDebugCodeContext IDebugCodeContext;
#endif 	/* __IDebugCodeContext_FWD_DEFINED__ */


#ifndef __IEnumDebugCodeContexts_FWD_DEFINED__
#define __IEnumDebugCodeContexts_FWD_DEFINED__
typedef interface IEnumDebugCodeContexts IEnumDebugCodeContexts;
#endif 	/* __IEnumDebugCodeContexts_FWD_DEFINED__ */


#ifndef __IDebugStackFrame_FWD_DEFINED__
#define __IDebugStackFrame_FWD_DEFINED__
typedef interface IDebugStackFrame IDebugStackFrame;
#endif 	/* __IDebugStackFrame_FWD_DEFINED__ */


#ifndef __IDebugExpressionContext_FWD_DEFINED__
#define __IDebugExpressionContext_FWD_DEFINED__
typedef interface IDebugExpressionContext IDebugExpressionContext;
#endif 	/* __IDebugExpressionContext_FWD_DEFINED__ */


#ifndef __IDebugStackFrameSniffer_FWD_DEFINED__
#define __IDebugStackFrameSniffer_FWD_DEFINED__
typedef interface IDebugStackFrameSniffer IDebugStackFrameSniffer;
#endif 	/* __IDebugStackFrameSniffer_FWD_DEFINED__ */


#ifndef __IDebugExpression_FWD_DEFINED__
#define __IDebugExpression_FWD_DEFINED__
typedef interface IDebugExpression IDebugExpression;
#endif 	/* __IDebugExpression_FWD_DEFINED__ */


#ifndef __IDebugSyncOperation_FWD_DEFINED__
#define __IDebugSyncOperation_FWD_DEFINED__
typedef interface IDebugSyncOperation IDebugSyncOperation;
#endif 	/* __IDebugSyncOperation_FWD_DEFINED__ */


#ifndef __IDebugAsyncOperation_FWD_DEFINED__
#define __IDebugAsyncOperation_FWD_DEFINED__
typedef interface IDebugAsyncOperation IDebugAsyncOperation;
#endif 	/* __IDebugAsyncOperation_FWD_DEFINED__ */


#ifndef __IDebugAsyncOperationCallBack_FWD_DEFINED__
#define __IDebugAsyncOperationCallBack_FWD_DEFINED__
typedef interface IDebugAsyncOperationCallBack IDebugAsyncOperationCallBack;
#endif 	/* __IDebugAsyncOperationCallBack_FWD_DEFINED__ */


#ifndef __IDebugExpressionCallBack_FWD_DEFINED__
#define __IDebugExpressionCallBack_FWD_DEFINED__
typedef interface IDebugExpressionCallBack IDebugExpressionCallBack;
#endif 	/* __IDebugExpressionCallBack_FWD_DEFINED__ */


#ifndef __IEnumDebugExpressionContexts_FWD_DEFINED__
#define __IEnumDebugExpressionContexts_FWD_DEFINED__
typedef interface IEnumDebugExpressionContexts IEnumDebugExpressionContexts;
#endif 	/* __IEnumDebugExpressionContexts_FWD_DEFINED__ */


#ifndef __IProvideExpressionContexts_FWD_DEFINED__
#define __IProvideExpressionContexts_FWD_DEFINED__
typedef interface IProvideExpressionContexts IProvideExpressionContexts;
#endif 	/* __IProvideExpressionContexts_FWD_DEFINED__ */


#ifndef __IDebugFormatter_FWD_DEFINED__
#define __IDebugFormatter_FWD_DEFINED__
typedef interface IDebugFormatter IDebugFormatter;
#endif 	/* __IDebugFormatter_FWD_DEFINED__ */


#ifndef __IDebugProperty_FWD_DEFINED__
#define __IDebugProperty_FWD_DEFINED__
typedef interface IDebugProperty IDebugProperty;
#endif 	/* __IDebugProperty_FWD_DEFINED__ */


#ifndef __IEnumDebugPropertyInfo_FWD_DEFINED__
#define __IEnumDebugPropertyInfo_FWD_DEFINED__
typedef interface IEnumDebugPropertyInfo IEnumDebugPropertyInfo;
#endif 	/* __IEnumDebugPropertyInfo_FWD_DEFINED__ */


#ifndef __IDebugDocumentHelper_FWD_DEFINED__
#define __IDebugDocumentHelper_FWD_DEFINED__
typedef interface IDebugDocumentHelper IDebugDocumentHelper;
#endif 	/* __IDebugDocumentHelper_FWD_DEFINED__ */


#ifndef __IDebugDocumentHost_FWD_DEFINED__
#define __IDebugDocumentHost_FWD_DEFINED__
typedef interface IDebugDocumentHost IDebugDocumentHost;
#endif 	/* __IDebugDocumentHost_FWD_DEFINED__ */


#ifndef __IDebugDocumentInfo_FWD_DEFINED__
#define __IDebugDocumentInfo_FWD_DEFINED__
typedef interface IDebugDocumentInfo IDebugDocumentInfo;
#endif 	/* __IDebugDocumentInfo_FWD_DEFINED__ */


#ifndef __IDebugDocumentProvider_FWD_DEFINED__
#define __IDebugDocumentProvider_FWD_DEFINED__
typedef interface IDebugDocumentProvider IDebugDocumentProvider;
#endif 	/* __IDebugDocumentProvider_FWD_DEFINED__ */


#ifndef __IDebugDocument_FWD_DEFINED__
#define __IDebugDocument_FWD_DEFINED__
typedef interface IDebugDocument IDebugDocument;
#endif 	/* __IDebugDocument_FWD_DEFINED__ */


#ifndef __IDebugDocumentText_FWD_DEFINED__
#define __IDebugDocumentText_FWD_DEFINED__
typedef interface IDebugDocumentText IDebugDocumentText;
#endif 	/* __IDebugDocumentText_FWD_DEFINED__ */


#ifndef __IDebugDocumentTextEvents_FWD_DEFINED__
#define __IDebugDocumentTextEvents_FWD_DEFINED__
typedef interface IDebugDocumentTextEvents IDebugDocumentTextEvents;
#endif 	/* __IDebugDocumentTextEvents_FWD_DEFINED__ */


#ifndef __IDebugDocumentTextAuthor_FWD_DEFINED__
#define __IDebugDocumentTextAuthor_FWD_DEFINED__
typedef interface IDebugDocumentTextAuthor IDebugDocumentTextAuthor;
#endif 	/* __IDebugDocumentTextAuthor_FWD_DEFINED__ */


#ifndef __IDebugDocumentContext_FWD_DEFINED__
#define __IDebugDocumentContext_FWD_DEFINED__
typedef interface IDebugDocumentContext IDebugDocumentContext;
#endif 	/* __IDebugDocumentContext_FWD_DEFINED__ */


#ifndef __IDebugApplicationNode_FWD_DEFINED__
#define __IDebugApplicationNode_FWD_DEFINED__
typedef interface IDebugApplicationNode IDebugApplicationNode;
#endif 	/* __IDebugApplicationNode_FWD_DEFINED__ */


#ifndef __IDebugApplicationNodeEvents_FWD_DEFINED__
#define __IDebugApplicationNodeEvents_FWD_DEFINED__
typedef interface IDebugApplicationNodeEvents IDebugApplicationNodeEvents;
#endif 	/* __IDebugApplicationNodeEvents_FWD_DEFINED__ */


#ifndef __IDebugSessionProvider_FWD_DEFINED__
#define __IDebugSessionProvider_FWD_DEFINED__
typedef interface IDebugSessionProvider IDebugSessionProvider;
#endif 	/* __IDebugSessionProvider_FWD_DEFINED__ */


#ifndef __IApplicationDebugger_FWD_DEFINED__
#define __IApplicationDebugger_FWD_DEFINED__
typedef interface IApplicationDebugger IApplicationDebugger;
#endif 	/* __IApplicationDebugger_FWD_DEFINED__ */


#ifndef __IMachineDebugManager_FWD_DEFINED__
#define __IMachineDebugManager_FWD_DEFINED__
typedef interface IMachineDebugManager IMachineDebugManager;
#endif 	/* __IMachineDebugManager_FWD_DEFINED__ */


#ifndef __IMachineDebugManagerCookie_FWD_DEFINED__
#define __IMachineDebugManagerCookie_FWD_DEFINED__
typedef interface IMachineDebugManagerCookie IMachineDebugManagerCookie;
#endif 	/* __IMachineDebugManagerCookie_FWD_DEFINED__ */


#ifndef __IMachineDebugManagerEvents_FWD_DEFINED__
#define __IMachineDebugManagerEvents_FWD_DEFINED__
typedef interface IMachineDebugManagerEvents IMachineDebugManagerEvents;
#endif 	/* __IMachineDebugManagerEvents_FWD_DEFINED__ */


#ifndef __IEnumRemoteDebugApplications_FWD_DEFINED__
#define __IEnumRemoteDebugApplications_FWD_DEFINED__
typedef interface IEnumRemoteDebugApplications IEnumRemoteDebugApplications;
#endif 	/* __IEnumRemoteDebugApplications_FWD_DEFINED__ */


#ifndef __IProcessDebugManager_FWD_DEFINED__
#define __IProcessDebugManager_FWD_DEFINED__
typedef interface IProcessDebugManager IProcessDebugManager;
#endif 	/* __IProcessDebugManager_FWD_DEFINED__ */


#ifndef __IRemoteDebugApplication_FWD_DEFINED__
#define __IRemoteDebugApplication_FWD_DEFINED__
typedef interface IRemoteDebugApplication IRemoteDebugApplication;
#endif 	/* __IRemoteDebugApplication_FWD_DEFINED__ */


#ifndef __IDebugApplication_FWD_DEFINED__
#define __IDebugApplication_FWD_DEFINED__
typedef interface IDebugApplication IDebugApplication;
#endif 	/* __IDebugApplication_FWD_DEFINED__ */


#ifndef __IRemoteDebugApplicationThread_FWD_DEFINED__
#define __IRemoteDebugApplicationThread_FWD_DEFINED__
typedef interface IRemoteDebugApplicationThread IRemoteDebugApplicationThread;
#endif 	/* __IRemoteDebugApplicationThread_FWD_DEFINED__ */


#ifndef __IDebugApplicationThread_FWD_DEFINED__
#define __IDebugApplicationThread_FWD_DEFINED__
typedef interface IDebugApplicationThread IDebugApplicationThread;
#endif 	/* __IDebugApplicationThread_FWD_DEFINED__ */


#ifndef __IEnumRemoteDebugApplicationThreads_FWD_DEFINED__
#define __IEnumRemoteDebugApplicationThreads_FWD_DEFINED__
typedef interface IEnumRemoteDebugApplicationThreads IEnumRemoteDebugApplicationThreads;
#endif 	/* __IEnumRemoteDebugApplicationThreads_FWD_DEFINED__ */


#ifndef __IDebugThreadCall_FWD_DEFINED__
#define __IDebugThreadCall_FWD_DEFINED__
typedef interface IDebugThreadCall IDebugThreadCall;
#endif 	/* __IDebugThreadCall_FWD_DEFINED__ */


#ifndef __IEnumDebugApplicationNodes_FWD_DEFINED__
#define __IEnumDebugApplicationNodes_FWD_DEFINED__
typedef interface IEnumDebugApplicationNodes IEnumDebugApplicationNodes;
#endif 	/* __IEnumDebugApplicationNodes_FWD_DEFINED__ */


#ifndef __IEnumDebugStackFrames_FWD_DEFINED__
#define __IEnumDebugStackFrames_FWD_DEFINED__
typedef interface IEnumDebugStackFrames IEnumDebugStackFrames;
#endif 	/* __IEnumDebugStackFrames_FWD_DEFINED__ */


#ifndef __IDebugStackFrameSnifferEx_FWD_DEFINED__
#define __IDebugStackFrameSnifferEx_FWD_DEFINED__
typedef interface IDebugStackFrameSnifferEx IDebugStackFrameSnifferEx;
#endif 	/* __IDebugStackFrameSnifferEx_FWD_DEFINED__ */


#ifndef __IDebugDocumentTextExternalAuthor_FWD_DEFINED__
#define __IDebugDocumentTextExternalAuthor_FWD_DEFINED__
typedef interface IDebugDocumentTextExternalAuthor IDebugDocumentTextExternalAuthor;
#endif 	/* __IDebugDocumentTextExternalAuthor_FWD_DEFINED__ */


#ifndef __IApplicationDebuggerUI_FWD_DEFINED__
#define __IApplicationDebuggerUI_FWD_DEFINED__
typedef interface IApplicationDebuggerUI IApplicationDebuggerUI;
#endif 	/* __IApplicationDebuggerUI_FWD_DEFINED__ */


#ifndef __IRemoteDebugApplicationEvents_FWD_DEFINED__
#define __IRemoteDebugApplicationEvents_FWD_DEFINED__
typedef interface IRemoteDebugApplicationEvents IRemoteDebugApplicationEvents;
#endif 	/* __IRemoteDebugApplicationEvents_FWD_DEFINED__ */


#ifndef __IDebugCookie_FWD_DEFINED__
#define __IDebugCookie_FWD_DEFINED__
typedef interface IDebugCookie IDebugCookie;
#endif 	/* __IDebugCookie_FWD_DEFINED__ */


#ifndef __ISimpleConnectionPoint_FWD_DEFINED__
#define __ISimpleConnectionPoint_FWD_DEFINED__
typedef interface ISimpleConnectionPoint ISimpleConnectionPoint;
#endif 	/* __ISimpleConnectionPoint_FWD_DEFINED__ */


#ifndef __IDebugHelper_FWD_DEFINED__
#define __IDebugHelper_FWD_DEFINED__
typedef interface IDebugHelper IDebugHelper;
#endif 	/* __IDebugHelper_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "activscp.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

/* interface __MIDL_itf_activdbg_0000 */
/* [local] */ 

//=--------------------------------------------------------------------------=
// ActivDbg.h
//=--------------------------------------------------------------------------=
// (C) Copyright 1996 Microsoft Corporation.  All Rights Reserved.
//
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//=--------------------------------------------------------------------------=
//
#pragma comment(lib,"uuid.lib")
//
// Declarations for ActiveX Script authoring/Debugging.
//

#ifndef __ActivDbg_h
#define __ActivDbg_h



extern RPC_IF_HANDLE __MIDL_itf_activdbg_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_activdbg_0000_v0_0_s_ifspec;


#ifndef __ActivDbg_LIBRARY_DEFINED__
#define __ActivDbg_LIBRARY_DEFINED__

/* library ActivDbg */
/* [helpstring][version][uuid] */ 



















































typedef 
enum tagBREAKPOINT_STATE
    {	BREAKPOINT_DELETED	= 0,
	BREAKPOINT_DISABLED	= 1,
	BREAKPOINT_ENABLED	= 2
    } 	BREAKPOINT_STATE;

typedef DWORD APPBREAKFLAGS;

#define	APPBREAKFLAG_DEBUGGER_BLOCK	( 0x1 )

#define	APPBREAKFLAG_DEBUGGER_HALT	( 0x2 )

#define	APPBREAKFLAG_STEP	( 0x10000 )

#define	APPBREAKFLAG_NESTED	( 0x20000 )

#define	APPBREAKFLAG_STEPTYPE_SOURCE	( 0 )

#define	APPBREAKFLAG_STEPTYPE_BYTECODE	( 0x100000 )

#define	APPBREAKFLAG_STEPTYPE_MACHINE	( 0x200000 )

#define	APPBREAKFLAG_STEPTYPE_MASK	( 0xf00000 )

#define	APPBREAKFLAG_IN_BREAKPOINT	( 0x80000000 )

typedef 
enum tagBREAKREASON
    {	BREAKREASON_STEP	= 0,
	BREAKREASON_BREAKPOINT	= BREAKREASON_STEP + 1,
	BREAKREASON_DEBUGGER_BLOCK	= BREAKREASON_BREAKPOINT + 1,
	BREAKREASON_HOST_INITIATED	= BREAKREASON_DEBUGGER_BLOCK + 1,
	BREAKREASON_LANGUAGE_INITIATED	= BREAKREASON_HOST_INITIATED + 1,
	BREAKREASON_DEBUGGER_HALT	= BREAKREASON_LANGUAGE_INITIATED + 1,
	BREAKREASON_ERROR	= BREAKREASON_DEBUGGER_HALT + 1
    } 	BREAKREASON;

typedef 
enum tagBREAKRESUME_ACTION
    {	BREAKRESUMEACTION_ABORT	= 0,
	BREAKRESUMEACTION_CONTINUE	= BREAKRESUMEACTION_ABORT + 1,
	BREAKRESUMEACTION_STEP_INTO	= BREAKRESUMEACTION_CONTINUE + 1,
	BREAKRESUMEACTION_STEP_OVER	= BREAKRESUMEACTION_STEP_INTO + 1,
	BREAKRESUMEACTION_STEP_OUT	= BREAKRESUMEACTION_STEP_OVER + 1
    } 	BREAKRESUMEACTION;

typedef 
enum tagDOCUMENTNAMETYPE
    {	DOCUMENTNAMETYPE_APPNODE	= 0,
	DOCUMENTNAMETYPE_TITLE	= DOCUMENTNAMETYPE_APPNODE + 1,
	DOCUMENTNAMETYPE_FILE_TAIL	= DOCUMENTNAMETYPE_TITLE + 1,
	DOCUMENTNAMETYPE_URL	= DOCUMENTNAMETYPE_FILE_TAIL + 1
    } 	DOCUMENTNAMETYPE;

typedef WORD SOURCE_TEXT_ATTR;

#define	SOURCETEXT_ATTR_KEYWORD	( 0x1 )

#define	SOURCETEXT_ATTR_COMMENT	( 0x2 )

#define	SOURCETEXT_ATTR_NONSOURCE	( 0x4 )

#define	SOURCETEXT_ATTR_OPERATOR	( 0x8 )

#define	SOURCETEXT_ATTR_NUMBER	( 0x10 )

#define	SOURCETEXT_ATTR_STRING	( 0x20 )

#define	SOURCETEXT_ATTR_FUNCTION_START	( 0x40 )

typedef DWORD TEXT_DOC_ATTR;

#define	TEXT_DOC_ATTR_READONLY	( 0x1 )

#define	DEBUG_TEXT_ISEXPRESSION	( 0x1 )

#define	DEBUG_TEXT_RETURNVALUE	( 0x2 )

#define	DEBUG_TEXT_NOSIDEEFFECTS	( 0x4 )

#define	DEBUG_TEXT_ALLOWBREAKPOINTS	( 0x8 )

#define	DEBUG_TEXT_ALLOWERRORREPORT	( 0x10 )

typedef 
enum tagERRORRESUMEACTION
    {	ERRORRESUMEACTION_ReexecuteErrorStatement	= 0,
	ERRORRESUMEACTION_AbortCallAndReturnErrorToCaller	= ERRORRESUMEACTION_ReexecuteErrorStatement + 1,
	ERRORRESUMEACTION_SkipErrorStatement	= ERRORRESUMEACTION_AbortCallAndReturnErrorToCaller + 1
    } 	ERRORRESUMEACTION;

typedef struct DebugPropertyInfo
    {
    DWORD m_dwValidFields;
    BSTR m_bstrName;
    BSTR m_bstrType;
    BSTR m_bstrValue;
    BSTR m_bstrFullName;
    DWORD m_dwAttrib;
    IDebugProperty *m_pDebugProp;
    } 	DebugPropertyInfo;

typedef struct tagDebugStackFrameDescriptor
    {
    IDebugStackFrame *pdsf;
    DWORD dwMin;
    DWORD dwLim;
    BOOL fFinal;
    IUnknown *punkFinal;
    } 	DebugStackFrameDescriptor;

EXTERN_C const CLSID CLSID_DefaultDebugSessionProvider;
EXTERN_C const CLSID CLSID_MachineDebugManager;
EXTERN_C const CLSID CLSID_ProcessDebugManager;
EXTERN_C const CLSID CLSID_DebugHelper;

#endif  // __ActivDbg_h


EXTERN_C const IID LIBID_ActivDbg;

#ifndef __IActiveScriptDebug_INTERFACE_DEFINED__
#define __IActiveScriptDebug_INTERFACE_DEFINED__

/* interface IActiveScriptDebug */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IActiveScriptDebug;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("51973C10-CB0C-11d0-B5C9-00A0244A0E7A")
    IActiveScriptDebug : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetScriptTextAttributes( 
            /* [size_is][in] */ LPCOLESTR pstrCode,
            /* [in] */ ULONG uNumCodeChars,
            /* [in] */ LPCOLESTR pstrDelimiter,
            /* [in] */ DWORD dwFlags,
            /* [size_is][out][in] */ SOURCE_TEXT_ATTR *pattr) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetScriptletTextAttributes( 
            /* [size_is][in] */ LPCOLESTR pstrCode,
            /* [in] */ ULONG uNumCodeChars,
            /* [in] */ LPCOLESTR pstrDelimiter,
            /* [in] */ DWORD dwFlags,
            /* [size_is][out][in] */ SOURCE_TEXT_ATTR *pattr) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE EnumCodeContextsOfPosition( 
            /* [in] */ DWORD dwSourceContext,
            /* [in] */ ULONG uCharacterOffset,
            /* [in] */ ULONG uNumChars,
            /* [out] */ IEnumDebugCodeContexts **ppescc) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IActiveScriptDebugVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IActiveScriptDebug * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IActiveScriptDebug * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IActiveScriptDebug * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetScriptTextAttributes )( 
            IActiveScriptDebug * This,
            /* [size_is][in] */ LPCOLESTR pstrCode,
            /* [in] */ ULONG uNumCodeChars,
            /* [in] */ LPCOLESTR pstrDelimiter,
            /* [in] */ DWORD dwFlags,
            /* [size_is][out][in] */ SOURCE_TEXT_ATTR *pattr);
        
        HRESULT ( STDMETHODCALLTYPE *GetScriptletTextAttributes )( 
            IActiveScriptDebug * This,
            /* [size_is][in] */ LPCOLESTR pstrCode,
            /* [in] */ ULONG uNumCodeChars,
            /* [in] */ LPCOLESTR pstrDelimiter,
            /* [in] */ DWORD dwFlags,
            /* [size_is][out][in] */ SOURCE_TEXT_ATTR *pattr);
        
        HRESULT ( STDMETHODCALLTYPE *EnumCodeContextsOfPosition )( 
            IActiveScriptDebug * This,
            /* [in] */ DWORD dwSourceContext,
            /* [in] */ ULONG uCharacterOffset,
            /* [in] */ ULONG uNumChars,
            /* [out] */ IEnumDebugCodeContexts **ppescc);
        
        END_INTERFACE
    } IActiveScriptDebugVtbl;

    interface IActiveScriptDebug
    {
        CONST_VTBL struct IActiveScriptDebugVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IActiveScriptDebug_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IActiveScriptDebug_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IActiveScriptDebug_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IActiveScriptDebug_GetScriptTextAttributes(This,pstrCode,uNumCodeChars,pstrDelimiter,dwFlags,pattr)	\
    (This)->lpVtbl -> GetScriptTextAttributes(This,pstrCode,uNumCodeChars,pstrDelimiter,dwFlags,pattr)

#define IActiveScriptDebug_GetScriptletTextAttributes(This,pstrCode,uNumCodeChars,pstrDelimiter,dwFlags,pattr)	\
    (This)->lpVtbl -> GetScriptletTextAttributes(This,pstrCode,uNumCodeChars,pstrDelimiter,dwFlags,pattr)

#define IActiveScriptDebug_EnumCodeContextsOfPosition(This,dwSourceContext,uCharacterOffset,uNumChars,ppescc)	\
    (This)->lpVtbl -> EnumCodeContextsOfPosition(This,dwSourceContext,uCharacterOffset,uNumChars,ppescc)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IActiveScriptDebug_GetScriptTextAttributes_Proxy( 
    IActiveScriptDebug * This,
    /* [size_is][in] */ LPCOLESTR pstrCode,
    /* [in] */ ULONG uNumCodeChars,
    /* [in] */ LPCOLESTR pstrDelimiter,
    /* [in] */ DWORD dwFlags,
    /* [size_is][out][in] */ SOURCE_TEXT_ATTR *pattr);


void __RPC_STUB IActiveScriptDebug_GetScriptTextAttributes_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IActiveScriptDebug_GetScriptletTextAttributes_Proxy( 
    IActiveScriptDebug * This,
    /* [size_is][in] */ LPCOLESTR pstrCode,
    /* [in] */ ULONG uNumCodeChars,
    /* [in] */ LPCOLESTR pstrDelimiter,
    /* [in] */ DWORD dwFlags,
    /* [size_is][out][in] */ SOURCE_TEXT_ATTR *pattr);


void __RPC_STUB IActiveScriptDebug_GetScriptletTextAttributes_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IActiveScriptDebug_EnumCodeContextsOfPosition_Proxy( 
    IActiveScriptDebug * This,
    /* [in] */ DWORD dwSourceContext,
    /* [in] */ ULONG uCharacterOffset,
    /* [in] */ ULONG uNumChars,
    /* [out] */ IEnumDebugCodeContexts **ppescc);


void __RPC_STUB IActiveScriptDebug_EnumCodeContextsOfPosition_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IActiveScriptDebug_INTERFACE_DEFINED__ */


#ifndef __IActiveScriptErrorDebug_INTERFACE_DEFINED__
#define __IActiveScriptErrorDebug_INTERFACE_DEFINED__

/* interface IActiveScriptErrorDebug */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IActiveScriptErrorDebug;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("51973C12-CB0C-11d0-B5C9-00A0244A0E7A")
    IActiveScriptErrorDebug : public IActiveScriptError
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetDocumentContext( 
            /* [out] */ IDebugDocumentContext **ppssc) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetStackFrame( 
            /* [out] */ IDebugStackFrame **ppdsf) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IActiveScriptErrorDebugVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IActiveScriptErrorDebug * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IActiveScriptErrorDebug * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IActiveScriptErrorDebug * This);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *GetExceptionInfo )( 
            IActiveScriptErrorDebug * This,
            /* [out] */ EXCEPINFO *pexcepinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetSourcePosition )( 
            IActiveScriptErrorDebug * This,
            /* [out] */ DWORD *pdwSourceContext,
            /* [out] */ ULONG *pulLineNumber,
            /* [out] */ LONG *plCharacterPosition);
        
        HRESULT ( STDMETHODCALLTYPE *GetSourceLineText )( 
            IActiveScriptErrorDebug * This,
            /* [out] */ BSTR *pbstrSourceLine);
        
        HRESULT ( STDMETHODCALLTYPE *GetDocumentContext )( 
            IActiveScriptErrorDebug * This,
            /* [out] */ IDebugDocumentContext **ppssc);
        
        HRESULT ( STDMETHODCALLTYPE *GetStackFrame )( 
            IActiveScriptErrorDebug * This,
            /* [out] */ IDebugStackFrame **ppdsf);
        
        END_INTERFACE
    } IActiveScriptErrorDebugVtbl;

    interface IActiveScriptErrorDebug
    {
        CONST_VTBL struct IActiveScriptErrorDebugVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IActiveScriptErrorDebug_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IActiveScriptErrorDebug_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IActiveScriptErrorDebug_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IActiveScriptErrorDebug_GetExceptionInfo(This,pexcepinfo)	\
    (This)->lpVtbl -> GetExceptionInfo(This,pexcepinfo)

#define IActiveScriptErrorDebug_GetSourcePosition(This,pdwSourceContext,pulLineNumber,plCharacterPosition)	\
    (This)->lpVtbl -> GetSourcePosition(This,pdwSourceContext,pulLineNumber,plCharacterPosition)

#define IActiveScriptErrorDebug_GetSourceLineText(This,pbstrSourceLine)	\
    (This)->lpVtbl -> GetSourceLineText(This,pbstrSourceLine)


#define IActiveScriptErrorDebug_GetDocumentContext(This,ppssc)	\
    (This)->lpVtbl -> GetDocumentContext(This,ppssc)

#define IActiveScriptErrorDebug_GetStackFrame(This,ppdsf)	\
    (This)->lpVtbl -> GetStackFrame(This,ppdsf)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IActiveScriptErrorDebug_GetDocumentContext_Proxy( 
    IActiveScriptErrorDebug * This,
    /* [out] */ IDebugDocumentContext **ppssc);


void __RPC_STUB IActiveScriptErrorDebug_GetDocumentContext_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IActiveScriptErrorDebug_GetStackFrame_Proxy( 
    IActiveScriptErrorDebug * This,
    /* [out] */ IDebugStackFrame **ppdsf);


void __RPC_STUB IActiveScriptErrorDebug_GetStackFrame_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IActiveScriptErrorDebug_INTERFACE_DEFINED__ */


#ifndef __IActiveScriptError_INTERFACE_DEFINED__
#define __IActiveScriptError_INTERFACE_DEFINED__

/* interface IActiveScriptError */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IActiveScriptError;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("EAE1BA61-A4ED-11cf-8F20-00805F2CD064")
    IActiveScriptError : public IUnknown
    {
    public:
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE GetExceptionInfo( 
            /* [out] */ EXCEPINFO *pexcepinfo) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetSourcePosition( 
            /* [out] */ DWORD *pdwSourceContext,
            /* [out] */ ULONG *pulLineNumber,
            /* [out] */ LONG *plCharacterPosition) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetSourceLineText( 
            /* [out] */ BSTR *pbstrSourceLine) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IActiveScriptErrorVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IActiveScriptError * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IActiveScriptError * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IActiveScriptError * This);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *GetExceptionInfo )( 
            IActiveScriptError * This,
            /* [out] */ EXCEPINFO *pexcepinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetSourcePosition )( 
            IActiveScriptError * This,
            /* [out] */ DWORD *pdwSourceContext,
            /* [out] */ ULONG *pulLineNumber,
            /* [out] */ LONG *plCharacterPosition);
        
        HRESULT ( STDMETHODCALLTYPE *GetSourceLineText )( 
            IActiveScriptError * This,
            /* [out] */ BSTR *pbstrSourceLine);
        
        END_INTERFACE
    } IActiveScriptErrorVtbl;

    interface IActiveScriptError
    {
        CONST_VTBL struct IActiveScriptErrorVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IActiveScriptError_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IActiveScriptError_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IActiveScriptError_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IActiveScriptError_GetExceptionInfo(This,pexcepinfo)	\
    (This)->lpVtbl -> GetExceptionInfo(This,pexcepinfo)

#define IActiveScriptError_GetSourcePosition(This,pdwSourceContext,pulLineNumber,plCharacterPosition)	\
    (This)->lpVtbl -> GetSourcePosition(This,pdwSourceContext,pulLineNumber,plCharacterPosition)

#define IActiveScriptError_GetSourceLineText(This,pbstrSourceLine)	\
    (This)->lpVtbl -> GetSourceLineText(This,pbstrSourceLine)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [call_as] */ HRESULT STDMETHODCALLTYPE IActiveScriptError_RemoteGetExceptionInfo_Proxy( 
    IActiveScriptError * This,
    /* [out] */ EXCEPINFO *pexcepinfo);


void __RPC_STUB IActiveScriptError_RemoteGetExceptionInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IActiveScriptError_GetSourcePosition_Proxy( 
    IActiveScriptError * This,
    /* [out] */ DWORD *pdwSourceContext,
    /* [out] */ ULONG *pulLineNumber,
    /* [out] */ LONG *plCharacterPosition);


void __RPC_STUB IActiveScriptError_GetSourcePosition_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IActiveScriptError_GetSourceLineText_Proxy( 
    IActiveScriptError * This,
    /* [out] */ BSTR *pbstrSourceLine);


void __RPC_STUB IActiveScriptError_GetSourceLineText_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IActiveScriptError_INTERFACE_DEFINED__ */


#ifndef __IActiveScriptSiteDebug_INTERFACE_DEFINED__
#define __IActiveScriptSiteDebug_INTERFACE_DEFINED__

/* interface IActiveScriptSiteDebug */
/* [local][unique][uuid][object] */ 


EXTERN_C const IID IID_IActiveScriptSiteDebug;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("51973C11-CB0C-11d0-B5C9-00A0244A0E7A")
    IActiveScriptSiteDebug : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetDocumentContextFromPosition( 
            /* [in] */ DWORD dwSourceContext,
            /* [in] */ ULONG uCharacterOffset,
            /* [in] */ ULONG uNumChars,
            /* [out] */ IDebugDocumentContext **ppsc) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetApplication( 
            /* [out] */ IDebugApplication **ppda) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetRootApplicationNode( 
            /* [out] */ IDebugApplicationNode **ppdanRoot) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnScriptErrorDebug( 
            /* [in] */ IActiveScriptErrorDebug *pErrorDebug,
            /* [out] */ BOOL *pfEnterDebugger,
            /* [out] */ BOOL *pfCallOnScriptErrorWhenContinuing) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IActiveScriptSiteDebugVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IActiveScriptSiteDebug * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IActiveScriptSiteDebug * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IActiveScriptSiteDebug * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetDocumentContextFromPosition )( 
            IActiveScriptSiteDebug * This,
            /* [in] */ DWORD dwSourceContext,
            /* [in] */ ULONG uCharacterOffset,
            /* [in] */ ULONG uNumChars,
            /* [out] */ IDebugDocumentContext **ppsc);
        
        HRESULT ( STDMETHODCALLTYPE *GetApplication )( 
            IActiveScriptSiteDebug * This,
            /* [out] */ IDebugApplication **ppda);
        
        HRESULT ( STDMETHODCALLTYPE *GetRootApplicationNode )( 
            IActiveScriptSiteDebug * This,
            /* [out] */ IDebugApplicationNode **ppdanRoot);
        
        HRESULT ( STDMETHODCALLTYPE *OnScriptErrorDebug )( 
            IActiveScriptSiteDebug * This,
            /* [in] */ IActiveScriptErrorDebug *pErrorDebug,
            /* [out] */ BOOL *pfEnterDebugger,
            /* [out] */ BOOL *pfCallOnScriptErrorWhenContinuing);
        
        END_INTERFACE
    } IActiveScriptSiteDebugVtbl;

    interface IActiveScriptSiteDebug
    {
        CONST_VTBL struct IActiveScriptSiteDebugVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IActiveScriptSiteDebug_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IActiveScriptSiteDebug_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IActiveScriptSiteDebug_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IActiveScriptSiteDebug_GetDocumentContextFromPosition(This,dwSourceContext,uCharacterOffset,uNumChars,ppsc)	\
    (This)->lpVtbl -> GetDocumentContextFromPosition(This,dwSourceContext,uCharacterOffset,uNumChars,ppsc)

#define IActiveScriptSiteDebug_GetApplication(This,ppda)	\
    (This)->lpVtbl -> GetApplication(This,ppda)

#define IActiveScriptSiteDebug_GetRootApplicationNode(This,ppdanRoot)	\
    (This)->lpVtbl -> GetRootApplicationNode(This,ppdanRoot)

#define IActiveScriptSiteDebug_OnScriptErrorDebug(This,pErrorDebug,pfEnterDebugger,pfCallOnScriptErrorWhenContinuing)	\
    (This)->lpVtbl -> OnScriptErrorDebug(This,pErrorDebug,pfEnterDebugger,pfCallOnScriptErrorWhenContinuing)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IActiveScriptSiteDebug_GetDocumentContextFromPosition_Proxy( 
    IActiveScriptSiteDebug * This,
    /* [in] */ DWORD dwSourceContext,
    /* [in] */ ULONG uCharacterOffset,
    /* [in] */ ULONG uNumChars,
    /* [out] */ IDebugDocumentContext **ppsc);


void __RPC_STUB IActiveScriptSiteDebug_GetDocumentContextFromPosition_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IActiveScriptSiteDebug_GetApplication_Proxy( 
    IActiveScriptSiteDebug * This,
    /* [out] */ IDebugApplication **ppda);


void __RPC_STUB IActiveScriptSiteDebug_GetApplication_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IActiveScriptSiteDebug_GetRootApplicationNode_Proxy( 
    IActiveScriptSiteDebug * This,
    /* [out] */ IDebugApplicationNode **ppdanRoot);


void __RPC_STUB IActiveScriptSiteDebug_GetRootApplicationNode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IActiveScriptSiteDebug_OnScriptErrorDebug_Proxy( 
    IActiveScriptSiteDebug * This,
    /* [in] */ IActiveScriptErrorDebug *pErrorDebug,
    /* [out] */ BOOL *pfEnterDebugger,
    /* [out] */ BOOL *pfCallOnScriptErrorWhenContinuing);


void __RPC_STUB IActiveScriptSiteDebug_OnScriptErrorDebug_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IActiveScriptSiteDebug_INTERFACE_DEFINED__ */


#ifndef __IDebugCodeContext_INTERFACE_DEFINED__
#define __IDebugCodeContext_INTERFACE_DEFINED__

/* interface IDebugCodeContext */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IDebugCodeContext;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("51973C13-CB0C-11d0-B5C9-00A0244A0E7A")
    IDebugCodeContext : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetDocumentContext( 
            /* [out] */ IDebugDocumentContext **ppsc) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetBreakPoint( 
            /* [in] */ BREAKPOINT_STATE bps) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDebugCodeContextVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDebugCodeContext * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDebugCodeContext * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDebugCodeContext * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetDocumentContext )( 
            IDebugCodeContext * This,
            /* [out] */ IDebugDocumentContext **ppsc);
        
        HRESULT ( STDMETHODCALLTYPE *SetBreakPoint )( 
            IDebugCodeContext * This,
            /* [in] */ BREAKPOINT_STATE bps);
        
        END_INTERFACE
    } IDebugCodeContextVtbl;

    interface IDebugCodeContext
    {
        CONST_VTBL struct IDebugCodeContextVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDebugCodeContext_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDebugCodeContext_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDebugCodeContext_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDebugCodeContext_GetDocumentContext(This,ppsc)	\
    (This)->lpVtbl -> GetDocumentContext(This,ppsc)

#define IDebugCodeContext_SetBreakPoint(This,bps)	\
    (This)->lpVtbl -> SetBreakPoint(This,bps)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IDebugCodeContext_GetDocumentContext_Proxy( 
    IDebugCodeContext * This,
    /* [out] */ IDebugDocumentContext **ppsc);


void __RPC_STUB IDebugCodeContext_GetDocumentContext_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugCodeContext_SetBreakPoint_Proxy( 
    IDebugCodeContext * This,
    /* [in] */ BREAKPOINT_STATE bps);


void __RPC_STUB IDebugCodeContext_SetBreakPoint_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDebugCodeContext_INTERFACE_DEFINED__ */


#ifndef __IEnumDebugCodeContexts_INTERFACE_DEFINED__
#define __IEnumDebugCodeContexts_INTERFACE_DEFINED__

/* interface IEnumDebugCodeContexts */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IEnumDebugCodeContexts;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("51973C1d-CB0C-11d0-B5C9-00A0244A0E7A")
    IEnumDebugCodeContexts : public IUnknown
    {
    public:
        virtual /* [local] */ HRESULT __stdcall Next( 
            /* [in] */ ULONG celt,
            /* [out] */ IDebugCodeContext **pscc,
            /* [out] */ ULONG *pceltFetched) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Skip( 
            /* [in] */ ULONG celt) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Reset( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Clone( 
            /* [out] */ IEnumDebugCodeContexts **ppescc) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IEnumDebugCodeContextsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IEnumDebugCodeContexts * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IEnumDebugCodeContexts * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IEnumDebugCodeContexts * This);
        
        /* [local] */ HRESULT ( __stdcall *Next )( 
            IEnumDebugCodeContexts * This,
            /* [in] */ ULONG celt,
            /* [out] */ IDebugCodeContext **pscc,
            /* [out] */ ULONG *pceltFetched);
        
        HRESULT ( STDMETHODCALLTYPE *Skip )( 
            IEnumDebugCodeContexts * This,
            /* [in] */ ULONG celt);
        
        HRESULT ( STDMETHODCALLTYPE *Reset )( 
            IEnumDebugCodeContexts * This);
        
        HRESULT ( STDMETHODCALLTYPE *Clone )( 
            IEnumDebugCodeContexts * This,
            /* [out] */ IEnumDebugCodeContexts **ppescc);
        
        END_INTERFACE
    } IEnumDebugCodeContextsVtbl;

    interface IEnumDebugCodeContexts
    {
        CONST_VTBL struct IEnumDebugCodeContextsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEnumDebugCodeContexts_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IEnumDebugCodeContexts_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IEnumDebugCodeContexts_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IEnumDebugCodeContexts_Next(This,celt,pscc,pceltFetched)	\
    (This)->lpVtbl -> Next(This,celt,pscc,pceltFetched)

#define IEnumDebugCodeContexts_Skip(This,celt)	\
    (This)->lpVtbl -> Skip(This,celt)

#define IEnumDebugCodeContexts_Reset(This)	\
    (This)->lpVtbl -> Reset(This)

#define IEnumDebugCodeContexts_Clone(This,ppescc)	\
    (This)->lpVtbl -> Clone(This,ppescc)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [call_as] */ HRESULT __stdcall IEnumDebugCodeContexts_RemoteNext_Proxy( 
    IEnumDebugCodeContexts * This,
    /* [in] */ ULONG celt,
    /* [length_is][size_is][out] */ IDebugCodeContext **pscc,
    /* [out] */ ULONG *pceltFetched);


void __RPC_STUB IEnumDebugCodeContexts_RemoteNext_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEnumDebugCodeContexts_Skip_Proxy( 
    IEnumDebugCodeContexts * This,
    /* [in] */ ULONG celt);


void __RPC_STUB IEnumDebugCodeContexts_Skip_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEnumDebugCodeContexts_Reset_Proxy( 
    IEnumDebugCodeContexts * This);


void __RPC_STUB IEnumDebugCodeContexts_Reset_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEnumDebugCodeContexts_Clone_Proxy( 
    IEnumDebugCodeContexts * This,
    /* [out] */ IEnumDebugCodeContexts **ppescc);


void __RPC_STUB IEnumDebugCodeContexts_Clone_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IEnumDebugCodeContexts_INTERFACE_DEFINED__ */


#ifndef __IDebugStackFrame_INTERFACE_DEFINED__
#define __IDebugStackFrame_INTERFACE_DEFINED__

/* interface IDebugStackFrame */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IDebugStackFrame;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("51973C17-CB0C-11d0-B5C9-00A0244A0E7A")
    IDebugStackFrame : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetCodeContext( 
            /* [out] */ IDebugCodeContext **ppcc) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetDescriptionString( 
            /* [in] */ BOOL fLong,
            /* [out] */ BSTR *pbstrDescription) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetLanguageString( 
            /* [in] */ BOOL fLong,
            /* [out] */ BSTR *pbstrLanguage) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetThread( 
            /* [out] */ IDebugApplicationThread **ppat) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetDebugProperty( 
            /* [out] */ IDebugProperty **ppDebugProp) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDebugStackFrameVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDebugStackFrame * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDebugStackFrame * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDebugStackFrame * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetCodeContext )( 
            IDebugStackFrame * This,
            /* [out] */ IDebugCodeContext **ppcc);
        
        HRESULT ( STDMETHODCALLTYPE *GetDescriptionString )( 
            IDebugStackFrame * This,
            /* [in] */ BOOL fLong,
            /* [out] */ BSTR *pbstrDescription);
        
        HRESULT ( STDMETHODCALLTYPE *GetLanguageString )( 
            IDebugStackFrame * This,
            /* [in] */ BOOL fLong,
            /* [out] */ BSTR *pbstrLanguage);
        
        HRESULT ( STDMETHODCALLTYPE *GetThread )( 
            IDebugStackFrame * This,
            /* [out] */ IDebugApplicationThread **ppat);
        
        HRESULT ( STDMETHODCALLTYPE *GetDebugProperty )( 
            IDebugStackFrame * This,
            /* [out] */ IDebugProperty **ppDebugProp);
        
        END_INTERFACE
    } IDebugStackFrameVtbl;

    interface IDebugStackFrame
    {
        CONST_VTBL struct IDebugStackFrameVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDebugStackFrame_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDebugStackFrame_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDebugStackFrame_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDebugStackFrame_GetCodeContext(This,ppcc)	\
    (This)->lpVtbl -> GetCodeContext(This,ppcc)

#define IDebugStackFrame_GetDescriptionString(This,fLong,pbstrDescription)	\
    (This)->lpVtbl -> GetDescriptionString(This,fLong,pbstrDescription)

#define IDebugStackFrame_GetLanguageString(This,fLong,pbstrLanguage)	\
    (This)->lpVtbl -> GetLanguageString(This,fLong,pbstrLanguage)

#define IDebugStackFrame_GetThread(This,ppat)	\
    (This)->lpVtbl -> GetThread(This,ppat)

#define IDebugStackFrame_GetDebugProperty(This,ppDebugProp)	\
    (This)->lpVtbl -> GetDebugProperty(This,ppDebugProp)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IDebugStackFrame_GetCodeContext_Proxy( 
    IDebugStackFrame * This,
    /* [out] */ IDebugCodeContext **ppcc);


void __RPC_STUB IDebugStackFrame_GetCodeContext_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugStackFrame_GetDescriptionString_Proxy( 
    IDebugStackFrame * This,
    /* [in] */ BOOL fLong,
    /* [out] */ BSTR *pbstrDescription);


void __RPC_STUB IDebugStackFrame_GetDescriptionString_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugStackFrame_GetLanguageString_Proxy( 
    IDebugStackFrame * This,
    /* [in] */ BOOL fLong,
    /* [out] */ BSTR *pbstrLanguage);


void __RPC_STUB IDebugStackFrame_GetLanguageString_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugStackFrame_GetThread_Proxy( 
    IDebugStackFrame * This,
    /* [out] */ IDebugApplicationThread **ppat);


void __RPC_STUB IDebugStackFrame_GetThread_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugStackFrame_GetDebugProperty_Proxy( 
    IDebugStackFrame * This,
    /* [out] */ IDebugProperty **ppDebugProp);


void __RPC_STUB IDebugStackFrame_GetDebugProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDebugStackFrame_INTERFACE_DEFINED__ */


#ifndef __IDebugExpressionContext_INTERFACE_DEFINED__
#define __IDebugExpressionContext_INTERFACE_DEFINED__

/* interface IDebugExpressionContext */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IDebugExpressionContext;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("51973C15-CB0C-11d0-B5C9-00A0244A0E7A")
    IDebugExpressionContext : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE ParseLanguageText( 
            /* [in] */ LPCOLESTR pstrCode,
            /* [in] */ UINT nRadix,
            /* [in] */ LPCOLESTR pstrDelimiter,
            /* [in] */ DWORD dwFlags,
            /* [out] */ IDebugExpression **ppe) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetLanguageInfo( 
            /* [out] */ BSTR *pbstrLanguageName,
            /* [out] */ GUID *pLanguageID) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDebugExpressionContextVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDebugExpressionContext * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDebugExpressionContext * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDebugExpressionContext * This);
        
        HRESULT ( STDMETHODCALLTYPE *ParseLanguageText )( 
            IDebugExpressionContext * This,
            /* [in] */ LPCOLESTR pstrCode,
            /* [in] */ UINT nRadix,
            /* [in] */ LPCOLESTR pstrDelimiter,
            /* [in] */ DWORD dwFlags,
            /* [out] */ IDebugExpression **ppe);
        
        HRESULT ( STDMETHODCALLTYPE *GetLanguageInfo )( 
            IDebugExpressionContext * This,
            /* [out] */ BSTR *pbstrLanguageName,
            /* [out] */ GUID *pLanguageID);
        
        END_INTERFACE
    } IDebugExpressionContextVtbl;

    interface IDebugExpressionContext
    {
        CONST_VTBL struct IDebugExpressionContextVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDebugExpressionContext_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDebugExpressionContext_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDebugExpressionContext_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDebugExpressionContext_ParseLanguageText(This,pstrCode,nRadix,pstrDelimiter,dwFlags,ppe)	\
    (This)->lpVtbl -> ParseLanguageText(This,pstrCode,nRadix,pstrDelimiter,dwFlags,ppe)

#define IDebugExpressionContext_GetLanguageInfo(This,pbstrLanguageName,pLanguageID)	\
    (This)->lpVtbl -> GetLanguageInfo(This,pbstrLanguageName,pLanguageID)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IDebugExpressionContext_ParseLanguageText_Proxy( 
    IDebugExpressionContext * This,
    /* [in] */ LPCOLESTR pstrCode,
    /* [in] */ UINT nRadix,
    /* [in] */ LPCOLESTR pstrDelimiter,
    /* [in] */ DWORD dwFlags,
    /* [out] */ IDebugExpression **ppe);


void __RPC_STUB IDebugExpressionContext_ParseLanguageText_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugExpressionContext_GetLanguageInfo_Proxy( 
    IDebugExpressionContext * This,
    /* [out] */ BSTR *pbstrLanguageName,
    /* [out] */ GUID *pLanguageID);


void __RPC_STUB IDebugExpressionContext_GetLanguageInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDebugExpressionContext_INTERFACE_DEFINED__ */


#ifndef __IDebugStackFrameSniffer_INTERFACE_DEFINED__
#define __IDebugStackFrameSniffer_INTERFACE_DEFINED__

/* interface IDebugStackFrameSniffer */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IDebugStackFrameSniffer;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("51973C18-CB0C-11d0-B5C9-00A0244A0E7A")
    IDebugStackFrameSniffer : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE EnumStackFrames( 
            /* [out] */ IEnumDebugStackFrames **ppedsf) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDebugStackFrameSnifferVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDebugStackFrameSniffer * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDebugStackFrameSniffer * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDebugStackFrameSniffer * This);
        
        HRESULT ( STDMETHODCALLTYPE *EnumStackFrames )( 
            IDebugStackFrameSniffer * This,
            /* [out] */ IEnumDebugStackFrames **ppedsf);
        
        END_INTERFACE
    } IDebugStackFrameSnifferVtbl;

    interface IDebugStackFrameSniffer
    {
        CONST_VTBL struct IDebugStackFrameSnifferVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDebugStackFrameSniffer_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDebugStackFrameSniffer_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDebugStackFrameSniffer_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDebugStackFrameSniffer_EnumStackFrames(This,ppedsf)	\
    (This)->lpVtbl -> EnumStackFrames(This,ppedsf)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IDebugStackFrameSniffer_EnumStackFrames_Proxy( 
    IDebugStackFrameSniffer * This,
    /* [out] */ IEnumDebugStackFrames **ppedsf);


void __RPC_STUB IDebugStackFrameSniffer_EnumStackFrames_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDebugStackFrameSniffer_INTERFACE_DEFINED__ */


#ifndef __IDebugExpression_INTERFACE_DEFINED__
#define __IDebugExpression_INTERFACE_DEFINED__

/* interface IDebugExpression */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IDebugExpression;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("51973C14-CB0C-11d0-B5C9-00A0244A0E7A")
    IDebugExpression : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Start( 
            /* [in] */ IDebugExpressionCallBack *pdecb) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Abort( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE QueryIsComplete( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetResultAsString( 
            /* [out] */ HRESULT *phrResult,
            /* [out] */ BSTR *pbstrResult) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetResultAsDebugProperty( 
            /* [out] */ HRESULT *phrResult,
            /* [out] */ IDebugProperty **ppdp) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDebugExpressionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDebugExpression * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDebugExpression * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDebugExpression * This);
        
        HRESULT ( STDMETHODCALLTYPE *Start )( 
            IDebugExpression * This,
            /* [in] */ IDebugExpressionCallBack *pdecb);
        
        HRESULT ( STDMETHODCALLTYPE *Abort )( 
            IDebugExpression * This);
        
        HRESULT ( STDMETHODCALLTYPE *QueryIsComplete )( 
            IDebugExpression * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetResultAsString )( 
            IDebugExpression * This,
            /* [out] */ HRESULT *phrResult,
            /* [out] */ BSTR *pbstrResult);
        
        HRESULT ( STDMETHODCALLTYPE *GetResultAsDebugProperty )( 
            IDebugExpression * This,
            /* [out] */ HRESULT *phrResult,
            /* [out] */ IDebugProperty **ppdp);
        
        END_INTERFACE
    } IDebugExpressionVtbl;

    interface IDebugExpression
    {
        CONST_VTBL struct IDebugExpressionVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDebugExpression_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDebugExpression_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDebugExpression_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDebugExpression_Start(This,pdecb)	\
    (This)->lpVtbl -> Start(This,pdecb)

#define IDebugExpression_Abort(This)	\
    (This)->lpVtbl -> Abort(This)

#define IDebugExpression_QueryIsComplete(This)	\
    (This)->lpVtbl -> QueryIsComplete(This)

#define IDebugExpression_GetResultAsString(This,phrResult,pbstrResult)	\
    (This)->lpVtbl -> GetResultAsString(This,phrResult,pbstrResult)

#define IDebugExpression_GetResultAsDebugProperty(This,phrResult,ppdp)	\
    (This)->lpVtbl -> GetResultAsDebugProperty(This,phrResult,ppdp)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IDebugExpression_Start_Proxy( 
    IDebugExpression * This,
    /* [in] */ IDebugExpressionCallBack *pdecb);


void __RPC_STUB IDebugExpression_Start_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugExpression_Abort_Proxy( 
    IDebugExpression * This);


void __RPC_STUB IDebugExpression_Abort_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugExpression_QueryIsComplete_Proxy( 
    IDebugExpression * This);


void __RPC_STUB IDebugExpression_QueryIsComplete_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugExpression_GetResultAsString_Proxy( 
    IDebugExpression * This,
    /* [out] */ HRESULT *phrResult,
    /* [out] */ BSTR *pbstrResult);


void __RPC_STUB IDebugExpression_GetResultAsString_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugExpression_GetResultAsDebugProperty_Proxy( 
    IDebugExpression * This,
    /* [out] */ HRESULT *phrResult,
    /* [out] */ IDebugProperty **ppdp);


void __RPC_STUB IDebugExpression_GetResultAsDebugProperty_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDebugExpression_INTERFACE_DEFINED__ */


#ifndef __IDebugSyncOperation_INTERFACE_DEFINED__
#define __IDebugSyncOperation_INTERFACE_DEFINED__

/* interface IDebugSyncOperation */
/* [local][unique][uuid][object] */ 


EXTERN_C const IID IID_IDebugSyncOperation;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("51973C1a-CB0C-11d0-B5C9-00A0244A0E7A")
    IDebugSyncOperation : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetTargetThread( 
            /* [out] */ IDebugApplicationThread **ppatTarget) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Execute( 
            /* [out] */ IUnknown **ppunkResult) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE InProgressAbort( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDebugSyncOperationVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDebugSyncOperation * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDebugSyncOperation * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDebugSyncOperation * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTargetThread )( 
            IDebugSyncOperation * This,
            /* [out] */ IDebugApplicationThread **ppatTarget);
        
        HRESULT ( STDMETHODCALLTYPE *Execute )( 
            IDebugSyncOperation * This,
            /* [out] */ IUnknown **ppunkResult);
        
        HRESULT ( STDMETHODCALLTYPE *InProgressAbort )( 
            IDebugSyncOperation * This);
        
        END_INTERFACE
    } IDebugSyncOperationVtbl;

    interface IDebugSyncOperation
    {
        CONST_VTBL struct IDebugSyncOperationVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDebugSyncOperation_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDebugSyncOperation_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDebugSyncOperation_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDebugSyncOperation_GetTargetThread(This,ppatTarget)	\
    (This)->lpVtbl -> GetTargetThread(This,ppatTarget)

#define IDebugSyncOperation_Execute(This,ppunkResult)	\
    (This)->lpVtbl -> Execute(This,ppunkResult)

#define IDebugSyncOperation_InProgressAbort(This)	\
    (This)->lpVtbl -> InProgressAbort(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IDebugSyncOperation_GetTargetThread_Proxy( 
    IDebugSyncOperation * This,
    /* [out] */ IDebugApplicationThread **ppatTarget);


void __RPC_STUB IDebugSyncOperation_GetTargetThread_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugSyncOperation_Execute_Proxy( 
    IDebugSyncOperation * This,
    /* [out] */ IUnknown **ppunkResult);


void __RPC_STUB IDebugSyncOperation_Execute_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugSyncOperation_InProgressAbort_Proxy( 
    IDebugSyncOperation * This);


void __RPC_STUB IDebugSyncOperation_InProgressAbort_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDebugSyncOperation_INTERFACE_DEFINED__ */


#ifndef __IDebugAsyncOperation_INTERFACE_DEFINED__
#define __IDebugAsyncOperation_INTERFACE_DEFINED__

/* interface IDebugAsyncOperation */
/* [local][unique][uuid][object] */ 


EXTERN_C const IID IID_IDebugAsyncOperation;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("51973C1b-CB0C-11d0-B5C9-00A0244A0E7A")
    IDebugAsyncOperation : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetSyncDebugOperation( 
            /* [out] */ IDebugSyncOperation **ppsdo) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Start( 
            IDebugAsyncOperationCallBack *padocb) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Abort( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE QueryIsComplete( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetResult( 
            /* [out] */ HRESULT *phrResult,
            /* [out] */ IUnknown **ppunkResult) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDebugAsyncOperationVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDebugAsyncOperation * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDebugAsyncOperation * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDebugAsyncOperation * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetSyncDebugOperation )( 
            IDebugAsyncOperation * This,
            /* [out] */ IDebugSyncOperation **ppsdo);
        
        HRESULT ( STDMETHODCALLTYPE *Start )( 
            IDebugAsyncOperation * This,
            IDebugAsyncOperationCallBack *padocb);
        
        HRESULT ( STDMETHODCALLTYPE *Abort )( 
            IDebugAsyncOperation * This);
        
        HRESULT ( STDMETHODCALLTYPE *QueryIsComplete )( 
            IDebugAsyncOperation * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetResult )( 
            IDebugAsyncOperation * This,
            /* [out] */ HRESULT *phrResult,
            /* [out] */ IUnknown **ppunkResult);
        
        END_INTERFACE
    } IDebugAsyncOperationVtbl;

    interface IDebugAsyncOperation
    {
        CONST_VTBL struct IDebugAsyncOperationVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDebugAsyncOperation_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDebugAsyncOperation_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDebugAsyncOperation_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDebugAsyncOperation_GetSyncDebugOperation(This,ppsdo)	\
    (This)->lpVtbl -> GetSyncDebugOperation(This,ppsdo)

#define IDebugAsyncOperation_Start(This,padocb)	\
    (This)->lpVtbl -> Start(This,padocb)

#define IDebugAsyncOperation_Abort(This)	\
    (This)->lpVtbl -> Abort(This)

#define IDebugAsyncOperation_QueryIsComplete(This)	\
    (This)->lpVtbl -> QueryIsComplete(This)

#define IDebugAsyncOperation_GetResult(This,phrResult,ppunkResult)	\
    (This)->lpVtbl -> GetResult(This,phrResult,ppunkResult)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IDebugAsyncOperation_GetSyncDebugOperation_Proxy( 
    IDebugAsyncOperation * This,
    /* [out] */ IDebugSyncOperation **ppsdo);


void __RPC_STUB IDebugAsyncOperation_GetSyncDebugOperation_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugAsyncOperation_Start_Proxy( 
    IDebugAsyncOperation * This,
    IDebugAsyncOperationCallBack *padocb);


void __RPC_STUB IDebugAsyncOperation_Start_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugAsyncOperation_Abort_Proxy( 
    IDebugAsyncOperation * This);


void __RPC_STUB IDebugAsyncOperation_Abort_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugAsyncOperation_QueryIsComplete_Proxy( 
    IDebugAsyncOperation * This);


void __RPC_STUB IDebugAsyncOperation_QueryIsComplete_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugAsyncOperation_GetResult_Proxy( 
    IDebugAsyncOperation * This,
    /* [out] */ HRESULT *phrResult,
    /* [out] */ IUnknown **ppunkResult);


void __RPC_STUB IDebugAsyncOperation_GetResult_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDebugAsyncOperation_INTERFACE_DEFINED__ */


#ifndef __IDebugAsyncOperationCallBack_INTERFACE_DEFINED__
#define __IDebugAsyncOperationCallBack_INTERFACE_DEFINED__

/* interface IDebugAsyncOperationCallBack */
/* [local][unique][uuid][object] */ 


EXTERN_C const IID IID_IDebugAsyncOperationCallBack;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("51973C1c-CB0C-11d0-B5C9-00A0244A0E7A")
    IDebugAsyncOperationCallBack : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE onComplete( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDebugAsyncOperationCallBackVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDebugAsyncOperationCallBack * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDebugAsyncOperationCallBack * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDebugAsyncOperationCallBack * This);
        
        HRESULT ( STDMETHODCALLTYPE *onComplete )( 
            IDebugAsyncOperationCallBack * This);
        
        END_INTERFACE
    } IDebugAsyncOperationCallBackVtbl;

    interface IDebugAsyncOperationCallBack
    {
        CONST_VTBL struct IDebugAsyncOperationCallBackVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDebugAsyncOperationCallBack_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDebugAsyncOperationCallBack_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDebugAsyncOperationCallBack_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDebugAsyncOperationCallBack_onComplete(This)	\
    (This)->lpVtbl -> onComplete(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IDebugAsyncOperationCallBack_onComplete_Proxy( 
    IDebugAsyncOperationCallBack * This);


void __RPC_STUB IDebugAsyncOperationCallBack_onComplete_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDebugAsyncOperationCallBack_INTERFACE_DEFINED__ */


#ifndef __IDebugExpressionCallBack_INTERFACE_DEFINED__
#define __IDebugExpressionCallBack_INTERFACE_DEFINED__

/* interface IDebugExpressionCallBack */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IDebugExpressionCallBack;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("51973C16-CB0C-11d0-B5C9-00A0244A0E7A")
    IDebugExpressionCallBack : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE onComplete( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDebugExpressionCallBackVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDebugExpressionCallBack * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDebugExpressionCallBack * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDebugExpressionCallBack * This);
        
        HRESULT ( STDMETHODCALLTYPE *onComplete )( 
            IDebugExpressionCallBack * This);
        
        END_INTERFACE
    } IDebugExpressionCallBackVtbl;

    interface IDebugExpressionCallBack
    {
        CONST_VTBL struct IDebugExpressionCallBackVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDebugExpressionCallBack_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDebugExpressionCallBack_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDebugExpressionCallBack_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDebugExpressionCallBack_onComplete(This)	\
    (This)->lpVtbl -> onComplete(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IDebugExpressionCallBack_onComplete_Proxy( 
    IDebugExpressionCallBack * This);


void __RPC_STUB IDebugExpressionCallBack_onComplete_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDebugExpressionCallBack_INTERFACE_DEFINED__ */


#ifndef __IEnumDebugExpressionContexts_INTERFACE_DEFINED__
#define __IEnumDebugExpressionContexts_INTERFACE_DEFINED__

/* interface IEnumDebugExpressionContexts */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IEnumDebugExpressionContexts;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("51973C40-CB0C-11d0-B5C9-00A0244A0E7A")
    IEnumDebugExpressionContexts : public IUnknown
    {
    public:
        virtual /* [local] */ HRESULT __stdcall Next( 
            /* [in] */ ULONG celt,
            /* [out] */ IDebugExpressionContext **ppdec,
            /* [out] */ ULONG *pceltFetched) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Skip( 
            /* [in] */ ULONG celt) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Reset( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Clone( 
            /* [out] */ IEnumDebugExpressionContexts **ppedec) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IEnumDebugExpressionContextsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IEnumDebugExpressionContexts * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IEnumDebugExpressionContexts * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IEnumDebugExpressionContexts * This);
        
        /* [local] */ HRESULT ( __stdcall *Next )( 
            IEnumDebugExpressionContexts * This,
            /* [in] */ ULONG celt,
            /* [out] */ IDebugExpressionContext **ppdec,
            /* [out] */ ULONG *pceltFetched);
        
        HRESULT ( STDMETHODCALLTYPE *Skip )( 
            IEnumDebugExpressionContexts * This,
            /* [in] */ ULONG celt);
        
        HRESULT ( STDMETHODCALLTYPE *Reset )( 
            IEnumDebugExpressionContexts * This);
        
        HRESULT ( STDMETHODCALLTYPE *Clone )( 
            IEnumDebugExpressionContexts * This,
            /* [out] */ IEnumDebugExpressionContexts **ppedec);
        
        END_INTERFACE
    } IEnumDebugExpressionContextsVtbl;

    interface IEnumDebugExpressionContexts
    {
        CONST_VTBL struct IEnumDebugExpressionContextsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEnumDebugExpressionContexts_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IEnumDebugExpressionContexts_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IEnumDebugExpressionContexts_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IEnumDebugExpressionContexts_Next(This,celt,ppdec,pceltFetched)	\
    (This)->lpVtbl -> Next(This,celt,ppdec,pceltFetched)

#define IEnumDebugExpressionContexts_Skip(This,celt)	\
    (This)->lpVtbl -> Skip(This,celt)

#define IEnumDebugExpressionContexts_Reset(This)	\
    (This)->lpVtbl -> Reset(This)

#define IEnumDebugExpressionContexts_Clone(This,ppedec)	\
    (This)->lpVtbl -> Clone(This,ppedec)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [call_as] */ HRESULT __stdcall IEnumDebugExpressionContexts_RemoteNext_Proxy( 
    IEnumDebugExpressionContexts * This,
    /* [in] */ ULONG celt,
    /* [length_is][size_is][out] */ IDebugExpressionContext **pprgdec,
    /* [out] */ ULONG *pceltFetched);


void __RPC_STUB IEnumDebugExpressionContexts_RemoteNext_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEnumDebugExpressionContexts_Skip_Proxy( 
    IEnumDebugExpressionContexts * This,
    /* [in] */ ULONG celt);


void __RPC_STUB IEnumDebugExpressionContexts_Skip_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEnumDebugExpressionContexts_Reset_Proxy( 
    IEnumDebugExpressionContexts * This);


void __RPC_STUB IEnumDebugExpressionContexts_Reset_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEnumDebugExpressionContexts_Clone_Proxy( 
    IEnumDebugExpressionContexts * This,
    /* [out] */ IEnumDebugExpressionContexts **ppedec);


void __RPC_STUB IEnumDebugExpressionContexts_Clone_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IEnumDebugExpressionContexts_INTERFACE_DEFINED__ */


#ifndef __IProvideExpressionContexts_INTERFACE_DEFINED__
#define __IProvideExpressionContexts_INTERFACE_DEFINED__

/* interface IProvideExpressionContexts */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IProvideExpressionContexts;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("51973C41-CB0C-11d0-B5C9-00A0244A0E7A")
    IProvideExpressionContexts : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE EnumExpressionContexts( 
            /* [out] */ IEnumDebugExpressionContexts **ppedec) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IProvideExpressionContextsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IProvideExpressionContexts * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IProvideExpressionContexts * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IProvideExpressionContexts * This);
        
        HRESULT ( STDMETHODCALLTYPE *EnumExpressionContexts )( 
            IProvideExpressionContexts * This,
            /* [out] */ IEnumDebugExpressionContexts **ppedec);
        
        END_INTERFACE
    } IProvideExpressionContextsVtbl;

    interface IProvideExpressionContexts
    {
        CONST_VTBL struct IProvideExpressionContextsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IProvideExpressionContexts_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IProvideExpressionContexts_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IProvideExpressionContexts_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IProvideExpressionContexts_EnumExpressionContexts(This,ppedec)	\
    (This)->lpVtbl -> EnumExpressionContexts(This,ppedec)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IProvideExpressionContexts_EnumExpressionContexts_Proxy( 
    IProvideExpressionContexts * This,
    /* [out] */ IEnumDebugExpressionContexts **ppedec);


void __RPC_STUB IProvideExpressionContexts_EnumExpressionContexts_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IProvideExpressionContexts_INTERFACE_DEFINED__ */


#ifndef __IDebugFormatter_INTERFACE_DEFINED__
#define __IDebugFormatter_INTERFACE_DEFINED__

/* interface IDebugFormatter */
/* [local][unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IDebugFormatter;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("51973C3d-CB0C-11d0-B5C9-00A0244A0E7A")
    IDebugFormatter : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetStringForVariant( 
            /* [in] */ VARIANT *pvar,
            /* [out] */ BSTR *pbstrValue) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetVariantForString( 
            /* [in] */ LPCOLESTR pwstrValue,
            /* [out] */ VARIANT *pvar) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetStringForVarType( 
            /* [in] */ VARTYPE vt,
            /* [in] */ TYPEDESC *ptdescArrayType,
            /* [out] */ BSTR *pbstr) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDebugFormatterVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDebugFormatter * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDebugFormatter * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDebugFormatter * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetStringForVariant )( 
            IDebugFormatter * This,
            /* [in] */ VARIANT *pvar,
            /* [out] */ BSTR *pbstrValue);
        
        HRESULT ( STDMETHODCALLTYPE *GetVariantForString )( 
            IDebugFormatter * This,
            /* [in] */ LPCOLESTR pwstrValue,
            /* [out] */ VARIANT *pvar);
        
        HRESULT ( STDMETHODCALLTYPE *GetStringForVarType )( 
            IDebugFormatter * This,
            /* [in] */ VARTYPE vt,
            /* [in] */ TYPEDESC *ptdescArrayType,
            /* [out] */ BSTR *pbstr);
        
        END_INTERFACE
    } IDebugFormatterVtbl;

    interface IDebugFormatter
    {
        CONST_VTBL struct IDebugFormatterVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDebugFormatter_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDebugFormatter_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDebugFormatter_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDebugFormatter_GetStringForVariant(This,pvar,pbstrValue)	\
    (This)->lpVtbl -> GetStringForVariant(This,pvar,pbstrValue)

#define IDebugFormatter_GetVariantForString(This,pwstrValue,pvar)	\
    (This)->lpVtbl -> GetVariantForString(This,pwstrValue,pvar)

#define IDebugFormatter_GetStringForVarType(This,vt,ptdescArrayType,pbstr)	\
    (This)->lpVtbl -> GetStringForVarType(This,vt,ptdescArrayType,pbstr)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IDebugFormatter_GetStringForVariant_Proxy( 
    IDebugFormatter * This,
    /* [in] */ VARIANT *pvar,
    /* [out] */ BSTR *pbstrValue);


void __RPC_STUB IDebugFormatter_GetStringForVariant_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugFormatter_GetVariantForString_Proxy( 
    IDebugFormatter * This,
    /* [in] */ LPCOLESTR pwstrValue,
    /* [out] */ VARIANT *pvar);


void __RPC_STUB IDebugFormatter_GetVariantForString_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugFormatter_GetStringForVarType_Proxy( 
    IDebugFormatter * This,
    /* [in] */ VARTYPE vt,
    /* [in] */ TYPEDESC *ptdescArrayType,
    /* [out] */ BSTR *pbstr);


void __RPC_STUB IDebugFormatter_GetStringForVarType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDebugFormatter_INTERFACE_DEFINED__ */


#ifndef __IDebugProperty_INTERFACE_DEFINED__
#define __IDebugProperty_INTERFACE_DEFINED__

/* interface IDebugProperty */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IDebugProperty;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("51973C50-CB0C-11d0-B5C9-00A0244A0E7A")
    IDebugProperty : public IUnknown
    {
    public:
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE GetPropertyInfo( 
            /* [in] */ DWORD dwFieldSpec,
            /* [in] */ UINT nRadix,
            /* [out] */ DebugPropertyInfo *pPropertyInfo) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetExtendedInfo( 
            /* [in] */ ULONG cInfos,
            /* [in][size_is] */ GUID *rgguidExtendedInfo,
            /* [out][size_is] */ VARIANT *rgvar) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetValueAsString( 
            /* [in] */ LPCOLESTR pszValue,
            /* [in] */ UINT nRadix) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE EnumMembers( 
            /* [in] */ DWORD dwFieldSpec,
            /* [in] */ UINT nRadix,
            /* [in] */ REFIID refiid,
            /* [out] */ IEnumDebugPropertyInfo **ppepi) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetParent( 
            /* [out] */ IDebugProperty **ppDebugProp) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDebugPropertyVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDebugProperty * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDebugProperty * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDebugProperty * This);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *GetPropertyInfo )( 
            IDebugProperty * This,
            /* [in] */ DWORD dwFieldSpec,
            /* [in] */ UINT nRadix,
            /* [out] */ DebugPropertyInfo *pPropertyInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetExtendedInfo )( 
            IDebugProperty * This,
            /* [in] */ ULONG cInfos,
            /* [in][size_is] */ GUID *rgguidExtendedInfo,
            /* [out][size_is] */ VARIANT *rgvar);
        
        HRESULT ( STDMETHODCALLTYPE *SetValueAsString )( 
            IDebugProperty * This,
            /* [in] */ LPCOLESTR pszValue,
            /* [in] */ UINT nRadix);
        
        HRESULT ( STDMETHODCALLTYPE *EnumMembers )( 
            IDebugProperty * This,
            /* [in] */ DWORD dwFieldSpec,
            /* [in] */ UINT nRadix,
            /* [in] */ REFIID refiid,
            /* [out] */ IEnumDebugPropertyInfo **ppepi);
        
        HRESULT ( STDMETHODCALLTYPE *GetParent )( 
            IDebugProperty * This,
            /* [out] */ IDebugProperty **ppDebugProp);
        
        END_INTERFACE
    } IDebugPropertyVtbl;

    interface IDebugProperty
    {
        CONST_VTBL struct IDebugPropertyVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDebugProperty_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDebugProperty_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDebugProperty_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDebugProperty_GetPropertyInfo(This,dwFieldSpec,nRadix,pPropertyInfo)	\
    (This)->lpVtbl -> GetPropertyInfo(This,dwFieldSpec,nRadix,pPropertyInfo)

#define IDebugProperty_GetExtendedInfo(This,cInfos,rgguidExtendedInfo,rgvar)	\
    (This)->lpVtbl -> GetExtendedInfo(This,cInfos,rgguidExtendedInfo,rgvar)

#define IDebugProperty_SetValueAsString(This,pszValue,nRadix)	\
    (This)->lpVtbl -> SetValueAsString(This,pszValue,nRadix)

#define IDebugProperty_EnumMembers(This,dwFieldSpec,nRadix,refiid,ppepi)	\
    (This)->lpVtbl -> EnumMembers(This,dwFieldSpec,nRadix,refiid,ppepi)

#define IDebugProperty_GetParent(This,ppDebugProp)	\
    (This)->lpVtbl -> GetParent(This,ppDebugProp)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [local] */ HRESULT STDMETHODCALLTYPE IDebugProperty_GetPropertyInfo_Proxy( 
    IDebugProperty * This,
    /* [in] */ DWORD dwFieldSpec,
    /* [in] */ UINT nRadix,
    /* [out] */ DebugPropertyInfo *pPropertyInfo);


void __RPC_STUB IDebugProperty_GetPropertyInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugProperty_GetExtendedInfo_Proxy( 
    IDebugProperty * This,
    /* [in] */ ULONG cInfos,
    /* [in][size_is] */ GUID *rgguidExtendedInfo,
    /* [out][size_is] */ VARIANT *rgvar);


void __RPC_STUB IDebugProperty_GetExtendedInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugProperty_SetValueAsString_Proxy( 
    IDebugProperty * This,
    /* [in] */ LPCOLESTR pszValue,
    /* [in] */ UINT nRadix);


void __RPC_STUB IDebugProperty_SetValueAsString_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugProperty_EnumMembers_Proxy( 
    IDebugProperty * This,
    /* [in] */ DWORD dwFieldSpec,
    /* [in] */ UINT nRadix,
    /* [in] */ REFIID refiid,
    /* [out] */ IEnumDebugPropertyInfo **ppepi);


void __RPC_STUB IDebugProperty_EnumMembers_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugProperty_GetParent_Proxy( 
    IDebugProperty * This,
    /* [out] */ IDebugProperty **ppDebugProp);


void __RPC_STUB IDebugProperty_GetParent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDebugProperty_INTERFACE_DEFINED__ */


#ifndef __IEnumDebugPropertyInfo_INTERFACE_DEFINED__
#define __IEnumDebugPropertyInfo_INTERFACE_DEFINED__

/* interface IEnumDebugPropertyInfo */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IEnumDebugPropertyInfo;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("51973C51-CB0C-11d0-B5C9-00A0244A0E7A")
    IEnumDebugPropertyInfo : public IUnknown
    {
    public:
        virtual /* [local] */ HRESULT STDMETHODCALLTYPE Next( 
            /* [in] */ ULONG celt,
            /* [out] */ DebugPropertyInfo *pi,
            /* [out] */ ULONG *pcEltsfetched) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Skip( 
            /* [in] */ ULONG celt) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Reset( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Clone( 
            /* [out] */ IEnumDebugPropertyInfo **ppepi) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCount( 
            /* [out] */ ULONG *pcelt) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IEnumDebugPropertyInfoVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IEnumDebugPropertyInfo * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IEnumDebugPropertyInfo * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IEnumDebugPropertyInfo * This);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Next )( 
            IEnumDebugPropertyInfo * This,
            /* [in] */ ULONG celt,
            /* [out] */ DebugPropertyInfo *pi,
            /* [out] */ ULONG *pcEltsfetched);
        
        HRESULT ( STDMETHODCALLTYPE *Skip )( 
            IEnumDebugPropertyInfo * This,
            /* [in] */ ULONG celt);
        
        HRESULT ( STDMETHODCALLTYPE *Reset )( 
            IEnumDebugPropertyInfo * This);
        
        HRESULT ( STDMETHODCALLTYPE *Clone )( 
            IEnumDebugPropertyInfo * This,
            /* [out] */ IEnumDebugPropertyInfo **ppepi);
        
        HRESULT ( STDMETHODCALLTYPE *GetCount )( 
            IEnumDebugPropertyInfo * This,
            /* [out] */ ULONG *pcelt);
        
        END_INTERFACE
    } IEnumDebugPropertyInfoVtbl;

    interface IEnumDebugPropertyInfo
    {
        CONST_VTBL struct IEnumDebugPropertyInfoVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEnumDebugPropertyInfo_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IEnumDebugPropertyInfo_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IEnumDebugPropertyInfo_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IEnumDebugPropertyInfo_Next(This,celt,pi,pcEltsfetched)	\
    (This)->lpVtbl -> Next(This,celt,pi,pcEltsfetched)

#define IEnumDebugPropertyInfo_Skip(This,celt)	\
    (This)->lpVtbl -> Skip(This,celt)

#define IEnumDebugPropertyInfo_Reset(This)	\
    (This)->lpVtbl -> Reset(This)

#define IEnumDebugPropertyInfo_Clone(This,ppepi)	\
    (This)->lpVtbl -> Clone(This,ppepi)

#define IEnumDebugPropertyInfo_GetCount(This,pcelt)	\
    (This)->lpVtbl -> GetCount(This,pcelt)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [local] */ HRESULT STDMETHODCALLTYPE IEnumDebugPropertyInfo_Next_Proxy( 
    IEnumDebugPropertyInfo * This,
    /* [in] */ ULONG celt,
    /* [out] */ DebugPropertyInfo *pi,
    /* [out] */ ULONG *pcEltsfetched);


void __RPC_STUB IEnumDebugPropertyInfo_Next_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEnumDebugPropertyInfo_Skip_Proxy( 
    IEnumDebugPropertyInfo * This,
    /* [in] */ ULONG celt);


void __RPC_STUB IEnumDebugPropertyInfo_Skip_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEnumDebugPropertyInfo_Reset_Proxy( 
    IEnumDebugPropertyInfo * This);


void __RPC_STUB IEnumDebugPropertyInfo_Reset_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEnumDebugPropertyInfo_Clone_Proxy( 
    IEnumDebugPropertyInfo * This,
    /* [out] */ IEnumDebugPropertyInfo **ppepi);


void __RPC_STUB IEnumDebugPropertyInfo_Clone_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEnumDebugPropertyInfo_GetCount_Proxy( 
    IEnumDebugPropertyInfo * This,
    /* [out] */ ULONG *pcelt);


void __RPC_STUB IEnumDebugPropertyInfo_GetCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IEnumDebugPropertyInfo_INTERFACE_DEFINED__ */


#ifndef __IDebugDocumentHelper_INTERFACE_DEFINED__
#define __IDebugDocumentHelper_INTERFACE_DEFINED__

/* interface IDebugDocumentHelper */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IDebugDocumentHelper;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("51973C26-CB0C-11d0-B5C9-00A0244A0E7A")
    IDebugDocumentHelper : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Init( 
            /* [in] */ IDebugApplication *pda,
            /* [string][in] */ LPCOLESTR pszShortName,
            /* [string][in] */ LPCOLESTR pszLongName,
            /* [in] */ TEXT_DOC_ATTR docAttr) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Attach( 
            /* [in] */ IDebugDocumentHelper *pddhParent) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Detach( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AddUnicodeText( 
            /* [string][in] */ LPCOLESTR pszText) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AddDBCSText( 
            /* [string][in] */ LPCSTR pszText) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetDebugDocumentHost( 
            /* [in] */ IDebugDocumentHost *pddh) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AddDeferredText( 
            /* [in] */ ULONG cChars,
            /* [in] */ DWORD dwTextStartCookie) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DefineScriptBlock( 
            /* [in] */ ULONG ulCharOffset,
            /* [in] */ ULONG cChars,
            /* [in] */ IActiveScript *pas,
            /* [in] */ BOOL fScriptlet,
            /* [out] */ DWORD *pdwSourceContext) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetDefaultTextAttr( 
            SOURCE_TEXT_ATTR staTextAttr) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetTextAttributes( 
            /* [in] */ ULONG ulCharOffset,
            /* [in] */ ULONG cChars,
            /* [size_is][length_is][in] */ SOURCE_TEXT_ATTR *pstaTextAttr) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetLongName( 
            /* [string][in] */ LPCOLESTR pszLongName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetShortName( 
            /* [string][in] */ LPCOLESTR pszShortName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetDocumentAttr( 
            /* [in] */ TEXT_DOC_ATTR pszAttributes) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetDebugApplicationNode( 
            /* [out] */ IDebugApplicationNode **ppdan) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetScriptBlockInfo( 
            /* [in] */ DWORD dwSourceContext,
            /* [out] */ IActiveScript **ppasd,
            /* [out] */ ULONG *piCharPos,
            /* [out] */ ULONG *pcChars) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateDebugDocumentContext( 
            /* [in] */ ULONG iCharPos,
            /* [in] */ ULONG cChars,
            /* [out] */ IDebugDocumentContext **ppddc) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE BringDocumentToTop( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE BringDocumentContextToTop( 
            /* [in] */ IDebugDocumentContext *pddc) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDebugDocumentHelperVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDebugDocumentHelper * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDebugDocumentHelper * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDebugDocumentHelper * This);
        
        HRESULT ( STDMETHODCALLTYPE *Init )( 
            IDebugDocumentHelper * This,
            /* [in] */ IDebugApplication *pda,
            /* [string][in] */ LPCOLESTR pszShortName,
            /* [string][in] */ LPCOLESTR pszLongName,
            /* [in] */ TEXT_DOC_ATTR docAttr);
        
        HRESULT ( STDMETHODCALLTYPE *Attach )( 
            IDebugDocumentHelper * This,
            /* [in] */ IDebugDocumentHelper *pddhParent);
        
        HRESULT ( STDMETHODCALLTYPE *Detach )( 
            IDebugDocumentHelper * This);
        
        HRESULT ( STDMETHODCALLTYPE *AddUnicodeText )( 
            IDebugDocumentHelper * This,
            /* [string][in] */ LPCOLESTR pszText);
        
        HRESULT ( STDMETHODCALLTYPE *AddDBCSText )( 
            IDebugDocumentHelper * This,
            /* [string][in] */ LPCSTR pszText);
        
        HRESULT ( STDMETHODCALLTYPE *SetDebugDocumentHost )( 
            IDebugDocumentHelper * This,
            /* [in] */ IDebugDocumentHost *pddh);
        
        HRESULT ( STDMETHODCALLTYPE *AddDeferredText )( 
            IDebugDocumentHelper * This,
            /* [in] */ ULONG cChars,
            /* [in] */ DWORD dwTextStartCookie);
        
        HRESULT ( STDMETHODCALLTYPE *DefineScriptBlock )( 
            IDebugDocumentHelper * This,
            /* [in] */ ULONG ulCharOffset,
            /* [in] */ ULONG cChars,
            /* [in] */ IActiveScript *pas,
            /* [in] */ BOOL fScriptlet,
            /* [out] */ DWORD *pdwSourceContext);
        
        HRESULT ( STDMETHODCALLTYPE *SetDefaultTextAttr )( 
            IDebugDocumentHelper * This,
            SOURCE_TEXT_ATTR staTextAttr);
        
        HRESULT ( STDMETHODCALLTYPE *SetTextAttributes )( 
            IDebugDocumentHelper * This,
            /* [in] */ ULONG ulCharOffset,
            /* [in] */ ULONG cChars,
            /* [size_is][length_is][in] */ SOURCE_TEXT_ATTR *pstaTextAttr);
        
        HRESULT ( STDMETHODCALLTYPE *SetLongName )( 
            IDebugDocumentHelper * This,
            /* [string][in] */ LPCOLESTR pszLongName);
        
        HRESULT ( STDMETHODCALLTYPE *SetShortName )( 
            IDebugDocumentHelper * This,
            /* [string][in] */ LPCOLESTR pszShortName);
        
        HRESULT ( STDMETHODCALLTYPE *SetDocumentAttr )( 
            IDebugDocumentHelper * This,
            /* [in] */ TEXT_DOC_ATTR pszAttributes);
        
        HRESULT ( STDMETHODCALLTYPE *GetDebugApplicationNode )( 
            IDebugDocumentHelper * This,
            /* [out] */ IDebugApplicationNode **ppdan);
        
        HRESULT ( STDMETHODCALLTYPE *GetScriptBlockInfo )( 
            IDebugDocumentHelper * This,
            /* [in] */ DWORD dwSourceContext,
            /* [out] */ IActiveScript **ppasd,
            /* [out] */ ULONG *piCharPos,
            /* [out] */ ULONG *pcChars);
        
        HRESULT ( STDMETHODCALLTYPE *CreateDebugDocumentContext )( 
            IDebugDocumentHelper * This,
            /* [in] */ ULONG iCharPos,
            /* [in] */ ULONG cChars,
            /* [out] */ IDebugDocumentContext **ppddc);
        
        HRESULT ( STDMETHODCALLTYPE *BringDocumentToTop )( 
            IDebugDocumentHelper * This);
        
        HRESULT ( STDMETHODCALLTYPE *BringDocumentContextToTop )( 
            IDebugDocumentHelper * This,
            /* [in] */ IDebugDocumentContext *pddc);
        
        END_INTERFACE
    } IDebugDocumentHelperVtbl;

    interface IDebugDocumentHelper
    {
        CONST_VTBL struct IDebugDocumentHelperVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDebugDocumentHelper_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDebugDocumentHelper_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDebugDocumentHelper_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDebugDocumentHelper_Init(This,pda,pszShortName,pszLongName,docAttr)	\
    (This)->lpVtbl -> Init(This,pda,pszShortName,pszLongName,docAttr)

#define IDebugDocumentHelper_Attach(This,pddhParent)	\
    (This)->lpVtbl -> Attach(This,pddhParent)

#define IDebugDocumentHelper_Detach(This)	\
    (This)->lpVtbl -> Detach(This)

#define IDebugDocumentHelper_AddUnicodeText(This,pszText)	\
    (This)->lpVtbl -> AddUnicodeText(This,pszText)

#define IDebugDocumentHelper_AddDBCSText(This,pszText)	\
    (This)->lpVtbl -> AddDBCSText(This,pszText)

#define IDebugDocumentHelper_SetDebugDocumentHost(This,pddh)	\
    (This)->lpVtbl -> SetDebugDocumentHost(This,pddh)

#define IDebugDocumentHelper_AddDeferredText(This,cChars,dwTextStartCookie)	\
    (This)->lpVtbl -> AddDeferredText(This,cChars,dwTextStartCookie)

#define IDebugDocumentHelper_DefineScriptBlock(This,ulCharOffset,cChars,pas,fScriptlet,pdwSourceContext)	\
    (This)->lpVtbl -> DefineScriptBlock(This,ulCharOffset,cChars,pas,fScriptlet,pdwSourceContext)

#define IDebugDocumentHelper_SetDefaultTextAttr(This,staTextAttr)	\
    (This)->lpVtbl -> SetDefaultTextAttr(This,staTextAttr)

#define IDebugDocumentHelper_SetTextAttributes(This,ulCharOffset,cChars,pstaTextAttr)	\
    (This)->lpVtbl -> SetTextAttributes(This,ulCharOffset,cChars,pstaTextAttr)

#define IDebugDocumentHelper_SetLongName(This,pszLongName)	\
    (This)->lpVtbl -> SetLongName(This,pszLongName)

#define IDebugDocumentHelper_SetShortName(This,pszShortName)	\
    (This)->lpVtbl -> SetShortName(This,pszShortName)

#define IDebugDocumentHelper_SetDocumentAttr(This,pszAttributes)	\
    (This)->lpVtbl -> SetDocumentAttr(This,pszAttributes)

#define IDebugDocumentHelper_GetDebugApplicationNode(This,ppdan)	\
    (This)->lpVtbl -> GetDebugApplicationNode(This,ppdan)

#define IDebugDocumentHelper_GetScriptBlockInfo(This,dwSourceContext,ppasd,piCharPos,pcChars)	\
    (This)->lpVtbl -> GetScriptBlockInfo(This,dwSourceContext,ppasd,piCharPos,pcChars)

#define IDebugDocumentHelper_CreateDebugDocumentContext(This,iCharPos,cChars,ppddc)	\
    (This)->lpVtbl -> CreateDebugDocumentContext(This,iCharPos,cChars,ppddc)

#define IDebugDocumentHelper_BringDocumentToTop(This)	\
    (This)->lpVtbl -> BringDocumentToTop(This)

#define IDebugDocumentHelper_BringDocumentContextToTop(This,pddc)	\
    (This)->lpVtbl -> BringDocumentContextToTop(This,pddc)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IDebugDocumentHelper_Init_Proxy( 
    IDebugDocumentHelper * This,
    /* [in] */ IDebugApplication *pda,
    /* [string][in] */ LPCOLESTR pszShortName,
    /* [string][in] */ LPCOLESTR pszLongName,
    /* [in] */ TEXT_DOC_ATTR docAttr);


void __RPC_STUB IDebugDocumentHelper_Init_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugDocumentHelper_Attach_Proxy( 
    IDebugDocumentHelper * This,
    /* [in] */ IDebugDocumentHelper *pddhParent);


void __RPC_STUB IDebugDocumentHelper_Attach_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugDocumentHelper_Detach_Proxy( 
    IDebugDocumentHelper * This);


void __RPC_STUB IDebugDocumentHelper_Detach_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugDocumentHelper_AddUnicodeText_Proxy( 
    IDebugDocumentHelper * This,
    /* [string][in] */ LPCOLESTR pszText);


void __RPC_STUB IDebugDocumentHelper_AddUnicodeText_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugDocumentHelper_AddDBCSText_Proxy( 
    IDebugDocumentHelper * This,
    /* [string][in] */ LPCSTR pszText);


void __RPC_STUB IDebugDocumentHelper_AddDBCSText_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugDocumentHelper_SetDebugDocumentHost_Proxy( 
    IDebugDocumentHelper * This,
    /* [in] */ IDebugDocumentHost *pddh);


void __RPC_STUB IDebugDocumentHelper_SetDebugDocumentHost_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugDocumentHelper_AddDeferredText_Proxy( 
    IDebugDocumentHelper * This,
    /* [in] */ ULONG cChars,
    /* [in] */ DWORD dwTextStartCookie);


void __RPC_STUB IDebugDocumentHelper_AddDeferredText_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugDocumentHelper_DefineScriptBlock_Proxy( 
    IDebugDocumentHelper * This,
    /* [in] */ ULONG ulCharOffset,
    /* [in] */ ULONG cChars,
    /* [in] */ IActiveScript *pas,
    /* [in] */ BOOL fScriptlet,
    /* [out] */ DWORD *pdwSourceContext);


void __RPC_STUB IDebugDocumentHelper_DefineScriptBlock_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugDocumentHelper_SetDefaultTextAttr_Proxy( 
    IDebugDocumentHelper * This,
    SOURCE_TEXT_ATTR staTextAttr);


void __RPC_STUB IDebugDocumentHelper_SetDefaultTextAttr_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugDocumentHelper_SetTextAttributes_Proxy( 
    IDebugDocumentHelper * This,
    /* [in] */ ULONG ulCharOffset,
    /* [in] */ ULONG cChars,
    /* [size_is][length_is][in] */ SOURCE_TEXT_ATTR *pstaTextAttr);


void __RPC_STUB IDebugDocumentHelper_SetTextAttributes_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugDocumentHelper_SetLongName_Proxy( 
    IDebugDocumentHelper * This,
    /* [string][in] */ LPCOLESTR pszLongName);


void __RPC_STUB IDebugDocumentHelper_SetLongName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugDocumentHelper_SetShortName_Proxy( 
    IDebugDocumentHelper * This,
    /* [string][in] */ LPCOLESTR pszShortName);


void __RPC_STUB IDebugDocumentHelper_SetShortName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugDocumentHelper_SetDocumentAttr_Proxy( 
    IDebugDocumentHelper * This,
    /* [in] */ TEXT_DOC_ATTR pszAttributes);


void __RPC_STUB IDebugDocumentHelper_SetDocumentAttr_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugDocumentHelper_GetDebugApplicationNode_Proxy( 
    IDebugDocumentHelper * This,
    /* [out] */ IDebugApplicationNode **ppdan);


void __RPC_STUB IDebugDocumentHelper_GetDebugApplicationNode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugDocumentHelper_GetScriptBlockInfo_Proxy( 
    IDebugDocumentHelper * This,
    /* [in] */ DWORD dwSourceContext,
    /* [out] */ IActiveScript **ppasd,
    /* [out] */ ULONG *piCharPos,
    /* [out] */ ULONG *pcChars);


void __RPC_STUB IDebugDocumentHelper_GetScriptBlockInfo_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugDocumentHelper_CreateDebugDocumentContext_Proxy( 
    IDebugDocumentHelper * This,
    /* [in] */ ULONG iCharPos,
    /* [in] */ ULONG cChars,
    /* [out] */ IDebugDocumentContext **ppddc);


void __RPC_STUB IDebugDocumentHelper_CreateDebugDocumentContext_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugDocumentHelper_BringDocumentToTop_Proxy( 
    IDebugDocumentHelper * This);


void __RPC_STUB IDebugDocumentHelper_BringDocumentToTop_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugDocumentHelper_BringDocumentContextToTop_Proxy( 
    IDebugDocumentHelper * This,
    /* [in] */ IDebugDocumentContext *pddc);


void __RPC_STUB IDebugDocumentHelper_BringDocumentContextToTop_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDebugDocumentHelper_INTERFACE_DEFINED__ */


#ifndef __IDebugDocumentHost_INTERFACE_DEFINED__
#define __IDebugDocumentHost_INTERFACE_DEFINED__

/* interface IDebugDocumentHost */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IDebugDocumentHost;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("51973C27-CB0C-11d0-B5C9-00A0244A0E7A")
    IDebugDocumentHost : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetDeferredText( 
            /* [in] */ DWORD dwTextStartCookie,
            /* [size_is][length_is][out][in] */ WCHAR *pcharText,
            /* [size_is][length_is][out][in] */ SOURCE_TEXT_ATTR *pstaTextAttr,
            /* [out][in] */ ULONG *pcNumChars,
            /* [in] */ ULONG cMaxChars) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetScriptTextAttributes( 
            /* [size_is][in] */ LPCOLESTR pstrCode,
            /* [in] */ ULONG uNumCodeChars,
            /* [in] */ LPCOLESTR pstrDelimiter,
            /* [in] */ DWORD dwFlags,
            /* [size_is][out][in] */ SOURCE_TEXT_ATTR *pattr) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnCreateDocumentContext( 
            /* [out] */ IUnknown **ppunkOuter) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetPathName( 
            /* [out] */ BSTR *pbstrLongName,
            /* [out] */ BOOL *pfIsOriginalFile) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetFileName( 
            /* [out] */ BSTR *pbstrShortName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE NotifyChanged( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDebugDocumentHostVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDebugDocumentHost * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDebugDocumentHost * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDebugDocumentHost * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetDeferredText )( 
            IDebugDocumentHost * This,
            /* [in] */ DWORD dwTextStartCookie,
            /* [size_is][length_is][out][in] */ WCHAR *pcharText,
            /* [size_is][length_is][out][in] */ SOURCE_TEXT_ATTR *pstaTextAttr,
            /* [out][in] */ ULONG *pcNumChars,
            /* [in] */ ULONG cMaxChars);
        
        HRESULT ( STDMETHODCALLTYPE *GetScriptTextAttributes )( 
            IDebugDocumentHost * This,
            /* [size_is][in] */ LPCOLESTR pstrCode,
            /* [in] */ ULONG uNumCodeChars,
            /* [in] */ LPCOLESTR pstrDelimiter,
            /* [in] */ DWORD dwFlags,
            /* [size_is][out][in] */ SOURCE_TEXT_ATTR *pattr);
        
        HRESULT ( STDMETHODCALLTYPE *OnCreateDocumentContext )( 
            IDebugDocumentHost * This,
            /* [out] */ IUnknown **ppunkOuter);
        
        HRESULT ( STDMETHODCALLTYPE *GetPathName )( 
            IDebugDocumentHost * This,
            /* [out] */ BSTR *pbstrLongName,
            /* [out] */ BOOL *pfIsOriginalFile);
        
        HRESULT ( STDMETHODCALLTYPE *GetFileName )( 
            IDebugDocumentHost * This,
            /* [out] */ BSTR *pbstrShortName);
        
        HRESULT ( STDMETHODCALLTYPE *NotifyChanged )( 
            IDebugDocumentHost * This);
        
        END_INTERFACE
    } IDebugDocumentHostVtbl;

    interface IDebugDocumentHost
    {
        CONST_VTBL struct IDebugDocumentHostVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDebugDocumentHost_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDebugDocumentHost_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDebugDocumentHost_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDebugDocumentHost_GetDeferredText(This,dwTextStartCookie,pcharText,pstaTextAttr,pcNumChars,cMaxChars)	\
    (This)->lpVtbl -> GetDeferredText(This,dwTextStartCookie,pcharText,pstaTextAttr,pcNumChars,cMaxChars)

#define IDebugDocumentHost_GetScriptTextAttributes(This,pstrCode,uNumCodeChars,pstrDelimiter,dwFlags,pattr)	\
    (This)->lpVtbl -> GetScriptTextAttributes(This,pstrCode,uNumCodeChars,pstrDelimiter,dwFlags,pattr)

#define IDebugDocumentHost_OnCreateDocumentContext(This,ppunkOuter)	\
    (This)->lpVtbl -> OnCreateDocumentContext(This,ppunkOuter)

#define IDebugDocumentHost_GetPathName(This,pbstrLongName,pfIsOriginalFile)	\
    (This)->lpVtbl -> GetPathName(This,pbstrLongName,pfIsOriginalFile)

#define IDebugDocumentHost_GetFileName(This,pbstrShortName)	\
    (This)->lpVtbl -> GetFileName(This,pbstrShortName)

#define IDebugDocumentHost_NotifyChanged(This)	\
    (This)->lpVtbl -> NotifyChanged(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IDebugDocumentHost_GetDeferredText_Proxy( 
    IDebugDocumentHost * This,
    /* [in] */ DWORD dwTextStartCookie,
    /* [size_is][length_is][out][in] */ WCHAR *pcharText,
    /* [size_is][length_is][out][in] */ SOURCE_TEXT_ATTR *pstaTextAttr,
    /* [out][in] */ ULONG *pcNumChars,
    /* [in] */ ULONG cMaxChars);


void __RPC_STUB IDebugDocumentHost_GetDeferredText_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugDocumentHost_GetScriptTextAttributes_Proxy( 
    IDebugDocumentHost * This,
    /* [size_is][in] */ LPCOLESTR pstrCode,
    /* [in] */ ULONG uNumCodeChars,
    /* [in] */ LPCOLESTR pstrDelimiter,
    /* [in] */ DWORD dwFlags,
    /* [size_is][out][in] */ SOURCE_TEXT_ATTR *pattr);


void __RPC_STUB IDebugDocumentHost_GetScriptTextAttributes_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugDocumentHost_OnCreateDocumentContext_Proxy( 
    IDebugDocumentHost * This,
    /* [out] */ IUnknown **ppunkOuter);


void __RPC_STUB IDebugDocumentHost_OnCreateDocumentContext_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugDocumentHost_GetPathName_Proxy( 
    IDebugDocumentHost * This,
    /* [out] */ BSTR *pbstrLongName,
    /* [out] */ BOOL *pfIsOriginalFile);


void __RPC_STUB IDebugDocumentHost_GetPathName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugDocumentHost_GetFileName_Proxy( 
    IDebugDocumentHost * This,
    /* [out] */ BSTR *pbstrShortName);


void __RPC_STUB IDebugDocumentHost_GetFileName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugDocumentHost_NotifyChanged_Proxy( 
    IDebugDocumentHost * This);


void __RPC_STUB IDebugDocumentHost_NotifyChanged_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDebugDocumentHost_INTERFACE_DEFINED__ */


#ifndef __IDebugDocumentInfo_INTERFACE_DEFINED__
#define __IDebugDocumentInfo_INTERFACE_DEFINED__

/* interface IDebugDocumentInfo */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IDebugDocumentInfo;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("51973C1f-CB0C-11d0-B5C9-00A0244A0E7A")
    IDebugDocumentInfo : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetName( 
            /* [in] */ DOCUMENTNAMETYPE dnt,
            /* [out] */ BSTR *pbstrName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetDocumentClassId( 
            /* [out] */ CLSID *pclsidDocument) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDebugDocumentInfoVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDebugDocumentInfo * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDebugDocumentInfo * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDebugDocumentInfo * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetName )( 
            IDebugDocumentInfo * This,
            /* [in] */ DOCUMENTNAMETYPE dnt,
            /* [out] */ BSTR *pbstrName);
        
        HRESULT ( STDMETHODCALLTYPE *GetDocumentClassId )( 
            IDebugDocumentInfo * This,
            /* [out] */ CLSID *pclsidDocument);
        
        END_INTERFACE
    } IDebugDocumentInfoVtbl;

    interface IDebugDocumentInfo
    {
        CONST_VTBL struct IDebugDocumentInfoVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDebugDocumentInfo_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDebugDocumentInfo_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDebugDocumentInfo_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDebugDocumentInfo_GetName(This,dnt,pbstrName)	\
    (This)->lpVtbl -> GetName(This,dnt,pbstrName)

#define IDebugDocumentInfo_GetDocumentClassId(This,pclsidDocument)	\
    (This)->lpVtbl -> GetDocumentClassId(This,pclsidDocument)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IDebugDocumentInfo_GetName_Proxy( 
    IDebugDocumentInfo * This,
    /* [in] */ DOCUMENTNAMETYPE dnt,
    /* [out] */ BSTR *pbstrName);


void __RPC_STUB IDebugDocumentInfo_GetName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugDocumentInfo_GetDocumentClassId_Proxy( 
    IDebugDocumentInfo * This,
    /* [out] */ CLSID *pclsidDocument);


void __RPC_STUB IDebugDocumentInfo_GetDocumentClassId_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDebugDocumentInfo_INTERFACE_DEFINED__ */


#ifndef __IDebugDocumentProvider_INTERFACE_DEFINED__
#define __IDebugDocumentProvider_INTERFACE_DEFINED__

/* interface IDebugDocumentProvider */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IDebugDocumentProvider;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("51973C20-CB0C-11d0-B5C9-00A0244A0E7A")
    IDebugDocumentProvider : public IDebugDocumentInfo
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetDocument( 
            /* [out] */ IDebugDocument **ppssd) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDebugDocumentProviderVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDebugDocumentProvider * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDebugDocumentProvider * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDebugDocumentProvider * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetName )( 
            IDebugDocumentProvider * This,
            /* [in] */ DOCUMENTNAMETYPE dnt,
            /* [out] */ BSTR *pbstrName);
        
        HRESULT ( STDMETHODCALLTYPE *GetDocumentClassId )( 
            IDebugDocumentProvider * This,
            /* [out] */ CLSID *pclsidDocument);
        
        HRESULT ( STDMETHODCALLTYPE *GetDocument )( 
            IDebugDocumentProvider * This,
            /* [out] */ IDebugDocument **ppssd);
        
        END_INTERFACE
    } IDebugDocumentProviderVtbl;

    interface IDebugDocumentProvider
    {
        CONST_VTBL struct IDebugDocumentProviderVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDebugDocumentProvider_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDebugDocumentProvider_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDebugDocumentProvider_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDebugDocumentProvider_GetName(This,dnt,pbstrName)	\
    (This)->lpVtbl -> GetName(This,dnt,pbstrName)

#define IDebugDocumentProvider_GetDocumentClassId(This,pclsidDocument)	\
    (This)->lpVtbl -> GetDocumentClassId(This,pclsidDocument)


#define IDebugDocumentProvider_GetDocument(This,ppssd)	\
    (This)->lpVtbl -> GetDocument(This,ppssd)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IDebugDocumentProvider_GetDocument_Proxy( 
    IDebugDocumentProvider * This,
    /* [out] */ IDebugDocument **ppssd);


void __RPC_STUB IDebugDocumentProvider_GetDocument_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDebugDocumentProvider_INTERFACE_DEFINED__ */


#ifndef __IDebugDocument_INTERFACE_DEFINED__
#define __IDebugDocument_INTERFACE_DEFINED__

/* interface IDebugDocument */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IDebugDocument;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("51973C21-CB0C-11d0-B5C9-00A0244A0E7A")
    IDebugDocument : public IDebugDocumentInfo
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IDebugDocumentVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDebugDocument * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDebugDocument * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDebugDocument * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetName )( 
            IDebugDocument * This,
            /* [in] */ DOCUMENTNAMETYPE dnt,
            /* [out] */ BSTR *pbstrName);
        
        HRESULT ( STDMETHODCALLTYPE *GetDocumentClassId )( 
            IDebugDocument * This,
            /* [out] */ CLSID *pclsidDocument);
        
        END_INTERFACE
    } IDebugDocumentVtbl;

    interface IDebugDocument
    {
        CONST_VTBL struct IDebugDocumentVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDebugDocument_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDebugDocument_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDebugDocument_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDebugDocument_GetName(This,dnt,pbstrName)	\
    (This)->lpVtbl -> GetName(This,dnt,pbstrName)

#define IDebugDocument_GetDocumentClassId(This,pclsidDocument)	\
    (This)->lpVtbl -> GetDocumentClassId(This,pclsidDocument)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDebugDocument_INTERFACE_DEFINED__ */


#ifndef __IDebugDocumentText_INTERFACE_DEFINED__
#define __IDebugDocumentText_INTERFACE_DEFINED__

/* interface IDebugDocumentText */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IDebugDocumentText;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("51973C22-CB0C-11d0-B5C9-00A0244A0E7A")
    IDebugDocumentText : public IDebugDocument
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetDocumentAttributes( 
            /* [out] */ TEXT_DOC_ATTR *ptextdocattr) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetSize( 
            /* [out] */ ULONG *pcNumLines,
            /* [out] */ ULONG *pcNumChars) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetPositionOfLine( 
            /* [in] */ ULONG cLineNumber,
            /* [out] */ ULONG *pcCharacterPosition) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetLineOfPosition( 
            /* [in] */ ULONG cCharacterPosition,
            /* [out] */ ULONG *pcLineNumber,
            /* [out] */ ULONG *pcCharacterOffsetInLine) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetText( 
            /* [in] */ ULONG cCharacterPosition,
            /* [size_is][length_is][out][in] */ WCHAR *pcharText,
            /* [full][size_is][length_is][out][in] */ SOURCE_TEXT_ATTR *pstaTextAttr,
            /* [out][in] */ ULONG *pcNumChars,
            /* [in] */ ULONG cMaxChars) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetPositionOfContext( 
            /* [in] */ IDebugDocumentContext *psc,
            /* [out] */ ULONG *pcCharacterPosition,
            /* [out] */ ULONG *cNumChars) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetContextOfPosition( 
            /* [in] */ ULONG cCharacterPosition,
            /* [in] */ ULONG cNumChars,
            /* [out] */ IDebugDocumentContext **ppsc) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDebugDocumentTextVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDebugDocumentText * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDebugDocumentText * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDebugDocumentText * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetName )( 
            IDebugDocumentText * This,
            /* [in] */ DOCUMENTNAMETYPE dnt,
            /* [out] */ BSTR *pbstrName);
        
        HRESULT ( STDMETHODCALLTYPE *GetDocumentClassId )( 
            IDebugDocumentText * This,
            /* [out] */ CLSID *pclsidDocument);
        
        HRESULT ( STDMETHODCALLTYPE *GetDocumentAttributes )( 
            IDebugDocumentText * This,
            /* [out] */ TEXT_DOC_ATTR *ptextdocattr);
        
        HRESULT ( STDMETHODCALLTYPE *GetSize )( 
            IDebugDocumentText * This,
            /* [out] */ ULONG *pcNumLines,
            /* [out] */ ULONG *pcNumChars);
        
        HRESULT ( STDMETHODCALLTYPE *GetPositionOfLine )( 
            IDebugDocumentText * This,
            /* [in] */ ULONG cLineNumber,
            /* [out] */ ULONG *pcCharacterPosition);
        
        HRESULT ( STDMETHODCALLTYPE *GetLineOfPosition )( 
            IDebugDocumentText * This,
            /* [in] */ ULONG cCharacterPosition,
            /* [out] */ ULONG *pcLineNumber,
            /* [out] */ ULONG *pcCharacterOffsetInLine);
        
        HRESULT ( STDMETHODCALLTYPE *GetText )( 
            IDebugDocumentText * This,
            /* [in] */ ULONG cCharacterPosition,
            /* [size_is][length_is][out][in] */ WCHAR *pcharText,
            /* [full][size_is][length_is][out][in] */ SOURCE_TEXT_ATTR *pstaTextAttr,
            /* [out][in] */ ULONG *pcNumChars,
            /* [in] */ ULONG cMaxChars);
        
        HRESULT ( STDMETHODCALLTYPE *GetPositionOfContext )( 
            IDebugDocumentText * This,
            /* [in] */ IDebugDocumentContext *psc,
            /* [out] */ ULONG *pcCharacterPosition,
            /* [out] */ ULONG *cNumChars);
        
        HRESULT ( STDMETHODCALLTYPE *GetContextOfPosition )( 
            IDebugDocumentText * This,
            /* [in] */ ULONG cCharacterPosition,
            /* [in] */ ULONG cNumChars,
            /* [out] */ IDebugDocumentContext **ppsc);
        
        END_INTERFACE
    } IDebugDocumentTextVtbl;

    interface IDebugDocumentText
    {
        CONST_VTBL struct IDebugDocumentTextVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDebugDocumentText_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDebugDocumentText_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDebugDocumentText_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDebugDocumentText_GetName(This,dnt,pbstrName)	\
    (This)->lpVtbl -> GetName(This,dnt,pbstrName)

#define IDebugDocumentText_GetDocumentClassId(This,pclsidDocument)	\
    (This)->lpVtbl -> GetDocumentClassId(This,pclsidDocument)



#define IDebugDocumentText_GetDocumentAttributes(This,ptextdocattr)	\
    (This)->lpVtbl -> GetDocumentAttributes(This,ptextdocattr)

#define IDebugDocumentText_GetSize(This,pcNumLines,pcNumChars)	\
    (This)->lpVtbl -> GetSize(This,pcNumLines,pcNumChars)

#define IDebugDocumentText_GetPositionOfLine(This,cLineNumber,pcCharacterPosition)	\
    (This)->lpVtbl -> GetPositionOfLine(This,cLineNumber,pcCharacterPosition)

#define IDebugDocumentText_GetLineOfPosition(This,cCharacterPosition,pcLineNumber,pcCharacterOffsetInLine)	\
    (This)->lpVtbl -> GetLineOfPosition(This,cCharacterPosition,pcLineNumber,pcCharacterOffsetInLine)

#define IDebugDocumentText_GetText(This,cCharacterPosition,pcharText,pstaTextAttr,pcNumChars,cMaxChars)	\
    (This)->lpVtbl -> GetText(This,cCharacterPosition,pcharText,pstaTextAttr,pcNumChars,cMaxChars)

#define IDebugDocumentText_GetPositionOfContext(This,psc,pcCharacterPosition,cNumChars)	\
    (This)->lpVtbl -> GetPositionOfContext(This,psc,pcCharacterPosition,cNumChars)

#define IDebugDocumentText_GetContextOfPosition(This,cCharacterPosition,cNumChars,ppsc)	\
    (This)->lpVtbl -> GetContextOfPosition(This,cCharacterPosition,cNumChars,ppsc)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IDebugDocumentText_GetDocumentAttributes_Proxy( 
    IDebugDocumentText * This,
    /* [out] */ TEXT_DOC_ATTR *ptextdocattr);


void __RPC_STUB IDebugDocumentText_GetDocumentAttributes_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugDocumentText_GetSize_Proxy( 
    IDebugDocumentText * This,
    /* [out] */ ULONG *pcNumLines,
    /* [out] */ ULONG *pcNumChars);


void __RPC_STUB IDebugDocumentText_GetSize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugDocumentText_GetPositionOfLine_Proxy( 
    IDebugDocumentText * This,
    /* [in] */ ULONG cLineNumber,
    /* [out] */ ULONG *pcCharacterPosition);


void __RPC_STUB IDebugDocumentText_GetPositionOfLine_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugDocumentText_GetLineOfPosition_Proxy( 
    IDebugDocumentText * This,
    /* [in] */ ULONG cCharacterPosition,
    /* [out] */ ULONG *pcLineNumber,
    /* [out] */ ULONG *pcCharacterOffsetInLine);


void __RPC_STUB IDebugDocumentText_GetLineOfPosition_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugDocumentText_GetText_Proxy( 
    IDebugDocumentText * This,
    /* [in] */ ULONG cCharacterPosition,
    /* [size_is][length_is][out][in] */ WCHAR *pcharText,
    /* [full][size_is][length_is][out][in] */ SOURCE_TEXT_ATTR *pstaTextAttr,
    /* [out][in] */ ULONG *pcNumChars,
    /* [in] */ ULONG cMaxChars);


void __RPC_STUB IDebugDocumentText_GetText_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugDocumentText_GetPositionOfContext_Proxy( 
    IDebugDocumentText * This,
    /* [in] */ IDebugDocumentContext *psc,
    /* [out] */ ULONG *pcCharacterPosition,
    /* [out] */ ULONG *cNumChars);


void __RPC_STUB IDebugDocumentText_GetPositionOfContext_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugDocumentText_GetContextOfPosition_Proxy( 
    IDebugDocumentText * This,
    /* [in] */ ULONG cCharacterPosition,
    /* [in] */ ULONG cNumChars,
    /* [out] */ IDebugDocumentContext **ppsc);


void __RPC_STUB IDebugDocumentText_GetContextOfPosition_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDebugDocumentText_INTERFACE_DEFINED__ */


#ifndef __IDebugDocumentTextEvents_INTERFACE_DEFINED__
#define __IDebugDocumentTextEvents_INTERFACE_DEFINED__

/* interface IDebugDocumentTextEvents */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IDebugDocumentTextEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("51973C23-CB0C-11d0-B5C9-00A0244A0E7A")
    IDebugDocumentTextEvents : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE onDestroy( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE onInsertText( 
            /* [in] */ ULONG cCharacterPosition,
            /* [in] */ ULONG cNumToInsert) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE onRemoveText( 
            /* [in] */ ULONG cCharacterPosition,
            /* [in] */ ULONG cNumToRemove) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE onReplaceText( 
            /* [in] */ ULONG cCharacterPosition,
            /* [in] */ ULONG cNumToReplace) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE onUpdateTextAttributes( 
            /* [in] */ ULONG cCharacterPosition,
            /* [in] */ ULONG cNumToUpdate) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE onUpdateDocumentAttributes( 
            /* [in] */ TEXT_DOC_ATTR textdocattr) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDebugDocumentTextEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDebugDocumentTextEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDebugDocumentTextEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDebugDocumentTextEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *onDestroy )( 
            IDebugDocumentTextEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *onInsertText )( 
            IDebugDocumentTextEvents * This,
            /* [in] */ ULONG cCharacterPosition,
            /* [in] */ ULONG cNumToInsert);
        
        HRESULT ( STDMETHODCALLTYPE *onRemoveText )( 
            IDebugDocumentTextEvents * This,
            /* [in] */ ULONG cCharacterPosition,
            /* [in] */ ULONG cNumToRemove);
        
        HRESULT ( STDMETHODCALLTYPE *onReplaceText )( 
            IDebugDocumentTextEvents * This,
            /* [in] */ ULONG cCharacterPosition,
            /* [in] */ ULONG cNumToReplace);
        
        HRESULT ( STDMETHODCALLTYPE *onUpdateTextAttributes )( 
            IDebugDocumentTextEvents * This,
            /* [in] */ ULONG cCharacterPosition,
            /* [in] */ ULONG cNumToUpdate);
        
        HRESULT ( STDMETHODCALLTYPE *onUpdateDocumentAttributes )( 
            IDebugDocumentTextEvents * This,
            /* [in] */ TEXT_DOC_ATTR textdocattr);
        
        END_INTERFACE
    } IDebugDocumentTextEventsVtbl;

    interface IDebugDocumentTextEvents
    {
        CONST_VTBL struct IDebugDocumentTextEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDebugDocumentTextEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDebugDocumentTextEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDebugDocumentTextEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDebugDocumentTextEvents_onDestroy(This)	\
    (This)->lpVtbl -> onDestroy(This)

#define IDebugDocumentTextEvents_onInsertText(This,cCharacterPosition,cNumToInsert)	\
    (This)->lpVtbl -> onInsertText(This,cCharacterPosition,cNumToInsert)

#define IDebugDocumentTextEvents_onRemoveText(This,cCharacterPosition,cNumToRemove)	\
    (This)->lpVtbl -> onRemoveText(This,cCharacterPosition,cNumToRemove)

#define IDebugDocumentTextEvents_onReplaceText(This,cCharacterPosition,cNumToReplace)	\
    (This)->lpVtbl -> onReplaceText(This,cCharacterPosition,cNumToReplace)

#define IDebugDocumentTextEvents_onUpdateTextAttributes(This,cCharacterPosition,cNumToUpdate)	\
    (This)->lpVtbl -> onUpdateTextAttributes(This,cCharacterPosition,cNumToUpdate)

#define IDebugDocumentTextEvents_onUpdateDocumentAttributes(This,textdocattr)	\
    (This)->lpVtbl -> onUpdateDocumentAttributes(This,textdocattr)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IDebugDocumentTextEvents_onDestroy_Proxy( 
    IDebugDocumentTextEvents * This);


void __RPC_STUB IDebugDocumentTextEvents_onDestroy_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugDocumentTextEvents_onInsertText_Proxy( 
    IDebugDocumentTextEvents * This,
    /* [in] */ ULONG cCharacterPosition,
    /* [in] */ ULONG cNumToInsert);


void __RPC_STUB IDebugDocumentTextEvents_onInsertText_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugDocumentTextEvents_onRemoveText_Proxy( 
    IDebugDocumentTextEvents * This,
    /* [in] */ ULONG cCharacterPosition,
    /* [in] */ ULONG cNumToRemove);


void __RPC_STUB IDebugDocumentTextEvents_onRemoveText_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugDocumentTextEvents_onReplaceText_Proxy( 
    IDebugDocumentTextEvents * This,
    /* [in] */ ULONG cCharacterPosition,
    /* [in] */ ULONG cNumToReplace);


void __RPC_STUB IDebugDocumentTextEvents_onReplaceText_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugDocumentTextEvents_onUpdateTextAttributes_Proxy( 
    IDebugDocumentTextEvents * This,
    /* [in] */ ULONG cCharacterPosition,
    /* [in] */ ULONG cNumToUpdate);


void __RPC_STUB IDebugDocumentTextEvents_onUpdateTextAttributes_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugDocumentTextEvents_onUpdateDocumentAttributes_Proxy( 
    IDebugDocumentTextEvents * This,
    /* [in] */ TEXT_DOC_ATTR textdocattr);


void __RPC_STUB IDebugDocumentTextEvents_onUpdateDocumentAttributes_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDebugDocumentTextEvents_INTERFACE_DEFINED__ */


#ifndef __IDebugDocumentTextAuthor_INTERFACE_DEFINED__
#define __IDebugDocumentTextAuthor_INTERFACE_DEFINED__

/* interface IDebugDocumentTextAuthor */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IDebugDocumentTextAuthor;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("51973C24-CB0C-11d0-B5C9-00A0244A0E7A")
    IDebugDocumentTextAuthor : public IDebugDocumentText
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE InsertText( 
            /* [in] */ ULONG cCharacterPosition,
            /* [in] */ ULONG cNumToInsert,
            /* [size_is][in] */ OLECHAR pcharText[  ]) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RemoveText( 
            /* [in] */ ULONG cCharacterPosition,
            /* [in] */ ULONG cNumToRemove) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ReplaceText( 
            /* [in] */ ULONG cCharacterPosition,
            /* [in] */ ULONG cNumToReplace,
            /* [size_is][in] */ OLECHAR pcharText[  ]) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDebugDocumentTextAuthorVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDebugDocumentTextAuthor * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDebugDocumentTextAuthor * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDebugDocumentTextAuthor * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetName )( 
            IDebugDocumentTextAuthor * This,
            /* [in] */ DOCUMENTNAMETYPE dnt,
            /* [out] */ BSTR *pbstrName);
        
        HRESULT ( STDMETHODCALLTYPE *GetDocumentClassId )( 
            IDebugDocumentTextAuthor * This,
            /* [out] */ CLSID *pclsidDocument);
        
        HRESULT ( STDMETHODCALLTYPE *GetDocumentAttributes )( 
            IDebugDocumentTextAuthor * This,
            /* [out] */ TEXT_DOC_ATTR *ptextdocattr);
        
        HRESULT ( STDMETHODCALLTYPE *GetSize )( 
            IDebugDocumentTextAuthor * This,
            /* [out] */ ULONG *pcNumLines,
            /* [out] */ ULONG *pcNumChars);
        
        HRESULT ( STDMETHODCALLTYPE *GetPositionOfLine )( 
            IDebugDocumentTextAuthor * This,
            /* [in] */ ULONG cLineNumber,
            /* [out] */ ULONG *pcCharacterPosition);
        
        HRESULT ( STDMETHODCALLTYPE *GetLineOfPosition )( 
            IDebugDocumentTextAuthor * This,
            /* [in] */ ULONG cCharacterPosition,
            /* [out] */ ULONG *pcLineNumber,
            /* [out] */ ULONG *pcCharacterOffsetInLine);
        
        HRESULT ( STDMETHODCALLTYPE *GetText )( 
            IDebugDocumentTextAuthor * This,
            /* [in] */ ULONG cCharacterPosition,
            /* [size_is][length_is][out][in] */ WCHAR *pcharText,
            /* [full][size_is][length_is][out][in] */ SOURCE_TEXT_ATTR *pstaTextAttr,
            /* [out][in] */ ULONG *pcNumChars,
            /* [in] */ ULONG cMaxChars);
        
        HRESULT ( STDMETHODCALLTYPE *GetPositionOfContext )( 
            IDebugDocumentTextAuthor * This,
            /* [in] */ IDebugDocumentContext *psc,
            /* [out] */ ULONG *pcCharacterPosition,
            /* [out] */ ULONG *cNumChars);
        
        HRESULT ( STDMETHODCALLTYPE *GetContextOfPosition )( 
            IDebugDocumentTextAuthor * This,
            /* [in] */ ULONG cCharacterPosition,
            /* [in] */ ULONG cNumChars,
            /* [out] */ IDebugDocumentContext **ppsc);
        
        HRESULT ( STDMETHODCALLTYPE *InsertText )( 
            IDebugDocumentTextAuthor * This,
            /* [in] */ ULONG cCharacterPosition,
            /* [in] */ ULONG cNumToInsert,
            /* [size_is][in] */ OLECHAR pcharText[  ]);
        
        HRESULT ( STDMETHODCALLTYPE *RemoveText )( 
            IDebugDocumentTextAuthor * This,
            /* [in] */ ULONG cCharacterPosition,
            /* [in] */ ULONG cNumToRemove);
        
        HRESULT ( STDMETHODCALLTYPE *ReplaceText )( 
            IDebugDocumentTextAuthor * This,
            /* [in] */ ULONG cCharacterPosition,
            /* [in] */ ULONG cNumToReplace,
            /* [size_is][in] */ OLECHAR pcharText[  ]);
        
        END_INTERFACE
    } IDebugDocumentTextAuthorVtbl;

    interface IDebugDocumentTextAuthor
    {
        CONST_VTBL struct IDebugDocumentTextAuthorVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDebugDocumentTextAuthor_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDebugDocumentTextAuthor_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDebugDocumentTextAuthor_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDebugDocumentTextAuthor_GetName(This,dnt,pbstrName)	\
    (This)->lpVtbl -> GetName(This,dnt,pbstrName)

#define IDebugDocumentTextAuthor_GetDocumentClassId(This,pclsidDocument)	\
    (This)->lpVtbl -> GetDocumentClassId(This,pclsidDocument)



#define IDebugDocumentTextAuthor_GetDocumentAttributes(This,ptextdocattr)	\
    (This)->lpVtbl -> GetDocumentAttributes(This,ptextdocattr)

#define IDebugDocumentTextAuthor_GetSize(This,pcNumLines,pcNumChars)	\
    (This)->lpVtbl -> GetSize(This,pcNumLines,pcNumChars)

#define IDebugDocumentTextAuthor_GetPositionOfLine(This,cLineNumber,pcCharacterPosition)	\
    (This)->lpVtbl -> GetPositionOfLine(This,cLineNumber,pcCharacterPosition)

#define IDebugDocumentTextAuthor_GetLineOfPosition(This,cCharacterPosition,pcLineNumber,pcCharacterOffsetInLine)	\
    (This)->lpVtbl -> GetLineOfPosition(This,cCharacterPosition,pcLineNumber,pcCharacterOffsetInLine)

#define IDebugDocumentTextAuthor_GetText(This,cCharacterPosition,pcharText,pstaTextAttr,pcNumChars,cMaxChars)	\
    (This)->lpVtbl -> GetText(This,cCharacterPosition,pcharText,pstaTextAttr,pcNumChars,cMaxChars)

#define IDebugDocumentTextAuthor_GetPositionOfContext(This,psc,pcCharacterPosition,cNumChars)	\
    (This)->lpVtbl -> GetPositionOfContext(This,psc,pcCharacterPosition,cNumChars)

#define IDebugDocumentTextAuthor_GetContextOfPosition(This,cCharacterPosition,cNumChars,ppsc)	\
    (This)->lpVtbl -> GetContextOfPosition(This,cCharacterPosition,cNumChars,ppsc)


#define IDebugDocumentTextAuthor_InsertText(This,cCharacterPosition,cNumToInsert,pcharText)	\
    (This)->lpVtbl -> InsertText(This,cCharacterPosition,cNumToInsert,pcharText)

#define IDebugDocumentTextAuthor_RemoveText(This,cCharacterPosition,cNumToRemove)	\
    (This)->lpVtbl -> RemoveText(This,cCharacterPosition,cNumToRemove)

#define IDebugDocumentTextAuthor_ReplaceText(This,cCharacterPosition,cNumToReplace,pcharText)	\
    (This)->lpVtbl -> ReplaceText(This,cCharacterPosition,cNumToReplace,pcharText)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IDebugDocumentTextAuthor_InsertText_Proxy( 
    IDebugDocumentTextAuthor * This,
    /* [in] */ ULONG cCharacterPosition,
    /* [in] */ ULONG cNumToInsert,
    /* [size_is][in] */ OLECHAR pcharText[  ]);


void __RPC_STUB IDebugDocumentTextAuthor_InsertText_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugDocumentTextAuthor_RemoveText_Proxy( 
    IDebugDocumentTextAuthor * This,
    /* [in] */ ULONG cCharacterPosition,
    /* [in] */ ULONG cNumToRemove);


void __RPC_STUB IDebugDocumentTextAuthor_RemoveText_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugDocumentTextAuthor_ReplaceText_Proxy( 
    IDebugDocumentTextAuthor * This,
    /* [in] */ ULONG cCharacterPosition,
    /* [in] */ ULONG cNumToReplace,
    /* [size_is][in] */ OLECHAR pcharText[  ]);


void __RPC_STUB IDebugDocumentTextAuthor_ReplaceText_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDebugDocumentTextAuthor_INTERFACE_DEFINED__ */


#ifndef __IDebugDocumentContext_INTERFACE_DEFINED__
#define __IDebugDocumentContext_INTERFACE_DEFINED__

/* interface IDebugDocumentContext */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IDebugDocumentContext;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("51973C28-CB0C-11d0-B5C9-00A0244A0E7A")
    IDebugDocumentContext : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetDocument( 
            /* [out] */ IDebugDocument **ppsd) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE EnumCodeContexts( 
            /* [out] */ IEnumDebugCodeContexts **ppescc) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDebugDocumentContextVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDebugDocumentContext * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDebugDocumentContext * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDebugDocumentContext * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetDocument )( 
            IDebugDocumentContext * This,
            /* [out] */ IDebugDocument **ppsd);
        
        HRESULT ( STDMETHODCALLTYPE *EnumCodeContexts )( 
            IDebugDocumentContext * This,
            /* [out] */ IEnumDebugCodeContexts **ppescc);
        
        END_INTERFACE
    } IDebugDocumentContextVtbl;

    interface IDebugDocumentContext
    {
        CONST_VTBL struct IDebugDocumentContextVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDebugDocumentContext_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDebugDocumentContext_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDebugDocumentContext_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDebugDocumentContext_GetDocument(This,ppsd)	\
    (This)->lpVtbl -> GetDocument(This,ppsd)

#define IDebugDocumentContext_EnumCodeContexts(This,ppescc)	\
    (This)->lpVtbl -> EnumCodeContexts(This,ppescc)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IDebugDocumentContext_GetDocument_Proxy( 
    IDebugDocumentContext * This,
    /* [out] */ IDebugDocument **ppsd);


void __RPC_STUB IDebugDocumentContext_GetDocument_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugDocumentContext_EnumCodeContexts_Proxy( 
    IDebugDocumentContext * This,
    /* [out] */ IEnumDebugCodeContexts **ppescc);


void __RPC_STUB IDebugDocumentContext_EnumCodeContexts_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDebugDocumentContext_INTERFACE_DEFINED__ */


#ifndef __IDebugApplicationNode_INTERFACE_DEFINED__
#define __IDebugApplicationNode_INTERFACE_DEFINED__

/* interface IDebugApplicationNode */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IDebugApplicationNode;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("51973C34-CB0C-11d0-B5C9-00A0244A0E7A")
    IDebugApplicationNode : public IDebugDocumentProvider
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE EnumChildren( 
            /* [out] */ IEnumDebugApplicationNodes **pperddp) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetParent( 
            /* [out] */ IDebugApplicationNode **pprddp) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetDocumentProvider( 
            /* [in] */ IDebugDocumentProvider *pddp) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Close( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Attach( 
            /* [in] */ IDebugApplicationNode *pdanParent) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Detach( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDebugApplicationNodeVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDebugApplicationNode * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDebugApplicationNode * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDebugApplicationNode * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetName )( 
            IDebugApplicationNode * This,
            /* [in] */ DOCUMENTNAMETYPE dnt,
            /* [out] */ BSTR *pbstrName);
        
        HRESULT ( STDMETHODCALLTYPE *GetDocumentClassId )( 
            IDebugApplicationNode * This,
            /* [out] */ CLSID *pclsidDocument);
        
        HRESULT ( STDMETHODCALLTYPE *GetDocument )( 
            IDebugApplicationNode * This,
            /* [out] */ IDebugDocument **ppssd);
        
        HRESULT ( STDMETHODCALLTYPE *EnumChildren )( 
            IDebugApplicationNode * This,
            /* [out] */ IEnumDebugApplicationNodes **pperddp);
        
        HRESULT ( STDMETHODCALLTYPE *GetParent )( 
            IDebugApplicationNode * This,
            /* [out] */ IDebugApplicationNode **pprddp);
        
        HRESULT ( STDMETHODCALLTYPE *SetDocumentProvider )( 
            IDebugApplicationNode * This,
            /* [in] */ IDebugDocumentProvider *pddp);
        
        HRESULT ( STDMETHODCALLTYPE *Close )( 
            IDebugApplicationNode * This);
        
        HRESULT ( STDMETHODCALLTYPE *Attach )( 
            IDebugApplicationNode * This,
            /* [in] */ IDebugApplicationNode *pdanParent);
        
        HRESULT ( STDMETHODCALLTYPE *Detach )( 
            IDebugApplicationNode * This);
        
        END_INTERFACE
    } IDebugApplicationNodeVtbl;

    interface IDebugApplicationNode
    {
        CONST_VTBL struct IDebugApplicationNodeVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDebugApplicationNode_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDebugApplicationNode_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDebugApplicationNode_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDebugApplicationNode_GetName(This,dnt,pbstrName)	\
    (This)->lpVtbl -> GetName(This,dnt,pbstrName)

#define IDebugApplicationNode_GetDocumentClassId(This,pclsidDocument)	\
    (This)->lpVtbl -> GetDocumentClassId(This,pclsidDocument)


#define IDebugApplicationNode_GetDocument(This,ppssd)	\
    (This)->lpVtbl -> GetDocument(This,ppssd)


#define IDebugApplicationNode_EnumChildren(This,pperddp)	\
    (This)->lpVtbl -> EnumChildren(This,pperddp)

#define IDebugApplicationNode_GetParent(This,pprddp)	\
    (This)->lpVtbl -> GetParent(This,pprddp)

#define IDebugApplicationNode_SetDocumentProvider(This,pddp)	\
    (This)->lpVtbl -> SetDocumentProvider(This,pddp)

#define IDebugApplicationNode_Close(This)	\
    (This)->lpVtbl -> Close(This)

#define IDebugApplicationNode_Attach(This,pdanParent)	\
    (This)->lpVtbl -> Attach(This,pdanParent)

#define IDebugApplicationNode_Detach(This)	\
    (This)->lpVtbl -> Detach(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IDebugApplicationNode_EnumChildren_Proxy( 
    IDebugApplicationNode * This,
    /* [out] */ IEnumDebugApplicationNodes **pperddp);


void __RPC_STUB IDebugApplicationNode_EnumChildren_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugApplicationNode_GetParent_Proxy( 
    IDebugApplicationNode * This,
    /* [out] */ IDebugApplicationNode **pprddp);


void __RPC_STUB IDebugApplicationNode_GetParent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugApplicationNode_SetDocumentProvider_Proxy( 
    IDebugApplicationNode * This,
    /* [in] */ IDebugDocumentProvider *pddp);


void __RPC_STUB IDebugApplicationNode_SetDocumentProvider_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugApplicationNode_Close_Proxy( 
    IDebugApplicationNode * This);


void __RPC_STUB IDebugApplicationNode_Close_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugApplicationNode_Attach_Proxy( 
    IDebugApplicationNode * This,
    /* [in] */ IDebugApplicationNode *pdanParent);


void __RPC_STUB IDebugApplicationNode_Attach_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugApplicationNode_Detach_Proxy( 
    IDebugApplicationNode * This);


void __RPC_STUB IDebugApplicationNode_Detach_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDebugApplicationNode_INTERFACE_DEFINED__ */


#ifndef __IDebugApplicationNodeEvents_INTERFACE_DEFINED__
#define __IDebugApplicationNodeEvents_INTERFACE_DEFINED__

/* interface IDebugApplicationNodeEvents */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IDebugApplicationNodeEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("51973C35-CB0C-11d0-B5C9-00A0244A0E7A")
    IDebugApplicationNodeEvents : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE onAddChild( 
            /* [in] */ IDebugApplicationNode *prddpChild) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE onRemoveChild( 
            /* [in] */ IDebugApplicationNode *prddpChild) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE onDetach( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE onAttach( 
            /* [in] */ IDebugApplicationNode *prddpParent) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDebugApplicationNodeEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDebugApplicationNodeEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDebugApplicationNodeEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDebugApplicationNodeEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *onAddChild )( 
            IDebugApplicationNodeEvents * This,
            /* [in] */ IDebugApplicationNode *prddpChild);
        
        HRESULT ( STDMETHODCALLTYPE *onRemoveChild )( 
            IDebugApplicationNodeEvents * This,
            /* [in] */ IDebugApplicationNode *prddpChild);
        
        HRESULT ( STDMETHODCALLTYPE *onDetach )( 
            IDebugApplicationNodeEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *onAttach )( 
            IDebugApplicationNodeEvents * This,
            /* [in] */ IDebugApplicationNode *prddpParent);
        
        END_INTERFACE
    } IDebugApplicationNodeEventsVtbl;

    interface IDebugApplicationNodeEvents
    {
        CONST_VTBL struct IDebugApplicationNodeEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDebugApplicationNodeEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDebugApplicationNodeEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDebugApplicationNodeEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDebugApplicationNodeEvents_onAddChild(This,prddpChild)	\
    (This)->lpVtbl -> onAddChild(This,prddpChild)

#define IDebugApplicationNodeEvents_onRemoveChild(This,prddpChild)	\
    (This)->lpVtbl -> onRemoveChild(This,prddpChild)

#define IDebugApplicationNodeEvents_onDetach(This)	\
    (This)->lpVtbl -> onDetach(This)

#define IDebugApplicationNodeEvents_onAttach(This,prddpParent)	\
    (This)->lpVtbl -> onAttach(This,prddpParent)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IDebugApplicationNodeEvents_onAddChild_Proxy( 
    IDebugApplicationNodeEvents * This,
    /* [in] */ IDebugApplicationNode *prddpChild);


void __RPC_STUB IDebugApplicationNodeEvents_onAddChild_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugApplicationNodeEvents_onRemoveChild_Proxy( 
    IDebugApplicationNodeEvents * This,
    /* [in] */ IDebugApplicationNode *prddpChild);


void __RPC_STUB IDebugApplicationNodeEvents_onRemoveChild_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugApplicationNodeEvents_onDetach_Proxy( 
    IDebugApplicationNodeEvents * This);


void __RPC_STUB IDebugApplicationNodeEvents_onDetach_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugApplicationNodeEvents_onAttach_Proxy( 
    IDebugApplicationNodeEvents * This,
    /* [in] */ IDebugApplicationNode *prddpParent);


void __RPC_STUB IDebugApplicationNodeEvents_onAttach_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDebugApplicationNodeEvents_INTERFACE_DEFINED__ */


#ifndef __IDebugSessionProvider_INTERFACE_DEFINED__
#define __IDebugSessionProvider_INTERFACE_DEFINED__

/* interface IDebugSessionProvider */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IDebugSessionProvider;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("51973C29-CB0C-11d0-B5C9-00A0244A0E7A")
    IDebugSessionProvider : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE StartDebugSession( 
            /* [in] */ IRemoteDebugApplication *pda) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDebugSessionProviderVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDebugSessionProvider * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDebugSessionProvider * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDebugSessionProvider * This);
        
        HRESULT ( STDMETHODCALLTYPE *StartDebugSession )( 
            IDebugSessionProvider * This,
            /* [in] */ IRemoteDebugApplication *pda);
        
        END_INTERFACE
    } IDebugSessionProviderVtbl;

    interface IDebugSessionProvider
    {
        CONST_VTBL struct IDebugSessionProviderVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDebugSessionProvider_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDebugSessionProvider_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDebugSessionProvider_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDebugSessionProvider_StartDebugSession(This,pda)	\
    (This)->lpVtbl -> StartDebugSession(This,pda)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IDebugSessionProvider_StartDebugSession_Proxy( 
    IDebugSessionProvider * This,
    /* [in] */ IRemoteDebugApplication *pda);


void __RPC_STUB IDebugSessionProvider_StartDebugSession_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDebugSessionProvider_INTERFACE_DEFINED__ */


#ifndef __IApplicationDebugger_INTERFACE_DEFINED__
#define __IApplicationDebugger_INTERFACE_DEFINED__

/* interface IApplicationDebugger */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IApplicationDebugger;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("51973C2a-CB0C-11d0-B5C9-00A0244A0E7A")
    IApplicationDebugger : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE QueryAlive( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateInstanceAtDebugger( 
            /* [in] */ REFCLSID rclsid,
            /* [in] */ IUnknown *pUnkOuter,
            /* [in] */ DWORD dwClsContext,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ IUnknown **ppvObject) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE onDebugOutput( 
            /* [in] */ LPCOLESTR pstr) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE onHandleBreakPoint( 
            /* [in] */ IRemoteDebugApplicationThread *prpt,
            /* [in] */ BREAKREASON br,
            /* [in] */ IActiveScriptErrorDebug *pError) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE onClose( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE onDebuggerEvent( 
            /* [in] */ REFIID riid,
            /* [in] */ IUnknown *punk) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IApplicationDebuggerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IApplicationDebugger * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IApplicationDebugger * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IApplicationDebugger * This);
        
        HRESULT ( STDMETHODCALLTYPE *QueryAlive )( 
            IApplicationDebugger * This);
        
        HRESULT ( STDMETHODCALLTYPE *CreateInstanceAtDebugger )( 
            IApplicationDebugger * This,
            /* [in] */ REFCLSID rclsid,
            /* [in] */ IUnknown *pUnkOuter,
            /* [in] */ DWORD dwClsContext,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ IUnknown **ppvObject);
        
        HRESULT ( STDMETHODCALLTYPE *onDebugOutput )( 
            IApplicationDebugger * This,
            /* [in] */ LPCOLESTR pstr);
        
        HRESULT ( STDMETHODCALLTYPE *onHandleBreakPoint )( 
            IApplicationDebugger * This,
            /* [in] */ IRemoteDebugApplicationThread *prpt,
            /* [in] */ BREAKREASON br,
            /* [in] */ IActiveScriptErrorDebug *pError);
        
        HRESULT ( STDMETHODCALLTYPE *onClose )( 
            IApplicationDebugger * This);
        
        HRESULT ( STDMETHODCALLTYPE *onDebuggerEvent )( 
            IApplicationDebugger * This,
            /* [in] */ REFIID riid,
            /* [in] */ IUnknown *punk);
        
        END_INTERFACE
    } IApplicationDebuggerVtbl;

    interface IApplicationDebugger
    {
        CONST_VTBL struct IApplicationDebuggerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IApplicationDebugger_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IApplicationDebugger_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IApplicationDebugger_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IApplicationDebugger_QueryAlive(This)	\
    (This)->lpVtbl -> QueryAlive(This)

#define IApplicationDebugger_CreateInstanceAtDebugger(This,rclsid,pUnkOuter,dwClsContext,riid,ppvObject)	\
    (This)->lpVtbl -> CreateInstanceAtDebugger(This,rclsid,pUnkOuter,dwClsContext,riid,ppvObject)

#define IApplicationDebugger_onDebugOutput(This,pstr)	\
    (This)->lpVtbl -> onDebugOutput(This,pstr)

#define IApplicationDebugger_onHandleBreakPoint(This,prpt,br,pError)	\
    (This)->lpVtbl -> onHandleBreakPoint(This,prpt,br,pError)

#define IApplicationDebugger_onClose(This)	\
    (This)->lpVtbl -> onClose(This)

#define IApplicationDebugger_onDebuggerEvent(This,riid,punk)	\
    (This)->lpVtbl -> onDebuggerEvent(This,riid,punk)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IApplicationDebugger_QueryAlive_Proxy( 
    IApplicationDebugger * This);


void __RPC_STUB IApplicationDebugger_QueryAlive_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IApplicationDebugger_CreateInstanceAtDebugger_Proxy( 
    IApplicationDebugger * This,
    /* [in] */ REFCLSID rclsid,
    /* [in] */ IUnknown *pUnkOuter,
    /* [in] */ DWORD dwClsContext,
    /* [in] */ REFIID riid,
    /* [iid_is][out] */ IUnknown **ppvObject);


void __RPC_STUB IApplicationDebugger_CreateInstanceAtDebugger_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IApplicationDebugger_onDebugOutput_Proxy( 
    IApplicationDebugger * This,
    /* [in] */ LPCOLESTR pstr);


void __RPC_STUB IApplicationDebugger_onDebugOutput_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IApplicationDebugger_onHandleBreakPoint_Proxy( 
    IApplicationDebugger * This,
    /* [in] */ IRemoteDebugApplicationThread *prpt,
    /* [in] */ BREAKREASON br,
    /* [in] */ IActiveScriptErrorDebug *pError);


void __RPC_STUB IApplicationDebugger_onHandleBreakPoint_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IApplicationDebugger_onClose_Proxy( 
    IApplicationDebugger * This);


void __RPC_STUB IApplicationDebugger_onClose_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IApplicationDebugger_onDebuggerEvent_Proxy( 
    IApplicationDebugger * This,
    /* [in] */ REFIID riid,
    /* [in] */ IUnknown *punk);


void __RPC_STUB IApplicationDebugger_onDebuggerEvent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IApplicationDebugger_INTERFACE_DEFINED__ */


#ifndef __IMachineDebugManager_INTERFACE_DEFINED__
#define __IMachineDebugManager_INTERFACE_DEFINED__

/* interface IMachineDebugManager */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IMachineDebugManager;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("51973C2c-CB0C-11d0-B5C9-00A0244A0E7A")
    IMachineDebugManager : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE AddApplication( 
            /* [in] */ IRemoteDebugApplication *pda,
            /* [out] */ DWORD *pdwAppCookie) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RemoveApplication( 
            /* [in] */ DWORD dwAppCookie) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE EnumApplications( 
            /* [out] */ IEnumRemoteDebugApplications **ppeda) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMachineDebugManagerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMachineDebugManager * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMachineDebugManager * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMachineDebugManager * This);
        
        HRESULT ( STDMETHODCALLTYPE *AddApplication )( 
            IMachineDebugManager * This,
            /* [in] */ IRemoteDebugApplication *pda,
            /* [out] */ DWORD *pdwAppCookie);
        
        HRESULT ( STDMETHODCALLTYPE *RemoveApplication )( 
            IMachineDebugManager * This,
            /* [in] */ DWORD dwAppCookie);
        
        HRESULT ( STDMETHODCALLTYPE *EnumApplications )( 
            IMachineDebugManager * This,
            /* [out] */ IEnumRemoteDebugApplications **ppeda);
        
        END_INTERFACE
    } IMachineDebugManagerVtbl;

    interface IMachineDebugManager
    {
        CONST_VTBL struct IMachineDebugManagerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMachineDebugManager_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMachineDebugManager_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IMachineDebugManager_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IMachineDebugManager_AddApplication(This,pda,pdwAppCookie)	\
    (This)->lpVtbl -> AddApplication(This,pda,pdwAppCookie)

#define IMachineDebugManager_RemoveApplication(This,dwAppCookie)	\
    (This)->lpVtbl -> RemoveApplication(This,dwAppCookie)

#define IMachineDebugManager_EnumApplications(This,ppeda)	\
    (This)->lpVtbl -> EnumApplications(This,ppeda)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IMachineDebugManager_AddApplication_Proxy( 
    IMachineDebugManager * This,
    /* [in] */ IRemoteDebugApplication *pda,
    /* [out] */ DWORD *pdwAppCookie);


void __RPC_STUB IMachineDebugManager_AddApplication_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMachineDebugManager_RemoveApplication_Proxy( 
    IMachineDebugManager * This,
    /* [in] */ DWORD dwAppCookie);


void __RPC_STUB IMachineDebugManager_RemoveApplication_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMachineDebugManager_EnumApplications_Proxy( 
    IMachineDebugManager * This,
    /* [out] */ IEnumRemoteDebugApplications **ppeda);


void __RPC_STUB IMachineDebugManager_EnumApplications_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMachineDebugManager_INTERFACE_DEFINED__ */


#ifndef __IMachineDebugManagerCookie_INTERFACE_DEFINED__
#define __IMachineDebugManagerCookie_INTERFACE_DEFINED__

/* interface IMachineDebugManagerCookie */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IMachineDebugManagerCookie;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("51973C2d-CB0C-11d0-B5C9-00A0244A0E7A")
    IMachineDebugManagerCookie : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE AddApplication( 
            /* [in] */ IRemoteDebugApplication *pda,
            /* [in] */ DWORD dwDebugAppCookie,
            /* [out] */ DWORD *pdwAppCookie) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RemoveApplication( 
            /* [in] */ DWORD dwDebugAppCookie,
            /* [in] */ DWORD dwAppCookie) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE EnumApplications( 
            /* [out] */ IEnumRemoteDebugApplications **ppeda) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMachineDebugManagerCookieVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMachineDebugManagerCookie * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMachineDebugManagerCookie * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMachineDebugManagerCookie * This);
        
        HRESULT ( STDMETHODCALLTYPE *AddApplication )( 
            IMachineDebugManagerCookie * This,
            /* [in] */ IRemoteDebugApplication *pda,
            /* [in] */ DWORD dwDebugAppCookie,
            /* [out] */ DWORD *pdwAppCookie);
        
        HRESULT ( STDMETHODCALLTYPE *RemoveApplication )( 
            IMachineDebugManagerCookie * This,
            /* [in] */ DWORD dwDebugAppCookie,
            /* [in] */ DWORD dwAppCookie);
        
        HRESULT ( STDMETHODCALLTYPE *EnumApplications )( 
            IMachineDebugManagerCookie * This,
            /* [out] */ IEnumRemoteDebugApplications **ppeda);
        
        END_INTERFACE
    } IMachineDebugManagerCookieVtbl;

    interface IMachineDebugManagerCookie
    {
        CONST_VTBL struct IMachineDebugManagerCookieVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMachineDebugManagerCookie_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMachineDebugManagerCookie_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IMachineDebugManagerCookie_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IMachineDebugManagerCookie_AddApplication(This,pda,dwDebugAppCookie,pdwAppCookie)	\
    (This)->lpVtbl -> AddApplication(This,pda,dwDebugAppCookie,pdwAppCookie)

#define IMachineDebugManagerCookie_RemoveApplication(This,dwDebugAppCookie,dwAppCookie)	\
    (This)->lpVtbl -> RemoveApplication(This,dwDebugAppCookie,dwAppCookie)

#define IMachineDebugManagerCookie_EnumApplications(This,ppeda)	\
    (This)->lpVtbl -> EnumApplications(This,ppeda)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IMachineDebugManagerCookie_AddApplication_Proxy( 
    IMachineDebugManagerCookie * This,
    /* [in] */ IRemoteDebugApplication *pda,
    /* [in] */ DWORD dwDebugAppCookie,
    /* [out] */ DWORD *pdwAppCookie);


void __RPC_STUB IMachineDebugManagerCookie_AddApplication_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMachineDebugManagerCookie_RemoveApplication_Proxy( 
    IMachineDebugManagerCookie * This,
    /* [in] */ DWORD dwDebugAppCookie,
    /* [in] */ DWORD dwAppCookie);


void __RPC_STUB IMachineDebugManagerCookie_RemoveApplication_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMachineDebugManagerCookie_EnumApplications_Proxy( 
    IMachineDebugManagerCookie * This,
    /* [out] */ IEnumRemoteDebugApplications **ppeda);


void __RPC_STUB IMachineDebugManagerCookie_EnumApplications_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMachineDebugManagerCookie_INTERFACE_DEFINED__ */


#ifndef __IMachineDebugManagerEvents_INTERFACE_DEFINED__
#define __IMachineDebugManagerEvents_INTERFACE_DEFINED__

/* interface IMachineDebugManagerEvents */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IMachineDebugManagerEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("51973C2e-CB0C-11d0-B5C9-00A0244A0E7A")
    IMachineDebugManagerEvents : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE onAddApplication( 
            /* [in] */ IRemoteDebugApplication *pda,
            /* [in] */ DWORD dwAppCookie) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE onRemoveApplication( 
            /* [in] */ IRemoteDebugApplication *pda,
            /* [in] */ DWORD dwAppCookie) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMachineDebugManagerEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMachineDebugManagerEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMachineDebugManagerEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMachineDebugManagerEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *onAddApplication )( 
            IMachineDebugManagerEvents * This,
            /* [in] */ IRemoteDebugApplication *pda,
            /* [in] */ DWORD dwAppCookie);
        
        HRESULT ( STDMETHODCALLTYPE *onRemoveApplication )( 
            IMachineDebugManagerEvents * This,
            /* [in] */ IRemoteDebugApplication *pda,
            /* [in] */ DWORD dwAppCookie);
        
        END_INTERFACE
    } IMachineDebugManagerEventsVtbl;

    interface IMachineDebugManagerEvents
    {
        CONST_VTBL struct IMachineDebugManagerEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMachineDebugManagerEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMachineDebugManagerEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IMachineDebugManagerEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IMachineDebugManagerEvents_onAddApplication(This,pda,dwAppCookie)	\
    (This)->lpVtbl -> onAddApplication(This,pda,dwAppCookie)

#define IMachineDebugManagerEvents_onRemoveApplication(This,pda,dwAppCookie)	\
    (This)->lpVtbl -> onRemoveApplication(This,pda,dwAppCookie)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IMachineDebugManagerEvents_onAddApplication_Proxy( 
    IMachineDebugManagerEvents * This,
    /* [in] */ IRemoteDebugApplication *pda,
    /* [in] */ DWORD dwAppCookie);


void __RPC_STUB IMachineDebugManagerEvents_onAddApplication_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IMachineDebugManagerEvents_onRemoveApplication_Proxy( 
    IMachineDebugManagerEvents * This,
    /* [in] */ IRemoteDebugApplication *pda,
    /* [in] */ DWORD dwAppCookie);


void __RPC_STUB IMachineDebugManagerEvents_onRemoveApplication_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMachineDebugManagerEvents_INTERFACE_DEFINED__ */


#ifndef __IEnumRemoteDebugApplications_INTERFACE_DEFINED__
#define __IEnumRemoteDebugApplications_INTERFACE_DEFINED__

/* interface IEnumRemoteDebugApplications */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IEnumRemoteDebugApplications;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("51973C3b-CB0C-11d0-B5C9-00A0244A0E7A")
    IEnumRemoteDebugApplications : public IUnknown
    {
    public:
        virtual /* [local] */ HRESULT __stdcall Next( 
            /* [in] */ ULONG celt,
            /* [out] */ IRemoteDebugApplication **ppda,
            /* [out] */ ULONG *pceltFetched) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Skip( 
            /* [in] */ ULONG celt) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Reset( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Clone( 
            /* [out] */ IEnumRemoteDebugApplications **ppessd) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IEnumRemoteDebugApplicationsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IEnumRemoteDebugApplications * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IEnumRemoteDebugApplications * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IEnumRemoteDebugApplications * This);
        
        /* [local] */ HRESULT ( __stdcall *Next )( 
            IEnumRemoteDebugApplications * This,
            /* [in] */ ULONG celt,
            /* [out] */ IRemoteDebugApplication **ppda,
            /* [out] */ ULONG *pceltFetched);
        
        HRESULT ( STDMETHODCALLTYPE *Skip )( 
            IEnumRemoteDebugApplications * This,
            /* [in] */ ULONG celt);
        
        HRESULT ( STDMETHODCALLTYPE *Reset )( 
            IEnumRemoteDebugApplications * This);
        
        HRESULT ( STDMETHODCALLTYPE *Clone )( 
            IEnumRemoteDebugApplications * This,
            /* [out] */ IEnumRemoteDebugApplications **ppessd);
        
        END_INTERFACE
    } IEnumRemoteDebugApplicationsVtbl;

    interface IEnumRemoteDebugApplications
    {
        CONST_VTBL struct IEnumRemoteDebugApplicationsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEnumRemoteDebugApplications_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IEnumRemoteDebugApplications_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IEnumRemoteDebugApplications_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IEnumRemoteDebugApplications_Next(This,celt,ppda,pceltFetched)	\
    (This)->lpVtbl -> Next(This,celt,ppda,pceltFetched)

#define IEnumRemoteDebugApplications_Skip(This,celt)	\
    (This)->lpVtbl -> Skip(This,celt)

#define IEnumRemoteDebugApplications_Reset(This)	\
    (This)->lpVtbl -> Reset(This)

#define IEnumRemoteDebugApplications_Clone(This,ppessd)	\
    (This)->lpVtbl -> Clone(This,ppessd)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [call_as] */ HRESULT __stdcall IEnumRemoteDebugApplications_RemoteNext_Proxy( 
    IEnumRemoteDebugApplications * This,
    /* [in] */ ULONG celt,
    /* [length_is][size_is][out] */ IRemoteDebugApplication **ppda,
    /* [out] */ ULONG *pceltFetched);


void __RPC_STUB IEnumRemoteDebugApplications_RemoteNext_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEnumRemoteDebugApplications_Skip_Proxy( 
    IEnumRemoteDebugApplications * This,
    /* [in] */ ULONG celt);


void __RPC_STUB IEnumRemoteDebugApplications_Skip_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEnumRemoteDebugApplications_Reset_Proxy( 
    IEnumRemoteDebugApplications * This);


void __RPC_STUB IEnumRemoteDebugApplications_Reset_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEnumRemoteDebugApplications_Clone_Proxy( 
    IEnumRemoteDebugApplications * This,
    /* [out] */ IEnumRemoteDebugApplications **ppessd);


void __RPC_STUB IEnumRemoteDebugApplications_Clone_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IEnumRemoteDebugApplications_INTERFACE_DEFINED__ */


#ifndef __IProcessDebugManager_INTERFACE_DEFINED__
#define __IProcessDebugManager_INTERFACE_DEFINED__

/* interface IProcessDebugManager */
/* [local][unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IProcessDebugManager;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("51973C2f-CB0C-11d0-B5C9-00A0244A0E7A")
    IProcessDebugManager : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE CreateApplication( 
            /* [out] */ IDebugApplication **ppda) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetDefaultApplication( 
            /* [out] */ IDebugApplication **ppda) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AddApplication( 
            /* [in] */ IDebugApplication *pda,
            /* [out] */ DWORD *pdwAppCookie) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RemoveApplication( 
            /* [in] */ DWORD dwAppCookie) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateDebugDocumentHelper( 
            /* [in] */ IUnknown *punkOuter,
            /* [out] */ IDebugDocumentHelper **pddh) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IProcessDebugManagerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IProcessDebugManager * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IProcessDebugManager * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IProcessDebugManager * This);
        
        HRESULT ( STDMETHODCALLTYPE *CreateApplication )( 
            IProcessDebugManager * This,
            /* [out] */ IDebugApplication **ppda);
        
        HRESULT ( STDMETHODCALLTYPE *GetDefaultApplication )( 
            IProcessDebugManager * This,
            /* [out] */ IDebugApplication **ppda);
        
        HRESULT ( STDMETHODCALLTYPE *AddApplication )( 
            IProcessDebugManager * This,
            /* [in] */ IDebugApplication *pda,
            /* [out] */ DWORD *pdwAppCookie);
        
        HRESULT ( STDMETHODCALLTYPE *RemoveApplication )( 
            IProcessDebugManager * This,
            /* [in] */ DWORD dwAppCookie);
        
        HRESULT ( STDMETHODCALLTYPE *CreateDebugDocumentHelper )( 
            IProcessDebugManager * This,
            /* [in] */ IUnknown *punkOuter,
            /* [out] */ IDebugDocumentHelper **pddh);
        
        END_INTERFACE
    } IProcessDebugManagerVtbl;

    interface IProcessDebugManager
    {
        CONST_VTBL struct IProcessDebugManagerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IProcessDebugManager_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IProcessDebugManager_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IProcessDebugManager_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IProcessDebugManager_CreateApplication(This,ppda)	\
    (This)->lpVtbl -> CreateApplication(This,ppda)

#define IProcessDebugManager_GetDefaultApplication(This,ppda)	\
    (This)->lpVtbl -> GetDefaultApplication(This,ppda)

#define IProcessDebugManager_AddApplication(This,pda,pdwAppCookie)	\
    (This)->lpVtbl -> AddApplication(This,pda,pdwAppCookie)

#define IProcessDebugManager_RemoveApplication(This,dwAppCookie)	\
    (This)->lpVtbl -> RemoveApplication(This,dwAppCookie)

#define IProcessDebugManager_CreateDebugDocumentHelper(This,punkOuter,pddh)	\
    (This)->lpVtbl -> CreateDebugDocumentHelper(This,punkOuter,pddh)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IProcessDebugManager_CreateApplication_Proxy( 
    IProcessDebugManager * This,
    /* [out] */ IDebugApplication **ppda);


void __RPC_STUB IProcessDebugManager_CreateApplication_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IProcessDebugManager_GetDefaultApplication_Proxy( 
    IProcessDebugManager * This,
    /* [out] */ IDebugApplication **ppda);


void __RPC_STUB IProcessDebugManager_GetDefaultApplication_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IProcessDebugManager_AddApplication_Proxy( 
    IProcessDebugManager * This,
    /* [in] */ IDebugApplication *pda,
    /* [out] */ DWORD *pdwAppCookie);


void __RPC_STUB IProcessDebugManager_AddApplication_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IProcessDebugManager_RemoveApplication_Proxy( 
    IProcessDebugManager * This,
    /* [in] */ DWORD dwAppCookie);


void __RPC_STUB IProcessDebugManager_RemoveApplication_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IProcessDebugManager_CreateDebugDocumentHelper_Proxy( 
    IProcessDebugManager * This,
    /* [in] */ IUnknown *punkOuter,
    /* [out] */ IDebugDocumentHelper **pddh);


void __RPC_STUB IProcessDebugManager_CreateDebugDocumentHelper_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IProcessDebugManager_INTERFACE_DEFINED__ */


#ifndef __IRemoteDebugApplication_INTERFACE_DEFINED__
#define __IRemoteDebugApplication_INTERFACE_DEFINED__

/* interface IRemoteDebugApplication */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IRemoteDebugApplication;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("51973C30-CB0C-11d0-B5C9-00A0244A0E7A")
    IRemoteDebugApplication : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE ResumeFromBreakPoint( 
            /* [in] */ IRemoteDebugApplicationThread *prptFocus,
            /* [in] */ BREAKRESUMEACTION bra,
            /* [in] */ ERRORRESUMEACTION era) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CauseBreak( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ConnectDebugger( 
            /* [in] */ IApplicationDebugger *pad) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DisconnectDebugger( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetDebugger( 
            /* [out] */ IApplicationDebugger **pad) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateInstanceAtApplication( 
            /* [in] */ REFCLSID rclsid,
            /* [in] */ IUnknown *pUnkOuter,
            /* [in] */ DWORD dwClsContext,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ IUnknown **ppvObject) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE QueryAlive( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE EnumThreads( 
            /* [out] */ IEnumRemoteDebugApplicationThreads **pperdat) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetName( 
            /* [out] */ BSTR *pbstrName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetRootNode( 
            /* [out] */ IDebugApplicationNode **ppdanRoot) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE EnumGlobalExpressionContexts( 
            /* [out] */ IEnumDebugExpressionContexts **ppedec) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IRemoteDebugApplicationVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IRemoteDebugApplication * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IRemoteDebugApplication * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IRemoteDebugApplication * This);
        
        HRESULT ( STDMETHODCALLTYPE *ResumeFromBreakPoint )( 
            IRemoteDebugApplication * This,
            /* [in] */ IRemoteDebugApplicationThread *prptFocus,
            /* [in] */ BREAKRESUMEACTION bra,
            /* [in] */ ERRORRESUMEACTION era);
        
        HRESULT ( STDMETHODCALLTYPE *CauseBreak )( 
            IRemoteDebugApplication * This);
        
        HRESULT ( STDMETHODCALLTYPE *ConnectDebugger )( 
            IRemoteDebugApplication * This,
            /* [in] */ IApplicationDebugger *pad);
        
        HRESULT ( STDMETHODCALLTYPE *DisconnectDebugger )( 
            IRemoteDebugApplication * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetDebugger )( 
            IRemoteDebugApplication * This,
            /* [out] */ IApplicationDebugger **pad);
        
        HRESULT ( STDMETHODCALLTYPE *CreateInstanceAtApplication )( 
            IRemoteDebugApplication * This,
            /* [in] */ REFCLSID rclsid,
            /* [in] */ IUnknown *pUnkOuter,
            /* [in] */ DWORD dwClsContext,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ IUnknown **ppvObject);
        
        HRESULT ( STDMETHODCALLTYPE *QueryAlive )( 
            IRemoteDebugApplication * This);
        
        HRESULT ( STDMETHODCALLTYPE *EnumThreads )( 
            IRemoteDebugApplication * This,
            /* [out] */ IEnumRemoteDebugApplicationThreads **pperdat);
        
        HRESULT ( STDMETHODCALLTYPE *GetName )( 
            IRemoteDebugApplication * This,
            /* [out] */ BSTR *pbstrName);
        
        HRESULT ( STDMETHODCALLTYPE *GetRootNode )( 
            IRemoteDebugApplication * This,
            /* [out] */ IDebugApplicationNode **ppdanRoot);
        
        HRESULT ( STDMETHODCALLTYPE *EnumGlobalExpressionContexts )( 
            IRemoteDebugApplication * This,
            /* [out] */ IEnumDebugExpressionContexts **ppedec);
        
        END_INTERFACE
    } IRemoteDebugApplicationVtbl;

    interface IRemoteDebugApplication
    {
        CONST_VTBL struct IRemoteDebugApplicationVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IRemoteDebugApplication_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IRemoteDebugApplication_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IRemoteDebugApplication_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IRemoteDebugApplication_ResumeFromBreakPoint(This,prptFocus,bra,era)	\
    (This)->lpVtbl -> ResumeFromBreakPoint(This,prptFocus,bra,era)

#define IRemoteDebugApplication_CauseBreak(This)	\
    (This)->lpVtbl -> CauseBreak(This)

#define IRemoteDebugApplication_ConnectDebugger(This,pad)	\
    (This)->lpVtbl -> ConnectDebugger(This,pad)

#define IRemoteDebugApplication_DisconnectDebugger(This)	\
    (This)->lpVtbl -> DisconnectDebugger(This)

#define IRemoteDebugApplication_GetDebugger(This,pad)	\
    (This)->lpVtbl -> GetDebugger(This,pad)

#define IRemoteDebugApplication_CreateInstanceAtApplication(This,rclsid,pUnkOuter,dwClsContext,riid,ppvObject)	\
    (This)->lpVtbl -> CreateInstanceAtApplication(This,rclsid,pUnkOuter,dwClsContext,riid,ppvObject)

#define IRemoteDebugApplication_QueryAlive(This)	\
    (This)->lpVtbl -> QueryAlive(This)

#define IRemoteDebugApplication_EnumThreads(This,pperdat)	\
    (This)->lpVtbl -> EnumThreads(This,pperdat)

#define IRemoteDebugApplication_GetName(This,pbstrName)	\
    (This)->lpVtbl -> GetName(This,pbstrName)

#define IRemoteDebugApplication_GetRootNode(This,ppdanRoot)	\
    (This)->lpVtbl -> GetRootNode(This,ppdanRoot)

#define IRemoteDebugApplication_EnumGlobalExpressionContexts(This,ppedec)	\
    (This)->lpVtbl -> EnumGlobalExpressionContexts(This,ppedec)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IRemoteDebugApplication_ResumeFromBreakPoint_Proxy( 
    IRemoteDebugApplication * This,
    /* [in] */ IRemoteDebugApplicationThread *prptFocus,
    /* [in] */ BREAKRESUMEACTION bra,
    /* [in] */ ERRORRESUMEACTION era);


void __RPC_STUB IRemoteDebugApplication_ResumeFromBreakPoint_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IRemoteDebugApplication_CauseBreak_Proxy( 
    IRemoteDebugApplication * This);


void __RPC_STUB IRemoteDebugApplication_CauseBreak_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IRemoteDebugApplication_ConnectDebugger_Proxy( 
    IRemoteDebugApplication * This,
    /* [in] */ IApplicationDebugger *pad);


void __RPC_STUB IRemoteDebugApplication_ConnectDebugger_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IRemoteDebugApplication_DisconnectDebugger_Proxy( 
    IRemoteDebugApplication * This);


void __RPC_STUB IRemoteDebugApplication_DisconnectDebugger_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IRemoteDebugApplication_GetDebugger_Proxy( 
    IRemoteDebugApplication * This,
    /* [out] */ IApplicationDebugger **pad);


void __RPC_STUB IRemoteDebugApplication_GetDebugger_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IRemoteDebugApplication_CreateInstanceAtApplication_Proxy( 
    IRemoteDebugApplication * This,
    /* [in] */ REFCLSID rclsid,
    /* [in] */ IUnknown *pUnkOuter,
    /* [in] */ DWORD dwClsContext,
    /* [in] */ REFIID riid,
    /* [iid_is][out] */ IUnknown **ppvObject);


void __RPC_STUB IRemoteDebugApplication_CreateInstanceAtApplication_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IRemoteDebugApplication_QueryAlive_Proxy( 
    IRemoteDebugApplication * This);


void __RPC_STUB IRemoteDebugApplication_QueryAlive_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IRemoteDebugApplication_EnumThreads_Proxy( 
    IRemoteDebugApplication * This,
    /* [out] */ IEnumRemoteDebugApplicationThreads **pperdat);


void __RPC_STUB IRemoteDebugApplication_EnumThreads_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IRemoteDebugApplication_GetName_Proxy( 
    IRemoteDebugApplication * This,
    /* [out] */ BSTR *pbstrName);


void __RPC_STUB IRemoteDebugApplication_GetName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IRemoteDebugApplication_GetRootNode_Proxy( 
    IRemoteDebugApplication * This,
    /* [out] */ IDebugApplicationNode **ppdanRoot);


void __RPC_STUB IRemoteDebugApplication_GetRootNode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IRemoteDebugApplication_EnumGlobalExpressionContexts_Proxy( 
    IRemoteDebugApplication * This,
    /* [out] */ IEnumDebugExpressionContexts **ppedec);


void __RPC_STUB IRemoteDebugApplication_EnumGlobalExpressionContexts_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IRemoteDebugApplication_INTERFACE_DEFINED__ */


#ifndef __IDebugApplication_INTERFACE_DEFINED__
#define __IDebugApplication_INTERFACE_DEFINED__

/* interface IDebugApplication */
/* [local][unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IDebugApplication;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("51973C32-CB0C-11d0-B5C9-00A0244A0E7A")
    IDebugApplication : public IRemoteDebugApplication
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetName( 
            /* [in] */ LPCOLESTR pstrName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE StepOutComplete( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DebugOutput( 
            /* [in] */ LPCOLESTR pstr) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE StartDebugSession( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE HandleBreakPoint( 
            /* [in] */ BREAKREASON br,
            /* [out] */ BREAKRESUMEACTION *pbra) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Close( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetBreakFlags( 
            /* [out] */ APPBREAKFLAGS *pabf,
            /* [out] */ IRemoteDebugApplicationThread **pprdatSteppingThread) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetCurrentThread( 
            /* [out] */ IDebugApplicationThread **pat) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateAsyncDebugOperation( 
            /* [in] */ IDebugSyncOperation *psdo,
            /* [out] */ IDebugAsyncOperation **ppado) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AddStackFrameSniffer( 
            /* [in] */ IDebugStackFrameSniffer *pdsfs,
            /* [out] */ DWORD *pdwCookie) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RemoveStackFrameSniffer( 
            /* [in] */ DWORD dwCookie) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE QueryCurrentThreadIsDebuggerThread( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SynchronousCallInDebuggerThread( 
            /* [in] */ IDebugThreadCall *pptc,
            /* [in] */ DWORD dwParam1,
            /* [in] */ DWORD dwParam2,
            /* [in] */ DWORD dwParam3) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateApplicationNode( 
            /* [out] */ IDebugApplicationNode **ppdanNew) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE FireDebuggerEvent( 
            /* [in] */ REFGUID riid,
            /* [in] */ IUnknown *punk) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE HandleRuntimeError( 
            /* [in] */ IActiveScriptErrorDebug *pErrorDebug,
            /* [in] */ IActiveScriptSite *pScriptSite,
            /* [out] */ BREAKRESUMEACTION *pbra,
            /* [out] */ ERRORRESUMEACTION *perra,
            /* [out] */ BOOL *pfCallOnScriptError) = 0;
        
        virtual BOOL STDMETHODCALLTYPE FCanJitDebug( void) = 0;
        
        virtual BOOL STDMETHODCALLTYPE FIsAutoJitDebugEnabled( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AddGlobalExpressionContextProvider( 
            /* [in] */ IProvideExpressionContexts *pdsfs,
            /* [out] */ DWORD *pdwCookie) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE RemoveGlobalExpressionContextProvider( 
            /* [in] */ DWORD dwCookie) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDebugApplicationVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDebugApplication * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDebugApplication * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDebugApplication * This);
        
        HRESULT ( STDMETHODCALLTYPE *ResumeFromBreakPoint )( 
            IDebugApplication * This,
            /* [in] */ IRemoteDebugApplicationThread *prptFocus,
            /* [in] */ BREAKRESUMEACTION bra,
            /* [in] */ ERRORRESUMEACTION era);
        
        HRESULT ( STDMETHODCALLTYPE *CauseBreak )( 
            IDebugApplication * This);
        
        HRESULT ( STDMETHODCALLTYPE *ConnectDebugger )( 
            IDebugApplication * This,
            /* [in] */ IApplicationDebugger *pad);
        
        HRESULT ( STDMETHODCALLTYPE *DisconnectDebugger )( 
            IDebugApplication * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetDebugger )( 
            IDebugApplication * This,
            /* [out] */ IApplicationDebugger **pad);
        
        HRESULT ( STDMETHODCALLTYPE *CreateInstanceAtApplication )( 
            IDebugApplication * This,
            /* [in] */ REFCLSID rclsid,
            /* [in] */ IUnknown *pUnkOuter,
            /* [in] */ DWORD dwClsContext,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ IUnknown **ppvObject);
        
        HRESULT ( STDMETHODCALLTYPE *QueryAlive )( 
            IDebugApplication * This);
        
        HRESULT ( STDMETHODCALLTYPE *EnumThreads )( 
            IDebugApplication * This,
            /* [out] */ IEnumRemoteDebugApplicationThreads **pperdat);
        
        HRESULT ( STDMETHODCALLTYPE *GetName )( 
            IDebugApplication * This,
            /* [out] */ BSTR *pbstrName);
        
        HRESULT ( STDMETHODCALLTYPE *GetRootNode )( 
            IDebugApplication * This,
            /* [out] */ IDebugApplicationNode **ppdanRoot);
        
        HRESULT ( STDMETHODCALLTYPE *EnumGlobalExpressionContexts )( 
            IDebugApplication * This,
            /* [out] */ IEnumDebugExpressionContexts **ppedec);
        
        HRESULT ( STDMETHODCALLTYPE *SetName )( 
            IDebugApplication * This,
            /* [in] */ LPCOLESTR pstrName);
        
        HRESULT ( STDMETHODCALLTYPE *StepOutComplete )( 
            IDebugApplication * This);
        
        HRESULT ( STDMETHODCALLTYPE *DebugOutput )( 
            IDebugApplication * This,
            /* [in] */ LPCOLESTR pstr);
        
        HRESULT ( STDMETHODCALLTYPE *StartDebugSession )( 
            IDebugApplication * This);
        
        HRESULT ( STDMETHODCALLTYPE *HandleBreakPoint )( 
            IDebugApplication * This,
            /* [in] */ BREAKREASON br,
            /* [out] */ BREAKRESUMEACTION *pbra);
        
        HRESULT ( STDMETHODCALLTYPE *Close )( 
            IDebugApplication * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetBreakFlags )( 
            IDebugApplication * This,
            /* [out] */ APPBREAKFLAGS *pabf,
            /* [out] */ IRemoteDebugApplicationThread **pprdatSteppingThread);
        
        HRESULT ( STDMETHODCALLTYPE *GetCurrentThread )( 
            IDebugApplication * This,
            /* [out] */ IDebugApplicationThread **pat);
        
        HRESULT ( STDMETHODCALLTYPE *CreateAsyncDebugOperation )( 
            IDebugApplication * This,
            /* [in] */ IDebugSyncOperation *psdo,
            /* [out] */ IDebugAsyncOperation **ppado);
        
        HRESULT ( STDMETHODCALLTYPE *AddStackFrameSniffer )( 
            IDebugApplication * This,
            /* [in] */ IDebugStackFrameSniffer *pdsfs,
            /* [out] */ DWORD *pdwCookie);
        
        HRESULT ( STDMETHODCALLTYPE *RemoveStackFrameSniffer )( 
            IDebugApplication * This,
            /* [in] */ DWORD dwCookie);
        
        HRESULT ( STDMETHODCALLTYPE *QueryCurrentThreadIsDebuggerThread )( 
            IDebugApplication * This);
        
        HRESULT ( STDMETHODCALLTYPE *SynchronousCallInDebuggerThread )( 
            IDebugApplication * This,
            /* [in] */ IDebugThreadCall *pptc,
            /* [in] */ DWORD dwParam1,
            /* [in] */ DWORD dwParam2,
            /* [in] */ DWORD dwParam3);
        
        HRESULT ( STDMETHODCALLTYPE *CreateApplicationNode )( 
            IDebugApplication * This,
            /* [out] */ IDebugApplicationNode **ppdanNew);
        
        HRESULT ( STDMETHODCALLTYPE *FireDebuggerEvent )( 
            IDebugApplication * This,
            /* [in] */ REFGUID riid,
            /* [in] */ IUnknown *punk);
        
        HRESULT ( STDMETHODCALLTYPE *HandleRuntimeError )( 
            IDebugApplication * This,
            /* [in] */ IActiveScriptErrorDebug *pErrorDebug,
            /* [in] */ IActiveScriptSite *pScriptSite,
            /* [out] */ BREAKRESUMEACTION *pbra,
            /* [out] */ ERRORRESUMEACTION *perra,
            /* [out] */ BOOL *pfCallOnScriptError);
        
        BOOL ( STDMETHODCALLTYPE *FCanJitDebug )( 
            IDebugApplication * This);
        
        BOOL ( STDMETHODCALLTYPE *FIsAutoJitDebugEnabled )( 
            IDebugApplication * This);
        
        HRESULT ( STDMETHODCALLTYPE *AddGlobalExpressionContextProvider )( 
            IDebugApplication * This,
            /* [in] */ IProvideExpressionContexts *pdsfs,
            /* [out] */ DWORD *pdwCookie);
        
        HRESULT ( STDMETHODCALLTYPE *RemoveGlobalExpressionContextProvider )( 
            IDebugApplication * This,
            /* [in] */ DWORD dwCookie);
        
        END_INTERFACE
    } IDebugApplicationVtbl;

    interface IDebugApplication
    {
        CONST_VTBL struct IDebugApplicationVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDebugApplication_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDebugApplication_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDebugApplication_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDebugApplication_ResumeFromBreakPoint(This,prptFocus,bra,era)	\
    (This)->lpVtbl -> ResumeFromBreakPoint(This,prptFocus,bra,era)

#define IDebugApplication_CauseBreak(This)	\
    (This)->lpVtbl -> CauseBreak(This)

#define IDebugApplication_ConnectDebugger(This,pad)	\
    (This)->lpVtbl -> ConnectDebugger(This,pad)

#define IDebugApplication_DisconnectDebugger(This)	\
    (This)->lpVtbl -> DisconnectDebugger(This)

#define IDebugApplication_GetDebugger(This,pad)	\
    (This)->lpVtbl -> GetDebugger(This,pad)

#define IDebugApplication_CreateInstanceAtApplication(This,rclsid,pUnkOuter,dwClsContext,riid,ppvObject)	\
    (This)->lpVtbl -> CreateInstanceAtApplication(This,rclsid,pUnkOuter,dwClsContext,riid,ppvObject)

#define IDebugApplication_QueryAlive(This)	\
    (This)->lpVtbl -> QueryAlive(This)

#define IDebugApplication_EnumThreads(This,pperdat)	\
    (This)->lpVtbl -> EnumThreads(This,pperdat)

#define IDebugApplication_GetName(This,pbstrName)	\
    (This)->lpVtbl -> GetName(This,pbstrName)

#define IDebugApplication_GetRootNode(This,ppdanRoot)	\
    (This)->lpVtbl -> GetRootNode(This,ppdanRoot)

#define IDebugApplication_EnumGlobalExpressionContexts(This,ppedec)	\
    (This)->lpVtbl -> EnumGlobalExpressionContexts(This,ppedec)


#define IDebugApplication_SetName(This,pstrName)	\
    (This)->lpVtbl -> SetName(This,pstrName)

#define IDebugApplication_StepOutComplete(This)	\
    (This)->lpVtbl -> StepOutComplete(This)

#define IDebugApplication_DebugOutput(This,pstr)	\
    (This)->lpVtbl -> DebugOutput(This,pstr)

#define IDebugApplication_StartDebugSession(This)	\
    (This)->lpVtbl -> StartDebugSession(This)

#define IDebugApplication_HandleBreakPoint(This,br,pbra)	\
    (This)->lpVtbl -> HandleBreakPoint(This,br,pbra)

#define IDebugApplication_Close(This)	\
    (This)->lpVtbl -> Close(This)

#define IDebugApplication_GetBreakFlags(This,pabf,pprdatSteppingThread)	\
    (This)->lpVtbl -> GetBreakFlags(This,pabf,pprdatSteppingThread)

#define IDebugApplication_GetCurrentThread(This,pat)	\
    (This)->lpVtbl -> GetCurrentThread(This,pat)

#define IDebugApplication_CreateAsyncDebugOperation(This,psdo,ppado)	\
    (This)->lpVtbl -> CreateAsyncDebugOperation(This,psdo,ppado)

#define IDebugApplication_AddStackFrameSniffer(This,pdsfs,pdwCookie)	\
    (This)->lpVtbl -> AddStackFrameSniffer(This,pdsfs,pdwCookie)

#define IDebugApplication_RemoveStackFrameSniffer(This,dwCookie)	\
    (This)->lpVtbl -> RemoveStackFrameSniffer(This,dwCookie)

#define IDebugApplication_QueryCurrentThreadIsDebuggerThread(This)	\
    (This)->lpVtbl -> QueryCurrentThreadIsDebuggerThread(This)

#define IDebugApplication_SynchronousCallInDebuggerThread(This,pptc,dwParam1,dwParam2,dwParam3)	\
    (This)->lpVtbl -> SynchronousCallInDebuggerThread(This,pptc,dwParam1,dwParam2,dwParam3)

#define IDebugApplication_CreateApplicationNode(This,ppdanNew)	\
    (This)->lpVtbl -> CreateApplicationNode(This,ppdanNew)

#define IDebugApplication_FireDebuggerEvent(This,riid,punk)	\
    (This)->lpVtbl -> FireDebuggerEvent(This,riid,punk)

#define IDebugApplication_HandleRuntimeError(This,pErrorDebug,pScriptSite,pbra,perra,pfCallOnScriptError)	\
    (This)->lpVtbl -> HandleRuntimeError(This,pErrorDebug,pScriptSite,pbra,perra,pfCallOnScriptError)

#define IDebugApplication_FCanJitDebug(This)	\
    (This)->lpVtbl -> FCanJitDebug(This)

#define IDebugApplication_FIsAutoJitDebugEnabled(This)	\
    (This)->lpVtbl -> FIsAutoJitDebugEnabled(This)

#define IDebugApplication_AddGlobalExpressionContextProvider(This,pdsfs,pdwCookie)	\
    (This)->lpVtbl -> AddGlobalExpressionContextProvider(This,pdsfs,pdwCookie)

#define IDebugApplication_RemoveGlobalExpressionContextProvider(This,dwCookie)	\
    (This)->lpVtbl -> RemoveGlobalExpressionContextProvider(This,dwCookie)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IDebugApplication_SetName_Proxy( 
    IDebugApplication * This,
    /* [in] */ LPCOLESTR pstrName);


void __RPC_STUB IDebugApplication_SetName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugApplication_StepOutComplete_Proxy( 
    IDebugApplication * This);


void __RPC_STUB IDebugApplication_StepOutComplete_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugApplication_DebugOutput_Proxy( 
    IDebugApplication * This,
    /* [in] */ LPCOLESTR pstr);


void __RPC_STUB IDebugApplication_DebugOutput_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugApplication_StartDebugSession_Proxy( 
    IDebugApplication * This);


void __RPC_STUB IDebugApplication_StartDebugSession_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugApplication_HandleBreakPoint_Proxy( 
    IDebugApplication * This,
    /* [in] */ BREAKREASON br,
    /* [out] */ BREAKRESUMEACTION *pbra);


void __RPC_STUB IDebugApplication_HandleBreakPoint_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugApplication_Close_Proxy( 
    IDebugApplication * This);


void __RPC_STUB IDebugApplication_Close_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugApplication_GetBreakFlags_Proxy( 
    IDebugApplication * This,
    /* [out] */ APPBREAKFLAGS *pabf,
    /* [out] */ IRemoteDebugApplicationThread **pprdatSteppingThread);


void __RPC_STUB IDebugApplication_GetBreakFlags_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugApplication_GetCurrentThread_Proxy( 
    IDebugApplication * This,
    /* [out] */ IDebugApplicationThread **pat);


void __RPC_STUB IDebugApplication_GetCurrentThread_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugApplication_CreateAsyncDebugOperation_Proxy( 
    IDebugApplication * This,
    /* [in] */ IDebugSyncOperation *psdo,
    /* [out] */ IDebugAsyncOperation **ppado);


void __RPC_STUB IDebugApplication_CreateAsyncDebugOperation_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugApplication_AddStackFrameSniffer_Proxy( 
    IDebugApplication * This,
    /* [in] */ IDebugStackFrameSniffer *pdsfs,
    /* [out] */ DWORD *pdwCookie);


void __RPC_STUB IDebugApplication_AddStackFrameSniffer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugApplication_RemoveStackFrameSniffer_Proxy( 
    IDebugApplication * This,
    /* [in] */ DWORD dwCookie);


void __RPC_STUB IDebugApplication_RemoveStackFrameSniffer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugApplication_QueryCurrentThreadIsDebuggerThread_Proxy( 
    IDebugApplication * This);


void __RPC_STUB IDebugApplication_QueryCurrentThreadIsDebuggerThread_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugApplication_SynchronousCallInDebuggerThread_Proxy( 
    IDebugApplication * This,
    /* [in] */ IDebugThreadCall *pptc,
    /* [in] */ DWORD dwParam1,
    /* [in] */ DWORD dwParam2,
    /* [in] */ DWORD dwParam3);


void __RPC_STUB IDebugApplication_SynchronousCallInDebuggerThread_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugApplication_CreateApplicationNode_Proxy( 
    IDebugApplication * This,
    /* [out] */ IDebugApplicationNode **ppdanNew);


void __RPC_STUB IDebugApplication_CreateApplicationNode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugApplication_FireDebuggerEvent_Proxy( 
    IDebugApplication * This,
    /* [in] */ REFGUID riid,
    /* [in] */ IUnknown *punk);


void __RPC_STUB IDebugApplication_FireDebuggerEvent_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugApplication_HandleRuntimeError_Proxy( 
    IDebugApplication * This,
    /* [in] */ IActiveScriptErrorDebug *pErrorDebug,
    /* [in] */ IActiveScriptSite *pScriptSite,
    /* [out] */ BREAKRESUMEACTION *pbra,
    /* [out] */ ERRORRESUMEACTION *perra,
    /* [out] */ BOOL *pfCallOnScriptError);


void __RPC_STUB IDebugApplication_HandleRuntimeError_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


BOOL STDMETHODCALLTYPE IDebugApplication_FCanJitDebug_Proxy( 
    IDebugApplication * This);


void __RPC_STUB IDebugApplication_FCanJitDebug_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


BOOL STDMETHODCALLTYPE IDebugApplication_FIsAutoJitDebugEnabled_Proxy( 
    IDebugApplication * This);


void __RPC_STUB IDebugApplication_FIsAutoJitDebugEnabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugApplication_AddGlobalExpressionContextProvider_Proxy( 
    IDebugApplication * This,
    /* [in] */ IProvideExpressionContexts *pdsfs,
    /* [out] */ DWORD *pdwCookie);


void __RPC_STUB IDebugApplication_AddGlobalExpressionContextProvider_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugApplication_RemoveGlobalExpressionContextProvider_Proxy( 
    IDebugApplication * This,
    /* [in] */ DWORD dwCookie);


void __RPC_STUB IDebugApplication_RemoveGlobalExpressionContextProvider_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDebugApplication_INTERFACE_DEFINED__ */


#ifndef __IRemoteDebugApplicationThread_INTERFACE_DEFINED__
#define __IRemoteDebugApplicationThread_INTERFACE_DEFINED__

/* interface IRemoteDebugApplicationThread */
/* [unique][uuid][object] */ 

typedef DWORD THREAD_STATE;

#define	THREAD_STATE_RUNNING	( 0x1 )

#define	THREAD_STATE_SUSPENDED	( 0x2 )

#define	THREAD_BLOCKED	( 0x4 )

#define	THREAD_OUT_OF_CONTEXT	( 0x8 )


EXTERN_C const IID IID_IRemoteDebugApplicationThread;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("51973C37-CB0C-11d0-B5C9-00A0244A0E7A")
    IRemoteDebugApplicationThread : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetSystemThreadId( 
            /* [out] */ DWORD *dwThreadId) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetApplication( 
            /* [out] */ IRemoteDebugApplication **pprda) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE EnumStackFrames( 
            /* [out] */ IEnumDebugStackFrames **ppedsf) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetDescription( 
            /* [out] */ BSTR *pbstrDescription,
            /* [out] */ BSTR *pbstrState) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetNextStatement( 
            /* [in] */ IDebugStackFrame *pStackFrame,
            /* [in] */ IDebugCodeContext *pCodeContext) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetState( 
            /* [out] */ DWORD *pState) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Suspend( 
            /* [out] */ DWORD *pdwCount) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Resume( 
            /* [out] */ DWORD *pdwCount) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetSuspendCount( 
            /* [out] */ DWORD *pdwCount) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IRemoteDebugApplicationThreadVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IRemoteDebugApplicationThread * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IRemoteDebugApplicationThread * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IRemoteDebugApplicationThread * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetSystemThreadId )( 
            IRemoteDebugApplicationThread * This,
            /* [out] */ DWORD *dwThreadId);
        
        HRESULT ( STDMETHODCALLTYPE *GetApplication )( 
            IRemoteDebugApplicationThread * This,
            /* [out] */ IRemoteDebugApplication **pprda);
        
        HRESULT ( STDMETHODCALLTYPE *EnumStackFrames )( 
            IRemoteDebugApplicationThread * This,
            /* [out] */ IEnumDebugStackFrames **ppedsf);
        
        HRESULT ( STDMETHODCALLTYPE *GetDescription )( 
            IRemoteDebugApplicationThread * This,
            /* [out] */ BSTR *pbstrDescription,
            /* [out] */ BSTR *pbstrState);
        
        HRESULT ( STDMETHODCALLTYPE *SetNextStatement )( 
            IRemoteDebugApplicationThread * This,
            /* [in] */ IDebugStackFrame *pStackFrame,
            /* [in] */ IDebugCodeContext *pCodeContext);
        
        HRESULT ( STDMETHODCALLTYPE *GetState )( 
            IRemoteDebugApplicationThread * This,
            /* [out] */ DWORD *pState);
        
        HRESULT ( STDMETHODCALLTYPE *Suspend )( 
            IRemoteDebugApplicationThread * This,
            /* [out] */ DWORD *pdwCount);
        
        HRESULT ( STDMETHODCALLTYPE *Resume )( 
            IRemoteDebugApplicationThread * This,
            /* [out] */ DWORD *pdwCount);
        
        HRESULT ( STDMETHODCALLTYPE *GetSuspendCount )( 
            IRemoteDebugApplicationThread * This,
            /* [out] */ DWORD *pdwCount);
        
        END_INTERFACE
    } IRemoteDebugApplicationThreadVtbl;

    interface IRemoteDebugApplicationThread
    {
        CONST_VTBL struct IRemoteDebugApplicationThreadVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IRemoteDebugApplicationThread_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IRemoteDebugApplicationThread_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IRemoteDebugApplicationThread_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IRemoteDebugApplicationThread_GetSystemThreadId(This,dwThreadId)	\
    (This)->lpVtbl -> GetSystemThreadId(This,dwThreadId)

#define IRemoteDebugApplicationThread_GetApplication(This,pprda)	\
    (This)->lpVtbl -> GetApplication(This,pprda)

#define IRemoteDebugApplicationThread_EnumStackFrames(This,ppedsf)	\
    (This)->lpVtbl -> EnumStackFrames(This,ppedsf)

#define IRemoteDebugApplicationThread_GetDescription(This,pbstrDescription,pbstrState)	\
    (This)->lpVtbl -> GetDescription(This,pbstrDescription,pbstrState)

#define IRemoteDebugApplicationThread_SetNextStatement(This,pStackFrame,pCodeContext)	\
    (This)->lpVtbl -> SetNextStatement(This,pStackFrame,pCodeContext)

#define IRemoteDebugApplicationThread_GetState(This,pState)	\
    (This)->lpVtbl -> GetState(This,pState)

#define IRemoteDebugApplicationThread_Suspend(This,pdwCount)	\
    (This)->lpVtbl -> Suspend(This,pdwCount)

#define IRemoteDebugApplicationThread_Resume(This,pdwCount)	\
    (This)->lpVtbl -> Resume(This,pdwCount)

#define IRemoteDebugApplicationThread_GetSuspendCount(This,pdwCount)	\
    (This)->lpVtbl -> GetSuspendCount(This,pdwCount)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IRemoteDebugApplicationThread_GetSystemThreadId_Proxy( 
    IRemoteDebugApplicationThread * This,
    /* [out] */ DWORD *dwThreadId);


void __RPC_STUB IRemoteDebugApplicationThread_GetSystemThreadId_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IRemoteDebugApplicationThread_GetApplication_Proxy( 
    IRemoteDebugApplicationThread * This,
    /* [out] */ IRemoteDebugApplication **pprda);


void __RPC_STUB IRemoteDebugApplicationThread_GetApplication_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IRemoteDebugApplicationThread_EnumStackFrames_Proxy( 
    IRemoteDebugApplicationThread * This,
    /* [out] */ IEnumDebugStackFrames **ppedsf);


void __RPC_STUB IRemoteDebugApplicationThread_EnumStackFrames_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IRemoteDebugApplicationThread_GetDescription_Proxy( 
    IRemoteDebugApplicationThread * This,
    /* [out] */ BSTR *pbstrDescription,
    /* [out] */ BSTR *pbstrState);


void __RPC_STUB IRemoteDebugApplicationThread_GetDescription_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IRemoteDebugApplicationThread_SetNextStatement_Proxy( 
    IRemoteDebugApplicationThread * This,
    /* [in] */ IDebugStackFrame *pStackFrame,
    /* [in] */ IDebugCodeContext *pCodeContext);


void __RPC_STUB IRemoteDebugApplicationThread_SetNextStatement_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IRemoteDebugApplicationThread_GetState_Proxy( 
    IRemoteDebugApplicationThread * This,
    /* [out] */ DWORD *pState);


void __RPC_STUB IRemoteDebugApplicationThread_GetState_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IRemoteDebugApplicationThread_Suspend_Proxy( 
    IRemoteDebugApplicationThread * This,
    /* [out] */ DWORD *pdwCount);


void __RPC_STUB IRemoteDebugApplicationThread_Suspend_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IRemoteDebugApplicationThread_Resume_Proxy( 
    IRemoteDebugApplicationThread * This,
    /* [out] */ DWORD *pdwCount);


void __RPC_STUB IRemoteDebugApplicationThread_Resume_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IRemoteDebugApplicationThread_GetSuspendCount_Proxy( 
    IRemoteDebugApplicationThread * This,
    /* [out] */ DWORD *pdwCount);


void __RPC_STUB IRemoteDebugApplicationThread_GetSuspendCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IRemoteDebugApplicationThread_INTERFACE_DEFINED__ */


#ifndef __IDebugApplicationThread_INTERFACE_DEFINED__
#define __IDebugApplicationThread_INTERFACE_DEFINED__

/* interface IDebugApplicationThread */
/* [local][unique][uuid][object] */ 


EXTERN_C const IID IID_IDebugApplicationThread;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("51973C38-CB0C-11d0-B5C9-00A0244A0E7A")
    IDebugApplicationThread : public IRemoteDebugApplicationThread
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SynchronousCallIntoThread( 
            /* [in] */ IDebugThreadCall *pstcb,
            /* [in] */ DWORD dwParam1,
            /* [in] */ DWORD dwParam2,
            /* [in] */ DWORD dwParam3) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE QueryIsCurrentThread( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE QueryIsDebuggerThread( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetDescription( 
            /* [in] */ LPCOLESTR pstrDescription) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE SetStateString( 
            /* [in] */ LPCOLESTR pstrState) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDebugApplicationThreadVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDebugApplicationThread * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDebugApplicationThread * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDebugApplicationThread * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetSystemThreadId )( 
            IDebugApplicationThread * This,
            /* [out] */ DWORD *dwThreadId);
        
        HRESULT ( STDMETHODCALLTYPE *GetApplication )( 
            IDebugApplicationThread * This,
            /* [out] */ IRemoteDebugApplication **pprda);
        
        HRESULT ( STDMETHODCALLTYPE *EnumStackFrames )( 
            IDebugApplicationThread * This,
            /* [out] */ IEnumDebugStackFrames **ppedsf);
        
        HRESULT ( STDMETHODCALLTYPE *GetDescription )( 
            IDebugApplicationThread * This,
            /* [out] */ BSTR *pbstrDescription,
            /* [out] */ BSTR *pbstrState);
        
        HRESULT ( STDMETHODCALLTYPE *SetNextStatement )( 
            IDebugApplicationThread * This,
            /* [in] */ IDebugStackFrame *pStackFrame,
            /* [in] */ IDebugCodeContext *pCodeContext);
        
        HRESULT ( STDMETHODCALLTYPE *GetState )( 
            IDebugApplicationThread * This,
            /* [out] */ DWORD *pState);
        
        HRESULT ( STDMETHODCALLTYPE *Suspend )( 
            IDebugApplicationThread * This,
            /* [out] */ DWORD *pdwCount);
        
        HRESULT ( STDMETHODCALLTYPE *Resume )( 
            IDebugApplicationThread * This,
            /* [out] */ DWORD *pdwCount);
        
        HRESULT ( STDMETHODCALLTYPE *GetSuspendCount )( 
            IDebugApplicationThread * This,
            /* [out] */ DWORD *pdwCount);
        
        HRESULT ( STDMETHODCALLTYPE *SynchronousCallIntoThread )( 
            IDebugApplicationThread * This,
            /* [in] */ IDebugThreadCall *pstcb,
            /* [in] */ DWORD dwParam1,
            /* [in] */ DWORD dwParam2,
            /* [in] */ DWORD dwParam3);
        
        HRESULT ( STDMETHODCALLTYPE *QueryIsCurrentThread )( 
            IDebugApplicationThread * This);
        
        HRESULT ( STDMETHODCALLTYPE *QueryIsDebuggerThread )( 
            IDebugApplicationThread * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetDescription )( 
            IDebugApplicationThread * This,
            /* [in] */ LPCOLESTR pstrDescription);
        
        HRESULT ( STDMETHODCALLTYPE *SetStateString )( 
            IDebugApplicationThread * This,
            /* [in] */ LPCOLESTR pstrState);
        
        END_INTERFACE
    } IDebugApplicationThreadVtbl;

    interface IDebugApplicationThread
    {
        CONST_VTBL struct IDebugApplicationThreadVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDebugApplicationThread_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDebugApplicationThread_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDebugApplicationThread_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDebugApplicationThread_GetSystemThreadId(This,dwThreadId)	\
    (This)->lpVtbl -> GetSystemThreadId(This,dwThreadId)

#define IDebugApplicationThread_GetApplication(This,pprda)	\
    (This)->lpVtbl -> GetApplication(This,pprda)

#define IDebugApplicationThread_EnumStackFrames(This,ppedsf)	\
    (This)->lpVtbl -> EnumStackFrames(This,ppedsf)

#define IDebugApplicationThread_GetDescription(This,pbstrDescription,pbstrState)	\
    (This)->lpVtbl -> GetDescription(This,pbstrDescription,pbstrState)

#define IDebugApplicationThread_SetNextStatement(This,pStackFrame,pCodeContext)	\
    (This)->lpVtbl -> SetNextStatement(This,pStackFrame,pCodeContext)

#define IDebugApplicationThread_GetState(This,pState)	\
    (This)->lpVtbl -> GetState(This,pState)

#define IDebugApplicationThread_Suspend(This,pdwCount)	\
    (This)->lpVtbl -> Suspend(This,pdwCount)

#define IDebugApplicationThread_Resume(This,pdwCount)	\
    (This)->lpVtbl -> Resume(This,pdwCount)

#define IDebugApplicationThread_GetSuspendCount(This,pdwCount)	\
    (This)->lpVtbl -> GetSuspendCount(This,pdwCount)


#define IDebugApplicationThread_SynchronousCallIntoThread(This,pstcb,dwParam1,dwParam2,dwParam3)	\
    (This)->lpVtbl -> SynchronousCallIntoThread(This,pstcb,dwParam1,dwParam2,dwParam3)

#define IDebugApplicationThread_QueryIsCurrentThread(This)	\
    (This)->lpVtbl -> QueryIsCurrentThread(This)

#define IDebugApplicationThread_QueryIsDebuggerThread(This)	\
    (This)->lpVtbl -> QueryIsDebuggerThread(This)

#define IDebugApplicationThread_SetDescription(This,pstrDescription)	\
    (This)->lpVtbl -> SetDescription(This,pstrDescription)

#define IDebugApplicationThread_SetStateString(This,pstrState)	\
    (This)->lpVtbl -> SetStateString(This,pstrState)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IDebugApplicationThread_SynchronousCallIntoThread_Proxy( 
    IDebugApplicationThread * This,
    /* [in] */ IDebugThreadCall *pstcb,
    /* [in] */ DWORD dwParam1,
    /* [in] */ DWORD dwParam2,
    /* [in] */ DWORD dwParam3);


void __RPC_STUB IDebugApplicationThread_SynchronousCallIntoThread_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugApplicationThread_QueryIsCurrentThread_Proxy( 
    IDebugApplicationThread * This);


void __RPC_STUB IDebugApplicationThread_QueryIsCurrentThread_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugApplicationThread_QueryIsDebuggerThread_Proxy( 
    IDebugApplicationThread * This);


void __RPC_STUB IDebugApplicationThread_QueryIsDebuggerThread_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugApplicationThread_SetDescription_Proxy( 
    IDebugApplicationThread * This,
    /* [in] */ LPCOLESTR pstrDescription);


void __RPC_STUB IDebugApplicationThread_SetDescription_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugApplicationThread_SetStateString_Proxy( 
    IDebugApplicationThread * This,
    /* [in] */ LPCOLESTR pstrState);


void __RPC_STUB IDebugApplicationThread_SetStateString_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDebugApplicationThread_INTERFACE_DEFINED__ */


#ifndef __IEnumRemoteDebugApplicationThreads_INTERFACE_DEFINED__
#define __IEnumRemoteDebugApplicationThreads_INTERFACE_DEFINED__

/* interface IEnumRemoteDebugApplicationThreads */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IEnumRemoteDebugApplicationThreads;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("51973C3c-CB0C-11d0-B5C9-00A0244A0E7A")
    IEnumRemoteDebugApplicationThreads : public IUnknown
    {
    public:
        virtual /* [local] */ HRESULT __stdcall Next( 
            /* [in] */ ULONG celt,
            /* [out] */ IRemoteDebugApplicationThread **pprdat,
            /* [out] */ ULONG *pceltFetched) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Skip( 
            /* [in] */ ULONG celt) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Reset( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Clone( 
            /* [out] */ IEnumRemoteDebugApplicationThreads **pperdat) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IEnumRemoteDebugApplicationThreadsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IEnumRemoteDebugApplicationThreads * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IEnumRemoteDebugApplicationThreads * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IEnumRemoteDebugApplicationThreads * This);
        
        /* [local] */ HRESULT ( __stdcall *Next )( 
            IEnumRemoteDebugApplicationThreads * This,
            /* [in] */ ULONG celt,
            /* [out] */ IRemoteDebugApplicationThread **pprdat,
            /* [out] */ ULONG *pceltFetched);
        
        HRESULT ( STDMETHODCALLTYPE *Skip )( 
            IEnumRemoteDebugApplicationThreads * This,
            /* [in] */ ULONG celt);
        
        HRESULT ( STDMETHODCALLTYPE *Reset )( 
            IEnumRemoteDebugApplicationThreads * This);
        
        HRESULT ( STDMETHODCALLTYPE *Clone )( 
            IEnumRemoteDebugApplicationThreads * This,
            /* [out] */ IEnumRemoteDebugApplicationThreads **pperdat);
        
        END_INTERFACE
    } IEnumRemoteDebugApplicationThreadsVtbl;

    interface IEnumRemoteDebugApplicationThreads
    {
        CONST_VTBL struct IEnumRemoteDebugApplicationThreadsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEnumRemoteDebugApplicationThreads_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IEnumRemoteDebugApplicationThreads_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IEnumRemoteDebugApplicationThreads_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IEnumRemoteDebugApplicationThreads_Next(This,celt,pprdat,pceltFetched)	\
    (This)->lpVtbl -> Next(This,celt,pprdat,pceltFetched)

#define IEnumRemoteDebugApplicationThreads_Skip(This,celt)	\
    (This)->lpVtbl -> Skip(This,celt)

#define IEnumRemoteDebugApplicationThreads_Reset(This)	\
    (This)->lpVtbl -> Reset(This)

#define IEnumRemoteDebugApplicationThreads_Clone(This,pperdat)	\
    (This)->lpVtbl -> Clone(This,pperdat)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [call_as] */ HRESULT __stdcall IEnumRemoteDebugApplicationThreads_RemoteNext_Proxy( 
    IEnumRemoteDebugApplicationThreads * This,
    /* [in] */ ULONG celt,
    /* [length_is][size_is][out] */ IRemoteDebugApplicationThread **ppdat,
    /* [out] */ ULONG *pceltFetched);


void __RPC_STUB IEnumRemoteDebugApplicationThreads_RemoteNext_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEnumRemoteDebugApplicationThreads_Skip_Proxy( 
    IEnumRemoteDebugApplicationThreads * This,
    /* [in] */ ULONG celt);


void __RPC_STUB IEnumRemoteDebugApplicationThreads_Skip_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEnumRemoteDebugApplicationThreads_Reset_Proxy( 
    IEnumRemoteDebugApplicationThreads * This);


void __RPC_STUB IEnumRemoteDebugApplicationThreads_Reset_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEnumRemoteDebugApplicationThreads_Clone_Proxy( 
    IEnumRemoteDebugApplicationThreads * This,
    /* [out] */ IEnumRemoteDebugApplicationThreads **pperdat);


void __RPC_STUB IEnumRemoteDebugApplicationThreads_Clone_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IEnumRemoteDebugApplicationThreads_INTERFACE_DEFINED__ */


#ifndef __IDebugThreadCall_INTERFACE_DEFINED__
#define __IDebugThreadCall_INTERFACE_DEFINED__

/* interface IDebugThreadCall */
/* [local][unique][uuid][object] */ 


EXTERN_C const IID IID_IDebugThreadCall;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("51973C36-CB0C-11d0-B5C9-00A0244A0E7A")
    IDebugThreadCall : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE ThreadCallHandler( 
            /* [in] */ DWORD dwParam1,
            /* [in] */ DWORD dwParam2,
            /* [in] */ DWORD dwParam3) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDebugThreadCallVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDebugThreadCall * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDebugThreadCall * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDebugThreadCall * This);
        
        HRESULT ( STDMETHODCALLTYPE *ThreadCallHandler )( 
            IDebugThreadCall * This,
            /* [in] */ DWORD dwParam1,
            /* [in] */ DWORD dwParam2,
            /* [in] */ DWORD dwParam3);
        
        END_INTERFACE
    } IDebugThreadCallVtbl;

    interface IDebugThreadCall
    {
        CONST_VTBL struct IDebugThreadCallVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDebugThreadCall_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDebugThreadCall_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDebugThreadCall_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDebugThreadCall_ThreadCallHandler(This,dwParam1,dwParam2,dwParam3)	\
    (This)->lpVtbl -> ThreadCallHandler(This,dwParam1,dwParam2,dwParam3)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IDebugThreadCall_ThreadCallHandler_Proxy( 
    IDebugThreadCall * This,
    /* [in] */ DWORD dwParam1,
    /* [in] */ DWORD dwParam2,
    /* [in] */ DWORD dwParam3);


void __RPC_STUB IDebugThreadCall_ThreadCallHandler_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDebugThreadCall_INTERFACE_DEFINED__ */


#ifndef __IEnumDebugApplicationNodes_INTERFACE_DEFINED__
#define __IEnumDebugApplicationNodes_INTERFACE_DEFINED__

/* interface IEnumDebugApplicationNodes */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IEnumDebugApplicationNodes;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("51973C3a-CB0C-11d0-B5C9-00A0244A0E7A")
    IEnumDebugApplicationNodes : public IUnknown
    {
    public:
        virtual /* [local] */ HRESULT __stdcall Next( 
            /* [in] */ ULONG celt,
            /* [out] */ IDebugApplicationNode **pprddp,
            /* [out] */ ULONG *pceltFetched) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Skip( 
            /* [in] */ ULONG celt) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Reset( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Clone( 
            /* [out] */ IEnumDebugApplicationNodes **pperddp) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IEnumDebugApplicationNodesVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IEnumDebugApplicationNodes * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IEnumDebugApplicationNodes * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IEnumDebugApplicationNodes * This);
        
        /* [local] */ HRESULT ( __stdcall *Next )( 
            IEnumDebugApplicationNodes * This,
            /* [in] */ ULONG celt,
            /* [out] */ IDebugApplicationNode **pprddp,
            /* [out] */ ULONG *pceltFetched);
        
        HRESULT ( STDMETHODCALLTYPE *Skip )( 
            IEnumDebugApplicationNodes * This,
            /* [in] */ ULONG celt);
        
        HRESULT ( STDMETHODCALLTYPE *Reset )( 
            IEnumDebugApplicationNodes * This);
        
        HRESULT ( STDMETHODCALLTYPE *Clone )( 
            IEnumDebugApplicationNodes * This,
            /* [out] */ IEnumDebugApplicationNodes **pperddp);
        
        END_INTERFACE
    } IEnumDebugApplicationNodesVtbl;

    interface IEnumDebugApplicationNodes
    {
        CONST_VTBL struct IEnumDebugApplicationNodesVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEnumDebugApplicationNodes_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IEnumDebugApplicationNodes_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IEnumDebugApplicationNodes_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IEnumDebugApplicationNodes_Next(This,celt,pprddp,pceltFetched)	\
    (This)->lpVtbl -> Next(This,celt,pprddp,pceltFetched)

#define IEnumDebugApplicationNodes_Skip(This,celt)	\
    (This)->lpVtbl -> Skip(This,celt)

#define IEnumDebugApplicationNodes_Reset(This)	\
    (This)->lpVtbl -> Reset(This)

#define IEnumDebugApplicationNodes_Clone(This,pperddp)	\
    (This)->lpVtbl -> Clone(This,pperddp)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [call_as] */ HRESULT __stdcall IEnumDebugApplicationNodes_RemoteNext_Proxy( 
    IEnumDebugApplicationNodes * This,
    /* [in] */ ULONG celt,
    /* [length_is][size_is][out] */ IDebugApplicationNode **pprddp,
    /* [out] */ ULONG *pceltFetched);


void __RPC_STUB IEnumDebugApplicationNodes_RemoteNext_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEnumDebugApplicationNodes_Skip_Proxy( 
    IEnumDebugApplicationNodes * This,
    /* [in] */ ULONG celt);


void __RPC_STUB IEnumDebugApplicationNodes_Skip_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEnumDebugApplicationNodes_Reset_Proxy( 
    IEnumDebugApplicationNodes * This);


void __RPC_STUB IEnumDebugApplicationNodes_Reset_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEnumDebugApplicationNodes_Clone_Proxy( 
    IEnumDebugApplicationNodes * This,
    /* [out] */ IEnumDebugApplicationNodes **pperddp);


void __RPC_STUB IEnumDebugApplicationNodes_Clone_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IEnumDebugApplicationNodes_INTERFACE_DEFINED__ */


#ifndef __IEnumDebugStackFrames_INTERFACE_DEFINED__
#define __IEnumDebugStackFrames_INTERFACE_DEFINED__

/* interface IEnumDebugStackFrames */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IEnumDebugStackFrames;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("51973C1e-CB0C-11d0-B5C9-00A0244A0E7A")
    IEnumDebugStackFrames : public IUnknown
    {
    public:
        virtual /* [local] */ HRESULT __stdcall Next( 
            /* [in] */ ULONG celt,
            /* [out] */ DebugStackFrameDescriptor *prgdsfd,
            /* [out] */ ULONG *pceltFetched) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Skip( 
            /* [in] */ ULONG celt) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Reset( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Clone( 
            /* [out] */ IEnumDebugStackFrames **ppedsf) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IEnumDebugStackFramesVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IEnumDebugStackFrames * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IEnumDebugStackFrames * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IEnumDebugStackFrames * This);
        
        /* [local] */ HRESULT ( __stdcall *Next )( 
            IEnumDebugStackFrames * This,
            /* [in] */ ULONG celt,
            /* [out] */ DebugStackFrameDescriptor *prgdsfd,
            /* [out] */ ULONG *pceltFetched);
        
        HRESULT ( STDMETHODCALLTYPE *Skip )( 
            IEnumDebugStackFrames * This,
            /* [in] */ ULONG celt);
        
        HRESULT ( STDMETHODCALLTYPE *Reset )( 
            IEnumDebugStackFrames * This);
        
        HRESULT ( STDMETHODCALLTYPE *Clone )( 
            IEnumDebugStackFrames * This,
            /* [out] */ IEnumDebugStackFrames **ppedsf);
        
        END_INTERFACE
    } IEnumDebugStackFramesVtbl;

    interface IEnumDebugStackFrames
    {
        CONST_VTBL struct IEnumDebugStackFramesVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEnumDebugStackFrames_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IEnumDebugStackFrames_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IEnumDebugStackFrames_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IEnumDebugStackFrames_Next(This,celt,prgdsfd,pceltFetched)	\
    (This)->lpVtbl -> Next(This,celt,prgdsfd,pceltFetched)

#define IEnumDebugStackFrames_Skip(This,celt)	\
    (This)->lpVtbl -> Skip(This,celt)

#define IEnumDebugStackFrames_Reset(This)	\
    (This)->lpVtbl -> Reset(This)

#define IEnumDebugStackFrames_Clone(This,ppedsf)	\
    (This)->lpVtbl -> Clone(This,ppedsf)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [call_as] */ HRESULT __stdcall IEnumDebugStackFrames_RemoteNext_Proxy( 
    IEnumDebugStackFrames * This,
    /* [in] */ ULONG celt,
    /* [length_is][size_is][out] */ DebugStackFrameDescriptor *prgdsfd,
    /* [out] */ ULONG *pceltFetched);


void __RPC_STUB IEnumDebugStackFrames_RemoteNext_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEnumDebugStackFrames_Skip_Proxy( 
    IEnumDebugStackFrames * This,
    /* [in] */ ULONG celt);


void __RPC_STUB IEnumDebugStackFrames_Skip_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEnumDebugStackFrames_Reset_Proxy( 
    IEnumDebugStackFrames * This);


void __RPC_STUB IEnumDebugStackFrames_Reset_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IEnumDebugStackFrames_Clone_Proxy( 
    IEnumDebugStackFrames * This,
    /* [out] */ IEnumDebugStackFrames **ppedsf);


void __RPC_STUB IEnumDebugStackFrames_Clone_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IEnumDebugStackFrames_INTERFACE_DEFINED__ */


#ifndef __IDebugStackFrameSnifferEx_INTERFACE_DEFINED__
#define __IDebugStackFrameSnifferEx_INTERFACE_DEFINED__

/* interface IDebugStackFrameSnifferEx */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IDebugStackFrameSnifferEx;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("51973C19-CB0C-11d0-B5C9-00A0244A0E7A")
    IDebugStackFrameSnifferEx : public IDebugStackFrameSniffer
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE EnumStackFramesEx( 
            /* [in] */ DWORD dwSpMin,
            /* [out] */ IEnumDebugStackFrames **ppedsf) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDebugStackFrameSnifferExVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDebugStackFrameSnifferEx * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDebugStackFrameSnifferEx * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDebugStackFrameSnifferEx * This);
        
        HRESULT ( STDMETHODCALLTYPE *EnumStackFrames )( 
            IDebugStackFrameSnifferEx * This,
            /* [out] */ IEnumDebugStackFrames **ppedsf);
        
        HRESULT ( STDMETHODCALLTYPE *EnumStackFramesEx )( 
            IDebugStackFrameSnifferEx * This,
            /* [in] */ DWORD dwSpMin,
            /* [out] */ IEnumDebugStackFrames **ppedsf);
        
        END_INTERFACE
    } IDebugStackFrameSnifferExVtbl;

    interface IDebugStackFrameSnifferEx
    {
        CONST_VTBL struct IDebugStackFrameSnifferExVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDebugStackFrameSnifferEx_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDebugStackFrameSnifferEx_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDebugStackFrameSnifferEx_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDebugStackFrameSnifferEx_EnumStackFrames(This,ppedsf)	\
    (This)->lpVtbl -> EnumStackFrames(This,ppedsf)


#define IDebugStackFrameSnifferEx_EnumStackFramesEx(This,dwSpMin,ppedsf)	\
    (This)->lpVtbl -> EnumStackFramesEx(This,dwSpMin,ppedsf)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IDebugStackFrameSnifferEx_EnumStackFramesEx_Proxy( 
    IDebugStackFrameSnifferEx * This,
    /* [in] */ DWORD dwSpMin,
    /* [out] */ IEnumDebugStackFrames **ppedsf);


void __RPC_STUB IDebugStackFrameSnifferEx_EnumStackFramesEx_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDebugStackFrameSnifferEx_INTERFACE_DEFINED__ */


#ifndef __IDebugDocumentTextExternalAuthor_INTERFACE_DEFINED__
#define __IDebugDocumentTextExternalAuthor_INTERFACE_DEFINED__

/* interface IDebugDocumentTextExternalAuthor */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IDebugDocumentTextExternalAuthor;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("51973C25-CB0C-11d0-B5C9-00A0244A0E7A")
    IDebugDocumentTextExternalAuthor : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetPathName( 
            /* [out] */ BSTR *pbstrLongName,
            /* [out] */ BOOL *pfIsOriginalFile) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetFileName( 
            /* [out] */ BSTR *pbstrShortName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE NotifyChanged( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDebugDocumentTextExternalAuthorVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDebugDocumentTextExternalAuthor * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDebugDocumentTextExternalAuthor * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDebugDocumentTextExternalAuthor * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetPathName )( 
            IDebugDocumentTextExternalAuthor * This,
            /* [out] */ BSTR *pbstrLongName,
            /* [out] */ BOOL *pfIsOriginalFile);
        
        HRESULT ( STDMETHODCALLTYPE *GetFileName )( 
            IDebugDocumentTextExternalAuthor * This,
            /* [out] */ BSTR *pbstrShortName);
        
        HRESULT ( STDMETHODCALLTYPE *NotifyChanged )( 
            IDebugDocumentTextExternalAuthor * This);
        
        END_INTERFACE
    } IDebugDocumentTextExternalAuthorVtbl;

    interface IDebugDocumentTextExternalAuthor
    {
        CONST_VTBL struct IDebugDocumentTextExternalAuthorVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDebugDocumentTextExternalAuthor_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDebugDocumentTextExternalAuthor_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDebugDocumentTextExternalAuthor_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDebugDocumentTextExternalAuthor_GetPathName(This,pbstrLongName,pfIsOriginalFile)	\
    (This)->lpVtbl -> GetPathName(This,pbstrLongName,pfIsOriginalFile)

#define IDebugDocumentTextExternalAuthor_GetFileName(This,pbstrShortName)	\
    (This)->lpVtbl -> GetFileName(This,pbstrShortName)

#define IDebugDocumentTextExternalAuthor_NotifyChanged(This)	\
    (This)->lpVtbl -> NotifyChanged(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IDebugDocumentTextExternalAuthor_GetPathName_Proxy( 
    IDebugDocumentTextExternalAuthor * This,
    /* [out] */ BSTR *pbstrLongName,
    /* [out] */ BOOL *pfIsOriginalFile);


void __RPC_STUB IDebugDocumentTextExternalAuthor_GetPathName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugDocumentTextExternalAuthor_GetFileName_Proxy( 
    IDebugDocumentTextExternalAuthor * This,
    /* [out] */ BSTR *pbstrShortName);


void __RPC_STUB IDebugDocumentTextExternalAuthor_GetFileName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugDocumentTextExternalAuthor_NotifyChanged_Proxy( 
    IDebugDocumentTextExternalAuthor * This);


void __RPC_STUB IDebugDocumentTextExternalAuthor_NotifyChanged_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDebugDocumentTextExternalAuthor_INTERFACE_DEFINED__ */


#ifndef __IApplicationDebuggerUI_INTERFACE_DEFINED__
#define __IApplicationDebuggerUI_INTERFACE_DEFINED__

/* interface IApplicationDebuggerUI */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IApplicationDebuggerUI;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("51973C2b-CB0C-11d0-B5C9-00A0244A0E7A")
    IApplicationDebuggerUI : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE BringDocumentToTop( 
            /* [in] */ IDebugDocumentText *pddt) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE BringDocumentContextToTop( 
            /* [in] */ IDebugDocumentContext *pddc) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IApplicationDebuggerUIVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IApplicationDebuggerUI * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IApplicationDebuggerUI * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IApplicationDebuggerUI * This);
        
        HRESULT ( STDMETHODCALLTYPE *BringDocumentToTop )( 
            IApplicationDebuggerUI * This,
            /* [in] */ IDebugDocumentText *pddt);
        
        HRESULT ( STDMETHODCALLTYPE *BringDocumentContextToTop )( 
            IApplicationDebuggerUI * This,
            /* [in] */ IDebugDocumentContext *pddc);
        
        END_INTERFACE
    } IApplicationDebuggerUIVtbl;

    interface IApplicationDebuggerUI
    {
        CONST_VTBL struct IApplicationDebuggerUIVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IApplicationDebuggerUI_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IApplicationDebuggerUI_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IApplicationDebuggerUI_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IApplicationDebuggerUI_BringDocumentToTop(This,pddt)	\
    (This)->lpVtbl -> BringDocumentToTop(This,pddt)

#define IApplicationDebuggerUI_BringDocumentContextToTop(This,pddc)	\
    (This)->lpVtbl -> BringDocumentContextToTop(This,pddc)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IApplicationDebuggerUI_BringDocumentToTop_Proxy( 
    IApplicationDebuggerUI * This,
    /* [in] */ IDebugDocumentText *pddt);


void __RPC_STUB IApplicationDebuggerUI_BringDocumentToTop_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IApplicationDebuggerUI_BringDocumentContextToTop_Proxy( 
    IApplicationDebuggerUI * This,
    /* [in] */ IDebugDocumentContext *pddc);


void __RPC_STUB IApplicationDebuggerUI_BringDocumentContextToTop_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IApplicationDebuggerUI_INTERFACE_DEFINED__ */


#ifndef __IRemoteDebugApplicationEvents_INTERFACE_DEFINED__
#define __IRemoteDebugApplicationEvents_INTERFACE_DEFINED__

/* interface IRemoteDebugApplicationEvents */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IRemoteDebugApplicationEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("51973C33-CB0C-11d0-B5C9-00A0244A0E7A")
    IRemoteDebugApplicationEvents : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE OnConnectDebugger( 
            /* [in] */ IApplicationDebugger *pad) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnDisconnectDebugger( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnSetName( 
            /* [in] */ LPCOLESTR pstrName) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnDebugOutput( 
            /* [in] */ LPCOLESTR pstr) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnClose( void) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnEnterBreakPoint( 
            /* [in] */ IRemoteDebugApplicationThread *prdat) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnLeaveBreakPoint( 
            /* [in] */ IRemoteDebugApplicationThread *prdat) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnCreateThread( 
            /* [in] */ IRemoteDebugApplicationThread *prdat) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnDestroyThread( 
            /* [in] */ IRemoteDebugApplicationThread *prdat) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnBreakFlagChange( 
            /* [in] */ APPBREAKFLAGS abf,
            /* [in] */ IRemoteDebugApplicationThread *prdatSteppingThread) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IRemoteDebugApplicationEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IRemoteDebugApplicationEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IRemoteDebugApplicationEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IRemoteDebugApplicationEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *OnConnectDebugger )( 
            IRemoteDebugApplicationEvents * This,
            /* [in] */ IApplicationDebugger *pad);
        
        HRESULT ( STDMETHODCALLTYPE *OnDisconnectDebugger )( 
            IRemoteDebugApplicationEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *OnSetName )( 
            IRemoteDebugApplicationEvents * This,
            /* [in] */ LPCOLESTR pstrName);
        
        HRESULT ( STDMETHODCALLTYPE *OnDebugOutput )( 
            IRemoteDebugApplicationEvents * This,
            /* [in] */ LPCOLESTR pstr);
        
        HRESULT ( STDMETHODCALLTYPE *OnClose )( 
            IRemoteDebugApplicationEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *OnEnterBreakPoint )( 
            IRemoteDebugApplicationEvents * This,
            /* [in] */ IRemoteDebugApplicationThread *prdat);
        
        HRESULT ( STDMETHODCALLTYPE *OnLeaveBreakPoint )( 
            IRemoteDebugApplicationEvents * This,
            /* [in] */ IRemoteDebugApplicationThread *prdat);
        
        HRESULT ( STDMETHODCALLTYPE *OnCreateThread )( 
            IRemoteDebugApplicationEvents * This,
            /* [in] */ IRemoteDebugApplicationThread *prdat);
        
        HRESULT ( STDMETHODCALLTYPE *OnDestroyThread )( 
            IRemoteDebugApplicationEvents * This,
            /* [in] */ IRemoteDebugApplicationThread *prdat);
        
        HRESULT ( STDMETHODCALLTYPE *OnBreakFlagChange )( 
            IRemoteDebugApplicationEvents * This,
            /* [in] */ APPBREAKFLAGS abf,
            /* [in] */ IRemoteDebugApplicationThread *prdatSteppingThread);
        
        END_INTERFACE
    } IRemoteDebugApplicationEventsVtbl;

    interface IRemoteDebugApplicationEvents
    {
        CONST_VTBL struct IRemoteDebugApplicationEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IRemoteDebugApplicationEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IRemoteDebugApplicationEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IRemoteDebugApplicationEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IRemoteDebugApplicationEvents_OnConnectDebugger(This,pad)	\
    (This)->lpVtbl -> OnConnectDebugger(This,pad)

#define IRemoteDebugApplicationEvents_OnDisconnectDebugger(This)	\
    (This)->lpVtbl -> OnDisconnectDebugger(This)

#define IRemoteDebugApplicationEvents_OnSetName(This,pstrName)	\
    (This)->lpVtbl -> OnSetName(This,pstrName)

#define IRemoteDebugApplicationEvents_OnDebugOutput(This,pstr)	\
    (This)->lpVtbl -> OnDebugOutput(This,pstr)

#define IRemoteDebugApplicationEvents_OnClose(This)	\
    (This)->lpVtbl -> OnClose(This)

#define IRemoteDebugApplicationEvents_OnEnterBreakPoint(This,prdat)	\
    (This)->lpVtbl -> OnEnterBreakPoint(This,prdat)

#define IRemoteDebugApplicationEvents_OnLeaveBreakPoint(This,prdat)	\
    (This)->lpVtbl -> OnLeaveBreakPoint(This,prdat)

#define IRemoteDebugApplicationEvents_OnCreateThread(This,prdat)	\
    (This)->lpVtbl -> OnCreateThread(This,prdat)

#define IRemoteDebugApplicationEvents_OnDestroyThread(This,prdat)	\
    (This)->lpVtbl -> OnDestroyThread(This,prdat)

#define IRemoteDebugApplicationEvents_OnBreakFlagChange(This,abf,prdatSteppingThread)	\
    (This)->lpVtbl -> OnBreakFlagChange(This,abf,prdatSteppingThread)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IRemoteDebugApplicationEvents_OnConnectDebugger_Proxy( 
    IRemoteDebugApplicationEvents * This,
    /* [in] */ IApplicationDebugger *pad);


void __RPC_STUB IRemoteDebugApplicationEvents_OnConnectDebugger_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IRemoteDebugApplicationEvents_OnDisconnectDebugger_Proxy( 
    IRemoteDebugApplicationEvents * This);


void __RPC_STUB IRemoteDebugApplicationEvents_OnDisconnectDebugger_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IRemoteDebugApplicationEvents_OnSetName_Proxy( 
    IRemoteDebugApplicationEvents * This,
    /* [in] */ LPCOLESTR pstrName);


void __RPC_STUB IRemoteDebugApplicationEvents_OnSetName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IRemoteDebugApplicationEvents_OnDebugOutput_Proxy( 
    IRemoteDebugApplicationEvents * This,
    /* [in] */ LPCOLESTR pstr);


void __RPC_STUB IRemoteDebugApplicationEvents_OnDebugOutput_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IRemoteDebugApplicationEvents_OnClose_Proxy( 
    IRemoteDebugApplicationEvents * This);


void __RPC_STUB IRemoteDebugApplicationEvents_OnClose_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IRemoteDebugApplicationEvents_OnEnterBreakPoint_Proxy( 
    IRemoteDebugApplicationEvents * This,
    /* [in] */ IRemoteDebugApplicationThread *prdat);


void __RPC_STUB IRemoteDebugApplicationEvents_OnEnterBreakPoint_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IRemoteDebugApplicationEvents_OnLeaveBreakPoint_Proxy( 
    IRemoteDebugApplicationEvents * This,
    /* [in] */ IRemoteDebugApplicationThread *prdat);


void __RPC_STUB IRemoteDebugApplicationEvents_OnLeaveBreakPoint_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IRemoteDebugApplicationEvents_OnCreateThread_Proxy( 
    IRemoteDebugApplicationEvents * This,
    /* [in] */ IRemoteDebugApplicationThread *prdat);


void __RPC_STUB IRemoteDebugApplicationEvents_OnCreateThread_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IRemoteDebugApplicationEvents_OnDestroyThread_Proxy( 
    IRemoteDebugApplicationEvents * This,
    /* [in] */ IRemoteDebugApplicationThread *prdat);


void __RPC_STUB IRemoteDebugApplicationEvents_OnDestroyThread_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IRemoteDebugApplicationEvents_OnBreakFlagChange_Proxy( 
    IRemoteDebugApplicationEvents * This,
    /* [in] */ APPBREAKFLAGS abf,
    /* [in] */ IRemoteDebugApplicationThread *prdatSteppingThread);


void __RPC_STUB IRemoteDebugApplicationEvents_OnBreakFlagChange_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IRemoteDebugApplicationEvents_INTERFACE_DEFINED__ */


#ifndef __IDebugCookie_INTERFACE_DEFINED__
#define __IDebugCookie_INTERFACE_DEFINED__

/* interface IDebugCookie */
/* [unique][helpstring][uuid][local][object] */ 


EXTERN_C const IID IID_IDebugCookie;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("51973C39-CB0C-11d0-B5C9-00A0244A0E7A")
    IDebugCookie : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE SetDebugCookie( 
            /* [in] */ DWORD dwDebugAppCookie) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDebugCookieVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDebugCookie * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDebugCookie * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDebugCookie * This);
        
        HRESULT ( STDMETHODCALLTYPE *SetDebugCookie )( 
            IDebugCookie * This,
            /* [in] */ DWORD dwDebugAppCookie);
        
        END_INTERFACE
    } IDebugCookieVtbl;

    interface IDebugCookie
    {
        CONST_VTBL struct IDebugCookieVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDebugCookie_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDebugCookie_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDebugCookie_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDebugCookie_SetDebugCookie(This,dwDebugAppCookie)	\
    (This)->lpVtbl -> SetDebugCookie(This,dwDebugAppCookie)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IDebugCookie_SetDebugCookie_Proxy( 
    IDebugCookie * This,
    /* [in] */ DWORD dwDebugAppCookie);


void __RPC_STUB IDebugCookie_SetDebugCookie_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDebugCookie_INTERFACE_DEFINED__ */


#ifndef __ISimpleConnectionPoint_INTERFACE_DEFINED__
#define __ISimpleConnectionPoint_INTERFACE_DEFINED__

/* interface ISimpleConnectionPoint */
/* [local][unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_ISimpleConnectionPoint;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("51973C3e-CB0C-11d0-B5C9-00A0244A0E7A")
    ISimpleConnectionPoint : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetEventCount( 
            /* [out] */ ULONG *pulCount) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DescribeEvents( 
            /* [in] */ ULONG iEvent,
            /* [in] */ ULONG cEvents,
            /* [length_is][size_is][out] */ DISPID *prgid,
            /* [length_is][size_is][out] */ BSTR *prgbstr,
            /* [out] */ ULONG *pcEventsFetched) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Advise( 
            /* [in] */ IDispatch *pdisp,
            /* [out] */ DWORD *pdwCookie) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Unadvise( 
            /* [in] */ DWORD dwCookie) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISimpleConnectionPointVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISimpleConnectionPoint * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISimpleConnectionPoint * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISimpleConnectionPoint * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetEventCount )( 
            ISimpleConnectionPoint * This,
            /* [out] */ ULONG *pulCount);
        
        HRESULT ( STDMETHODCALLTYPE *DescribeEvents )( 
            ISimpleConnectionPoint * This,
            /* [in] */ ULONG iEvent,
            /* [in] */ ULONG cEvents,
            /* [length_is][size_is][out] */ DISPID *prgid,
            /* [length_is][size_is][out] */ BSTR *prgbstr,
            /* [out] */ ULONG *pcEventsFetched);
        
        HRESULT ( STDMETHODCALLTYPE *Advise )( 
            ISimpleConnectionPoint * This,
            /* [in] */ IDispatch *pdisp,
            /* [out] */ DWORD *pdwCookie);
        
        HRESULT ( STDMETHODCALLTYPE *Unadvise )( 
            ISimpleConnectionPoint * This,
            /* [in] */ DWORD dwCookie);
        
        END_INTERFACE
    } ISimpleConnectionPointVtbl;

    interface ISimpleConnectionPoint
    {
        CONST_VTBL struct ISimpleConnectionPointVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISimpleConnectionPoint_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISimpleConnectionPoint_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISimpleConnectionPoint_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISimpleConnectionPoint_GetEventCount(This,pulCount)	\
    (This)->lpVtbl -> GetEventCount(This,pulCount)

#define ISimpleConnectionPoint_DescribeEvents(This,iEvent,cEvents,prgid,prgbstr,pcEventsFetched)	\
    (This)->lpVtbl -> DescribeEvents(This,iEvent,cEvents,prgid,prgbstr,pcEventsFetched)

#define ISimpleConnectionPoint_Advise(This,pdisp,pdwCookie)	\
    (This)->lpVtbl -> Advise(This,pdisp,pdwCookie)

#define ISimpleConnectionPoint_Unadvise(This,dwCookie)	\
    (This)->lpVtbl -> Unadvise(This,dwCookie)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE ISimpleConnectionPoint_GetEventCount_Proxy( 
    ISimpleConnectionPoint * This,
    /* [out] */ ULONG *pulCount);


void __RPC_STUB ISimpleConnectionPoint_GetEventCount_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ISimpleConnectionPoint_DescribeEvents_Proxy( 
    ISimpleConnectionPoint * This,
    /* [in] */ ULONG iEvent,
    /* [in] */ ULONG cEvents,
    /* [length_is][size_is][out] */ DISPID *prgid,
    /* [length_is][size_is][out] */ BSTR *prgbstr,
    /* [out] */ ULONG *pcEventsFetched);


void __RPC_STUB ISimpleConnectionPoint_DescribeEvents_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ISimpleConnectionPoint_Advise_Proxy( 
    ISimpleConnectionPoint * This,
    /* [in] */ IDispatch *pdisp,
    /* [out] */ DWORD *pdwCookie);


void __RPC_STUB ISimpleConnectionPoint_Advise_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ISimpleConnectionPoint_Unadvise_Proxy( 
    ISimpleConnectionPoint * This,
    /* [in] */ DWORD dwCookie);


void __RPC_STUB ISimpleConnectionPoint_Unadvise_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISimpleConnectionPoint_INTERFACE_DEFINED__ */


#ifndef __IDebugHelper_INTERFACE_DEFINED__
#define __IDebugHelper_INTERFACE_DEFINED__

/* interface IDebugHelper */
/* [local][unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IDebugHelper;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("51973C3f-CB0C-11d0-B5C9-00A0244A0E7A")
    IDebugHelper : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE CreatePropertyBrowser( 
            /* [in] */ VARIANT *pvar,
            /* [in] */ LPCOLESTR bstrName,
            /* [in] */ IDebugApplicationThread *pdat,
            /* [out] */ IDebugProperty **ppdob) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreatePropertyBrowserEx( 
            /* [in] */ VARIANT *pvar,
            /* [in] */ LPCOLESTR bstrName,
            /* [in] */ IDebugApplicationThread *pdat,
            /* [in] */ IDebugFormatter *pdf,
            /* [out] */ IDebugProperty **ppdob) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE CreateSimpleConnectionPoint( 
            /* [in] */ IDispatch *pdisp,
            /* [out] */ ISimpleConnectionPoint **ppscp) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDebugHelperVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDebugHelper * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDebugHelper * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDebugHelper * This);
        
        HRESULT ( STDMETHODCALLTYPE *CreatePropertyBrowser )( 
            IDebugHelper * This,
            /* [in] */ VARIANT *pvar,
            /* [in] */ LPCOLESTR bstrName,
            /* [in] */ IDebugApplicationThread *pdat,
            /* [out] */ IDebugProperty **ppdob);
        
        HRESULT ( STDMETHODCALLTYPE *CreatePropertyBrowserEx )( 
            IDebugHelper * This,
            /* [in] */ VARIANT *pvar,
            /* [in] */ LPCOLESTR bstrName,
            /* [in] */ IDebugApplicationThread *pdat,
            /* [in] */ IDebugFormatter *pdf,
            /* [out] */ IDebugProperty **ppdob);
        
        HRESULT ( STDMETHODCALLTYPE *CreateSimpleConnectionPoint )( 
            IDebugHelper * This,
            /* [in] */ IDispatch *pdisp,
            /* [out] */ ISimpleConnectionPoint **ppscp);
        
        END_INTERFACE
    } IDebugHelperVtbl;

    interface IDebugHelper
    {
        CONST_VTBL struct IDebugHelperVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDebugHelper_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDebugHelper_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDebugHelper_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDebugHelper_CreatePropertyBrowser(This,pvar,bstrName,pdat,ppdob)	\
    (This)->lpVtbl -> CreatePropertyBrowser(This,pvar,bstrName,pdat,ppdob)

#define IDebugHelper_CreatePropertyBrowserEx(This,pvar,bstrName,pdat,pdf,ppdob)	\
    (This)->lpVtbl -> CreatePropertyBrowserEx(This,pvar,bstrName,pdat,pdf,ppdob)

#define IDebugHelper_CreateSimpleConnectionPoint(This,pdisp,ppscp)	\
    (This)->lpVtbl -> CreateSimpleConnectionPoint(This,pdisp,ppscp)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IDebugHelper_CreatePropertyBrowser_Proxy( 
    IDebugHelper * This,
    /* [in] */ VARIANT *pvar,
    /* [in] */ LPCOLESTR bstrName,
    /* [in] */ IDebugApplicationThread *pdat,
    /* [out] */ IDebugProperty **ppdob);


void __RPC_STUB IDebugHelper_CreatePropertyBrowser_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugHelper_CreatePropertyBrowserEx_Proxy( 
    IDebugHelper * This,
    /* [in] */ VARIANT *pvar,
    /* [in] */ LPCOLESTR bstrName,
    /* [in] */ IDebugApplicationThread *pdat,
    /* [in] */ IDebugFormatter *pdf,
    /* [out] */ IDebugProperty **ppdob);


void __RPC_STUB IDebugHelper_CreatePropertyBrowserEx_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebugHelper_CreateSimpleConnectionPoint_Proxy( 
    IDebugHelper * This,
    /* [in] */ IDispatch *pdisp,
    /* [out] */ ISimpleConnectionPoint **ppscp);


void __RPC_STUB IDebugHelper_CreateSimpleConnectionPoint_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDebugHelper_INTERFACE_DEFINED__ */

#endif /* __ActivDbg_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


