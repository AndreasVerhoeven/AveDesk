#pragma once


struct ImagePixelBuffer
{
	DWORD* pixelBuffer;
	int   stride;
	UINT width;
	UINT height;
};

class CWaterAnimation
{
private:
	typedef long waterdata_T;

	UINT m_width;
	UINT m_height;

	// the algorithm encapsuled by this class is using two buffers.
	// one buffer will hold the active data which will be used for painting on the next call to
	// update paint. The other buffer will hold the previous data. When the next step of the animation
	// is performed, the buffers are switched and the active buffer will be composed out of the
	// unactive buffer.
	// We could have used one buffer instead and use a temporary buffer when doing
	// the next animation step, but that would cause a lot of heap fragmentation, plus
	// allocating and deallocating this temporary buffer every step will be a performance penalty.
	waterdata_T* m_waterDataA;
	waterdata_T* m_waterDataB;
	waterdata_T* m_activeWaterData;
	waterdata_T* m_unactiveWaterData;

	HBITMAP m_hBitmap;
	HDC m_hdc;
	DWORD* m_BmpPixelBuffer;

	bool m_bitmapIsUpdated;

	float m_damping;
	long m_maxWaterDifference;

public:
	CWaterAnimation(UINT width, UINT height);
	~CWaterAnimation();

	bool isValid()const;
	bool isBitmapUpdated()const;
	void nextAnimationStep();
	void draw(HDC hdc, int xCoord, int yCoord, ImagePixelBuffer* pixelBuffer);
	void makeDrip(int x, int y, int depth);

	void setDamping(float damping);
	float getDamping()const;

	void setMaxWaterDifference(long difference);
	long getMaxWaterDifference()const;
};
