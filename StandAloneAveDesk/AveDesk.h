

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0366 */
/* at Tue Jan 02 22:35:15 2007
 */
/* Compiler settings for .\AveDesk.idl:
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


#ifndef __AveDesk_h__
#define __AveDesk_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IDesklet_FWD_DEFINED__
#define __IDesklet_FWD_DEFINED__
typedef interface IDesklet IDesklet;
#endif 	/* __IDesklet_FWD_DEFINED__ */


#ifndef __IDesklets_FWD_DEFINED__
#define __IDesklets_FWD_DEFINED__
typedef interface IDesklets IDesklets;
#endif 	/* __IDesklets_FWD_DEFINED__ */


#ifndef __IApplication_FWD_DEFINED__
#define __IApplication_FWD_DEFINED__
typedef interface IApplication IApplication;
#endif 	/* __IApplication_FWD_DEFINED__ */


#ifndef __IPreferences_FWD_DEFINED__
#define __IPreferences_FWD_DEFINED__
typedef interface IPreferences IPreferences;
#endif 	/* __IPreferences_FWD_DEFINED__ */


#ifndef __IPreference_FWD_DEFINED__
#define __IPreference_FWD_DEFINED__
typedef interface IPreference IPreference;
#endif 	/* __IPreference_FWD_DEFINED__ */


#ifndef __IColorMatrix_FWD_DEFINED__
#define __IColorMatrix_FWD_DEFINED__
typedef interface IColorMatrix IColorMatrix;
#endif 	/* __IColorMatrix_FWD_DEFINED__ */


#ifndef __IMatrix_FWD_DEFINED__
#define __IMatrix_FWD_DEFINED__
typedef interface IMatrix IMatrix;
#endif 	/* __IMatrix_FWD_DEFINED__ */


#ifndef __ITimer_FWD_DEFINED__
#define __ITimer_FWD_DEFINED__
typedef interface ITimer ITimer;
#endif 	/* __ITimer_FWD_DEFINED__ */


#ifndef __IDebugHelperClient_FWD_DEFINED__
#define __IDebugHelperClient_FWD_DEFINED__
typedef interface IDebugHelperClient IDebugHelperClient;
#endif 	/* __IDebugHelperClient_FWD_DEFINED__ */


#ifndef __IDebugHelperServer_FWD_DEFINED__
#define __IDebugHelperServer_FWD_DEFINED__
typedef interface IDebugHelperServer IDebugHelperServer;
#endif 	/* __IDebugHelperServer_FWD_DEFINED__ */


#ifndef __IAveImage_FWD_DEFINED__
#define __IAveImage_FWD_DEFINED__
typedef interface IAveImage IAveImage;
#endif 	/* __IAveImage_FWD_DEFINED__ */


#ifndef __IAveImages_FWD_DEFINED__
#define __IAveImages_FWD_DEFINED__
typedef interface IAveImages IAveImages;
#endif 	/* __IAveImages_FWD_DEFINED__ */


#ifndef __IDeskletInfo_FWD_DEFINED__
#define __IDeskletInfo_FWD_DEFINED__
typedef interface IDeskletInfo IDeskletInfo;
#endif 	/* __IDeskletInfo_FWD_DEFINED__ */


#ifndef __IDeskletInfos_FWD_DEFINED__
#define __IDeskletInfos_FWD_DEFINED__
typedef interface IDeskletInfos IDeskletInfos;
#endif 	/* __IDeskletInfos_FWD_DEFINED__ */


#ifndef __ILayer_FWD_DEFINED__
#define __ILayer_FWD_DEFINED__
typedef interface ILayer ILayer;
#endif 	/* __ILayer_FWD_DEFINED__ */


#ifndef __ILayers_FWD_DEFINED__
#define __ILayers_FWD_DEFINED__
typedef interface ILayers ILayers;
#endif 	/* __ILayers_FWD_DEFINED__ */


#ifndef __IRenderCanvas_FWD_DEFINED__
#define __IRenderCanvas_FWD_DEFINED__
typedef interface IRenderCanvas IRenderCanvas;
#endif 	/* __IRenderCanvas_FWD_DEFINED__ */


#ifndef __IShowCase_FWD_DEFINED__
#define __IShowCase_FWD_DEFINED__
typedef interface IShowCase IShowCase;
#endif 	/* __IShowCase_FWD_DEFINED__ */


#ifndef __IDeskletSkin_FWD_DEFINED__
#define __IDeskletSkin_FWD_DEFINED__
typedef interface IDeskletSkin IDeskletSkin;
#endif 	/* __IDeskletSkin_FWD_DEFINED__ */


#ifndef __IDeskletSkins_FWD_DEFINED__
#define __IDeskletSkins_FWD_DEFINED__
typedef interface IDeskletSkins IDeskletSkins;
#endif 	/* __IDeskletSkins_FWD_DEFINED__ */


#ifndef ___IDeskletEvents_FWD_DEFINED__
#define ___IDeskletEvents_FWD_DEFINED__
typedef interface _IDeskletEvents _IDeskletEvents;
#endif 	/* ___IDeskletEvents_FWD_DEFINED__ */


#ifndef ___IDeskletsEvents_FWD_DEFINED__
#define ___IDeskletsEvents_FWD_DEFINED__
typedef interface _IDeskletsEvents _IDeskletsEvents;
#endif 	/* ___IDeskletsEvents_FWD_DEFINED__ */


#ifndef __IEffectLet_FWD_DEFINED__
#define __IEffectLet_FWD_DEFINED__
typedef interface IEffectLet IEffectLet;
#endif 	/* __IEffectLet_FWD_DEFINED__ */


#ifndef __IDebugConsole_FWD_DEFINED__
#define __IDebugConsole_FWD_DEFINED__
typedef interface IDebugConsole IDebugConsole;
#endif 	/* __IDebugConsole_FWD_DEFINED__ */


#ifndef __ISkinEnumerator_FWD_DEFINED__
#define __ISkinEnumerator_FWD_DEFINED__
typedef interface ISkinEnumerator ISkinEnumerator;
#endif 	/* __ISkinEnumerator_FWD_DEFINED__ */


#ifndef __IAveDeskletFile_FWD_DEFINED__
#define __IAveDeskletFile_FWD_DEFINED__
typedef interface IAveDeskletFile IAveDeskletFile;
#endif 	/* __IAveDeskletFile_FWD_DEFINED__ */


#ifndef __IAveThemeFile_FWD_DEFINED__
#define __IAveThemeFile_FWD_DEFINED__
typedef interface IAveThemeFile IAveThemeFile;
#endif 	/* __IAveThemeFile_FWD_DEFINED__ */


#ifndef __IAveInstallFile_FWD_DEFINED__
#define __IAveInstallFile_FWD_DEFINED__
typedef interface IAveInstallFile IAveInstallFile;
#endif 	/* __IAveInstallFile_FWD_DEFINED__ */


#ifndef __Application_FWD_DEFINED__
#define __Application_FWD_DEFINED__

#ifdef __cplusplus
typedef class Application Application;
#else
typedef struct Application Application;
#endif /* __cplusplus */

#endif 	/* __Application_FWD_DEFINED__ */


#ifndef __Desklets_FWD_DEFINED__
#define __Desklets_FWD_DEFINED__

#ifdef __cplusplus
typedef class Desklets Desklets;
#else
typedef struct Desklets Desklets;
#endif /* __cplusplus */

#endif 	/* __Desklets_FWD_DEFINED__ */


#ifndef __Desklet_FWD_DEFINED__
#define __Desklet_FWD_DEFINED__

#ifdef __cplusplus
typedef class Desklet Desklet;
#else
typedef struct Desklet Desklet;
#endif /* __cplusplus */

#endif 	/* __Desklet_FWD_DEFINED__ */


#ifndef __EffectLet_FWD_DEFINED__
#define __EffectLet_FWD_DEFINED__

#ifdef __cplusplus
typedef class EffectLet EffectLet;
#else
typedef struct EffectLet EffectLet;
#endif /* __cplusplus */

#endif 	/* __EffectLet_FWD_DEFINED__ */


#ifndef __Preferences_FWD_DEFINED__
#define __Preferences_FWD_DEFINED__

#ifdef __cplusplus
typedef class Preferences Preferences;
#else
typedef struct Preferences Preferences;
#endif /* __cplusplus */

#endif 	/* __Preferences_FWD_DEFINED__ */


#ifndef __Preference_FWD_DEFINED__
#define __Preference_FWD_DEFINED__

#ifdef __cplusplus
typedef class Preference Preference;
#else
typedef struct Preference Preference;
#endif /* __cplusplus */

#endif 	/* __Preference_FWD_DEFINED__ */


#ifndef __DebugHelperClient_FWD_DEFINED__
#define __DebugHelperClient_FWD_DEFINED__

#ifdef __cplusplus
typedef class DebugHelperClient DebugHelperClient;
#else
typedef struct DebugHelperClient DebugHelperClient;
#endif /* __cplusplus */

#endif 	/* __DebugHelperClient_FWD_DEFINED__ */


#ifndef __DebugHelperServer_FWD_DEFINED__
#define __DebugHelperServer_FWD_DEFINED__

#ifdef __cplusplus
typedef class DebugHelperServer DebugHelperServer;
#else
typedef struct DebugHelperServer DebugHelperServer;
#endif /* __cplusplus */

#endif 	/* __DebugHelperServer_FWD_DEFINED__ */


#ifndef __DebugConsole_FWD_DEFINED__
#define __DebugConsole_FWD_DEFINED__

#ifdef __cplusplus
typedef class DebugConsole DebugConsole;
#else
typedef struct DebugConsole DebugConsole;
#endif /* __cplusplus */

#endif 	/* __DebugConsole_FWD_DEFINED__ */


#ifndef __ADColorMatrix_FWD_DEFINED__
#define __ADColorMatrix_FWD_DEFINED__

#ifdef __cplusplus
typedef class ADColorMatrix ADColorMatrix;
#else
typedef struct ADColorMatrix ADColorMatrix;
#endif /* __cplusplus */

#endif 	/* __ADColorMatrix_FWD_DEFINED__ */


#ifndef __ADMatrix_FWD_DEFINED__
#define __ADMatrix_FWD_DEFINED__

#ifdef __cplusplus
typedef class ADMatrix ADMatrix;
#else
typedef struct ADMatrix ADMatrix;
#endif /* __cplusplus */

#endif 	/* __ADMatrix_FWD_DEFINED__ */


#ifndef __Timer_FWD_DEFINED__
#define __Timer_FWD_DEFINED__

#ifdef __cplusplus
typedef class Timer Timer;
#else
typedef struct Timer Timer;
#endif /* __cplusplus */

#endif 	/* __Timer_FWD_DEFINED__ */


#ifndef __AveImage_FWD_DEFINED__
#define __AveImage_FWD_DEFINED__

#ifdef __cplusplus
typedef class AveImage AveImage;
#else
typedef struct AveImage AveImage;
#endif /* __cplusplus */

#endif 	/* __AveImage_FWD_DEFINED__ */


#ifndef __AveImages_FWD_DEFINED__
#define __AveImages_FWD_DEFINED__

#ifdef __cplusplus
typedef class AveImages AveImages;
#else
typedef struct AveImages AveImages;
#endif /* __cplusplus */

#endif 	/* __AveImages_FWD_DEFINED__ */


#ifndef __DeskletInfo_FWD_DEFINED__
#define __DeskletInfo_FWD_DEFINED__

#ifdef __cplusplus
typedef class DeskletInfo DeskletInfo;
#else
typedef struct DeskletInfo DeskletInfo;
#endif /* __cplusplus */

#endif 	/* __DeskletInfo_FWD_DEFINED__ */


#ifndef __DeskletInfos_FWD_DEFINED__
#define __DeskletInfos_FWD_DEFINED__

#ifdef __cplusplus
typedef class DeskletInfos DeskletInfos;
#else
typedef struct DeskletInfos DeskletInfos;
#endif /* __cplusplus */

#endif 	/* __DeskletInfos_FWD_DEFINED__ */


#ifndef __oLayer_FWD_DEFINED__
#define __oLayer_FWD_DEFINED__

#ifdef __cplusplus
typedef class oLayer oLayer;
#else
typedef struct oLayer oLayer;
#endif /* __cplusplus */

#endif 	/* __oLayer_FWD_DEFINED__ */


#ifndef __Layers_FWD_DEFINED__
#define __Layers_FWD_DEFINED__

#ifdef __cplusplus
typedef class Layers Layers;
#else
typedef struct Layers Layers;
#endif /* __cplusplus */

#endif 	/* __Layers_FWD_DEFINED__ */


#ifndef __ShowCase_FWD_DEFINED__
#define __ShowCase_FWD_DEFINED__

#ifdef __cplusplus
typedef class ShowCase ShowCase;
#else
typedef struct ShowCase ShowCase;
#endif /* __cplusplus */

#endif 	/* __ShowCase_FWD_DEFINED__ */


#ifndef __SkinEnumerator_FWD_DEFINED__
#define __SkinEnumerator_FWD_DEFINED__

#ifdef __cplusplus
typedef class SkinEnumerator SkinEnumerator;
#else
typedef struct SkinEnumerator SkinEnumerator;
#endif /* __cplusplus */

#endif 	/* __SkinEnumerator_FWD_DEFINED__ */


#ifndef __AveDeskletFile_FWD_DEFINED__
#define __AveDeskletFile_FWD_DEFINED__

#ifdef __cplusplus
typedef class AveDeskletFile AveDeskletFile;
#else
typedef struct AveDeskletFile AveDeskletFile;
#endif /* __cplusplus */

#endif 	/* __AveDeskletFile_FWD_DEFINED__ */


#ifndef __AveThemeFile_FWD_DEFINED__
#define __AveThemeFile_FWD_DEFINED__

#ifdef __cplusplus
typedef class AveThemeFile AveThemeFile;
#else
typedef struct AveThemeFile AveThemeFile;
#endif /* __cplusplus */

#endif 	/* __AveThemeFile_FWD_DEFINED__ */


#ifndef __AveInstallFile_FWD_DEFINED__
#define __AveInstallFile_FWD_DEFINED__

#ifdef __cplusplus
typedef class AveInstallFile AveInstallFile;
#else
typedef struct AveInstallFile AveInstallFile;
#endif /* __cplusplus */

#endif 	/* __AveInstallFile_FWD_DEFINED__ */


#ifndef __DeskletSkin_FWD_DEFINED__
#define __DeskletSkin_FWD_DEFINED__

#ifdef __cplusplus
typedef class DeskletSkin DeskletSkin;
#else
typedef struct DeskletSkin DeskletSkin;
#endif /* __cplusplus */

#endif 	/* __DeskletSkin_FWD_DEFINED__ */


#ifndef __DeskletSkins_FWD_DEFINED__
#define __DeskletSkins_FWD_DEFINED__

#ifdef __cplusplus
typedef class DeskletSkins DeskletSkins;
#else
typedef struct DeskletSkins DeskletSkins;
#endif /* __cplusplus */

#endif 	/* __DeskletSkins_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 


#ifndef __AveDeskLib_LIBRARY_DEFINED__
#define __AveDeskLib_LIBRARY_DEFINED__

