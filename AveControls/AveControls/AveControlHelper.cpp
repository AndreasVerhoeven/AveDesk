// AveControlHelper.cpp: implementation of the CAveControlHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "AveControlHelper.h"
#include "AveConstants.h"
#include "graphics_funcs/shadowproperties.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAveControlHelper::CAveControlHelper()
{

}

CAveControlHelper::~CAveControlHelper()
{

}

void CAveControlHelper::ConvertToCShadowProperties(const AVE_SHADOWPROPERTIES* in, CShadowProperties& out)
{
	if(NULL == in)
		return;

	out.SetBlurRatio(in->blurRatio);
	out.SetColor(Color((ARGB)in->color));
	out.SetExtraHeight(in->extraHeight);
	out.SetExtraWidth(in->extraWidth);
	out.SetIntensity(in->intensity);
	out.SetXOffset(in->xOffset);
	out.SetYOffset(in->yOffset);
}

void CAveControlHelper::ConvertToShadowProperties(const CShadowProperties& in, AVE_SHADOWPROPERTIES* out)
{
	if(NULL == out)
		return;

	out->blurRatio = in.GetBlurRatio();
	out->color = (DWORD) in.GetColor().GetValue();
	out->extraHeight = in.GetExtraHeight();
	out->extraWidth = in.GetExtraWidth();
	out->intensity = in.GetIntensity();
	out->xOffset = in.GetXOffset();
	out->yOffset = in.GetYOffset();
}

size_t CAveControlHelper::CopyWStringToWCHARPtr(const std::wstring& ws, WCHAR* output, size_t bufLength)
{
	size_t bufferSizeForChars = bufLength - 1;
	size_t maxCharsToCopy = min(ws.length(), bufferSizeForChars);

	const WCHAR* txtBufPointer = ws.c_str();

	size_t i = 0;
	while(i++ < maxCharsToCopy)
		*output++ = *txtBufPointer++;
		
	*output = L'\0';

	return maxCharsToCopy + 1;
}

size_t CAveControlHelper::CopyStringToCHARPtr(const std::string& ws, char* output, size_t bufLength)
{
	size_t bufferSizeForChars = bufLength - 1;
	size_t maxCharsToCopy = min(ws.length(), bufferSizeForChars);

	const char* txtBufPointer = ws.c_str();

	size_t i = 0;
	while(i++ < maxCharsToCopy)
		*output++ = *txtBufPointer++;
		
	*output = L'\0';

	return maxCharsToCopy + 1;
}