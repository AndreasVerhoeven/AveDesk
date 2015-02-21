// AveEditBox.cpp: implementation of the CAveEditBox class.
//
//////////////////////////////////////////////////////////////////////

#include "AveEditBox.h"
#include "AveConstants.h"
#include "AveControlHelper.h"

#define AVE_EDITBOXMAXWIDTH 0x10000

const int CAveEditBox::CARET_TIMER_ID = 49339;
const int CAveEditBox::CARET_TIMER_INTERVAL = 1200;

const int CAveEditBox::SCROLL_TIMER_ID = 50593;
const int CAveEditBox::SCROLL_TIMER_INTERVAL = 50;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAveEditBox::CAveEditBox(IAveControlContainer* container, std::string name) :
	IAveControl(container, name),
	editHwnd(NULL),
	textColor(255, 0, 0, 0),
	selBgColor(255, 5, 9, 171),
	font(new Font(L"Lucida Sans Unicode", 19, FontStyleBold )),
	oldCursor(NULL),
	oldLong(0),
	bmp(NULL),
	caretVisible(FALSE),
	mouseIsDown(FALSE),
	movedCursor(FALSE),
	dummyBmp(1,1),
	noSmoothing(FALSE),
	scrollX(0),
	scrollTimerIsRunning(FALSE),
	isPassword(FALSE),
	onlyLoseFocusOnDeskletOut(FALSE),
	onChangeCallback(NULL),
	onEnterCallback(NULL),
	isInRightClickMenu(FALSE),
	dontHandleMouse(FALSE)
{
	mouseDownPoint.x = 0;
	mouseDownPoint.y = 0;
	mousePt.x = 0;
	mousePt.y = 0;

	DeskletLayerSetNoAlphaBlendedHitTesting(GetContainer()->GetDeskletHandle(), GetLayerId(), TRUE);

	selBgColor.SetFromCOLORREF(GetSysColor(COLOR_HIGHLIGHT));

	DWORD flags = ES_AUTOHSCROLL | ES_WANTRETURN;

	Graphics g(GetContainer()->GetDeskletHandle());
	editHwnd = CreateWindow(TEXT("EDIT"), "", flags, 0, 0, (int)font->GetHeight(&g), 20000, GetContainer()->GetDeskletHandle(), NULL, HINST_THISCOMPONENT, NULL);
}

void CAveEditBox::SetOnEditBoxChangeCallback(void* data, OnEditBoxChangeCallback func)
{
	changeCallbackData = data;
	onChangeCallback = func;
}

BOOL CAveEditBox::MouseRightClick(const POINT* pt, const SIZE* s, DWORD keys)
{
	isInRightClickMenu = TRUE;

	WPARAM wParam = (WPARAM)editHwnd;
	POINT pos = {0};
	GetCursorPos(&pos);
	LPARAM lParam = MAKELPARAM(pos.x, pos.y);
	::SendMessage(editHwnd, WM_CONTEXTMENU, wParam, lParam);
	isInRightClickMenu = FALSE;

	HWND foregroundWindow = GetForegroundWindow();
	WCHAR className[1024] = {0};
	GetClassNameW(foregroundWindow, className, 1024);
	if(foregroundWindow != GetContainer()->GetDeskletHandle() && foregroundWindow != editHwnd)
		KillFocus(NULL);
	else
		Update();

	return TRUE;
}

void CAveEditBox::SetIsPassword(BOOL val)
{
	if(val != isPassword)
	{
		isPassword = val;
		Update();
	}
}

BOOL CAveEditBox::GetIsPassword()const
{
	return isPassword;
}

std::wstring CAveEditBox::GetDrawingString()
{
	if(GetIsPassword())
		return std::wstring(SendMessage(editHwnd, WM_GETTEXTLENGTH, 0L, 0L), (wchar_t)0x25CF);
	else
		return GetText();
}


CAveEditBox::~CAveEditBox()
{
	DestroyWindow(editHwnd);
	delete font;
}

Font* CAveEditBox::GetFont()
{
	return font;
}

void CAveEditBox::SetFont(Font* f)
{
	delete font;
	font = f;
	Update();
}

Color CAveEditBox::GetTextColor()const
{
	return textColor;
}

