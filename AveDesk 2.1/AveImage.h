// AveImage.h : Deklaration von CImage

#ifndef __IMAGE_H_
#define __IMAGE_H_

#include "resource.h"       // Hauptsymbole
#include "colormatrix.h"
#include "matrix.h"

/////////////////////////////////////////////////////////////////////////////
// CImage
class ATL_NO_VTABLE CImage : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CImage, &CLSID_AveImage>,
	public ISupportErrorInfo,
  public IProvideClassInfo2Impl<&CLSID_AveImage, NULL, &LIBID_AveDeskLib>,
	public IDispatchImpl<IAveImage, &IID_IAveImage, &LIBID_AveDeskLib>
{
public:
	CImage()
	{
    bmp = NULL;
    colorMatrix = NULL;
    matrix = NULL;
	}
  ~CImage();

DECLARE_REGISTRY_RESOURCEID(IDR_IMAGE)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CImage)
	COM_INTERFACE_ENTRY(IAveImage)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
END_COM_MAP()

// Variables
CComBSTR name, path;
Bitmap *bmp;
CComObject<CColorMatrix> *colorMatrix;
CComObject<CMatrix> *matrix;
CComPtr<IEffectLet> parent;


// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

// IAveImage
public:
	STDMETHOD(Clone)(/*[in, optional]*/ VARIANT newWidth, /*[in, optional]*/ VARIANT newHeight, /*[out, retval]*/ IAveImage ** pVal);
	STDMETHOD(LoadImage)(/*[in]*/ BSTR FileName, /*[out, retval]*/ VARIANT_BOOL * pVal);
	STDMETHOD(get_Height)(/*[out, retval]*/ int *pVal);
	STDMETHOD(get_Width)(/*[out, retval]*/ int *pVal);
	STDMETHOD(get_Matrix)(/*[out, retval]*/ IMatrix* *pVal);
	STDMETHOD(get_ColorMatrix)(/*[out, retval]*/ IColorMatrix* *pVal);
	STDMETHOD(Render)(int Canvas, int left, int top, int width, int height);
	STDMETHOD(get_Path)(/*[out, retval]*/ BSTR *pVal);
	STDMETHOD(get_Name)(/*[out, retval]*/ BSTR *pVal);
	void LoadXML(IXMLDOMNode *nod);
};

#endif //__IMAGE_H_
