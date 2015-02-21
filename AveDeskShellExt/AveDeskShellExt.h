/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Tue Jun 28 02:05:46 2005
 */
/* Compiler settings for E:\Coding\Projects\AveDeskShellExt\AveDeskShellExt.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __AveDeskShellExt_h__
#define __AveDeskShellExt_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __ICAveDeskImageShellExt_FWD_DEFINED__
#define __ICAveDeskImageShellExt_FWD_DEFINED__
typedef interface ICAveDeskImageShellExt ICAveDeskImageShellExt;
#endif 	/* __ICAveDeskImageShellExt_FWD_DEFINED__ */


#ifndef __ICAveDeskThemeShellExt_FWD_DEFINED__
#define __ICAveDeskThemeShellExt_FWD_DEFINED__
typedef interface ICAveDeskThemeShellExt ICAveDeskThemeShellExt;
#endif 	/* __ICAveDeskThemeShellExt_FWD_DEFINED__ */


#ifndef __CAveDeskImageShellExt_FWD_DEFINED__
#define __CAveDeskImageShellExt_FWD_DEFINED__

#ifdef __cplusplus
typedef class CAveDeskImageShellExt CAveDeskImageShellExt;
#else
typedef struct CAveDeskImageShellExt CAveDeskImageShellExt;
#endif /* __cplusplus */

#endif 	/* __CAveDeskImageShellExt_FWD_DEFINED__ */


#ifndef __CAveDeskThemeShellExt_FWD_DEFINED__
#define __CAveDeskThemeShellExt_FWD_DEFINED__

#ifdef __cplusplus
typedef class CAveDeskThemeShellExt CAveDeskThemeShellExt;
#else
typedef struct CAveDeskThemeShellExt CAveDeskThemeShellExt;
#endif /* __cplusplus */

#endif 	/* __CAveDeskThemeShellExt_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __ICAveDeskImageShellExt_INTERFACE_DEFINED__
#define __ICAveDeskImageShellExt_INTERFACE_DEFINED__

/* interface ICAveDeskImageShellExt */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ICAveDeskImageShellExt;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("67F1EFEA-291F-4471-AE26-15C42C32F072")
    ICAveDeskImageShellExt : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct ICAveDeskImageShellExtVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ICAveDeskImageShellExt __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ICAveDeskImageShellExt __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ICAveDeskImageShellExt __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ICAveDeskImageShellExt __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ICAveDeskImageShellExt __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ICAveDeskImageShellExt __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ICAveDeskImageShellExt __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        END_INTERFACE
    } ICAveDeskImageShellExtVtbl;

    interface ICAveDeskImageShellExt
    {
        CONST_VTBL struct ICAveDeskImageShellExtVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICAveDeskImageShellExt_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICAveDeskImageShellExt_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ICAveDeskImageShellExt_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ICAveDeskImageShellExt_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ICAveDeskImageShellExt_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ICAveDeskImageShellExt_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ICAveDeskImageShellExt_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICAveDeskImageShellExt_INTERFACE_DEFINED__ */


#ifndef __ICAveDeskThemeShellExt_INTERFACE_DEFINED__
#define __ICAveDeskThemeShellExt_INTERFACE_DEFINED__

/* interface ICAveDeskThemeShellExt */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ICAveDeskThemeShellExt;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("D6EBC484-A1A4-41ED-A8B3-2991E420A4BE")
    ICAveDeskThemeShellExt : public IDispatch
    {
    public:
    };
    
#else 	/* C style interface */

    typedef struct ICAveDeskThemeShellExtVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ICAveDeskThemeShellExt __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ICAveDeskThemeShellExt __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ICAveDeskThemeShellExt __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ICAveDeskThemeShellExt __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ICAveDeskThemeShellExt __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ICAveDeskThemeShellExt __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ICAveDeskThemeShellExt __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        END_INTERFACE
    } ICAveDeskThemeShellExtVtbl;

    interface ICAveDeskThemeShellExt
    {
        CONST_VTBL struct ICAveDeskThemeShellExtVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICAveDeskThemeShellExt_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ICAveDeskThemeShellExt_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ICAveDeskThemeShellExt_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ICAveDeskThemeShellExt_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ICAveDeskThemeShellExt_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ICAveDeskThemeShellExt_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ICAveDeskThemeShellExt_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __ICAveDeskThemeShellExt_INTERFACE_DEFINED__ */



#ifndef __AVEDESKSHELLEXTLib_LIBRARY_DEFINED__
#define __AVEDESKSHELLEXTLib_LIBRARY_DEFINED__

/* library AVEDESKSHELLEXTLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_AVEDESKSHELLEXTLib;

EXTERN_C const CLSID CLSID_CAveDeskImageShellExt;

#ifdef __cplusplus

class DECLSPEC_UUID("6DD050D8-FBDF-4562-AB05-8BF879FAE89E")
CAveDeskImageShellExt;
#endif

EXTERN_C const CLSID CLSID_CAveDeskThemeShellExt;

#ifdef __cplusplus

class DECLSPEC_UUID("F756AF2A-EB0E-4D42-9495-B61BD6D99B82")
CAveDeskThemeShellExt;
#endif
#endif /* __AVEDESKSHELLEXTLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