void  CAveEditBox::SetTextColor(const Color& c)
{
	textColor = c;
	Update();
}

Color CAveEditBox::GetSelBgColor()const
{
	return selBgColor;
}

void  CAveEditBox::SetSelBgColor(const Color& c)
{
	selBgColor = c;
	Update();
}

void CAveEditBox::SetReadOnly(BOOL val)
{
	SendMessage(editHwnd, EM_SETREADONLY, (WPARAM)val, 0L);
}

BOOL CAveEditBox::GetReadOnly()const
{
	return GetWindowLong(editHwnd, GWL_STYLE) & ES_READONLY;
}

void CAveEditBox::SetOnlyLoseFocusOnDeskletOut(BOOL val)
{
	onlyLoseFocusOnDeskletOut = val;
}

BOOL CAveEditBox::GetOnlyLoseFocusOnDeskletOut()const
{
	return onlyLoseFocusOnDeskletOut;
}

DWORD_PTR CAveEditBox::InternalMessage(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& handled)
{
	DWORD_PTR res = IAveControl::InternalMessage(msg, wParam, lParam, handled);
	if(handled)
		return res;

	switch(msg)
	{
		case AV_SET_SELRANGE:
			SetSelRange((int)wParam, (int)lParam);
			handled = TRUE;
			return 0;
		break;

		case AV_GET_SETRANGE:
			if(NULL != wParam)
				*((int*)(wParam)) = GetSelStart();

			if(NULL != lParam)
				*((int*)(lParam)) = GetSelEnd();

			handled = TRUE;

			return 0;
		break;

		case AV_HAS_SELECTION:
			handled = TRUE;
			return HasSelection() ? TRUE : FALSE;
		break;

		case AV_NO_SELECTION:
			handled = TRUE;
			NoSelection();
			return 0;
		break;

		case AV_SET_TEXTCOLOR:
			handled = TRUE;
			SetTextColor(Color((ARGB)lParam));
			return 0;
		break;

		case AV_GET_TEXTCOLOR:
			handled = TRUE;
			return (DWORD_PTR) GetTextColor().GetValue();
		break;

		case AV_SET_SELBGCOLOR:
			handled = TRUE;
			SetSelBgColor(Color((ARGB)lParam));
			return 0;
		break;

		case AV_GET_SELBGCOLOR:
			handled = TRUE;
			return (DWORD_PTR) GetSelBgColor().GetValue();
		break;


		case AV_SET_FONT:
			handled = TRUE;

			if(NULL == lParam)
			{
				return FALSE;
			}
			else
			{
				AVE_FONT* af = (AVE_FONT*)lParam;
				SetFont(new Font(af->faceName, (REAL)af->size, af->style));

				return TRUE;
			}
		break;

		case AV_GET_FONT:
			handled = TRUE;

			if(NULL == lParam)
			{
				return FALSE;
			}
			else
			{
				AVE_FONT* af = (AVE_FONT*)lParam;

				Font* f = GetFont();

				af->style = f->GetStyle();
				af->size = (int)f->GetSize();
				FontFamily family;
				f->GetFamily(&family);
				family.GetFamilyName(af->faceName);

				return TRUE;
			}
		break;

		case AV_SET_TEXT:
			handled = TRUE;
			if(NULL == lParam)
			{
				return FALSE;
			}
			else
			{
				SetText((WCHAR*)lParam);
				return TRUE;
			}
		break;

		case AV_GET_TEXT:
			handled = TRUE;
			if(NULL == lParam)
			{
				return FALSE;
			}
			else
			{	
				return CAveControlHelper::CopyWStringToWCHARPtr(GetText(), (WCHAR*)lParam, (size_t)wParam);
			}
		break;

		case AV_GET_TEXT_LEN:
			handled = TRUE;
			return GetText().length();
		break;

		case AV_DELETE_SELECTION:
			handled = TRUE;
			DeleteSelection();
			return 0;
		break;

		case AV_SET_ONENTERCALLBACK:
			handled = TRUE;
			this->SetOnEnterCallback((void*)wParam, (OnEditBoxEnterCallback)lParam);
			return 0;
		break;

		case AV_SET_ONLY_LOSE_FOCUS_ON_DESKLET_OUT:
			handled = TRUE;
			SetOnlyLoseFocusOnDeskletOut(wParam != FALSE);
			return 0;
		break;

		case AV_GET_ONLY_LOSE_FOCUS_ON_DESKLET_OUT:
			handled = TRUE;
			return (DWORD_PTR) GetOnlyLoseFocusOnDeskletOut();
		break;

		case AV_SET_READONLY:
			handled = TRUE;
			SetReadOnly((BOOL)wParam);
			return 0;
		break;

		case AV_GET_READONLY:
			handled = TRUE;
			return (DWORD_PTR) GetReadOnly();
		break;

		case AV_SET_ISPASSWORD:
			handled = TRUE;
			SetIsPassword((BOOL)wParam);
			return 0;
		break;

		case AV_GET_ISPASSWORD:
			handled = TRUE;
			return (DWORD_PTR) GetIsPassword();
		break;

		case AV_SET_CHANGECALLBACK:
			handled = TRUE;
			SetOnEditBoxChangeCallback((void*)wParam, (OnEditBoxChangeCallback)lParam);
			return 0;
		break;

		default:
			handled = FALSE;
			return 0;
	}
}

