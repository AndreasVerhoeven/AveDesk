#include "StdAfx.h"
#include ".\wateranimation.h"
#include <math.h>

CWaterAnimation::CWaterAnimation(UINT width, UINT height) :
	m_width(width), m_height(height),
	m_waterDataA(NULL), m_waterDataB(NULL), 
	m_activeWaterData(NULL), m_unactiveWaterData(NULL),
	m_damping(0.002f), m_maxWaterDifference(127),
	m_BmpPixelBuffer(NULL), m_hBitmap(NULL),
	m_bitmapIsUpdated(false), m_hdc(NULL)
{
	size_t bufferSize(m_width * m_height);
	m_waterDataA = new waterdata_T[bufferSize];
	m_waterDataB = new waterdata_T[bufferSize];

	if(m_waterDataA != NULL)
		memset(m_waterDataA, 0, bufferSize * sizeof(waterdata_T));

	if(m_waterDataB != NULL)
		memset(m_waterDataB , 0, bufferSize * sizeof(waterdata_T));

	m_activeWaterData   = m_waterDataA;
	m_unactiveWaterData = m_waterDataB;

	BITMAPINFO bmi = {sizeof(BITMAPINFOHEADER)};
    bmi.bmiHeader.biWidth       = m_width;
    bmi.bmiHeader.biHeight      = -((int)m_height);
    bmi.bmiHeader.biPlanes      = 1;
    bmi.bmiHeader.biBitCount    = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

	HWND desktopHwnd(GetDesktopWindow());
	HDC hdc(GetDC(desktopHwnd));

	//m_BmpPixelBuffer = new DWORD[bufferSize];
	m_hdc = CreateCompatibleDC(hdc);
	m_hBitmap = CreateDIBSection(hdc , &bmi, DIB_RGB_COLORS, reinterpret_cast<VOID**>(&m_BmpPixelBuffer), NULL, 0L);
	SelectObject(m_hdc, m_hBitmap);
	ReleaseDC(desktopHwnd, hdc);
}

CWaterAnimation::~CWaterAnimation()
{
	delete[] m_activeWaterData;
	delete[] m_unactiveWaterData;
	DeleteObject(m_hBitmap);
	DeleteDC(m_hdc);
}

bool CWaterAnimation::isValid()const
// post: true has been returned iff the data for this object is valid.
{
	return m_waterDataA != NULL && m_waterDataB != NULL;
}

bool CWaterAnimation::isBitmapUpdated()const
// post: true has been iff the bitmap is updated to reflect changes in the water.
{
	return m_bitmapIsUpdated;
}

template<typename T> inline T readbuffer(T* buffer, size_t rowWidth, size_t x, size_t y)
{
	return buffer[y*rowWidth +x];
}

template<typename T> inline T in_between(T minVal, T val, T maxVal)
{
	if(val < minVal)
		val = minVal;
	else if(val > maxVal)
		val = maxVal;

	return val;
	//return min(max(minVal, val), maxVal);
}

template<typename T>T inline SquaredDist(T sx, T sy, T dx, T dy)
{
	return ((dx - sx) * (dx - sx)) + ((dy - sy) * (dy - sy));
}
void CWaterAnimation::makeDrip(int x, int y, int depth) 
{
	const int dripRadius = 24;
	int i,j,dist,finaldepth;
	int Drip_Radius_Sqr = dripRadius * dripRadius;
	RECT bounds = {0,0,m_width - 1,m_height - 1};
	for (j = y - dripRadius; j < y + dripRadius; j++)
	{
		for (i = x - dripRadius; i < x + dripRadius; i++)
		{
			if(i >= 0 && i < (int)m_width - 1 && j >= 0 && j < (int)m_height - 1)
			{
				dist = SquaredDist(x,y,i,j);
				if (dist < Drip_Radius_Sqr)
				{
					int sqrtDist = (int)sqrt((float)dist);
					finaldepth = (int)((float)depth * ((float)(dripRadius - sqrtDist)/(float)dripRadius));
					if (finaldepth > 127) finaldepth = 127;
					if (finaldepth < -127) finaldepth = -127;
					m_activeWaterData[j*m_width + i] = finaldepth;
					//SETBUFFER(m_WriteBuffer,i,j,finaldepth);
				}
			}
		}
	}
}

