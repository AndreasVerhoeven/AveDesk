// ContainerLayer.h: interface for the ContainerLayer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONTAINERLAYER_H__12E04662_CBCE_4A16_BC46_C6B14DDDCA11__INCLUDED_)
#define AFX_CONTAINERLAYER_H__12E04662_CBCE_4A16_BC46_C6B14DDDCA11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "layer.h"
#include "UpdateBounds.h"


class ContainerLayer : public Layer  
{
protected:
	bool isBuffered;
	CUpdateBounds m_UpdateRegion;
	SIZE bufferSize;

	Bitmap* buffer;

	SIZE CalculateBufferSize(int deskletWidth, int deskletHeight);

public:
	ContainerLayer(const char* name, Layer* parentLayer, bool isBuffered);
	virtual ~ContainerLayer();

	virtual bool CanBeDrawn(){return true;};

	virtual void SetIsBuffered(bool val, const SIZE* bufSize, const BOOL* append);
	virtual bool GetIsBuffered()const;

	virtual void SetImage(Image* bmp, bool clone);
	virtual void Draw(Graphics* g, int left, int top, int width, int height,ColorMatrix* oldMatrix);

	
	virtual bool HitTest(const POINT& point, const SIZE& size);

	virtual BOOL GetHitTestedLayer(const POINT& point, const SIZE& size, Layer*& hitLayer, bool mustBeVisible);

	virtual void DrawPartial(Graphics* g, int left, int top, int width, int height, ColorMatrix* oldMatrix,
			std::list<RECT>& rects);

	virtual void AddUpdateRect(const RECT& r, const SIZE& s);
	virtual SIZE GetBufferSize()const;

	virtual BOOL IsContainer()const{return TRUE;};

	virtual void InternalUpdate(int left, int top, int width, int height, std::list<RECT>* rects);

	virtual void DrawPartialRotation(Graphics* g, int left, int top, int width, int height, ColorMatrix* oldMatrix,
						std::list<RECT>& rects);

	virtual bool CanBeDrawn()const {return true;};

};

#endif // !defined(AFX_CONTAINERLAYER_H__12E04662_CBCE_4A16_BC46_C6B14DDDCA11__INCLUDED_)