BOOL CAveEditBox::MouseOn(const POINT* pt, const SIZE* s)
{
	HCURSOR beam = LoadCursor(NULL, IDC_IBEAM);
	oldCursor = DeskletSetCursor(GetContainer()->GetDeskletHandle(), beam);

	return FALSE;
}

BOOL CAveEditBox::MouseOut()
{
	DeskletSetCursor(GetContainer()->GetDeskletHandle(), oldCursor);

	return FALSE;
}

void CAveEditBox::Update()
{
	if(IsLocked())
		return;

	RECT r = GetRect();
	RectF visibleRect(0.0f,0.0f, (REAL)(r.right - r.left), (REAL)(r.bottom - r.top));
	RectF layoutRect((REAL)-GetScrollX(), 0.0f, (REAL)AVE_EDITBOXMAXWIDTH, (REAL)(r.bottom - r.top));

	std::wstring text = GetDrawingString();

	BOOL createdNewBmp = FALSE;
	if(NULL == bmp || (int)bmp->GetWidth() != (int)visibleRect.Width || (int)bmp->GetHeight() != (int)visibleRect.Height)
	{
		bmp = new Bitmap((int)visibleRect.Width, (int)visibleRect.Height);
		createdNewBmp = TRUE;
	}


	Graphics g(bmp);
	g.SetSmoothingMode(SmoothingModeAntiAlias);
	g.SetInterpolationMode(InterpolationModeHighQualityBicubic);

	if(!createdNewBmp)
	{
		CompositingMode oldMode = g.GetCompositingMode();
		g.SetCompositingMode(CompositingModeSourceCopy);

		Color eraserColor(0,0,0,0);
		g.Clear(eraserColor);

		g.SetCompositingMode(oldMode);
	}

	if(!GetNoSmoothing())
	{
		g.SetTextRenderingHint( TextRenderingHintAntiAlias);
		g.SetTextContrast(100);
	}
	else
	{
		g.SetTextRenderingHint(TextRenderingHintSystemDefault);
	}

	StringFormat format;
	SolidBrush fgBrush(textColor);
	SolidBrush opposideBrush(Color(textColor.GetA(), 255 - textColor.GetR(), 255 - textColor.GetG(), 255 - textColor.GetB()));

	int selStart = GetSelStart();
	int selEnd = GetSelEnd();
	int caretPos = GetCaretPos();
	if(HasSelection() && text.length() > 0)
	{
		CharacterRange range(selStart, selEnd - selStart);
		format.SetFormatFlags(StringFormatFlagsMeasureTrailingSpaces);
		format.SetMeasurableCharacterRanges(1, &range);

		UINT count = format.GetMeasurableCharacterRangeCount();
		if(count > 0)
		{
			Region region;
			g.MeasureCharacterRanges(text.c_str(), -1, font, layoutRect, &format, 1, &region);
			SolidBrush gray(Color(255,200,200,200));
			SolidBrush selBgBrush(selBgColor);
			g.FillRegion(&selBgBrush, &region);

			g.SetClip(&region, CombineModeReplace);
			g.DrawString(text.c_str(), -1, font, layoutRect, &format, &opposideBrush);
			g.SetClip(layoutRect, CombineModeReplace);
			g.SetClip(&region, CombineModeExclude);
		}
	}

	 g.DrawString(text.c_str(), -1, font, layoutRect, &format, &fgBrush);

	if(caretVisible)
	{
		DWORD pos = caretPos;

		if(pos >= text.length() && text.length() >= 1)
			pos = text.length() - 1;
		CharacterRange range(pos, 1);
		format.SetFormatFlags(StringFormatFlagsMeasureTrailingSpaces);
		format.SetMeasurableCharacterRanges(1, &range);
		
		UINT count = format.GetMeasurableCharacterRangeCount();
		if(count > 0)
		{
			g.SetClip(layoutRect, CombineModeReplace);

			std::wstring txt = text;
			if(txt.length() < 1)
			{
				range.First = 0;
				txt = L"A"; // dummy character
			}

			Region region;
			g.MeasureCharacterRanges(txt.c_str(), -1, font, layoutRect, &format, 1, &region);
			Rect bounds;
			region.GetBounds(&bounds, &g);

			Color caretColor = textColor;
			if(HasSelection())
				caretColor = Color(selBgColor.GetA(), 255 - selBgColor.GetR(), 255 - selBgColor.GetG(), 255 - selBgColor.GetB());

			SolidBrush caretBrush(caretColor);

			Pen caretPen(&caretBrush, 2.0f);

			if(caretPos != pos)
				bounds.X += bounds.Width;
			g.DrawLine(&caretPen, bounds.X, bounds.Y, bounds.X, bounds.Y + bounds.Height);

		}
	}


	DeskletLayerSetImage(GetContainer()->GetDeskletHandle(), GetLayerId(), GetRedraw(TRUE), bmp, FALSE);
}

