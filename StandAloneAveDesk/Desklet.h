// Desklet.h : Declaration of the CDesklet

#ifndef __DESKLET_H_
#define __DESKLET_H_

#include "resource.h"       // main symbols
#include "helper_funcs.h"
#include "colormatrix.h"
#include "matrix.h"
#include "olayers.h"
class Docklet;
/////////////////////////////////////////////////////////////////////////////
// CDesklet
class ATL_NO_VTABLE CDesklet : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDesklet, &CLSID_Desklet>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CDesklet>,
	public IDispatchImpl<IDesklet, &IID_IDesklet, &LIBID_AveDeskLib>,
  public CoElementOwnerImpl<Docklet>
{
public:
  CDesklet() :
      colorMatrix(NULL),
      matrix(NULL),
      layers(NULL)
	{
	}

  ~CDesklet();

  CComObject<CColorMatrix> *colorMatrix;
  CComObject<CMatrix> *matrix;
  CComObject<CoLayers> *layers;
  CComPtr<IDeskletInfo> about;
  CComPtr<IApplication> pApp;
  CComVariant tag;

//DECLARE_REGISTRY_RESOURCEID(IDR_DESKLET)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CDesklet)
	COM_INTERFACE_ENTRY(IDesklet)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
END_COM_MAP()
BEGIN_CONNECTION_POINT_MAP(CDesklet)
END_CONNECTION_POINT_MAP()

	


// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IDesklet
public:
	STDMETHOD(get_Skin)(/*[out, retval]*/ IDeskletSkin* *pVal);
	STDMETHOD(putref_Skin)(/*[in]*/ IDeskletSkin* newVal);
	STDMETHOD(FadeOut)(/*[in]*/ VARIANT* async, long durationInMs);
	STDMETHOD(FadeIn)(/*[in]*/VARIANT* async, long durationInMs);
	STDMETHOD(get_Tag)(/*[out, retval]*/ VARIANT* pVal);
	STDMETHOD(put_Tag)(/*[in]*/ VARIANT newVal);
	STDMETHOD(get_ObjectModel)(/*[out, retval]*/ IDispatch* *pVal);
	STDMETHOD(get_HWnd)(/*[out, retval]*/ int *pVal);
	STDMETHOD(Clone)(/*[out, retval]*/ IDesklet **pVal);
	STDMETHOD(get_Style)(/*[out, retval]*/ DeskletStyles *pVal);
	STDMETHOD(put_Style)(/*[in]*/ DeskletStyles newVal);
	STDMETHOD(PopFront)();
	STDMETHOD(get_Layers)(/*[out, retval]*/ ILayers* *pVal);
	STDMETHOD(get_About)(/*[out, retval]*/ IDeskletInfo* *pVal);
	STDMETHOD(Move)(/*[in]*/ int Left, /*[in, optional]*/ VARIANT *Top, /*[in, optional]*/ VARIANT *Width, /*[in, optional]*/ VARIANT *Height);
	STDMETHOD(get_Height)(/*[out, retval]*/ int *pVal);
	STDMETHOD(put_Height)(/*[in]*/ int newVal);
	STDMETHOD(get_Width)(/*[out, retval]*/ int *pVal);
	STDMETHOD(put_Width)(/*[in]*/ int newVal);
	STDMETHOD(get_Top)(/*[out, retval]*/ int *pVal);
	STDMETHOD(put_Top)(/*[in]*/ int newVal);
	STDMETHOD(get_Left)(/*[out, retval]*/ int *pVal);
	STDMETHOD(put_Left)(/*[in]*/ int newVal);
	STDMETHOD(get_Rotation)(/*[out, retval]*/ int *pVal);
	STDMETHOD(put_Rotation)(/*[in]*/ int newVal);
	STDMETHOD(get_Alpha)(/*[out, retval]*/ int *pVal);
	STDMETHOD(put_Alpha)(/*[in]*/ int newVal);
	STDMETHOD(get_Visible)(/*[out, retval]*/ VARIANT_BOOL *pVal);
	STDMETHOD(put_Visible)(/*[in]*/ VARIANT_BOOL newVal);
	STDMETHOD(get_PositionMatrix)(/*[out, retval]*/ IMatrix * *pVal);
	STDMETHOD(get_ColorMatrix)(/*[out, retval]*/ IColorMatrix* *pVal);
	STDMETHOD(Redraw)();
	STDMETHOD(get_Application)(/*[out, retval]*/ IApplication* *pVal);
	STDMETHOD(get_SubLabel)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_SubLabel)(/*[in]*/ BSTR newVal);
	STDMETHOD(get_Label)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(put_Label)(/*[in]*/ BSTR newVal);
};

#endif //__DESKLET_H_