void CWaterAnimation::draw(HDC hdc, int xCoord, int yCoord, ImagePixelBuffer* pixelBuffer)
// pre: isValid(), hdc != NULL, pixelBuffer != NULL, pixelBuffer's dimensions equal (width, height)
{
//	assert(hdc != NULL);
//	assert(pixelBuffer != NULL);
//	assert(pixelBuffer->width == m_width && pixelBuffer->height == m_height);
//	assert(isValid());

	//CopyMemory(m_BmpPixelBuffer, pixelBuffer->pixelBuffer, pixelBuffer->stride * pixelBuffer->height);

	int bufferFactor = m_height * m_width;
	DWORD* pixelBuf = m_BmpPixelBuffer;
	DWORD* texBuf = pixelBuffer->pixelBuffer;	

	DWORD	color;
	int		r,g,b,xoff,yoff;
	int cnt = 0;
	for (UINT j = 0; j < m_height; j++) 
	{
		for (UINT i = 0; i < m_width; i++, cnt++) 
		{
			
			xoff = i;
			if (i > 0 && i < (UINT)m_height - 1)
			{
				xoff -= (m_activeWaterData[cnt - 1]);
				xoff += (m_activeWaterData[cnt + 1]);
			}

			yoff = j;
			if (j > 0 && j < (UINT)(m_width - 1) && cnt >= (int)m_width && cnt < (int)((m_width-1)*m_height))
			{
				yoff -= m_activeWaterData[cnt - m_width];
				yoff += m_activeWaterData[cnt + m_width];
			}
			if (xoff < 0) xoff = 0;
			if (yoff < 0) yoff = 0;
			if (xoff >= (int) m_width) xoff  = (int)m_width  - 1;
			if (yoff >= (int) m_height) yoff = (int)m_height - 1;

			color = *(pixelBuffer->pixelBuffer + yoff * pixelBuffer->width + xoff);
			r = GetRValue(color);
			g = GetGValue(color);
			b = GetBValue(color);
			
			waterdata_T value  = m_activeWaterData[cnt];
			//waterdata_T value = 0;
			r += value;
			g += value;
			b += value;

			r = in_between(0, r, 255);
			g = in_between(0, g, 255);
			b = in_between(0, b, 255);

			m_BmpPixelBuffer[cnt] = RGB(r,g,b) | 0xFF000000;
			
			//m_BmpPixelBuffer[cnt] = color;//RGB(0,0,0);
		}
	}

	BitBlt(hdc, xCoord, yCoord, m_width, m_height, m_hdc, 0, 0, SRCCOPY);

}


void CWaterAnimation::nextAnimationStep()
// pre:  isValid()
// post: the 'water' data has been transformed into the next step.
{
//	assert(isValid());

	// switch buffers.
	m_activeWaterData   = (m_waterDataA == m_activeWaterData)   ? m_waterDataB : m_waterDataA;
	m_unactiveWaterData = (m_waterDataA == m_unactiveWaterData) ? m_waterDataB : m_waterDataA;

	// next we calculate each new element of the active buffer (new step)
	// by using the data from the unactive buffer(previous step).
	for(UINT y = 2; y < m_height - 2; ++y)
	{
		for(UINT x = 2; x < m_width - 2; ++x)
		{
			float value = (float)(
								readbuffer(m_unactiveWaterData, m_width, x-2,y) +
								readbuffer(m_unactiveWaterData, m_width, x+2,y) +
								readbuffer(m_unactiveWaterData, m_width, x,y-2) +
								readbuffer(m_unactiveWaterData, m_width, x,y+2) +
								readbuffer(m_unactiveWaterData, m_width, x-1,y) +
								readbuffer(m_unactiveWaterData, m_width, x+1,y) +
								readbuffer(m_unactiveWaterData, m_width, x,y-1) +
								readbuffer(m_unactiveWaterData, m_width, x,y+1) +
								readbuffer(m_unactiveWaterData, m_width, x-1,y-1) +
								readbuffer(m_unactiveWaterData, m_width, x+1,y-1) +
								readbuffer(m_unactiveWaterData, m_width, x-1,y+1) +
								readbuffer(m_unactiveWaterData, m_width, x+1,y+1)
								);

			value /= 6.0f;
			value -= readbuffer(m_activeWaterData, m_width, x,y);
			value -= (value * m_damping);

			value = in_between((float)-m_maxWaterDifference, value, (float)m_maxWaterDifference);

			//value = value >> 1;
			//value = value - 0;//readbuffer(m_activeWaterData, m_width, x, y);
			//value -= (value >> 10);
			//value = in_between(-m_maxWaterDifference, value, m_maxWaterDifference);

			m_activeWaterData[y*m_width+x] = static_cast<waterdata_T>(value);
		}
	}

	m_bitmapIsUpdated = false;
}

void CWaterAnimation::setDamping(float damping)
// pre:  isValid()
// post: the water's damping has been set tot damping.
{
	//assert(isValid());

	m_damping = damping;
}

float CWaterAnimation::getDamping()const
// pre:  isValid()
// post: the water's damping has been returned.
{
//	assert(isValid());

	return m_damping;
}

void CWaterAnimation::setMaxWaterDifference(long difference)
// pre:  isValid()
// post: the maximum difference of the water's height has been set to difference.
{
//	assert(isValid());

	m_maxWaterDifference = difference;
}

long CWaterAnimation::getMaxWaterDifference()const
// pre:  isValid()
// post: the maximum difference of the water's height has been returned
{
	return m_maxWaterDifference;
}