int CAveEditBox::GetSelStart()const
{
	DWORD st;
	DWORD end;
	SendMessage(editHwnd, EM_GETSEL, (WPARAM)&st, (LPARAM)&end);
	return st;

}

int CAveEditBox::GetSelEnd()const
{
	DWORD st;
	DWORD end;
	SendMessage(editHwnd, EM_GETSEL, (WPARAM)&st, (LPARAM)&end);

	return end;
}

void CAveEditBox::SetSelRange(int start, int end)
{
	int selStart = GetSelStart();
	int selEnd   = GetSelEnd();

	if(selStart != start || selEnd != end)
	{
		selStart = start;
		selEnd = end;

		SendMessage(editHwnd, EM_SETSEL, selStart, selEnd);

		Update();
	}
}

void CAveEditBox::SetSelStart(int index)
{
	int selStart = GetSelStart();
	int selEnd   = GetSelEnd();

	if(selStart != index)
	{
		selStart = index;
		SendMessage(editHwnd, EM_SETSEL, selStart, selEnd);
		Update();
	}	
}

void CAveEditBox::SetSelEnd(int index)
{
	int selStart = GetSelStart();
	int selEnd   = GetSelEnd();

	if(selEnd != index)
	{
		selEnd = index;
		SendMessage(editHwnd, EM_SETSEL, selStart, selEnd);
		Update();
	}	
}

bool CAveEditBox::HasSelection()const
{
	int selStart = GetSelStart();
	int selEnd   = GetSelEnd();

	return selStart != selEnd;
}

std::wstring CAveEditBox::GetText()
{
	DWORD strLen = SendMessageW(editHwnd, WM_GETTEXTLENGTH, 0, 0) + 1;
		
	WCHAR* str = new WCHAR[strLen];
	str[0] = L'\0';
	SendMessageW(editHwnd, WM_GETTEXT, strLen, (LPARAM)str);

	std::wstring text = str;
	delete[] str;

	return text;
}

void CAveEditBox::SetText(const std::wstring& str)
{
	std::wstring text = str;
	/*
	while(text.find(L'\r', 0))
		text = text.replace(L'\r', L'');*/

	SendMessageW(editHwnd, WM_SETTEXT, 0, (WPARAM)text.c_str());
	Update();
}

