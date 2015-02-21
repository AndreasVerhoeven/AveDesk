// AveImage.cpp : Implementierung von CImage
#include "stdafx.h"
#include "AveDesk.h"
#include "AveImage.h"
#include "effectlet.h" // for base path access

/////////////////////////////////////////////////////////////////////////////
// CImage

CImage::~CImage()
{
  if( bmp )
    delete bmp;

  if( colorMatrix )
    colorMatrix->Release();

  if( matrix )
    matrix->Release();
}

STDMETHODIMP CImage::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_IAveImage
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
    if (::InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}
// reuse preference utility function
extern void MapXml(CComBSTR nam, CComBSTR & tar, CComVariant & val, CComPtr<IXMLDOMNamedNodeMap> & list);
#ifndef MAPXML
#define MAPXML(t) MapXml(A2W(""#t), t, val, list)
#endif
extern Bitmap* CALLBACK DockletLoadGDIPlusImage(const char *szImage);
void CImage::LoadXML(IXMLDOMNode *nod)
{
  USES_CONVERSION;
  
  CComQIPtr<IXMLDOMNode> nodeElem = nod; // simplified attribute access
  CComPtr<IXMLDOMNamedNodeMap> list;
  nodeElem->get_attributes(&list.p);
  CComVariant val;
  
  MAPXML(name);  
  MAPXML(path);

  VARIANT_BOOL bDummy;
  LoadImage(path, &bDummy);
  
}

STDMETHODIMP CImage::get_Name(BSTR *pVal)
{
  name.CopyTo(pVal);
	return S_OK;
}

STDMETHODIMP CImage::get_Path(BSTR *pVal)
{
  path.CopyTo(pVal);
	return S_OK;
}

STDMETHODIMP CImage::Render(int Canvas, int left, int top, int width, int height)
{
  CComPtr<IRenderCanvas> cnv = (IRenderCanvas*)Canvas;
  try
  {

    if( !cnv.p )
      return E_INVALIDARG;

    Graphics *g = NULL;
    if( SUCCEEDED( cnv->GetGraphics((int*)&g)) )
    {
      GraphicsContainer cont = g->BeginContainer();
      ImageAttributes *pattr = NULL;

      if( matrix )
        g->SetTransform(&matrix->m);
      if( colorMatrix )
      {
		    pattr = new ImageAttributes;
        pattr->SetColorMatrix(
          &colorMatrix->colorMatrix,
          ColorMatrixFlagsDefault,
          ColorAdjustTypeBitmap);
      }
      g->DrawImage(
        bmp,
        Rect(left, top, width, height),
        0, 0, bmp->GetWidth(), bmp->GetHeight(),
        UnitPixel, pattr);
      g->EndContainer(cont);
      if( pattr )
        delete pattr;
    }
  }
  catch(...)
  {
    return E_INVALIDARG;
  }
	return S_OK;
}

STDMETHODIMP CImage::get_ColorMatrix(IColorMatrix **pVal)
{
  if( NULL == colorMatrix )
  {
    CComObject<CColorMatrix>::CreateInstance(&colorMatrix);
    colorMatrix->Set(ColorMatrixHelper::GetIdentityMatrix());
    colorMatrix->AddRef();
  }

  colorMatrix->QueryInterface(pVal);
	return S_OK;
}

STDMETHODIMP CImage::get_Matrix(IMatrix **pVal)
{
  if( NULL == matrix )
  {
    CComObject<CMatrix>::CreateInstance(&matrix);
    matrix->AddRef();
  }
  matrix->QueryInterface(pVal);
	return S_OK;
}

STDMETHODIMP CImage::get_Width(int *pVal)
{
  if( bmp )
    *pVal = bmp->GetWidth();
  else
    *pVal = 0;
	return S_OK;
}

STDMETHODIMP CImage::get_Height(int *pVal)
{
  if( bmp )
    *pVal = bmp->GetHeight();
  else
    *pVal = 0;
	return S_OK;
}

STDMETHODIMP CImage::LoadImage(BSTR FileName, VARIANT_BOOL *pVal)
{
  USES_CONVERSION;
  if( PathFileExistsW(FileName) )
  {
    bmp = DockletLoadGDIPlusImage(W2A(FileName));
  }
  else
  {
    CComObject<CEffectLet> *prt = dynamic_cast<CComObject<CEffectLet>*>(parent.p);
    if( prt )
    {
      CString basePath = W2T(prt->path);
      TCHAR *pImage = basePath.GetBuffer(MAX_PATH+1);     
      PathRemoveFileSpec(pImage);
      PathAddBackslash(pImage);
      PathAppend(pImage, W2T((FileName)));
      if( PathFileExists(pImage) )
        bmp = DockletLoadGDIPlusImage(T2A(pImage));
      basePath.ReleaseBuffer();
    }
  }                        
	return S_OK;
}

extern inline int ExtractInt(VARIANT* src, int defval, BOOL* bReturnedDefVal=NULL); // in Desklet.cpp

/*

  A cloned image only exists in memory. (Unless saved? Add SaveImage method?)
  newWidth and NewHeight can be omitted in script or automation;
  the vt member will be VT_EMPTY
*/
STDMETHODIMP CImage::Clone(VARIANT newWidth, VARIANT newHeight, IAveImage **pVal)
{
    if( pVal )
    {
        CComObject<CImage>* img = NULL;
        CComObject<CImage>::CreateInstance(&img);
        if( img )
        {
            int w = ExtractInt(&newWidth, bmp->GetWidth());
            int h = ExtractInt(&newHeight, bmp->GetHeight());
            Image * img2 = bmp->GetThumbnailImage(w, h);
            img->bmp = (Bitmap*)img2;
            img->name = name;
            img->path = L"";
        }
        img->QueryInterface(pVal);
    }
    return S_OK;
}