/* library AveDeskLib */
/* [helpstring][version][uuid] */ 





















typedef /* [uuid] */  DECLSPEC_UUID("BB5D3FA0-53D8-48ad-8829-BF56C4BE58D0") 
enum DeskletStyles
    {	styleAveNormal	= 0,
	styleBottom	= 1,
	styleTop	= 2,
	styleIcon	= 3,
	styleShowCase	= 4
    } 	DeskletStyles;


EXTERN_C const IID LIBID_AveDeskLib;

#ifndef __IDesklet_INTERFACE_DEFINED__
#define __IDesklet_INTERFACE_DEFINED__

/* interface IDesklet */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IDesklet;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("33E08302-3F13-40B5-9134-DEEAABE6B6AD")
    IDesklet : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Label( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Label( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_SubLabel( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_SubLabel( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Application( 
            /* [retval][out] */ IApplication **pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Redraw( void) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_ColorMatrix( 
            /* [retval][out] */ IColorMatrix **pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_PositionMatrix( 
            /* [retval][out] */ IMatrix **pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Visible( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Visible( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Alpha( 
            /* [retval][out] */ int *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Alpha( 
            /* [in] */ int newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Rotation( 
            /* [retval][out] */ int *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Rotation( 
            /* [in] */ int newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Left( 
            /* [retval][out] */ int *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Left( 
            /* [in] */ int newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Top( 
            /* [retval][out] */ int *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Top( 
            /* [in] */ int newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Width( 
            /* [retval][out] */ int *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Width( 
            /* [in] */ int newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Height( 
            /* [retval][out] */ int *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Height( 
            /* [in] */ int newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Move( 
            /* [in] */ int Left,
            /* [optional][in] */ VARIANT *Top,
            /* [optional][in] */ VARIANT *Width,
            /* [optional][in] */ VARIANT *Height) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_About( 
            /* [retval][out] */ IDeskletInfo **pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Layers( 
            /* [retval][out] */ ILayers **pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE PopFront( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Clone( 
            /* [retval][out] */ IDesklet **pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Style( 
            /* [retval][out] */ DeskletStyles *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Style( 
            /* [in] */ DeskletStyles newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_HWnd( 
            /* [retval][out] */ int *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ObjectModel( 
            /* [retval][out] */ IDispatch **pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Tag( 
            /* [retval][out] */ VARIANT *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Tag( 
            /* [in] */ VARIANT newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FadeIn( 
            /* [in] */ VARIANT *async,
            /* [defaultvalue][in] */ long durationInMs = -1) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE FadeOut( 
            /* [in] */ VARIANT *async,
            /* [defaultvalue][in] */ long durationInMs = -1) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Skin( 
            /* [retval][out] */ IDeskletSkin **pVal) = 0;
        
        virtual /* [helpstring][id][propputref] */ HRESULT STDMETHODCALLTYPE putref_Skin( 
            /* [in] */ IDeskletSkin *newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDeskletVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDesklet * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDesklet * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDesklet * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDesklet * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDesklet * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDesklet * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDesklet * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Label )( 
            IDesklet * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Label )( 
            IDesklet * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_SubLabel )( 
            IDesklet * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_SubLabel )( 
            IDesklet * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Application )( 
            IDesklet * This,
            /* [retval][out] */ IApplication **pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Redraw )( 
            IDesklet * This);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ColorMatrix )( 
            IDesklet * This,
            /* [retval][out] */ IColorMatrix **pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_PositionMatrix )( 
            IDesklet * This,
            /* [retval][out] */ IMatrix **pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Visible )( 
            IDesklet * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Visible )( 
            IDesklet * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Alpha )( 
            IDesklet * This,
            /* [retval][out] */ int *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Alpha )( 
            IDesklet * This,
            /* [in] */ int newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Rotation )( 
            IDesklet * This,
            /* [retval][out] */ int *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Rotation )( 
            IDesklet * This,
            /* [in] */ int newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Left )( 
            IDesklet * This,
            /* [retval][out] */ int *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Left )( 
            IDesklet * This,
            /* [in] */ int newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Top )( 
            IDesklet * This,
            /* [retval][out] */ int *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Top )( 
            IDesklet * This,
            /* [in] */ int newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Width )( 
            IDesklet * This,
            /* [retval][out] */ int *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Width )( 
            IDesklet * This,
            /* [in] */ int newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Height )( 
            IDesklet * This,
            /* [retval][out] */ int *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Height )( 
            IDesklet * This,
            /* [in] */ int newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Move )( 
            IDesklet * This,
            /* [in] */ int Left,
            /* [optional][in] */ VARIANT *Top,
            /* [optional][in] */ VARIANT *Width,
            /* [optional][in] */ VARIANT *Height);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_About )( 
            IDesklet * This,
            /* [retval][out] */ IDeskletInfo **pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Layers )( 
            IDesklet * This,
            /* [retval][out] */ ILayers **pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *PopFront )( 
            IDesklet * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Clone )( 
            IDesklet * This,
            /* [retval][out] */ IDesklet **pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Style )( 
            IDesklet * This,
            /* [retval][out] */ DeskletStyles *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Style )( 
            IDesklet * This,
            /* [in] */ DeskletStyles newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_HWnd )( 
            IDesklet * This,
            /* [retval][out] */ int *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ObjectModel )( 
            IDesklet * This,
            /* [retval][out] */ IDispatch **pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Tag )( 
            IDesklet * This,
            /* [retval][out] */ VARIANT *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Tag )( 
            IDesklet * This,
            /* [in] */ VARIANT newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *FadeIn )( 
            IDesklet * This,
            /* [in] */ VARIANT *async,
            /* [defaultvalue][in] */ long durationInMs);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *FadeOut )( 
            IDesklet * This,
            /* [in] */ VARIANT *async,
            /* [defaultvalue][in] */ long durationInMs);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Skin )( 
            IDesklet * This,
            /* [retval][out] */ IDeskletSkin **pVal);
        
        /* [helpstring][id][propputref] */ HRESULT ( STDMETHODCALLTYPE *putref_Skin )( 
            IDesklet * This,
            /* [in] */ IDeskletSkin *newVal);
        
        END_INTERFACE
    } IDeskletVtbl;

    interface IDesklet
    {
        CONST_VTBL struct IDeskletVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDesklet_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDesklet_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDesklet_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDesklet_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IDesklet_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IDesklet_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IDesklet_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IDesklet_get_Label(This,pVal)	\
    (This)->lpVtbl -> get_Label(This,pVal)

#define IDesklet_put_Label(This,newVal)	\
    (This)->lpVtbl -> put_Label(This,newVal)

#define IDesklet_get_SubLabel(This,pVal)	\
    (This)->lpVtbl -> get_SubLabel(This,pVal)

#define IDesklet_put_SubLabel(This,newVal)	\
    (This)->lpVtbl -> put_SubLabel(This,newVal)

#define IDesklet_get_Application(This,pVal)	\
    (This)->lpVtbl -> get_Application(This,pVal)

#define IDesklet_Redraw(This)	\
    (This)->lpVtbl -> Redraw(This)

#define IDesklet_get_ColorMatrix(This,pVal)	\
    (This)->lpVtbl -> get_ColorMatrix(This,pVal)

#define IDesklet_get_PositionMatrix(This,pVal)	\
    (This)->lpVtbl -> get_PositionMatrix(This,pVal)

#define IDesklet_get_Visible(This,pVal)	\
    (This)->lpVtbl -> get_Visible(This,pVal)

#define IDesklet_put_Visible(This,newVal)	\
    (This)->lpVtbl -> put_Visible(This,newVal)

#define IDesklet_get_Alpha(This,pVal)	\
    (This)->lpVtbl -> get_Alpha(This,pVal)

#define IDesklet_put_Alpha(This,newVal)	\
    (This)->lpVtbl -> put_Alpha(This,newVal)

#define IDesklet_get_Rotation(This,pVal)	\
    (This)->lpVtbl -> get_Rotation(This,pVal)

#define IDesklet_put_Rotation(This,newVal)	\
    (This)->lpVtbl -> put_Rotation(This,newVal)

#define IDesklet_get_Left(This,pVal)	\
    (This)->lpVtbl -> get_Left(This,pVal)

#define IDesklet_put_Left(This,newVal)	\
    (This)->lpVtbl -> put_Left(This,newVal)

#define IDesklet_get_Top(This,pVal)	\
    (This)->lpVtbl -> get_Top(This,pVal)

#define IDesklet_put_Top(This,newVal)	\
    (This)->lpVtbl -> put_Top(This,newVal)

#define IDesklet_get_Width(This,pVal)	\
    (This)->lpVtbl -> get_Width(This,pVal)

#define IDesklet_put_Width(This,newVal)	\
    (This)->lpVtbl -> put_Width(This,newVal)

#define IDesklet_get_Height(This,pVal)	\
    (This)->lpVtbl -> get_Height(This,pVal)

#define IDesklet_put_Height(This,newVal)	\
    (This)->lpVtbl -> put_Height(This,newVal)

#define IDesklet_Move(This,Left,Top,Width,Height)	\
    (This)->lpVtbl -> Move(This,Left,Top,Width,Height)

#define IDesklet_get_About(This,pVal)	\
    (This)->lpVtbl -> get_About(This,pVal)

#define IDesklet_get_Layers(This,pVal)	\
    (This)->lpVtbl -> get_Layers(This,pVal)

#define IDesklet_PopFront(This)	\
    (This)->lpVtbl -> PopFront(This)

#define IDesklet_Clone(This,pVal)	\
    (This)->lpVtbl -> Clone(This,pVal)

#define IDesklet_get_Style(This,pVal)	\
    (This)->lpVtbl -> get_Style(This,pVal)

#define IDesklet_put_Style(This,newVal)	\
    (This)->lpVtbl -> put_Style(This,newVal)

#define IDesklet_get_HWnd(This,pVal)	\
    (This)->lpVtbl -> get_HWnd(This,pVal)

#define IDesklet_get_ObjectModel(This,pVal)	\
    (This)->lpVtbl -> get_ObjectModel(This,pVal)

#define IDesklet_get_Tag(This,pVal)	\
    (This)->lpVtbl -> get_Tag(This,pVal)

#define IDesklet_put_Tag(This,newVal)	\
    (This)->lpVtbl -> put_Tag(This,newVal)

#define IDesklet_FadeIn(This,async,durationInMs)	\
    (This)->lpVtbl -> FadeIn(This,async,durationInMs)

#define IDesklet_FadeOut(This,async,durationInMs)	\
    (This)->lpVtbl -> FadeOut(This,async,durationInMs)

#define IDesklet_get_Skin(This,pVal)	\
    (This)->lpVtbl -> get_Skin(This,pVal)

#define IDesklet_putref_Skin(This,newVal)	\
    (This)->lpVtbl -> putref_Skin(This,newVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDesklet_get_Label_Proxy( 
    IDesklet * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IDesklet_get_Label_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IDesklet_put_Label_Proxy( 
    IDesklet * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IDesklet_put_Label_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDesklet_get_SubLabel_Proxy( 
    IDesklet * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IDesklet_get_SubLabel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IDesklet_put_SubLabel_Proxy( 
    IDesklet * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IDesklet_put_SubLabel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDesklet_get_Application_Proxy( 
    IDesklet * This,
    /* [retval][out] */ IApplication **pVal);


void __RPC_STUB IDesklet_get_Application_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDesklet_Redraw_Proxy( 
    IDesklet * This);


void __RPC_STUB IDesklet_Redraw_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE IDesklet_get_ColorMatrix_Proxy( 
    IDesklet * This,
    /* [retval][out] */ IColorMatrix **pVal);


void __RPC_STUB IDesklet_get_ColorMatrix_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDesklet_get_PositionMatrix_Proxy( 
    IDesklet * This,
    /* [retval][out] */ IMatrix **pVal);


void __RPC_STUB IDesklet_get_PositionMatrix_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDesklet_get_Visible_Proxy( 
    IDesklet * This,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IDesklet_get_Visible_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IDesklet_put_Visible_Proxy( 
    IDesklet * This,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB IDesklet_put_Visible_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDesklet_get_Alpha_Proxy( 
    IDesklet * This,
    /* [retval][out] */ int *pVal);


void __RPC_STUB IDesklet_get_Alpha_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IDesklet_put_Alpha_Proxy( 
    IDesklet * This,
    /* [in] */ int newVal);


void __RPC_STUB IDesklet_put_Alpha_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDesklet_get_Rotation_Proxy( 
    IDesklet * This,
    /* [retval][out] */ int *pVal);


void __RPC_STUB IDesklet_get_Rotation_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IDesklet_put_Rotation_Proxy( 
    IDesklet * This,
    /* [in] */ int newVal);


void __RPC_STUB IDesklet_put_Rotation_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDesklet_get_Left_Proxy( 
    IDesklet * This,
    /* [retval][out] */ int *pVal);


void __RPC_STUB IDesklet_get_Left_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IDesklet_put_Left_Proxy( 
    IDesklet * This,
    /* [in] */ int newVal);


void __RPC_STUB IDesklet_put_Left_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDesklet_get_Top_Proxy( 
    IDesklet * This,
    /* [retval][out] */ int *pVal);


void __RPC_STUB IDesklet_get_Top_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IDesklet_put_Top_Proxy( 
    IDesklet * This,
    /* [in] */ int newVal);


void __RPC_STUB IDesklet_put_Top_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDesklet_get_Width_Proxy( 
    IDesklet * This,
    /* [retval][out] */ int *pVal);


void __RPC_STUB IDesklet_get_Width_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IDesklet_put_Width_Proxy( 
    IDesklet * This,
    /* [in] */ int newVal);


void __RPC_STUB IDesklet_put_Width_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDesklet_get_Height_Proxy( 
    IDesklet * This,
    /* [retval][out] */ int *pVal);


void __RPC_STUB IDesklet_get_Height_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IDesklet_put_Height_Proxy( 
    IDesklet * This,
    /* [in] */ int newVal);


void __RPC_STUB IDesklet_put_Height_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDesklet_Move_Proxy( 
    IDesklet * This,
    /* [in] */ int Left,
    /* [optional][in] */ VARIANT *Top,
    /* [optional][in] */ VARIANT *Width,
    /* [optional][in] */ VARIANT *Height);


void __RPC_STUB IDesklet_Move_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDesklet_get_About_Proxy( 
    IDesklet * This,
    /* [retval][out] */ IDeskletInfo **pVal);


void __RPC_STUB IDesklet_get_About_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDesklet_get_Layers_Proxy( 
    IDesklet * This,
    /* [retval][out] */ ILayers **pVal);


void __RPC_STUB IDesklet_get_Layers_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDesklet_PopFront_Proxy( 
    IDesklet * This);


void __RPC_STUB IDesklet_PopFront_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDesklet_Clone_Proxy( 
    IDesklet * This,
    /* [retval][out] */ IDesklet **pVal);


void __RPC_STUB IDesklet_Clone_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDesklet_get_Style_Proxy( 
    IDesklet * This,
    /* [retval][out] */ DeskletStyles *pVal);


void __RPC_STUB IDesklet_get_Style_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IDesklet_put_Style_Proxy( 
    IDesklet * This,
    /* [in] */ DeskletStyles newVal);


void __RPC_STUB IDesklet_put_Style_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDesklet_get_HWnd_Proxy( 
    IDesklet * This,
    /* [retval][out] */ int *pVal);


void __RPC_STUB IDesklet_get_HWnd_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDesklet_get_ObjectModel_Proxy( 
    IDesklet * This,
    /* [retval][out] */ IDispatch **pVal);


void __RPC_STUB IDesklet_get_ObjectModel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDesklet_get_Tag_Proxy( 
    IDesklet * This,
    /* [retval][out] */ VARIANT *pVal);


void __RPC_STUB IDesklet_get_Tag_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IDesklet_put_Tag_Proxy( 
    IDesklet * This,
    /* [in] */ VARIANT newVal);


void __RPC_STUB IDesklet_put_Tag_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDesklet_FadeIn_Proxy( 
    IDesklet * This,
    /* [in] */ VARIANT *async,
    /* [defaultvalue][in] */ long durationInMs);


void __RPC_STUB IDesklet_FadeIn_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDesklet_FadeOut_Proxy( 
    IDesklet * This,
    /* [in] */ VARIANT *async,
    /* [defaultvalue][in] */ long durationInMs);


void __RPC_STUB IDesklet_FadeOut_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDesklet_get_Skin_Proxy( 
    IDesklet * This,
    /* [retval][out] */ IDeskletSkin **pVal);


void __RPC_STUB IDesklet_get_Skin_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propputref] */ HRESULT STDMETHODCALLTYPE IDesklet_putref_Skin_Proxy( 
    IDesklet * This,
    /* [in] */ IDeskletSkin *newVal);


void __RPC_STUB IDesklet_putref_Skin_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDesklet_INTERFACE_DEFINED__ */


#ifndef __IDesklets_INTERFACE_DEFINED__
#define __IDesklets_INTERFACE_DEFINED__

/* interface IDesklets */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IDesklets;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6C2E1D24-6115-43AD-B656-366DD643949E")
    IDesklets : public IDispatch
    {
    public:
        virtual /* [helpstring][restricted][propget][id] */ HRESULT STDMETHODCALLTYPE get__NewEnum( 
            /* [retval][out] */ IUnknown **pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Item( 
            /* [in] */ VARIANT *Index,
            /* [retval][out] */ IDesklet **pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Add( 
            /* [in] */ IDeskletInfo *What,
            /* [optional][in] */ VARIANT *Left,
            /* [optional][in] */ VARIANT *Top,
            /* [optional][in] */ VARIANT *Width,
            /* [optional][in] */ VARIANT *Height,
            /* [retval][out] */ IDesklet **pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddWithSkin( 
            /* [in] */ IDeskletInfo *What,
            /* [in] */ IDeskletSkin *Skin,
            /* [optional][in] */ VARIANT *Left,
            /* [optional][in] */ VARIANT *Top,
            /* [optional][in] */ VARIANT *Width,
            /* [optional][in] */ VARIANT *Height,
            /* [retval][out] */ IDesklet **pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDeskletsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDesklets * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDesklets * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDesklets * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDesklets * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDesklets * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDesklets * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDesklets * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][restricted][propget][id] */ HRESULT ( STDMETHODCALLTYPE *get__NewEnum )( 
            IDesklets * This,
            /* [retval][out] */ IUnknown **pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Item )( 
            IDesklets * This,
            /* [in] */ VARIANT *Index,
            /* [retval][out] */ IDesklet **pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            IDesklets * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Add )( 
            IDesklets * This,
            /* [in] */ IDeskletInfo *What,
            /* [optional][in] */ VARIANT *Left,
            /* [optional][in] */ VARIANT *Top,
            /* [optional][in] */ VARIANT *Width,
            /* [optional][in] */ VARIANT *Height,
            /* [retval][out] */ IDesklet **pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddWithSkin )( 
            IDesklets * This,
            /* [in] */ IDeskletInfo *What,
            /* [in] */ IDeskletSkin *Skin,
            /* [optional][in] */ VARIANT *Left,
            /* [optional][in] */ VARIANT *Top,
            /* [optional][in] */ VARIANT *Width,
            /* [optional][in] */ VARIANT *Height,
            /* [retval][out] */ IDesklet **pVal);
        
        END_INTERFACE
    } IDeskletsVtbl;

    interface IDesklets
    {
        CONST_VTBL struct IDeskletsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDesklets_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDesklets_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDesklets_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDesklets_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IDesklets_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IDesklets_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IDesklets_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IDesklets_get__NewEnum(This,pVal)	\
    (This)->lpVtbl -> get__NewEnum(This,pVal)

#define IDesklets_get_Item(This,Index,pVal)	\
    (This)->lpVtbl -> get_Item(This,Index,pVal)

#define IDesklets_get_Count(This,pVal)	\
    (This)->lpVtbl -> get_Count(This,pVal)

#define IDesklets_Add(This,What,Left,Top,Width,Height,pVal)	\
    (This)->lpVtbl -> Add(This,What,Left,Top,Width,Height,pVal)

#define IDesklets_AddWithSkin(This,What,Skin,Left,Top,Width,Height,pVal)	\
    (This)->lpVtbl -> AddWithSkin(This,What,Skin,Left,Top,Width,Height,pVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][restricted][propget][id] */ HRESULT STDMETHODCALLTYPE IDesklets_get__NewEnum_Proxy( 
    IDesklets * This,
    /* [retval][out] */ IUnknown **pVal);


void __RPC_STUB IDesklets_get__NewEnum_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDesklets_get_Item_Proxy( 
    IDesklets * This,
    /* [in] */ VARIANT *Index,
    /* [retval][out] */ IDesklet **pVal);


void __RPC_STUB IDesklets_get_Item_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDesklets_get_Count_Proxy( 
    IDesklets * This,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IDesklets_get_Count_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDesklets_Add_Proxy( 
    IDesklets * This,
    /* [in] */ IDeskletInfo *What,
    /* [optional][in] */ VARIANT *Left,
    /* [optional][in] */ VARIANT *Top,
    /* [optional][in] */ VARIANT *Width,
    /* [optional][in] */ VARIANT *Height,
    /* [retval][out] */ IDesklet **pVal);


void __RPC_STUB IDesklets_Add_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDesklets_AddWithSkin_Proxy( 
    IDesklets * This,
    /* [in] */ IDeskletInfo *What,
    /* [in] */ IDeskletSkin *Skin,
    /* [optional][in] */ VARIANT *Left,
    /* [optional][in] */ VARIANT *Top,
    /* [optional][in] */ VARIANT *Width,
    /* [optional][in] */ VARIANT *Height,
    /* [retval][out] */ IDesklet **pVal);


void __RPC_STUB IDesklets_AddWithSkin_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDesklets_INTERFACE_DEFINED__ */


#ifndef __IApplication_INTERFACE_DEFINED__
#define __IApplication_INTERFACE_DEFINED__

/* interface IApplication */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IApplication;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6DE0A391-1EDA-4D3D-BC53-F3EB538FF2ED")
    IApplication : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Close( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ShowControlPanel( void) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_RunningDesklets( 
            /* [retval][out] */ IDesklets **pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LoadImage( 
            /* [in] */ BSTR FileName,
            /* [retval][out] */ IAveImage **pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_AvailableDesklets( 
            /* [retval][out] */ IDeskletInfos **pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Sleep( 
            int ms) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SaveTheme( 
            /* [defaultvalue][in] */ BSTR Name = L"Theme.ini") = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LoadTheme( 
            /* [defaultvalue][in] */ BSTR Name = L"Theme.ini") = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ShowCase( 
            /* [retval][out] */ IShowCase **pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Version( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE OpenFile( 
            BSTR FileName) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ExePath( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ThemePath( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_UserPath( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Debug( 
            /* [retval][out] */ IDebugHelperClient **pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ShowThemeBrowserDialog( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SaveGlobalSettings( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ShowDeskletBrowserDialog( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IApplicationVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IApplication * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IApplication * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IApplication * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IApplication * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IApplication * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IApplication * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IApplication * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Close )( 
            IApplication * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ShowControlPanel )( 
            IApplication * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_RunningDesklets )( 
            IApplication * This,
            /* [retval][out] */ IDesklets **pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LoadImage )( 
            IApplication * This,
            /* [in] */ BSTR FileName,
            /* [retval][out] */ IAveImage **pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_AvailableDesklets )( 
            IApplication * This,
            /* [retval][out] */ IDeskletInfos **pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Sleep )( 
            IApplication * This,
            int ms);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SaveTheme )( 
            IApplication * This,
            /* [defaultvalue][in] */ BSTR Name);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LoadTheme )( 
            IApplication * This,
            /* [defaultvalue][in] */ BSTR Name);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ShowCase )( 
            IApplication * This,
            /* [retval][out] */ IShowCase **pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Version )( 
            IApplication * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *OpenFile )( 
            IApplication * This,
            BSTR FileName);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ExePath )( 
            IApplication * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ThemePath )( 
            IApplication * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_UserPath )( 
            IApplication * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Debug )( 
            IApplication * This,
            /* [retval][out] */ IDebugHelperClient **pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ShowThemeBrowserDialog )( 
            IApplication * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SaveGlobalSettings )( 
            IApplication * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ShowDeskletBrowserDialog )( 
            IApplication * This);
        
        END_INTERFACE
    } IApplicationVtbl;

    interface IApplication
    {
        CONST_VTBL struct IApplicationVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IApplication_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IApplication_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IApplication_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IApplication_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IApplication_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IApplication_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IApplication_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IApplication_Close(This)	\
    (This)->lpVtbl -> Close(This)

#define IApplication_ShowControlPanel(This)	\
    (This)->lpVtbl -> ShowControlPanel(This)

#define IApplication_get_RunningDesklets(This,pVal)	\
    (This)->lpVtbl -> get_RunningDesklets(This,pVal)

#define IApplication_LoadImage(This,FileName,pVal)	\
    (This)->lpVtbl -> LoadImage(This,FileName,pVal)

#define IApplication_get_AvailableDesklets(This,pVal)	\
    (This)->lpVtbl -> get_AvailableDesklets(This,pVal)

#define IApplication_Sleep(This,ms)	\
    (This)->lpVtbl -> Sleep(This,ms)

#define IApplication_SaveTheme(This,Name)	\
    (This)->lpVtbl -> SaveTheme(This,Name)

#define IApplication_LoadTheme(This,Name)	\
    (This)->lpVtbl -> LoadTheme(This,Name)

#define IApplication_get_ShowCase(This,pVal)	\
    (This)->lpVtbl -> get_ShowCase(This,pVal)

#define IApplication_get_Version(This,pVal)	\
    (This)->lpVtbl -> get_Version(This,pVal)

#define IApplication_OpenFile(This,FileName)	\
    (This)->lpVtbl -> OpenFile(This,FileName)

#define IApplication_get_ExePath(This,pVal)	\
    (This)->lpVtbl -> get_ExePath(This,pVal)

#define IApplication_get_ThemePath(This,pVal)	\
    (This)->lpVtbl -> get_ThemePath(This,pVal)

#define IApplication_get_UserPath(This,pVal)	\
    (This)->lpVtbl -> get_UserPath(This,pVal)

#define IApplication_get_Debug(This,pVal)	\
    (This)->lpVtbl -> get_Debug(This,pVal)

#define IApplication_ShowThemeBrowserDialog(This)	\
    (This)->lpVtbl -> ShowThemeBrowserDialog(This)

#define IApplication_SaveGlobalSettings(This)	\
    (This)->lpVtbl -> SaveGlobalSettings(This)

#define IApplication_ShowDeskletBrowserDialog(This)	\
    (This)->lpVtbl -> ShowDeskletBrowserDialog(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IApplication_Close_Proxy( 
    IApplication * This);


void __RPC_STUB IApplication_Close_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IApplication_ShowControlPanel_Proxy( 
    IApplication * This);


void __RPC_STUB IApplication_ShowControlPanel_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IApplication_get_RunningDesklets_Proxy( 
    IApplication * This,
    /* [retval][out] */ IDesklets **pVal);


void __RPC_STUB IApplication_get_RunningDesklets_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IApplication_LoadImage_Proxy( 
    IApplication * This,
    /* [in] */ BSTR FileName,
    /* [retval][out] */ IAveImage **pVal);


void __RPC_STUB IApplication_LoadImage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IApplication_get_AvailableDesklets_Proxy( 
    IApplication * This,
    /* [retval][out] */ IDeskletInfos **pVal);


void __RPC_STUB IApplication_get_AvailableDesklets_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IApplication_Sleep_Proxy( 
    IApplication * This,
    int ms);


void __RPC_STUB IApplication_Sleep_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IApplication_SaveTheme_Proxy( 
    IApplication * This,
    /* [defaultvalue][in] */ BSTR Name);


void __RPC_STUB IApplication_SaveTheme_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IApplication_LoadTheme_Proxy( 
    IApplication * This,
    /* [defaultvalue][in] */ BSTR Name);


void __RPC_STUB IApplication_LoadTheme_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IApplication_get_ShowCase_Proxy( 
    IApplication * This,
    /* [retval][out] */ IShowCase **pVal);


void __RPC_STUB IApplication_get_ShowCase_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IApplication_get_Version_Proxy( 
    IApplication * This,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IApplication_get_Version_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IApplication_OpenFile_Proxy( 
    IApplication * This,
    BSTR FileName);


void __RPC_STUB IApplication_OpenFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IApplication_get_ExePath_Proxy( 
    IApplication * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IApplication_get_ExePath_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IApplication_get_ThemePath_Proxy( 
    IApplication * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IApplication_get_ThemePath_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IApplication_get_UserPath_Proxy( 
    IApplication * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IApplication_get_UserPath_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IApplication_get_Debug_Proxy( 
    IApplication * This,
    /* [retval][out] */ IDebugHelperClient **pVal);


void __RPC_STUB IApplication_get_Debug_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IApplication_ShowThemeBrowserDialog_Proxy( 
    IApplication * This);


void __RPC_STUB IApplication_ShowThemeBrowserDialog_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IApplication_SaveGlobalSettings_Proxy( 
    IApplication * This);


void __RPC_STUB IApplication_SaveGlobalSettings_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IApplication_ShowDeskletBrowserDialog_Proxy( 
    IApplication * This);


void __RPC_STUB IApplication_ShowDeskletBrowserDialog_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IApplication_INTERFACE_DEFINED__ */


#ifndef __IPreferences_INTERFACE_DEFINED__
#define __IPreferences_INTERFACE_DEFINED__

/* interface IPreferences */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IPreferences;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("02C79C78-B5BC-427A-8AE6-0DE72EAAE54E")
    IPreferences : public IDispatch
    {
    public:
        virtual /* [restricted][helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get__NewEnum( 
            /* [retval][out] */ IUnknown **pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ReadAll( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE SaveAll( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Show( void) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_IniFile( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_IniFile( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_IniSection( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_IniSection( 
            /* [in] */ BSTR newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPreferencesVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IPreferences * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IPreferences * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IPreferences * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IPreferences * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IPreferences * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IPreferences * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IPreferences * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [restricted][helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get__NewEnum )( 
            IPreferences * This,
            /* [retval][out] */ IUnknown **pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ReadAll )( 
            IPreferences * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *SaveAll )( 
            IPreferences * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Show )( 
            IPreferences * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_IniFile )( 
            IPreferences * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_IniFile )( 
            IPreferences * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_IniSection )( 
            IPreferences * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_IniSection )( 
            IPreferences * This,
            /* [in] */ BSTR newVal);
        
        END_INTERFACE
    } IPreferencesVtbl;

    interface IPreferences
    {
        CONST_VTBL struct IPreferencesVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPreferences_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPreferences_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPreferences_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPreferences_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPreferences_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPreferences_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPreferences_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPreferences_get__NewEnum(This,pVal)	\
    (This)->lpVtbl -> get__NewEnum(This,pVal)

#define IPreferences_ReadAll(This)	\
    (This)->lpVtbl -> ReadAll(This)

#define IPreferences_SaveAll(This)	\
    (This)->lpVtbl -> SaveAll(This)

#define IPreferences_Show(This)	\
    (This)->lpVtbl -> Show(This)

#define IPreferences_get_IniFile(This,pVal)	\
    (This)->lpVtbl -> get_IniFile(This,pVal)

#define IPreferences_put_IniFile(This,newVal)	\
    (This)->lpVtbl -> put_IniFile(This,newVal)

#define IPreferences_get_IniSection(This,pVal)	\
    (This)->lpVtbl -> get_IniSection(This,pVal)

#define IPreferences_put_IniSection(This,newVal)	\
    (This)->lpVtbl -> put_IniSection(This,newVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [restricted][helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPreferences_get__NewEnum_Proxy( 
    IPreferences * This,
    /* [retval][out] */ IUnknown **pVal);


void __RPC_STUB IPreferences_get__NewEnum_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPreferences_ReadAll_Proxy( 
    IPreferences * This);


void __RPC_STUB IPreferences_ReadAll_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPreferences_SaveAll_Proxy( 
    IPreferences * This);


void __RPC_STUB IPreferences_SaveAll_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPreferences_Show_Proxy( 
    IPreferences * This);


void __RPC_STUB IPreferences_Show_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPreferences_get_IniFile_Proxy( 
    IPreferences * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IPreferences_get_IniFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPreferences_put_IniFile_Proxy( 
    IPreferences * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IPreferences_put_IniFile_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPreferences_get_IniSection_Proxy( 
    IPreferences * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IPreferences_get_IniSection_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPreferences_put_IniSection_Proxy( 
    IPreferences * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IPreferences_put_IniSection_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPreferences_INTERFACE_DEFINED__ */


#ifndef __IPreference_INTERFACE_DEFINED__
#define __IPreference_INTERFACE_DEFINED__

/* interface IPreference */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IPreference;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("80A161AB-0394-4101-B27A-0B55155E1929")
    IPreference : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Value( 
            /* [retval][out] */ VARIANT *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Value( 
            /* [in] */ VARIANT newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_DefVal( 
            /* [retval][out] */ VARIANT *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_DefVal( 
            /* [in] */ VARIANT newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Description( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Description( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Name( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Max( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Max( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Min( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Min( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Type( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Type( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Load( 
            BSTR IniFile,
            BSTR IniSection) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Save( 
            BSTR IniFile,
            BSTR IniSection) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IPreferenceVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IPreference * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IPreference * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IPreference * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IPreference * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IPreference * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IPreference * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IPreference * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Value )( 
            IPreference * This,
            /* [retval][out] */ VARIANT *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Value )( 
            IPreference * This,
            /* [in] */ VARIANT newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DefVal )( 
            IPreference * This,
            /* [retval][out] */ VARIANT *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_DefVal )( 
            IPreference * This,
            /* [in] */ VARIANT newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Description )( 
            IPreference * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Description )( 
            IPreference * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Name )( 
            IPreference * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Name )( 
            IPreference * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Max )( 
            IPreference * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Max )( 
            IPreference * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Min )( 
            IPreference * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Min )( 
            IPreference * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Type )( 
            IPreference * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Type )( 
            IPreference * This,
            /* [in] */ BSTR newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Load )( 
            IPreference * This,
            BSTR IniFile,
            BSTR IniSection);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Save )( 
            IPreference * This,
            BSTR IniFile,
            BSTR IniSection);
        
        END_INTERFACE
    } IPreferenceVtbl;

    interface IPreference
    {
        CONST_VTBL struct IPreferenceVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IPreference_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IPreference_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IPreference_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IPreference_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IPreference_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IPreference_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IPreference_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IPreference_get_Value(This,pVal)	\
    (This)->lpVtbl -> get_Value(This,pVal)

#define IPreference_put_Value(This,newVal)	\
    (This)->lpVtbl -> put_Value(This,newVal)

#define IPreference_get_DefVal(This,pVal)	\
    (This)->lpVtbl -> get_DefVal(This,pVal)

#define IPreference_put_DefVal(This,newVal)	\
    (This)->lpVtbl -> put_DefVal(This,newVal)

#define IPreference_get_Description(This,pVal)	\
    (This)->lpVtbl -> get_Description(This,pVal)

#define IPreference_put_Description(This,newVal)	\
    (This)->lpVtbl -> put_Description(This,newVal)

#define IPreference_get_Name(This,pVal)	\
    (This)->lpVtbl -> get_Name(This,pVal)

#define IPreference_put_Name(This,newVal)	\
    (This)->lpVtbl -> put_Name(This,newVal)

#define IPreference_get_Max(This,pVal)	\
    (This)->lpVtbl -> get_Max(This,pVal)

#define IPreference_put_Max(This,newVal)	\
    (This)->lpVtbl -> put_Max(This,newVal)

#define IPreference_get_Min(This,pVal)	\
    (This)->lpVtbl -> get_Min(This,pVal)

#define IPreference_put_Min(This,newVal)	\
    (This)->lpVtbl -> put_Min(This,newVal)

#define IPreference_get_Type(This,pVal)	\
    (This)->lpVtbl -> get_Type(This,pVal)

#define IPreference_put_Type(This,newVal)	\
    (This)->lpVtbl -> put_Type(This,newVal)

#define IPreference_Load(This,IniFile,IniSection)	\
    (This)->lpVtbl -> Load(This,IniFile,IniSection)

#define IPreference_Save(This,IniFile,IniSection)	\
    (This)->lpVtbl -> Save(This,IniFile,IniSection)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPreference_get_Value_Proxy( 
    IPreference * This,
    /* [retval][out] */ VARIANT *pVal);


void __RPC_STUB IPreference_get_Value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPreference_put_Value_Proxy( 
    IPreference * This,
    /* [in] */ VARIANT newVal);


void __RPC_STUB IPreference_put_Value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPreference_get_DefVal_Proxy( 
    IPreference * This,
    /* [retval][out] */ VARIANT *pVal);


void __RPC_STUB IPreference_get_DefVal_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPreference_put_DefVal_Proxy( 
    IPreference * This,
    /* [in] */ VARIANT newVal);


void __RPC_STUB IPreference_put_DefVal_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPreference_get_Description_Proxy( 
    IPreference * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IPreference_get_Description_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPreference_put_Description_Proxy( 
    IPreference * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IPreference_put_Description_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPreference_get_Name_Proxy( 
    IPreference * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IPreference_get_Name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPreference_put_Name_Proxy( 
    IPreference * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IPreference_put_Name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPreference_get_Max_Proxy( 
    IPreference * This,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IPreference_get_Max_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPreference_put_Max_Proxy( 
    IPreference * This,
    /* [in] */ long newVal);


void __RPC_STUB IPreference_put_Max_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPreference_get_Min_Proxy( 
    IPreference * This,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IPreference_get_Min_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPreference_put_Min_Proxy( 
    IPreference * This,
    /* [in] */ long newVal);


void __RPC_STUB IPreference_put_Min_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IPreference_get_Type_Proxy( 
    IPreference * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IPreference_get_Type_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IPreference_put_Type_Proxy( 
    IPreference * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB IPreference_put_Type_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPreference_Load_Proxy( 
    IPreference * This,
    BSTR IniFile,
    BSTR IniSection);


void __RPC_STUB IPreference_Load_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IPreference_Save_Proxy( 
    IPreference * This,
    BSTR IniFile,
    BSTR IniSection);


void __RPC_STUB IPreference_Save_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IPreference_INTERFACE_DEFINED__ */


#ifndef __IColorMatrix_INTERFACE_DEFINED__
#define __IColorMatrix_INTERFACE_DEFINED__

/* interface IColorMatrix */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IColorMatrix;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("4CA48898-449F-46AE-B7A5-47565B2F5217")
    IColorMatrix : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Value( 
            /* [in] */ int x,
            /* [in] */ int y,
            /* [retval][out] */ float *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Value( 
            /* [in] */ int x,
            /* [in] */ int y,
            /* [in] */ float newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Reset( void) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Hue( 
            /* [retval][out] */ float *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Hue( 
            /* [in] */ float newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Saturation( 
            /* [retval][out] */ float *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Saturation( 
            /* [in] */ float newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IColorMatrixVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IColorMatrix * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IColorMatrix * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IColorMatrix * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IColorMatrix * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IColorMatrix * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IColorMatrix * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IColorMatrix * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Value )( 
            IColorMatrix * This,
            /* [in] */ int x,
            /* [in] */ int y,
            /* [retval][out] */ float *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Value )( 
            IColorMatrix * This,
            /* [in] */ int x,
            /* [in] */ int y,
            /* [in] */ float newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Reset )( 
            IColorMatrix * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Hue )( 
            IColorMatrix * This,
            /* [retval][out] */ float *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Hue )( 
            IColorMatrix * This,
            /* [in] */ float newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Saturation )( 
            IColorMatrix * This,
            /* [retval][out] */ float *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Saturation )( 
            IColorMatrix * This,
            /* [in] */ float newVal);
        
        END_INTERFACE
    } IColorMatrixVtbl;

    interface IColorMatrix
    {
        CONST_VTBL struct IColorMatrixVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IColorMatrix_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IColorMatrix_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IColorMatrix_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IColorMatrix_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IColorMatrix_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IColorMatrix_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IColorMatrix_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IColorMatrix_get_Value(This,x,y,pVal)	\
    (This)->lpVtbl -> get_Value(This,x,y,pVal)

#define IColorMatrix_put_Value(This,x,y,newVal)	\
    (This)->lpVtbl -> put_Value(This,x,y,newVal)

#define IColorMatrix_Reset(This)	\
    (This)->lpVtbl -> Reset(This)

#define IColorMatrix_get_Hue(This,pVal)	\
    (This)->lpVtbl -> get_Hue(This,pVal)

#define IColorMatrix_put_Hue(This,newVal)	\
    (This)->lpVtbl -> put_Hue(This,newVal)

#define IColorMatrix_get_Saturation(This,pVal)	\
    (This)->lpVtbl -> get_Saturation(This,pVal)

#define IColorMatrix_put_Saturation(This,newVal)	\
    (This)->lpVtbl -> put_Saturation(This,newVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IColorMatrix_get_Value_Proxy( 
    IColorMatrix * This,
    /* [in] */ int x,
    /* [in] */ int y,
    /* [retval][out] */ float *pVal);


void __RPC_STUB IColorMatrix_get_Value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IColorMatrix_put_Value_Proxy( 
    IColorMatrix * This,
    /* [in] */ int x,
    /* [in] */ int y,
    /* [in] */ float newVal);


void __RPC_STUB IColorMatrix_put_Value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IColorMatrix_Reset_Proxy( 
    IColorMatrix * This);


void __RPC_STUB IColorMatrix_Reset_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IColorMatrix_get_Hue_Proxy( 
    IColorMatrix * This,
    /* [retval][out] */ float *pVal);


void __RPC_STUB IColorMatrix_get_Hue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IColorMatrix_put_Hue_Proxy( 
    IColorMatrix * This,
    /* [in] */ float newVal);


void __RPC_STUB IColorMatrix_put_Hue_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IColorMatrix_get_Saturation_Proxy( 
    IColorMatrix * This,
    /* [retval][out] */ float *pVal);


void __RPC_STUB IColorMatrix_get_Saturation_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IColorMatrix_put_Saturation_Proxy( 
    IColorMatrix * This,
    /* [in] */ float newVal);


void __RPC_STUB IColorMatrix_put_Saturation_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IColorMatrix_INTERFACE_DEFINED__ */


#ifndef __IMatrix_INTERFACE_DEFINED__
#define __IMatrix_INTERFACE_DEFINED__

/* interface IMatrix */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IMatrix;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("CC8A3A30-6DE9-487B-974E-49A045985E39")
    IMatrix : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Value( 
            /* [in] */ int x,
            /* [in] */ int y,
            /* [retval][out] */ float *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Value( 
            /* [in] */ int x,
            /* [in] */ int y,
            /* [in] */ float newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Rotate( 
            /* [in] */ float Angle,
            /* [defaultvalue][in] */ VARIANT_BOOL Prepend = -1) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Reset( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Translate( 
            /* [in] */ float offsetX,
            /* [in] */ float offsetY,
            /* [defaultvalue][in] */ VARIANT_BOOL Prepend = -1) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE RotateAt( 
            /* [in] */ float Angle,
            /* [in] */ float centerX,
            /* [in] */ float centerY,
            /* [defaultvalue][in] */ VARIANT_BOOL Prepend = -1) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMatrixVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMatrix * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMatrix * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMatrix * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IMatrix * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IMatrix * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IMatrix * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IMatrix * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Value )( 
            IMatrix * This,
            /* [in] */ int x,
            /* [in] */ int y,
            /* [retval][out] */ float *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Value )( 
            IMatrix * This,
            /* [in] */ int x,
            /* [in] */ int y,
            /* [in] */ float newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Rotate )( 
            IMatrix * This,
            /* [in] */ float Angle,
            /* [defaultvalue][in] */ VARIANT_BOOL Prepend);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Reset )( 
            IMatrix * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Translate )( 
            IMatrix * This,
            /* [in] */ float offsetX,
            /* [in] */ float offsetY,
            /* [defaultvalue][in] */ VARIANT_BOOL Prepend);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *RotateAt )( 
            IMatrix * This,
            /* [in] */ float Angle,
            /* [in] */ float centerX,
            /* [in] */ float centerY,
            /* [defaultvalue][in] */ VARIANT_BOOL Prepend);
        
        END_INTERFACE
    } IMatrixVtbl;

    interface IMatrix
    {
        CONST_VTBL struct IMatrixVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMatrix_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IMatrix_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IMatrix_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IMatrix_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IMatrix_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IMatrix_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IMatrix_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IMatrix_get_Value(This,x,y,pVal)	\
    (This)->lpVtbl -> get_Value(This,x,y,pVal)

#define IMatrix_put_Value(This,x,y,newVal)	\
    (This)->lpVtbl -> put_Value(This,x,y,newVal)

#define IMatrix_Rotate(This,Angle,Prepend)	\
    (This)->lpVtbl -> Rotate(This,Angle,Prepend)

#define IMatrix_Reset(This)	\
    (This)->lpVtbl -> Reset(This)

#define IMatrix_Translate(This,offsetX,offsetY,Prepend)	\
    (This)->lpVtbl -> Translate(This,offsetX,offsetY,Prepend)

#define IMatrix_RotateAt(This,Angle,centerX,centerY,Prepend)	\
    (This)->lpVtbl -> RotateAt(This,Angle,centerX,centerY,Prepend)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IMatrix_get_Value_Proxy( 
    IMatrix * This,
    /* [in] */ int x,
    /* [in] */ int y,
    /* [retval][out] */ float *pVal);


void __RPC_STUB IMatrix_get_Value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IMatrix_put_Value_Proxy( 
    IMatrix * This,
    /* [in] */ int x,
    /* [in] */ int y,
    /* [in] */ float newVal);


void __RPC_STUB IMatrix_put_Value_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMatrix_Rotate_Proxy( 
    IMatrix * This,
    /* [in] */ float Angle,
    /* [defaultvalue][in] */ VARIANT_BOOL Prepend);


void __RPC_STUB IMatrix_Rotate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMatrix_Reset_Proxy( 
    IMatrix * This);


void __RPC_STUB IMatrix_Reset_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMatrix_Translate_Proxy( 
    IMatrix * This,
    /* [in] */ float offsetX,
    /* [in] */ float offsetY,
    /* [defaultvalue][in] */ VARIANT_BOOL Prepend);


void __RPC_STUB IMatrix_Translate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IMatrix_RotateAt_Proxy( 
    IMatrix * This,
    /* [in] */ float Angle,
    /* [in] */ float centerX,
    /* [in] */ float centerY,
    /* [defaultvalue][in] */ VARIANT_BOOL Prepend);


void __RPC_STUB IMatrix_RotateAt_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IMatrix_INTERFACE_DEFINED__ */


#ifndef __ITimer_INTERFACE_DEFINED__
#define __ITimer_INTERFACE_DEFINED__

/* interface ITimer */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ITimer;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("186EF3B8-F257-4EA5-979F-89D383571284")
    ITimer : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Enabled( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Enabled( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Interval( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Interval( 
            /* [in] */ long newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ITimerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITimer * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITimer * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITimer * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ITimer * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ITimer * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ITimer * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ITimer * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Enabled )( 
            ITimer * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Enabled )( 
            ITimer * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Interval )( 
            ITimer * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Interval )( 
            ITimer * This,
            /* [in] */ long newVal);
        
        END_INTERFACE
    } ITimerVtbl;

    interface ITimer
    {
        CONST_VTBL struct ITimerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITimer_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ITimer_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ITimer_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ITimer_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ITimer_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ITimer_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ITimer_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ITimer_get_Enabled(This,pVal)	\
    (This)->lpVtbl -> get_Enabled(This,pVal)

#define ITimer_put_Enabled(This,newVal)	\
    (This)->lpVtbl -> put_Enabled(This,newVal)

#define ITimer_get_Interval(This,pVal)	\
    (This)->lpVtbl -> get_Interval(This,pVal)

#define ITimer_put_Interval(This,newVal)	\
    (This)->lpVtbl -> put_Interval(This,newVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ITimer_get_Enabled_Proxy( 
    ITimer * This,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB ITimer_get_Enabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ITimer_put_Enabled_Proxy( 
    ITimer * This,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB ITimer_put_Enabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ITimer_get_Interval_Proxy( 
    ITimer * This,
    /* [retval][out] */ long *pVal);


void __RPC_STUB ITimer_get_Interval_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ITimer_put_Interval_Proxy( 
    ITimer * This,
    /* [in] */ long newVal);


void __RPC_STUB ITimer_put_Interval_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ITimer_INTERFACE_DEFINED__ */


#ifndef __IDebugHelperClient_INTERFACE_DEFINED__
#define __IDebugHelperClient_INTERFACE_DEFINED__

/* interface IDebugHelperClient */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IDebugHelperClient;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("9006A921-8692-4121-8308-1C8740CFF6D3")
    IDebugHelperClient : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Print( 
            /* [in] */ BSTR Text) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Assert( 
            /* [in] */ VARIANT Expression) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Break( void) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Console( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Console( 
            /* [in] */ VARIANT_BOOL IsVisible) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDebugHelperClientVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDebugHelperClient * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDebugHelperClient * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDebugHelperClient * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDebugHelperClient * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDebugHelperClient * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDebugHelperClient * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDebugHelperClient * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Print )( 
            IDebugHelperClient * This,
            /* [in] */ BSTR Text);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Assert )( 
            IDebugHelperClient * This,
            /* [in] */ VARIANT Expression);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Break )( 
            IDebugHelperClient * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Console )( 
            IDebugHelperClient * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Console )( 
            IDebugHelperClient * This,
            /* [in] */ VARIANT_BOOL IsVisible);
        
        END_INTERFACE
    } IDebugHelperClientVtbl;

    interface IDebugHelperClient
    {
        CONST_VTBL struct IDebugHelperClientVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDebugHelperClient_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDebugHelperClient_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDebugHelperClient_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDebugHelperClient_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IDebugHelperClient_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IDebugHelperClient_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IDebugHelperClient_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IDebugHelperClient_Print(This,Text)	\
    (This)->lpVtbl -> Print(This,Text)

#define IDebugHelperClient_Assert(This,Expression)	\
    (This)->lpVtbl -> Assert(This,Expression)

#define IDebugHelperClient_Break(This)	\
    (This)->lpVtbl -> Break(This)

#define IDebugHelperClient_get_Console(This,pVal)	\
    (This)->lpVtbl -> get_Console(This,pVal)

#define IDebugHelperClient_put_Console(This,IsVisible)	\
    (This)->lpVtbl -> put_Console(This,IsVisible)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDebugHelperClient_Print_Proxy( 
    IDebugHelperClient * This,
    /* [in] */ BSTR Text);


void __RPC_STUB IDebugHelperClient_Print_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDebugHelperClient_Assert_Proxy( 
    IDebugHelperClient * This,
    /* [in] */ VARIANT Expression);


void __RPC_STUB IDebugHelperClient_Assert_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDebugHelperClient_Break_Proxy( 
    IDebugHelperClient * This);


void __RPC_STUB IDebugHelperClient_Break_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDebugHelperClient_get_Console_Proxy( 
    IDebugHelperClient * This,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IDebugHelperClient_get_Console_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IDebugHelperClient_put_Console_Proxy( 
    IDebugHelperClient * This,
    /* [in] */ VARIANT_BOOL IsVisible);


void __RPC_STUB IDebugHelperClient_put_Console_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDebugHelperClient_INTERFACE_DEFINED__ */


#ifndef __IDebugHelperServer_INTERFACE_DEFINED__
#define __IDebugHelperServer_INTERFACE_DEFINED__

/* interface IDebugHelperServer */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IDebugHelperServer;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("9006A921-8692-4121-8308-1C8740CFF6D4")
    IDebugHelperServer : public IUnknown
    {
    public:
        virtual /* [helpstring][id][restricted][hidden] */ HRESULT STDMETHODCALLTYPE AttachDebugClient( 
            /* [in] */ IUnknown *Document,
            /* [retval][out] */ IDebugHelperClient **pVal) = 0;
        
        virtual /* [helpstring][id][propget][restricted][hidden] */ HRESULT STDMETHODCALLTYPE get_DebugDocument( 
            /* [retval][out] */ IUnknown **pVal) = 0;
        
        virtual /* [helpstring][id][propputref][restricted][hidden] */ HRESULT STDMETHODCALLTYPE putref_DebugDocument( 
            /* [in] */ IUnknown *newVal) = 0;
        
        virtual /* [restricted][hidden][helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_DebugApplication( 
            /* [retval][out] */ IUnknown **pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_DebugApplicationContext( 
            /* [retval][out] */ DWORD *pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDebugHelperServerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDebugHelperServer * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDebugHelperServer * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDebugHelperServer * This);
        
        /* [helpstring][id][restricted][hidden] */ HRESULT ( STDMETHODCALLTYPE *AttachDebugClient )( 
            IDebugHelperServer * This,
            /* [in] */ IUnknown *Document,
            /* [retval][out] */ IDebugHelperClient **pVal);
        
        /* [helpstring][id][propget][restricted][hidden] */ HRESULT ( STDMETHODCALLTYPE *get_DebugDocument )( 
            IDebugHelperServer * This,
            /* [retval][out] */ IUnknown **pVal);
        
        /* [helpstring][id][propputref][restricted][hidden] */ HRESULT ( STDMETHODCALLTYPE *putref_DebugDocument )( 
            IDebugHelperServer * This,
            /* [in] */ IUnknown *newVal);
        
        /* [restricted][hidden][helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DebugApplication )( 
            IDebugHelperServer * This,
            /* [retval][out] */ IUnknown **pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DebugApplicationContext )( 
            IDebugHelperServer * This,
            /* [retval][out] */ DWORD *pVal);
        
        END_INTERFACE
    } IDebugHelperServerVtbl;

    interface IDebugHelperServer
    {
        CONST_VTBL struct IDebugHelperServerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDebugHelperServer_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDebugHelperServer_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDebugHelperServer_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDebugHelperServer_AttachDebugClient(This,Document,pVal)	\
    (This)->lpVtbl -> AttachDebugClient(This,Document,pVal)

#define IDebugHelperServer_get_DebugDocument(This,pVal)	\
    (This)->lpVtbl -> get_DebugDocument(This,pVal)

#define IDebugHelperServer_putref_DebugDocument(This,newVal)	\
    (This)->lpVtbl -> putref_DebugDocument(This,newVal)

#define IDebugHelperServer_get_DebugApplication(This,pVal)	\
    (This)->lpVtbl -> get_DebugApplication(This,pVal)

#define IDebugHelperServer_get_DebugApplicationContext(This,pVal)	\
    (This)->lpVtbl -> get_DebugApplicationContext(This,pVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][restricted][hidden] */ HRESULT STDMETHODCALLTYPE IDebugHelperServer_AttachDebugClient_Proxy( 
    IDebugHelperServer * This,
    /* [in] */ IUnknown *Document,
    /* [retval][out] */ IDebugHelperClient **pVal);


void __RPC_STUB IDebugHelperServer_AttachDebugClient_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget][restricted][hidden] */ HRESULT STDMETHODCALLTYPE IDebugHelperServer_get_DebugDocument_Proxy( 
    IDebugHelperServer * This,
    /* [retval][out] */ IUnknown **pVal);


void __RPC_STUB IDebugHelperServer_get_DebugDocument_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propputref][restricted][hidden] */ HRESULT STDMETHODCALLTYPE IDebugHelperServer_putref_DebugDocument_Proxy( 
    IDebugHelperServer * This,
    /* [in] */ IUnknown *newVal);


void __RPC_STUB IDebugHelperServer_putref_DebugDocument_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [restricted][hidden][helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDebugHelperServer_get_DebugApplication_Proxy( 
    IDebugHelperServer * This,
    /* [retval][out] */ IUnknown **pVal);


void __RPC_STUB IDebugHelperServer_get_DebugApplication_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDebugHelperServer_get_DebugApplicationContext_Proxy( 
    IDebugHelperServer * This,
    /* [retval][out] */ DWORD *pVal);


void __RPC_STUB IDebugHelperServer_get_DebugApplicationContext_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDebugHelperServer_INTERFACE_DEFINED__ */


#ifndef __IAveImage_INTERFACE_DEFINED__
#define __IAveImage_INTERFACE_DEFINED__

/* interface IAveImage */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IAveImage;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("059D563D-FBAA-47FB-B209-98D8D72BD1C7")
    IAveImage : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Path( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Render( 
            int Canvas,
            int left,
            int top,
            int width,
            int height) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ColorMatrix( 
            /* [retval][out] */ IColorMatrix **pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Matrix( 
            /* [retval][out] */ IMatrix **pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Width( 
            /* [retval][out] */ int *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Height( 
            /* [retval][out] */ int *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE LoadImage( 
            /* [in] */ BSTR FileName,
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Clone( 
            /* [optional][in] */ VARIANT newWidth,
            /* [optional][in] */ VARIANT newHeight,
            /* [retval][out] */ IAveImage **pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAveImageVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAveImage * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAveImage * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAveImage * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IAveImage * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IAveImage * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IAveImage * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IAveImage * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Name )( 
            IAveImage * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Path )( 
            IAveImage * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Render )( 
            IAveImage * This,
            int Canvas,
            int left,
            int top,
            int width,
            int height);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ColorMatrix )( 
            IAveImage * This,
            /* [retval][out] */ IColorMatrix **pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Matrix )( 
            IAveImage * This,
            /* [retval][out] */ IMatrix **pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Width )( 
            IAveImage * This,
            /* [retval][out] */ int *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Height )( 
            IAveImage * This,
            /* [retval][out] */ int *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *LoadImage )( 
            IAveImage * This,
            /* [in] */ BSTR FileName,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Clone )( 
            IAveImage * This,
            /* [optional][in] */ VARIANT newWidth,
            /* [optional][in] */ VARIANT newHeight,
            /* [retval][out] */ IAveImage **pVal);
        
        END_INTERFACE
    } IAveImageVtbl;

    interface IAveImage
    {
        CONST_VTBL struct IAveImageVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAveImage_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAveImage_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAveImage_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAveImage_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IAveImage_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IAveImage_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IAveImage_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IAveImage_get_Name(This,pVal)	\
    (This)->lpVtbl -> get_Name(This,pVal)

#define IAveImage_get_Path(This,pVal)	\
    (This)->lpVtbl -> get_Path(This,pVal)

#define IAveImage_Render(This,Canvas,left,top,width,height)	\
    (This)->lpVtbl -> Render(This,Canvas,left,top,width,height)

#define IAveImage_get_ColorMatrix(This,pVal)	\
    (This)->lpVtbl -> get_ColorMatrix(This,pVal)

#define IAveImage_get_Matrix(This,pVal)	\
    (This)->lpVtbl -> get_Matrix(This,pVal)

#define IAveImage_get_Width(This,pVal)	\
    (This)->lpVtbl -> get_Width(This,pVal)

#define IAveImage_get_Height(This,pVal)	\
    (This)->lpVtbl -> get_Height(This,pVal)

#define IAveImage_LoadImage(This,FileName,pVal)	\
    (This)->lpVtbl -> LoadImage(This,FileName,pVal)

#define IAveImage_Clone(This,newWidth,newHeight,pVal)	\
    (This)->lpVtbl -> Clone(This,newWidth,newHeight,pVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IAveImage_get_Name_Proxy( 
    IAveImage * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IAveImage_get_Name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IAveImage_get_Path_Proxy( 
    IAveImage * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IAveImage_get_Path_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IAveImage_Render_Proxy( 
    IAveImage * This,
    int Canvas,
    int left,
    int top,
    int width,
    int height);


void __RPC_STUB IAveImage_Render_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IAveImage_get_ColorMatrix_Proxy( 
    IAveImage * This,
    /* [retval][out] */ IColorMatrix **pVal);


void __RPC_STUB IAveImage_get_ColorMatrix_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IAveImage_get_Matrix_Proxy( 
    IAveImage * This,
    /* [retval][out] */ IMatrix **pVal);


void __RPC_STUB IAveImage_get_Matrix_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IAveImage_get_Width_Proxy( 
    IAveImage * This,
    /* [retval][out] */ int *pVal);


void __RPC_STUB IAveImage_get_Width_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IAveImage_get_Height_Proxy( 
    IAveImage * This,
    /* [retval][out] */ int *pVal);


void __RPC_STUB IAveImage_get_Height_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IAveImage_LoadImage_Proxy( 
    IAveImage * This,
    /* [in] */ BSTR FileName,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IAveImage_LoadImage_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IAveImage_Clone_Proxy( 
    IAveImage * This,
    /* [optional][in] */ VARIANT newWidth,
    /* [optional][in] */ VARIANT newHeight,
    /* [retval][out] */ IAveImage **pVal);


void __RPC_STUB IAveImage_Clone_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAveImage_INTERFACE_DEFINED__ */


#ifndef __IAveImages_INTERFACE_DEFINED__
#define __IAveImages_INTERFACE_DEFINED__

/* interface IAveImages */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IAveImages;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("78D0E8C9-AE97-40F2-AF7A-F66FFB5829AB")
    IAveImages : public IDispatch
    {
    public:
        virtual /* [restricted][helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get__NewEnum( 
            /* [retval][out] */ IUnknown **pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Item( 
            VARIANT *Index,
            /* [retval][out] */ IAveImage **pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long *pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAveImagesVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAveImages * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAveImages * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAveImages * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IAveImages * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IAveImages * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IAveImages * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IAveImages * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [restricted][helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get__NewEnum )( 
            IAveImages * This,
            /* [retval][out] */ IUnknown **pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Item )( 
            IAveImages * This,
            VARIANT *Index,
            /* [retval][out] */ IAveImage **pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            IAveImages * This,
            /* [retval][out] */ long *pVal);
        
        END_INTERFACE
    } IAveImagesVtbl;

    interface IAveImages
    {
        CONST_VTBL struct IAveImagesVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAveImages_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAveImages_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAveImages_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAveImages_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IAveImages_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IAveImages_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IAveImages_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IAveImages_get__NewEnum(This,pVal)	\
    (This)->lpVtbl -> get__NewEnum(This,pVal)

#define IAveImages_get_Item(This,Index,pVal)	\
    (This)->lpVtbl -> get_Item(This,Index,pVal)

#define IAveImages_get_Count(This,pVal)	\
    (This)->lpVtbl -> get_Count(This,pVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [restricted][helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IAveImages_get__NewEnum_Proxy( 
    IAveImages * This,
    /* [retval][out] */ IUnknown **pVal);


void __RPC_STUB IAveImages_get__NewEnum_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IAveImages_get_Item_Proxy( 
    IAveImages * This,
    VARIANT *Index,
    /* [retval][out] */ IAveImage **pVal);


void __RPC_STUB IAveImages_get_Item_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IAveImages_get_Count_Proxy( 
    IAveImages * This,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IAveImages_get_Count_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAveImages_INTERFACE_DEFINED__ */


#ifndef __IDeskletInfo_INTERFACE_DEFINED__
#define __IDeskletInfo_INTERFACE_DEFINED__

/* interface IDeskletInfo */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IDeskletInfo;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("8294571D-49C7-4680-831E-F8C54BD809E4")
    IDeskletInfo : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Author( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Comments( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_FileName( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Version( 
            /* [retval][out] */ int *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Skins( 
            /* [retval][out] */ IDeskletSkins **pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDeskletInfoVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDeskletInfo * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDeskletInfo * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDeskletInfo * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDeskletInfo * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDeskletInfo * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDeskletInfo * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDeskletInfo * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Name )( 
            IDeskletInfo * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Author )( 
            IDeskletInfo * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Comments )( 
            IDeskletInfo * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_FileName )( 
            IDeskletInfo * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Version )( 
            IDeskletInfo * This,
            /* [retval][out] */ int *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Skins )( 
            IDeskletInfo * This,
            /* [retval][out] */ IDeskletSkins **pVal);
        
        END_INTERFACE
    } IDeskletInfoVtbl;

    interface IDeskletInfo
    {
        CONST_VTBL struct IDeskletInfoVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDeskletInfo_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDeskletInfo_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDeskletInfo_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDeskletInfo_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IDeskletInfo_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IDeskletInfo_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IDeskletInfo_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IDeskletInfo_get_Name(This,pVal)	\
    (This)->lpVtbl -> get_Name(This,pVal)

#define IDeskletInfo_get_Author(This,pVal)	\
    (This)->lpVtbl -> get_Author(This,pVal)

#define IDeskletInfo_get_Comments(This,pVal)	\
    (This)->lpVtbl -> get_Comments(This,pVal)

#define IDeskletInfo_get_FileName(This,pVal)	\
    (This)->lpVtbl -> get_FileName(This,pVal)

#define IDeskletInfo_get_Version(This,pVal)	\
    (This)->lpVtbl -> get_Version(This,pVal)

#define IDeskletInfo_get_Skins(This,pVal)	\
    (This)->lpVtbl -> get_Skins(This,pVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDeskletInfo_get_Name_Proxy( 
    IDeskletInfo * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IDeskletInfo_get_Name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDeskletInfo_get_Author_Proxy( 
    IDeskletInfo * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IDeskletInfo_get_Author_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDeskletInfo_get_Comments_Proxy( 
    IDeskletInfo * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IDeskletInfo_get_Comments_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDeskletInfo_get_FileName_Proxy( 
    IDeskletInfo * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IDeskletInfo_get_FileName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDeskletInfo_get_Version_Proxy( 
    IDeskletInfo * This,
    /* [retval][out] */ int *pVal);


void __RPC_STUB IDeskletInfo_get_Version_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDeskletInfo_get_Skins_Proxy( 
    IDeskletInfo * This,
    /* [retval][out] */ IDeskletSkins **pVal);


void __RPC_STUB IDeskletInfo_get_Skins_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDeskletInfo_INTERFACE_DEFINED__ */


#ifndef __IDeskletInfos_INTERFACE_DEFINED__
#define __IDeskletInfos_INTERFACE_DEFINED__

/* interface IDeskletInfos */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IDeskletInfos;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("53DEBFCA-080A-4748-BE11-80B677DC99AC")
    IDeskletInfos : public IDispatch
    {
    public:
        virtual /* [restricted][helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get__NewEnum( 
            /* [retval][out] */ IUnknown **pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Item( 
            /* [in] */ VARIANT *Index,
            /* [retval][out] */ IDeskletInfo **pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Refresh( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Clear( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDeskletInfosVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDeskletInfos * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDeskletInfos * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDeskletInfos * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDeskletInfos * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDeskletInfos * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDeskletInfos * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDeskletInfos * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [restricted][helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get__NewEnum )( 
            IDeskletInfos * This,
            /* [retval][out] */ IUnknown **pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Item )( 
            IDeskletInfos * This,
            /* [in] */ VARIANT *Index,
            /* [retval][out] */ IDeskletInfo **pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            IDeskletInfos * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Refresh )( 
            IDeskletInfos * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Clear )( 
            IDeskletInfos * This);
        
        END_INTERFACE
    } IDeskletInfosVtbl;

    interface IDeskletInfos
    {
        CONST_VTBL struct IDeskletInfosVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDeskletInfos_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDeskletInfos_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDeskletInfos_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDeskletInfos_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IDeskletInfos_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IDeskletInfos_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IDeskletInfos_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IDeskletInfos_get__NewEnum(This,pVal)	\
    (This)->lpVtbl -> get__NewEnum(This,pVal)

#define IDeskletInfos_get_Item(This,Index,pVal)	\
    (This)->lpVtbl -> get_Item(This,Index,pVal)

#define IDeskletInfos_get_Count(This,pVal)	\
    (This)->lpVtbl -> get_Count(This,pVal)

#define IDeskletInfos_Refresh(This)	\
    (This)->lpVtbl -> Refresh(This)

#define IDeskletInfos_Clear(This)	\
    (This)->lpVtbl -> Clear(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [restricted][helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDeskletInfos_get__NewEnum_Proxy( 
    IDeskletInfos * This,
    /* [retval][out] */ IUnknown **pVal);


void __RPC_STUB IDeskletInfos_get__NewEnum_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDeskletInfos_get_Item_Proxy( 
    IDeskletInfos * This,
    /* [in] */ VARIANT *Index,
    /* [retval][out] */ IDeskletInfo **pVal);


void __RPC_STUB IDeskletInfos_get_Item_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDeskletInfos_get_Count_Proxy( 
    IDeskletInfos * This,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IDeskletInfos_get_Count_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDeskletInfos_Refresh_Proxy( 
    IDeskletInfos * This);


void __RPC_STUB IDeskletInfos_Refresh_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IDeskletInfos_Clear_Proxy( 
    IDeskletInfos * This);


void __RPC_STUB IDeskletInfos_Clear_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDeskletInfos_INTERFACE_DEFINED__ */


#ifndef __ILayer_INTERFACE_DEFINED__
#define __ILayer_INTERFACE_DEFINED__

/* interface ILayer */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ILayer;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("268FB2FF-2866-44CE-A3C7-85464D6E82BE")
    ILayer : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_FileName( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Visible( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Visible( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_CanBeIcon( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_CanBeIcon( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ILayerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ILayer * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ILayer * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ILayer * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ILayer * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ILayer * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ILayer * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ILayer * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Name )( 
            ILayer * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_FileName )( 
            ILayer * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Visible )( 
            ILayer * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Visible )( 
            ILayer * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_CanBeIcon )( 
            ILayer * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_CanBeIcon )( 
            ILayer * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        END_INTERFACE
    } ILayerVtbl;

    interface ILayer
    {
        CONST_VTBL struct ILayerVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ILayer_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ILayer_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ILayer_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ILayer_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ILayer_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ILayer_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ILayer_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ILayer_get_Name(This,pVal)	\
    (This)->lpVtbl -> get_Name(This,pVal)

#define ILayer_get_FileName(This,pVal)	\
    (This)->lpVtbl -> get_FileName(This,pVal)

#define ILayer_get_Visible(This,pVal)	\
    (This)->lpVtbl -> get_Visible(This,pVal)

#define ILayer_put_Visible(This,newVal)	\
    (This)->lpVtbl -> put_Visible(This,newVal)

#define ILayer_get_CanBeIcon(This,pVal)	\
    (This)->lpVtbl -> get_CanBeIcon(This,pVal)

#define ILayer_put_CanBeIcon(This,newVal)	\
    (This)->lpVtbl -> put_CanBeIcon(This,newVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ILayer_get_Name_Proxy( 
    ILayer * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB ILayer_get_Name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ILayer_get_FileName_Proxy( 
    ILayer * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB ILayer_get_FileName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ILayer_get_Visible_Proxy( 
    ILayer * This,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB ILayer_get_Visible_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ILayer_put_Visible_Proxy( 
    ILayer * This,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB ILayer_put_Visible_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ILayer_get_CanBeIcon_Proxy( 
    ILayer * This,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB ILayer_get_CanBeIcon_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE ILayer_put_CanBeIcon_Proxy( 
    ILayer * This,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB ILayer_put_CanBeIcon_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ILayer_INTERFACE_DEFINED__ */


#ifndef __ILayers_INTERFACE_DEFINED__
#define __ILayers_INTERFACE_DEFINED__

/* interface ILayers */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ILayers;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("269B8CE6-200A-4C8E-8D45-FDC880C586E2")
    ILayers : public IDispatch
    {
    public:
        virtual /* [restricted][helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get__NewEnum( 
            /* [retval][out] */ IUnknown **pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Item( 
            /* [in] */ VARIANT *Index,
            /* [retval][out] */ ILayer **pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ int *pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ILayersVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ILayers * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ILayers * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ILayers * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ILayers * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ILayers * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ILayers * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ILayers * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [restricted][helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get__NewEnum )( 
            ILayers * This,
            /* [retval][out] */ IUnknown **pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Item )( 
            ILayers * This,
            /* [in] */ VARIANT *Index,
            /* [retval][out] */ ILayer **pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            ILayers * This,
            /* [retval][out] */ int *pVal);
        
        END_INTERFACE
    } ILayersVtbl;

    interface ILayers
    {
        CONST_VTBL struct ILayersVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ILayers_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ILayers_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ILayers_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ILayers_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ILayers_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ILayers_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ILayers_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ILayers_get__NewEnum(This,pVal)	\
    (This)->lpVtbl -> get__NewEnum(This,pVal)

#define ILayers_get_Item(This,Index,pVal)	\
    (This)->lpVtbl -> get_Item(This,Index,pVal)

#define ILayers_get_Count(This,pVal)	\
    (This)->lpVtbl -> get_Count(This,pVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [restricted][helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ILayers_get__NewEnum_Proxy( 
    ILayers * This,
    /* [retval][out] */ IUnknown **pVal);


void __RPC_STUB ILayers_get__NewEnum_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ILayers_get_Item_Proxy( 
    ILayers * This,
    /* [in] */ VARIANT *Index,
    /* [retval][out] */ ILayer **pVal);


void __RPC_STUB ILayers_get_Item_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE ILayers_get_Count_Proxy( 
    ILayers * This,
    /* [retval][out] */ int *pVal);


void __RPC_STUB ILayers_get_Count_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ILayers_INTERFACE_DEFINED__ */


#ifndef __IRenderCanvas_INTERFACE_DEFINED__
#define __IRenderCanvas_INTERFACE_DEFINED__

/* interface IRenderCanvas */
/* [unique][uuid][dual][object] */ 


EXTERN_C const IID IID_IRenderCanvas;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("03EAD5B9-6670-4329-8AC0-A624DED8B816")
    IRenderCanvas : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetGraphics( 
            /* [retval][out] */ int *pVal) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE GetHDC( 
            /* [retval][out] */ int *pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IRenderCanvasVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IRenderCanvas * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IRenderCanvas * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IRenderCanvas * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IRenderCanvas * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IRenderCanvas * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IRenderCanvas * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IRenderCanvas * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetGraphics )( 
            IRenderCanvas * This,
            /* [retval][out] */ int *pVal);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE *GetHDC )( 
            IRenderCanvas * This,
            /* [retval][out] */ int *pVal);
        
        END_INTERFACE
    } IRenderCanvasVtbl;

    interface IRenderCanvas
    {
        CONST_VTBL struct IRenderCanvasVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IRenderCanvas_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IRenderCanvas_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IRenderCanvas_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IRenderCanvas_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IRenderCanvas_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IRenderCanvas_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IRenderCanvas_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IRenderCanvas_GetGraphics(This,pVal)	\
    (This)->lpVtbl -> GetGraphics(This,pVal)

#define IRenderCanvas_GetHDC(This,pVal)	\
    (This)->lpVtbl -> GetHDC(This,pVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id] */ HRESULT STDMETHODCALLTYPE IRenderCanvas_GetGraphics_Proxy( 
    IRenderCanvas * This,
    /* [retval][out] */ int *pVal);


void __RPC_STUB IRenderCanvas_GetGraphics_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE IRenderCanvas_GetHDC_Proxy( 
    IRenderCanvas * This,
    /* [retval][out] */ int *pVal);


void __RPC_STUB IRenderCanvas_GetHDC_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IRenderCanvas_INTERFACE_DEFINED__ */


#ifndef __IShowCase_INTERFACE_DEFINED__
#define __IShowCase_INTERFACE_DEFINED__

/* interface IShowCase */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IShowCase;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("7DB5140E-B90D-4DC6-9453-D28937E6A652")
    IShowCase : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Activate( void) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Deactivate( void) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Active( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Active( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_MaximumAlpha( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_MaximumAlpha( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_NumberOfAnimationFrames( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_NumberOfAnimationFrames( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_DoAnimation( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_DoAnimation( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Color( 
            /* [retval][out] */ /* external definition not present */ OLE_COLOR *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Color( 
            /* [in] */ /* external definition not present */ OLE_COLOR newVal) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ActivateOnAllDesklets( void) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Enabled( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_Enabled( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_IsClickThru( 
            /* [retval][out] */ VARIANT_BOOL *pVal) = 0;
        
        virtual /* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE put_IsClickThru( 
            /* [in] */ VARIANT_BOOL newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IShowCaseVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IShowCase * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IShowCase * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IShowCase * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IShowCase * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IShowCase * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IShowCase * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IShowCase * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Activate )( 
            IShowCase * This);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Deactivate )( 
            IShowCase * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Active )( 
            IShowCase * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Active )( 
            IShowCase * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_MaximumAlpha )( 
            IShowCase * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_MaximumAlpha )( 
            IShowCase * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_NumberOfAnimationFrames )( 
            IShowCase * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_NumberOfAnimationFrames )( 
            IShowCase * This,
            /* [in] */ long newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_DoAnimation )( 
            IShowCase * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_DoAnimation )( 
            IShowCase * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Color )( 
            IShowCase * This,
            /* [retval][out] */ /* external definition not present */ OLE_COLOR *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Color )( 
            IShowCase * This,
            /* [in] */ /* external definition not present */ OLE_COLOR newVal);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *ActivateOnAllDesklets )( 
            IShowCase * This);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Enabled )( 
            IShowCase * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_Enabled )( 
            IShowCase * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_IsClickThru )( 
            IShowCase * This,
            /* [retval][out] */ VARIANT_BOOL *pVal);
        
        /* [helpstring][id][propput] */ HRESULT ( STDMETHODCALLTYPE *put_IsClickThru )( 
            IShowCase * This,
            /* [in] */ VARIANT_BOOL newVal);
        
        END_INTERFACE
    } IShowCaseVtbl;

    interface IShowCase
    {
        CONST_VTBL struct IShowCaseVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IShowCase_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IShowCase_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IShowCase_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IShowCase_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IShowCase_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IShowCase_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IShowCase_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IShowCase_Activate(This)	\
    (This)->lpVtbl -> Activate(This)

#define IShowCase_Deactivate(This)	\
    (This)->lpVtbl -> Deactivate(This)

#define IShowCase_get_Active(This,pVal)	\
    (This)->lpVtbl -> get_Active(This,pVal)

#define IShowCase_put_Active(This,newVal)	\
    (This)->lpVtbl -> put_Active(This,newVal)

#define IShowCase_get_MaximumAlpha(This,pVal)	\
    (This)->lpVtbl -> get_MaximumAlpha(This,pVal)

#define IShowCase_put_MaximumAlpha(This,newVal)	\
    (This)->lpVtbl -> put_MaximumAlpha(This,newVal)

#define IShowCase_get_NumberOfAnimationFrames(This,pVal)	\
    (This)->lpVtbl -> get_NumberOfAnimationFrames(This,pVal)

#define IShowCase_put_NumberOfAnimationFrames(This,newVal)	\
    (This)->lpVtbl -> put_NumberOfAnimationFrames(This,newVal)

#define IShowCase_get_DoAnimation(This,pVal)	\
    (This)->lpVtbl -> get_DoAnimation(This,pVal)

#define IShowCase_put_DoAnimation(This,newVal)	\
    (This)->lpVtbl -> put_DoAnimation(This,newVal)

#define IShowCase_get_Color(This,pVal)	\
    (This)->lpVtbl -> get_Color(This,pVal)

#define IShowCase_put_Color(This,newVal)	\
    (This)->lpVtbl -> put_Color(This,newVal)

#define IShowCase_ActivateOnAllDesklets(This)	\
    (This)->lpVtbl -> ActivateOnAllDesklets(This)

#define IShowCase_get_Enabled(This,pVal)	\
    (This)->lpVtbl -> get_Enabled(This,pVal)

#define IShowCase_put_Enabled(This,newVal)	\
    (This)->lpVtbl -> put_Enabled(This,newVal)

#define IShowCase_get_IsClickThru(This,pVal)	\
    (This)->lpVtbl -> get_IsClickThru(This,pVal)

#define IShowCase_put_IsClickThru(This,newVal)	\
    (This)->lpVtbl -> put_IsClickThru(This,newVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IShowCase_Activate_Proxy( 
    IShowCase * This);


void __RPC_STUB IShowCase_Activate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IShowCase_Deactivate_Proxy( 
    IShowCase * This);


void __RPC_STUB IShowCase_Deactivate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IShowCase_get_Active_Proxy( 
    IShowCase * This,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IShowCase_get_Active_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IShowCase_put_Active_Proxy( 
    IShowCase * This,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB IShowCase_put_Active_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IShowCase_get_MaximumAlpha_Proxy( 
    IShowCase * This,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IShowCase_get_MaximumAlpha_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IShowCase_put_MaximumAlpha_Proxy( 
    IShowCase * This,
    /* [in] */ long newVal);


void __RPC_STUB IShowCase_put_MaximumAlpha_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IShowCase_get_NumberOfAnimationFrames_Proxy( 
    IShowCase * This,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IShowCase_get_NumberOfAnimationFrames_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IShowCase_put_NumberOfAnimationFrames_Proxy( 
    IShowCase * This,
    /* [in] */ long newVal);


void __RPC_STUB IShowCase_put_NumberOfAnimationFrames_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IShowCase_get_DoAnimation_Proxy( 
    IShowCase * This,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IShowCase_get_DoAnimation_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IShowCase_put_DoAnimation_Proxy( 
    IShowCase * This,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB IShowCase_put_DoAnimation_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IShowCase_get_Color_Proxy( 
    IShowCase * This,
    /* [retval][out] */ /* external definition not present */ OLE_COLOR *pVal);


void __RPC_STUB IShowCase_get_Color_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IShowCase_put_Color_Proxy( 
    IShowCase * This,
    /* [in] */ /* external definition not present */ OLE_COLOR newVal);


void __RPC_STUB IShowCase_put_Color_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IShowCase_ActivateOnAllDesklets_Proxy( 
    IShowCase * This);


void __RPC_STUB IShowCase_ActivateOnAllDesklets_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IShowCase_get_Enabled_Proxy( 
    IShowCase * This,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IShowCase_get_Enabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IShowCase_put_Enabled_Proxy( 
    IShowCase * This,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB IShowCase_put_Enabled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IShowCase_get_IsClickThru_Proxy( 
    IShowCase * This,
    /* [retval][out] */ VARIANT_BOOL *pVal);


void __RPC_STUB IShowCase_get_IsClickThru_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propput] */ HRESULT STDMETHODCALLTYPE IShowCase_put_IsClickThru_Proxy( 
    IShowCase * This,
    /* [in] */ VARIANT_BOOL newVal);


void __RPC_STUB IShowCase_put_IsClickThru_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IShowCase_INTERFACE_DEFINED__ */


#ifndef __IDeskletSkin_INTERFACE_DEFINED__
#define __IDeskletSkin_INTERFACE_DEFINED__

/* interface IDeskletSkin */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IDeskletSkin;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("40DD6B56-FBDB-4906-B23A-B57CDEC3D81F")
    IDeskletSkin : public IDispatch
    {
    public:
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Key( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Number( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Author( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Version( 
            /* [retval][out] */ long *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Comments( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDeskletSkinVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDeskletSkin * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDeskletSkin * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDeskletSkin * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDeskletSkin * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDeskletSkin * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDeskletSkin * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDeskletSkin * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Key )( 
            IDeskletSkin * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Number )( 
            IDeskletSkin * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Name )( 
            IDeskletSkin * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Author )( 
            IDeskletSkin * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Version )( 
            IDeskletSkin * This,
            /* [retval][out] */ long *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Comments )( 
            IDeskletSkin * This,
            /* [retval][out] */ BSTR *pVal);
        
        END_INTERFACE
    } IDeskletSkinVtbl;

    interface IDeskletSkin
    {
        CONST_VTBL struct IDeskletSkinVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDeskletSkin_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDeskletSkin_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDeskletSkin_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDeskletSkin_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IDeskletSkin_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IDeskletSkin_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IDeskletSkin_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IDeskletSkin_get_Key(This,pVal)	\
    (This)->lpVtbl -> get_Key(This,pVal)

#define IDeskletSkin_get_Number(This,pVal)	\
    (This)->lpVtbl -> get_Number(This,pVal)

#define IDeskletSkin_get_Name(This,pVal)	\
    (This)->lpVtbl -> get_Name(This,pVal)

#define IDeskletSkin_get_Author(This,pVal)	\
    (This)->lpVtbl -> get_Author(This,pVal)

#define IDeskletSkin_get_Version(This,pVal)	\
    (This)->lpVtbl -> get_Version(This,pVal)

#define IDeskletSkin_get_Comments(This,pVal)	\
    (This)->lpVtbl -> get_Comments(This,pVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDeskletSkin_get_Key_Proxy( 
    IDeskletSkin * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IDeskletSkin_get_Key_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDeskletSkin_get_Number_Proxy( 
    IDeskletSkin * This,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IDeskletSkin_get_Number_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDeskletSkin_get_Name_Proxy( 
    IDeskletSkin * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IDeskletSkin_get_Name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDeskletSkin_get_Author_Proxy( 
    IDeskletSkin * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IDeskletSkin_get_Author_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDeskletSkin_get_Version_Proxy( 
    IDeskletSkin * This,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IDeskletSkin_get_Version_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDeskletSkin_get_Comments_Proxy( 
    IDeskletSkin * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IDeskletSkin_get_Comments_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDeskletSkin_INTERFACE_DEFINED__ */


#ifndef __IDeskletSkins_INTERFACE_DEFINED__
#define __IDeskletSkins_INTERFACE_DEFINED__

/* interface IDeskletSkins */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IDeskletSkins;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("649E41F9-73AD-4CC7-A447-FC313C21FFD0")
    IDeskletSkins : public IDispatch
    {
    public:
        virtual /* [restricted][helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get__NewEnum( 
            /* [retval][out] */ IUnknown **pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Item( 
            /* [in] */ VARIANT *Index,
            /* [retval][out] */ IDeskletSkin **pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long *pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDeskletSkinsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDeskletSkins * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDeskletSkins * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDeskletSkins * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDeskletSkins * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDeskletSkins * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDeskletSkins * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDeskletSkins * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [restricted][helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get__NewEnum )( 
            IDeskletSkins * This,
            /* [retval][out] */ IUnknown **pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Item )( 
            IDeskletSkins * This,
            /* [in] */ VARIANT *Index,
            /* [retval][out] */ IDeskletSkin **pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Count )( 
            IDeskletSkins * This,
            /* [retval][out] */ long *pVal);
        
        END_INTERFACE
    } IDeskletSkinsVtbl;

    interface IDeskletSkins
    {
        CONST_VTBL struct IDeskletSkinsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDeskletSkins_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDeskletSkins_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDeskletSkins_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDeskletSkins_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IDeskletSkins_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IDeskletSkins_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IDeskletSkins_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IDeskletSkins_get__NewEnum(This,pVal)	\
    (This)->lpVtbl -> get__NewEnum(This,pVal)

#define IDeskletSkins_get_Item(This,Index,pVal)	\
    (This)->lpVtbl -> get_Item(This,Index,pVal)

#define IDeskletSkins_get_Count(This,pVal)	\
    (This)->lpVtbl -> get_Count(This,pVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [restricted][helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDeskletSkins_get__NewEnum_Proxy( 
    IDeskletSkins * This,
    /* [retval][out] */ IUnknown **pVal);


void __RPC_STUB IDeskletSkins_get__NewEnum_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDeskletSkins_get_Item_Proxy( 
    IDeskletSkins * This,
    /* [in] */ VARIANT *Index,
    /* [retval][out] */ IDeskletSkin **pVal);


void __RPC_STUB IDeskletSkins_get_Item_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IDeskletSkins_get_Count_Proxy( 
    IDeskletSkins * This,
    /* [retval][out] */ long *pVal);


void __RPC_STUB IDeskletSkins_get_Count_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDeskletSkins_INTERFACE_DEFINED__ */


#ifndef ___IDeskletEvents_DISPINTERFACE_DEFINED__
#define ___IDeskletEvents_DISPINTERFACE_DEFINED__

/* dispinterface _IDeskletEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__IDeskletEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("03B4FEC0-1F1C-4931-82FD-9C748834D944")
    _IDeskletEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _IDeskletEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _IDeskletEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _IDeskletEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _IDeskletEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _IDeskletEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _IDeskletEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _IDeskletEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _IDeskletEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _IDeskletEventsVtbl;

    interface _IDeskletEvents
    {
        CONST_VTBL struct _IDeskletEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IDeskletEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _IDeskletEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _IDeskletEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _IDeskletEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _IDeskletEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _IDeskletEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _IDeskletEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___IDeskletEvents_DISPINTERFACE_DEFINED__ */


#ifndef ___IDeskletsEvents_DISPINTERFACE_DEFINED__
#define ___IDeskletsEvents_DISPINTERFACE_DEFINED__

/* dispinterface _IDeskletsEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__IDeskletsEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("93053009-DE11-496B-B7B9-467A8FACEDE2")
    _IDeskletsEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _IDeskletsEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _IDeskletsEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _IDeskletsEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _IDeskletsEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _IDeskletsEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _IDeskletsEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _IDeskletsEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _IDeskletsEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _IDeskletsEventsVtbl;

    interface _IDeskletsEvents
    {
        CONST_VTBL struct _IDeskletsEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _IDeskletsEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define _IDeskletsEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define _IDeskletsEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define _IDeskletsEvents_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define _IDeskletsEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define _IDeskletsEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define _IDeskletsEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___IDeskletsEvents_DISPINTERFACE_DEFINED__ */


#ifndef __IEffectLet_INTERFACE_DEFINED__
#define __IEffectLet_INTERFACE_DEFINED__

/* interface IEffectLet */
/* [unique][helpstring][uuid][dual][object] */ 


EXTERN_C const IID IID_IEffectLet;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("B2CFD8D8-9B78-4029-9418-0E244298C91F")
    IEffectLet : public IDispatch
    {
    public:
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Preferences( 
            /* [retval][out] */ IPreferences **pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Desklet( 
            /* [retval][out] */ IDesklet **pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Ticker( 
            /* [retval][out] */ ITimer **pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_ColorMatrix( 
            /* [retval][out] */ IColorMatrix **pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Matrix( 
            /* [retval][out] */ IMatrix **pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Name( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Author( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Description( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Version( 
            /* [retval][out] */ BSTR *pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Images( 
            /* [retval][out] */ IAveImages **pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Application( 
            /* [retval][out] */ IApplication **pVal) = 0;
        
        virtual /* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE get_Canvas( 
            /* [retval][out] */ int *pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IEffectLetVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IEffectLet * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IEffectLet * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IEffectLet * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IEffectLet * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IEffectLet * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IEffectLet * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IEffectLet * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Preferences )( 
            IEffectLet * This,
            /* [retval][out] */ IPreferences **pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Desklet )( 
            IEffectLet * This,
            /* [retval][out] */ IDesklet **pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Ticker )( 
            IEffectLet * This,
            /* [retval][out] */ ITimer **pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_ColorMatrix )( 
            IEffectLet * This,
            /* [retval][out] */ IColorMatrix **pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Matrix )( 
            IEffectLet * This,
            /* [retval][out] */ IMatrix **pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Name )( 
            IEffectLet * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Author )( 
            IEffectLet * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Description )( 
            IEffectLet * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Version )( 
            IEffectLet * This,
            /* [retval][out] */ BSTR *pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Images )( 
            IEffectLet * This,
            /* [retval][out] */ IAveImages **pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Application )( 
            IEffectLet * This,
            /* [retval][out] */ IApplication **pVal);
        
        /* [helpstring][id][propget] */ HRESULT ( STDMETHODCALLTYPE *get_Canvas )( 
            IEffectLet * This,
            /* [retval][out] */ int *pVal);
        
        END_INTERFACE
    } IEffectLetVtbl;

    interface IEffectLet
    {
        CONST_VTBL struct IEffectLetVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IEffectLet_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IEffectLet_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IEffectLet_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IEffectLet_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IEffectLet_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IEffectLet_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IEffectLet_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IEffectLet_get_Preferences(This,pVal)	\
    (This)->lpVtbl -> get_Preferences(This,pVal)

#define IEffectLet_get_Desklet(This,pVal)	\
    (This)->lpVtbl -> get_Desklet(This,pVal)

#define IEffectLet_get_Ticker(This,pVal)	\
    (This)->lpVtbl -> get_Ticker(This,pVal)

#define IEffectLet_get_ColorMatrix(This,pVal)	\
    (This)->lpVtbl -> get_ColorMatrix(This,pVal)

#define IEffectLet_get_Matrix(This,pVal)	\
    (This)->lpVtbl -> get_Matrix(This,pVal)

#define IEffectLet_get_Name(This,pVal)	\
    (This)->lpVtbl -> get_Name(This,pVal)

#define IEffectLet_get_Author(This,pVal)	\
    (This)->lpVtbl -> get_Author(This,pVal)

#define IEffectLet_get_Description(This,pVal)	\
    (This)->lpVtbl -> get_Description(This,pVal)

#define IEffectLet_get_Version(This,pVal)	\
    (This)->lpVtbl -> get_Version(This,pVal)

#define IEffectLet_get_Images(This,pVal)	\
    (This)->lpVtbl -> get_Images(This,pVal)

#define IEffectLet_get_Application(This,pVal)	\
    (This)->lpVtbl -> get_Application(This,pVal)

#define IEffectLet_get_Canvas(This,pVal)	\
    (This)->lpVtbl -> get_Canvas(This,pVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id][propget] */ HRESULT STDMETHODCALLTYPE IEffectLet_get_Preferences_Proxy( 
    IEffectLet * This,
    /* [retval][out] */ IPreferences **pVal);


void __RPC_STUB IEffectLet_get_Preferences_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IEffectLet_get_Desklet_Proxy( 
    IEffectLet * This,
    /* [retval][out] */ IDesklet **pVal);


void __RPC_STUB IEffectLet_get_Desklet_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IEffectLet_get_Ticker_Proxy( 
    IEffectLet * This,
    /* [retval][out] */ ITimer **pVal);


void __RPC_STUB IEffectLet_get_Ticker_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IEffectLet_get_ColorMatrix_Proxy( 
    IEffectLet * This,
    /* [retval][out] */ IColorMatrix **pVal);


void __RPC_STUB IEffectLet_get_ColorMatrix_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IEffectLet_get_Matrix_Proxy( 
    IEffectLet * This,
    /* [retval][out] */ IMatrix **pVal);


void __RPC_STUB IEffectLet_get_Matrix_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IEffectLet_get_Name_Proxy( 
    IEffectLet * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IEffectLet_get_Name_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IEffectLet_get_Author_Proxy( 
    IEffectLet * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IEffectLet_get_Author_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IEffectLet_get_Description_Proxy( 
    IEffectLet * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IEffectLet_get_Description_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IEffectLet_get_Version_Proxy( 
    IEffectLet * This,
    /* [retval][out] */ BSTR *pVal);


void __RPC_STUB IEffectLet_get_Version_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IEffectLet_get_Images_Proxy( 
    IEffectLet * This,
    /* [retval][out] */ IAveImages **pVal);


void __RPC_STUB IEffectLet_get_Images_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IEffectLet_get_Application_Proxy( 
    IEffectLet * This,
    /* [retval][out] */ IApplication **pVal);


void __RPC_STUB IEffectLet_get_Application_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id][propget] */ HRESULT STDMETHODCALLTYPE IEffectLet_get_Canvas_Proxy( 
    IEffectLet * This,
    /* [retval][out] */ int *pVal);


void __RPC_STUB IEffectLet_get_Canvas_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IEffectLet_INTERFACE_DEFINED__ */


#ifndef __IDebugConsole_INTERFACE_DEFINED__
#define __IDebugConsole_INTERFACE_DEFINED__

/* interface IDebugConsole */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IDebugConsole;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("9006A921-8692-4121-8308-1C8740CFF6D2")
    IDebugConsole : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IDebugConsoleVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IDebugConsole * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IDebugConsole * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IDebugConsole * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IDebugConsole * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IDebugConsole * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IDebugConsole * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IDebugConsole * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IDebugConsoleVtbl;

    interface IDebugConsole
    {
        CONST_VTBL struct IDebugConsoleVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDebugConsole_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDebugConsole_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDebugConsole_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDebugConsole_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IDebugConsole_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IDebugConsole_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IDebugConsole_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IDebugConsole_INTERFACE_DEFINED__ */


#ifndef __ISkinEnumerator_INTERFACE_DEFINED__
#define __ISkinEnumerator_INTERFACE_DEFINED__

/* interface ISkinEnumerator */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ISkinEnumerator;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("5DF9D0A8-9968-428C-8969-208EF2C3C2E0")
    ISkinEnumerator : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE Next( 
            BSTR *pVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISkinEnumeratorVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ISkinEnumerator * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ISkinEnumerator * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ISkinEnumerator * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ISkinEnumerator * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ISkinEnumerator * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ISkinEnumerator * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ISkinEnumerator * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *Next )( 
            ISkinEnumerator * This,
            BSTR *pVal);
        
        END_INTERFACE
    } ISkinEnumeratorVtbl;

    interface ISkinEnumerator
    {
        CONST_VTBL struct ISkinEnumeratorVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISkinEnumerator_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISkinEnumerator_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISkinEnumerator_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISkinEnumerator_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ISkinEnumerator_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ISkinEnumerator_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ISkinEnumerator_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ISkinEnumerator_Next(This,pVal)	\
    (This)->lpVtbl -> Next(This,pVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE ISkinEnumerator_Next_Proxy( 
    ISkinEnumerator * This,
    BSTR *pVal);


void __RPC_STUB ISkinEnumerator_Next_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISkinEnumerator_INTERFACE_DEFINED__ */


#ifndef __IAveDeskletFile_INTERFACE_DEFINED__
#define __IAveDeskletFile_INTERFACE_DEFINED__

/* interface IAveDeskletFile */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IAveDeskletFile;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("43CB06F6-4FD6-4868-ACED-01F0E03262B5")
    IAveDeskletFile : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IAveDeskletFileVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAveDeskletFile * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAveDeskletFile * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAveDeskletFile * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IAveDeskletFile * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IAveDeskletFile * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IAveDeskletFile * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IAveDeskletFile * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IAveDeskletFileVtbl;

    interface IAveDeskletFile
    {
        CONST_VTBL struct IAveDeskletFileVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAveDeskletFile_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAveDeskletFile_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAveDeskletFile_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAveDeskletFile_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IAveDeskletFile_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IAveDeskletFile_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IAveDeskletFile_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IAveDeskletFile_INTERFACE_DEFINED__ */


#ifndef __IAveThemeFile_INTERFACE_DEFINED__
#define __IAveThemeFile_INTERFACE_DEFINED__

/* interface IAveThemeFile */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IAveThemeFile;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("22E63B9B-D236-4D07-8500-5E5950DEB6F1")
    IAveThemeFile : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IAveThemeFileVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAveThemeFile * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAveThemeFile * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAveThemeFile * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IAveThemeFile * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IAveThemeFile * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IAveThemeFile * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IAveThemeFile * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IAveThemeFileVtbl;

    interface IAveThemeFile
    {
        CONST_VTBL struct IAveThemeFileVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAveThemeFile_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAveThemeFile_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAveThemeFile_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAveThemeFile_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IAveThemeFile_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IAveThemeFile_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IAveThemeFile_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IAveThemeFile_INTERFACE_DEFINED__ */


#ifndef __IAveInstallFile_INTERFACE_DEFINED__
#define __IAveInstallFile_INTERFACE_DEFINED__

/* interface IAveInstallFile */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IAveInstallFile;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("C06675E2-3447-4FAC-AF3E-E598D2E6E990")
    IAveInstallFile : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct IAveInstallFileVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IAveInstallFile * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IAveInstallFile * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IAveInstallFile * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IAveInstallFile * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IAveInstallFile * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IAveInstallFile * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IAveInstallFile * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IAveInstallFileVtbl;

    interface IAveInstallFile
    {
        CONST_VTBL struct IAveInstallFileVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAveInstallFile_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAveInstallFile_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAveInstallFile_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAveInstallFile_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IAveInstallFile_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IAveInstallFile_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IAveInstallFile_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IAveInstallFile_INTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_Application;

#ifdef __cplusplus

class DECLSPEC_UUID("F47B7FC0-7482-49E0-9F16-7D6D8C9444DE")
Application;
#endif

EXTERN_C const CLSID CLSID_Desklets;

#ifdef __cplusplus

class DECLSPEC_UUID("21C83410-5CC7-49AD-97A1-B841C67415C6")
Desklets;
#endif

EXTERN_C const CLSID CLSID_Desklet;

#ifdef __cplusplus

class DECLSPEC_UUID("A1EAADEA-AD20-43F3-BCB4-F9AA864C24C1")
Desklet;
#endif

EXTERN_C const CLSID CLSID_EffectLet;

#ifdef __cplusplus

class DECLSPEC_UUID("6B91160A-4E1C-4131-BC82-C188EB1CDE8A")
EffectLet;
#endif

EXTERN_C const CLSID CLSID_Preferences;

#ifdef __cplusplus

class DECLSPEC_UUID("0E4CD517-CE73-4C5F-A4B6-4E82804022B4")
Preferences;
#endif

EXTERN_C const CLSID CLSID_Preference;

#ifdef __cplusplus

class DECLSPEC_UUID("53EA24DA-1454-4818-8096-37F234C0424A")
Preference;
#endif

EXTERN_C const CLSID CLSID_DebugHelperClient;

#ifdef __cplusplus

class DECLSPEC_UUID("1F91B833-C7CA-4C74-B40E-678859A7F9AF")
DebugHelperClient;
#endif

EXTERN_C const CLSID CLSID_DebugHelperServer;

#ifdef __cplusplus

class DECLSPEC_UUID("1F91B833-C7CA-4C74-B40E-678859A7F9AE")
DebugHelperServer;
#endif

EXTERN_C const CLSID CLSID_DebugConsole;

#ifdef __cplusplus

class DECLSPEC_UUID("1F91B833-C7CA-4C74-B40E-678859A7F9AD")
DebugConsole;
#endif

EXTERN_C const CLSID CLSID_ADColorMatrix;

#ifdef __cplusplus

class DECLSPEC_UUID("518C9FE7-2043-45B5-8A8A-C57DC530031D")
ADColorMatrix;
#endif

EXTERN_C const CLSID CLSID_ADMatrix;

#ifdef __cplusplus

class DECLSPEC_UUID("8BC74884-38AB-4185-BB2E-92D074E6CD58")
ADMatrix;
#endif

EXTERN_C const CLSID CLSID_Timer;

#ifdef __cplusplus

class DECLSPEC_UUID("BA838CBE-E917-4FBF-893E-CB48B292E2F9")
Timer;
#endif

EXTERN_C const CLSID CLSID_AveImage;

#ifdef __cplusplus

class DECLSPEC_UUID("1737D13A-0443-44F8-B2F8-1989B554A7E4")
AveImage;
#endif

EXTERN_C const CLSID CLSID_AveImages;

#ifdef __cplusplus

class DECLSPEC_UUID("84FB0245-179D-4640-9230-6785828C686D")
AveImages;
#endif

EXTERN_C const CLSID CLSID_DeskletInfo;

#ifdef __cplusplus

class DECLSPEC_UUID("F72C12CA-50F9-4E5E-8A40-BF5B59A06D59")
DeskletInfo;
#endif

EXTERN_C const CLSID CLSID_DeskletInfos;

#ifdef __cplusplus

class DECLSPEC_UUID("4AC8E525-4338-419C-8518-EA3F8F44C50F")
DeskletInfos;
#endif

EXTERN_C const CLSID CLSID_oLayer;

#ifdef __cplusplus

class DECLSPEC_UUID("FEA0F6C6-1F5C-406B-9CA3-A743A3205CE8")
oLayer;
#endif

EXTERN_C const CLSID CLSID_Layers;

#ifdef __cplusplus

class DECLSPEC_UUID("1D3CDD9F-8C1D-4738-8D29-60DA97D247F2")
Layers;
#endif

EXTERN_C const CLSID CLSID_ShowCase;

#ifdef __cplusplus

class DECLSPEC_UUID("83947EC4-5584-4CBE-9386-6161003F4C4C")
ShowCase;
#endif

EXTERN_C const CLSID CLSID_SkinEnumerator;

#ifdef __cplusplus

class DECLSPEC_UUID("5D033EFE-4518-4EE4-83E8-262850E64640")
SkinEnumerator;
#endif

EXTERN_C const CLSID CLSID_AveDeskletFile;

#ifdef __cplusplus

class DECLSPEC_UUID("0DCCF541-9CC0-462A-A012-0F0E040DB134")
AveDeskletFile;
#endif

EXTERN_C const CLSID CLSID_AveThemeFile;

#ifdef __cplusplus

class DECLSPEC_UUID("97FAB359-2942-461E-B48E-8B065C8A9AE9")
AveThemeFile;
#endif

EXTERN_C const CLSID CLSID_AveInstallFile;

#ifdef __cplusplus

class DECLSPEC_UUID("E971912D-4653-4EF4-9177-9E1DDDF18E6C")
AveInstallFile;
#endif

EXTERN_C const CLSID CLSID_DeskletSkin;

#ifdef __cplusplus

class DECLSPEC_UUID("7172844F-A976-4A38-96A6-87F88A8ECE88")
DeskletSkin;
#endif

EXTERN_C const CLSID CLSID_DeskletSkins;

#ifdef __cplusplus

class DECLSPEC_UUID("D98A8D0A-A567-4C18-9EBD-EA84E0A8C735")
DeskletSkins;
#endif
#endif /* __AveDeskLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