std::string CAveEditBox::GetTextA()
{
	std::string textA;

	DWORD strLen = SendMessage(editHwnd, WM_GETTEXTLENGTH, 0, 0) + 1;
		
	char* str = new char[strLen];
	str[0] = '\0';
	SendMessage(editHwnd, WM_GETTEXT, strLen, (LPARAM)str);

	textA = str;
	delete[] str;

	return textA;
}

void CAveEditBox::SetTextA(const std::string& str)
{
	SendMessage(editHwnd, WM_SETTEXT, 0, (WPARAM)str.c_str());
	std::wstring text = GetText();

	Update();

}

inline bool _AvePointInRect(Rect& r,const POINT& p)
{
	return (p.x > r.X && p.x < (r.X+r.Width) && p.y > (r.Y) && p.y < (r.Y + r.Height));
}


UINT CAveEditBox::GetSelIndexFromPoint(const POINT* pt, bool noSideCorrections)
{
	std::wstring text(GetDrawingString());
	if(text.length () == 0)
		return 0;

	POINT p = *pt;

	RECT r = GetRect();
	RectF layoutRect((REAL)-GetScrollX(),0.0f, (REAL)AVE_EDITBOXMAXWIDTH, (REAL)(r.bottom - r.top));

	/*
	if(!noSideCorrections)
	{
		if(p.y < 0)
		{
			p.y = GetRectForCharacterIndex(0).X + 1;
		}

		if(p.y > r.bottom - r.top)
		{
			p.y = r.bottom - r.top - GetRectForCharacterIndex(0).X - 1;
		}
	}

	//p.y += GetScrollY();
	*/

	Graphics g((Image*)&dummyBmp);
	g.SetSmoothingMode(SmoothingModeAntiAlias);
	g.SetInterpolationMode(InterpolationModeHighQualityBicubic);

	
	if(!GetNoSmoothing())
	{
		g.SetTextRenderingHint(TextRenderingHintAntiAlias);
		g.SetTextContrast(100);
	}
	else
	{
		g.SetTextRenderingHint(TextRenderingHintSingleBitPerPixel);
	}

	StringFormat format;
	size_t strLen = text.length();

	UINT left = 0;
	UINT right = static_cast<UINT>(strLen);
	// binary search
	bool done = false;
	Rect boundingRect;
	while(!done)
	{
		UINT mid = (left + right) / 2;
		if(mid == 0)
			mid = 1;
		CharacterRange range(mid, 1);
		format.SetFormatFlags(StringFormatFlagsMeasureTrailingSpaces);
		format.SetMeasurableCharacterRanges(1, &range);

		Region region;
		g.MeasureCharacterRanges(text.c_str(), -1, font, layoutRect, &format, 1, &region);

		Rect& r = boundingRect;
		region.GetBounds(&boundingRect, &g);
		if(_AvePointInRect(boundingRect, p)) // direct hit, we are lucky!
		{
			done = true;
			left = mid;
		}
		else
		{
			bool isBefore = p.x < r.X;
			if(isBefore)
				right = mid;
			else
				left = mid;

			/*
			bool isOnALineThatsHigher = p.y < r.Y;
			bool isOnLineThatsLower = p.y > r.Y + r.Height;
			bool isBefore = p.y < r.Y || (p.y <= r.Y + r.Height && p.x < r.X);
			if(isBefore)
				right = mid;
			else
				left = mid;
				*/

			done = (right - left) <= 1;
		}
	}

	if(pt->x > boundingRect.X + boundingRect.Width / 2)
		left += 1;

	return left;
}

BOOL CAveEditBox::MakeSureCaretIsVisible()
{
	int caretPos = GetCaretPos();
	Rect caretRect = GetRectForCharacterIndex(caretPos);

	RECT r = GetRect();
	int width = r.right - r.left;

	if(caretRect.X + 3 < 0)
	{
		return SetScrollX(GetScrollX() + caretRect.X);
	}
	else if(caretRect.X + caretRect.Width > width)
	{
		return SetScrollX(GetScrollX() + caretRect.X + caretRect.Width - width);
	}

	return FALSE;
}

Rect CAveEditBox::GetRectForCharacterIndex(int index)
{
	RECT r = GetRect();
	RectF layoutRect((REAL)-GetScrollX(), 0.0f, (REAL)AVE_EDITBOXMAXWIDTH, (REAL)(r.bottom - r.top));

	Graphics g((Image*)&dummyBmp);
	g.SetSmoothingMode(SmoothingModeAntiAlias);
	g.SetInterpolationMode(InterpolationModeHighQualityBicubic);
	if(!GetNoSmoothing())
	{
		g.SetTextRenderingHint(TextRenderingHintAntiAlias);
		g.SetTextContrast(100);
	}
	else
	{
		g.SetTextRenderingHint(TextRenderingHintSingleBitPerPixel);
	}

	Rect charRect;

	std::wstring txt = GetDrawingString();
	if(index >= (int)txt.length())
		txt += L"A"; // dummy char

	StringFormat format;
	CharacterRange range(index, 1);
	format.SetFormatFlags(StringFormatFlagsMeasureTrailingSpaces);
	format.SetMeasurableCharacterRanges(1, &range);

	UINT count = format.GetMeasurableCharacterRangeCount();

	Region region;
	g.MeasureCharacterRanges(txt.c_str(), -1, font, layoutRect, &format, 1, &region);
	region.GetBounds(&charRect, &g);

	return charRect;

}

BOOL CAveEditBox::SetScrollX(int val)
{
	int newVal = val;

	if(newVal < 0)
		newVal = 0;

	RECT r = GetRect();

	Rect lastCharRect = GetRectForCharacterIndex(GetDrawingString().length());
	int textWidth = lastCharRect.X + lastCharRect.Width + GetScrollX();
	int viewWidth = r.right - r.left;
	int maxScroll = max(textWidth - viewWidth,0);
	if(newVal > maxScroll)
		newVal = maxScroll;

	if(newVal != scrollX)
	{
		int diff = newVal - scrollX;
		mouseDownPoint.x -= diff;

		scrollX = newVal;
		Update();
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

int CAveEditBox::GetScrollX()const
{
	return scrollX;
}

int CAveEditBox::GetCaretPos()const
{
	return GetSelStart();
}

void CAveEditBox::DeleteSelection()
{
	SendMessage(editHwnd, WM_CLEAR, 0, 0);
	Update();
}

void CAveEditBox::NoSelection()
{
	SetSelRange(GetSelEnd(), GetSelEnd());
}

BOOL CAveEditBox::SetFocus()
{
	//OutputDebugString("setfocus\n");
	IAveControl::SetFocus();

	caretVisible = TRUE;

	//if(!(GetKeyState(VK_LBUTTON) >= 0 || (GetSystemMetrics(SM_SWAPBUTTON) && GetKeyState(VK_RBUTTON) >= 0)))
	// FIX: we actually don't care what mousebutton was pressed, only if the focus() was activated by a mouse
	//		event
	if(!(GetKeyState(VK_LBUTTON) >= 0 && GetKeyState(VK_RBUTTON) >= 0))
		dontHandleMouse = TRUE;

	SetSelRange(0, GetDrawingString().length());

	//Update();

	SetTimer(GetContainer()->GetDeskletHandle(), CARET_TIMER_ID, CARET_TIMER_INTERVAL, NULL);

	return TRUE;
}

void CAveEditBox::KillFocus(const char* newLayer)
{
	//OutputDebugString("killfocus\n");
	if(!isInRightClickMenu && (!GetOnlyLoseFocusOnDeskletOut() || NULL == newLayer))
	{
		dontHandleMouse = FALSE;

		IAveControl::KillFocus(newLayer);

		if(HasSelection())
			NoSelection();

		KillTimer(GetContainer()->GetDeskletHandle(), CARET_TIMER_ID);
		if(caretVisible)
		{
			caretVisible = FALSE;
			Update();
		}
	}
}


BOOL CAveEditBox::MouseDown(const POINT* pt, const SIZE* s, DWORD keys)
{
	//OutputDebugString("mousedown\n");
	mouseIsDown = TRUE;
	movedCursor = FALSE;

	mouseDownPoint = *pt;

	//if(!movedCursor)
	if(!dontHandleMouse)
	{
		UINT newSel = GetSelIndexFromPoint(pt);
		caretVisible = TRUE;
		SetSelRange(newSel, newSel);
	}

	//dontHandleMouse = FALSE;

	DeskletLayerSetCapture(GetContainer()->GetDeskletHandle(), GetLayerId());

	return TRUE;
}

BOOL CAveEditBox::MouseUp(const POINT* pt, const SIZE* s, DWORD keys)
{
	//OutputDebugString("mouseup\n");
	
	if(scrollTimerIsRunning)
	{
		scrollTimerIsRunning = FALSE;
		KillTimer(GetContainer()->GetDeskletHandle(), SCROLL_TIMER_ID);
	}

	DeskletLayerReleaseCapture(GetContainer()->GetDeskletHandle());

	dontHandleMouse = FALSE;
	movedCursor = FALSE;
	mouseIsDown = FALSE;
	return TRUE;
}

BOOL CAveEditBox::MouseWheelForward(const POINT* pt, const SIZE* s, DWORD keys, int numOfScrolls)
{

	return TRUE;
}

BOOL CAveEditBox::MouseWheelBackward(const POINT* pt, const SIZE* s, DWORD keys, int numOfScrolls)
{
	return TRUE;
}


BOOL CAveEditBox::MouseClick(const POINT* pt, const SIZE* s, DWORD keys)
{
	//OutputDebugString("mouseclick\n");
	if(hasFocus && !dontHandleMouse)
	{
		if(!movedCursor)
		{
			UINT newSel = GetSelIndexFromPoint(pt);
			caretVisible = TRUE;
			SetSelRange(newSel, newSel);
		}
	}

	return TRUE;
}


BOOL CAveEditBox::MouseMove(const POINT* pt, const SIZE* s)
{
	//OutputDebugString("mousemove\n");
	if(mouseIsDown && !dontHandleMouse)
	{
		mousePt = *pt;

		UINT newStart = GetSelIndexFromPoint(pt);
		UINT newEnd   = GetSelIndexFromPoint(&mouseDownPoint, true);

		movedCursor = TRUE;

		caretVisible = TRUE;
		SetSelRange(newStart, newEnd);


		RECT r = GetRect();
		OffsetRect(&r, -r.left, -r.top);
		if(!PtInRect(&r, *pt) && !scrollTimerIsRunning)
		{
			RECT r = GetRect();
			int width = r.right - r.left;
			if(mousePt.x < 0)
			{
				SetScrollX(GetScrollX() + mousePt.x);

			}else if(mousePt.x > width)
			{
				int diff = mousePt.x - width;
				SetScrollX(GetScrollX() + diff);
			}

			//scrollTimerIsRunning = TRUE;
			//SetTimer(GetContainer()->GetDeskletHandle(), SCROLL_TIMER_ID, SCROLL_TIMER_INTERVAL, NULL);
		}
		else if(scrollTimerIsRunning)
		{
			//scrollTimerIsRunning = FALSE;
			//KillTimer(GetContainer()->GetDeskletHandle(), SCROLL_TIMER_ID);
		}
	}

	return FALSE;
}

void CAveEditBox::SetOnEnterCallback(void* data, OnEditBoxEnterCallback func)
{
	callbackData = data;
	onEnterCallback = func;
}

void CAveEditBox::SetNoSmoothing(BOOL v)
{
	if(noSmoothing != v)
	{
		noSmoothing = v;
		Update();
	}
}

BOOL CAveEditBox::GetNoSmoothing()const
{
	return noSmoothing;
}


BOOL CAveEditBox::HandleCharMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
	int oldSelStart = GetSelStart();
	int oldSelEnd   = GetSelEnd();
	std::wstring oldText = GetText();

	SendMessageW(editHwnd, msg, wParam, lParam);

	bool needsUpdate = oldSelStart != GetSelStart() || oldSelEnd != GetSelEnd() || oldText != GetText();

	if(!MakeSureCaretIsVisible() && needsUpdate)
	{
		caretVisible = TRUE;
		Update();
	}

	return TRUE;
}

BOOL CAveEditBox::Message(UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(WM_COMMAND == msg)
	{
		DWORD wNotifyCode = HIWORD(wParam); // notification code 
		DWORD wID = LOWORD(wParam);         // item, control, or accelerator identifier 
		HWND hwndCtrl = (HWND) lParam;      // handle of control 

		if(hwndCtrl == editHwnd && wNotifyCode == EN_UPDATE)
		{
			if(onChangeCallback != NULL)
				onChangeCallback(changeCallbackData, this);

			return TRUE;
		}
	
	}
	else if(WM_KILLFOCUS == msg)
	{
		if(HasFocus() && GetOnlyLoseFocusOnDeskletOut())
			KillFocus(NULL);
	}
	
	/*
	else if(WM_KEYDOWN == msg && HasFocus())
	{
		CHAR ch = (TCHAR) wParam;

		int oldSelStart = GetSelStart();
		int oldSelEnd   = GetSelEnd();
		std::wstring oldText = GetText();
		
		SendMessage(editHwnd, WM_KEYDOWN, wParam, lParam);

		bool needsUpdate = oldSelStart != GetSelStart() || oldSelEnd != GetSelEnd() || oldText != GetText();
		if(!MakeSureCaretIsVisible() && needsUpdate)
		{
			caretVisible = TRUE;
			Update();
		}

		return TRUE;
	}

	if(WM_DEADCHAR == msg && HasFocus())
	{
		int oldSelStart = GetSelStart();
		int oldSelEnd   = GetSelEnd();
		std::wstring oldText = GetText();

		SendMessage(editHwnd, WM_DEADCHAR, wParam, lParam);

		bool needsUpdate = oldSelStart != GetSelStart() || oldSelEnd != GetSelEnd() || oldText != GetText();

		if(!MakeSureCaretIsVisible() && needsUpdate)
		{
			caretVisible = TRUE;
			Update();
		}

		return TRUE;		
	}
	else if( 0x0109 == msg && HasFocus())
	{
		int oldSelStart = GetSelStart();
		int oldSelEnd   = GetSelEnd();
		std::wstring oldText = GetText();

		SendMessage(editHwnd,  0x0109, wParam, lParam);

		bool needsUpdate = oldSelStart != GetSelStart() || oldSelEnd != GetSelEnd() || oldText != GetText();

		if(!MakeSureCaretIsVisible() && needsUpdate)
		{
			caretVisible = TRUE;
			Update();
		}

		return TRUE;		
	}
	else if(WM_CHAR == msg && HasFocus())
	{

		if(VK_RETURN == wParam)
		{
			if(onEnterCallback != NULL)
				onEnterCallback(callbackData, this);

			return TRUE;
		}

		int oldSelStart = GetSelStart();
		int oldSelEnd   = GetSelEnd();
		std::wstring oldText = GetText();

		SendMessage(editHwnd, WM_CHAR, wParam, lParam);

		bool needsUpdate = oldSelStart != GetSelStart() || oldSelEnd != GetSelEnd() || oldText != GetText();

		if(!MakeSureCaretIsVisible() && needsUpdate)
		{
			caretVisible = TRUE;
			Update();
		}

		return TRUE;
	}*/

	if(HasFocus())
	{
		switch(msg)
		{
			case WM_CHAR:
				if(VK_RETURN == wParam)
				{
					if(onEnterCallback != NULL)
					onEnterCallback(callbackData, this);
	
					return TRUE;
				}
			case 0x0109:
			case WM_DEADCHAR:
			case WM_KEYDOWN:
			case WM_KEYUP:
			case WM_SYSKEYDOWN:
			case WM_SYSKEYUP:
			case WM_SYSCHAR:
			case WM_SYSDEADCHAR:
				return HandleCharMessage(msg, wParam, lParam);
		}

		if(WM_TIMER == msg && CARET_TIMER_ID == wParam && HasFocus())
		{
			caretVisible = !caretVisible;
			Update();

			return TRUE;
		}
		else if(WM_TIMER == msg && SCROLL_TIMER_ID == wParam)
		{
			if(mousePt.y < 0) // scroll up
			{
				int diff = mousePt.x / 2;
				SetScrollX(GetScrollX() + mousePt.x);
			}
			else
			{
				RECT r = GetRect();
				int width = r.right - r.left;
				if(mousePt.x > width)
				{
					int diff = mousePt.x - width;
					SetScrollX(GetScrollX() + diff);
				}
			}
		}
		else if(WM_KILLFOCUS == msg)
		{

		}
	}

	return FALSE;
}
