// AveTextField.cpp: implementation of the CAveTextField class.
//
//////////////////////////////////////////////////////////////////////

#include "aveconstants.h"
#include "AveTextField.h"
#include "AveControlHelper.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const UINT CAveTextField::CARET_TIMER_ID = 3948;
const UINT CAveTextField::CARET_TIMER_INTERVAL = 1020;

const UINT CAveTextField::SCROLL_TIMER_ID = 5943;
const UINT CAveTextField::SCROLL_TIMER_INTERVAL = 50;

const UINT CAveTextField::AVE_TEXTBOXMAXHEIGHT = 0x100000;

EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)

CAveTextField::CAveTextField(IAveControlContainer* container, std::string name, BOOL noReturns) : 
	IAveControl(container, name),
		mouseIsDown(FALSE),
		mouseWentOut(FALSE),
		movedCursor(FALSE),
		caretVisible(FALSE),
		textColor(255, 0, 0, 0),
		selBgColor(255, 5, 9, 171),
		font(new Font(L"Lucida Sans Unicode", 12, FontStyleBold )),
		dummyBmp(1,1),
		oldCursor(NULL),
		oldLong(0),
		editHwnd(NULL),
		onlyLoseFocusOnDeskletOut(FALSE),
		noSmoothing(FALSE),
		noReturns(noReturns),
		callbackData(NULL),
		onEnterCallback(NULL),
		scrollY(0),
		scrollTimerIsRunning(FALSE),
		didScrollOnSide(FALSE),
		onScrollbarNotify(NULL),
		scrollBar(NULL),
		scrollbarWasVisible(FALSE),
		noScrollSet(FALSE),
		bmp(NULL),
		scrollOnlyWholeLines(TRUE),
		onChangeCallback(NULL),
		isInRightClickMenu(FALSE)

{

	mouseDownPoint.x = 0;
	mouseDownPoint.y = 0;
	mousePt.x = 0;
	mousePt.y = 0;

	selBgColor.SetFromCOLORREF(GetSysColor(COLOR_HIGHLIGHT));
	DeskletLayerSetNoAlphaBlendedHitTesting(GetContainer()->GetDeskletHandle(), GetLayerId(), 	TRUE);

	DWORD flags = ES_MULTILINE;
	if(!noReturns)
		flags |= ES_WANTRETURN;
	editHwnd = CreateWindow(TEXT("EDIT"), "", flags, 0, 0, 2000, 10000, GetContainer()->GetDeskletHandle(), NULL, HINST_THISCOMPONENT, NULL);
}


BOOL CAveTextField::MouseRightClick(const POINT* pt, const SIZE* s, DWORD keys)
{
	isInRightClickMenu = TRUE;

	WPARAM wParam = (WPARAM)editHwnd;
	POINT pos = {0};
	GetCursorPos(&pos);
	LPARAM lParam = MAKELPARAM(pos.x, pos.y);
	::SendMessage(editHwnd, WM_CONTEXTMENU, wParam, lParam);
	isInRightClickMenu = FALSE;
	MakeSureCaretIsVisible();
	Update();

	HWND foregroundWindow = GetForegroundWindow();
	WCHAR className[1024] = {0};
	GetClassNameW(foregroundWindow, className, 1024);
	if(foregroundWindow != GetContainer()->GetDeskletHandle() && foregroundWindow != editHwnd)
		KillFocus(NULL);

	return TRUE;
}

void CAveTextField::SetReadOnly(BOOL val)
{
	SendMessage(editHwnd, EM_SETREADONLY, (WPARAM)val, 0L);
}

BOOL CAveTextField::GetReadOnly()const
{
	return GetWindowLong(editHwnd, GWL_STYLE) & ES_READONLY;
}

DWORD_PTR CAveTextField::InternalMessage(UINT msg, WPARAM wParam, LPARAM lParam, BOOL& handled)
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

		case AV_SET_SCROLLBAR:
			handled = TRUE;
			SetScrollbar(reinterpret_cast<CAveVerticalScrollBar*>(lParam));
			return 0;
		break;

		case AV_SET_SCROLLBARCALLBACK:
			handled = TRUE;
			SetOnScrollbarNotifyCallback((void*)wParam, (OnScrollbarNotify)lParam);
			return 0;
		break;

		case AV_SET_ONENTERCALLBACK:
			handled = TRUE;
			this->SetOnEnterCallback((void*)wParam, (OnEnterCallback)lParam);
			return 0;
		break;

		case AV_SET_SMOOTHSCROLLING:
			handled = TRUE;
			this->SetScrollOnlyWholeLines((!(BOOL)wParam != FALSE));
			return 0;
		break;

		case AV_GET_SMOOTHSCROLLING:
			handled = TRUE;
			return (DWORD_PTR) GetScrollOnlyWholeLines();
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

		case AV_SET_CHANGECALLBACK:
			handled = TRUE;
			SetOnTextFieldChangeCallback((void*)wParam, (OnTextFieldChangeCallback)lParam);
			return 0;
		break;

		default:
			handled = FALSE;
			return 0;
	}
}

CAveTextField::~CAveTextField()
{
	DeskletLayerSetImage(GetContainer()->GetDeskletHandle(), GetLayerId(), FALSE, NULL, TRUE);
	delete font;
	DestroyWindow(editHwnd);

	delete bmp;
}

BOOL CAveTextField::GetScrollOnlyWholeLines()const
{
	return scrollOnlyWholeLines;
}

void CAveTextField::SetScrollOnlyWholeLines(BOOL val)
{
	scrollOnlyWholeLines = val;
}

void CAveTextField::SetOnEnterCallback(void* data, OnEnterCallback func)
{
	callbackData = data;
	onEnterCallback = func;
}

BOOL CAveTextField::NeedScrollBar()
{
	RECT r = GetRect();
	int height = r.bottom - r.top;
	Rect totalRect = this->GetRectForCharacterIndex(GetText().length());
	return totalRect.Y + totalRect.Height > height;
}

void CAveTextField::SetScrollbar(CAveVerticalScrollBar* scrollbar)
{
	if(scrollBar != NULL)
	{
		scrollBar->SetOnScroll(NULL, NULL);
	}

	scrollBar = scrollbar;
	if(scrollBar)
	{
		scrollBar->SetOnScroll(this, OnScrollNotify);
	}

	UpdateScrollbar();
}

void CAveTextField::SetOnScrollbarNotifyCallback(void* data, OnScrollbarNotify func)
{
	callbackDataScrollbar = data;
	onScrollbarNotify = func;
}

void CAveTextField::SetNoSmoothing(BOOL v)
{
	if(noSmoothing != v)
	{
		noSmoothing = v;
		Update();
	}
}

BOOL CAveTextField::MakeSureCaretIsVisible()
{
	int caretPos = GetCaretPos();
	Rect caretRect = GetRectForCharacterIndex(caretPos);

	RECT r = GetRect();
	int height = r.bottom - r.top;
	int top = GetScrollY();

	if(caretRect.Y + 3 < 0)
	{
		return SetScrollY(caretRect.Y + GetScrollY());
	}
	else if(caretRect.Y + caretRect.Height > height)
	{
		return SetScrollY(top + caretRect.Y + caretRect.Height - height);
	}

	return FALSE;
}

BOOL CAveTextField::GetNoSmoothing()const
{
	return noSmoothing;
}

void CAveTextField::SetOnlyLoseFocusOnDeskletOut(BOOL val)
{
	onlyLoseFocusOnDeskletOut = val;
}

BOOL CAveTextField::GetOnlyLoseFocusOnDeskletOut()const
{
	return onlyLoseFocusOnDeskletOut;
}

Font* CAveTextField::GetFont()
{
	return font;
}

void CAveTextField::SetFont(Font* f)
{
	delete font;
	font = f;
	Update();
}

Color CAveTextField::GetTextColor()const
{
	return textColor;
}

void  CAveTextField::SetTextColor(const Color& c)
{
	textColor = c;
	Update();
}

Color CAveTextField::GetSelBgColor()const
{
	return selBgColor;
}

void  CAveTextField::SetSelBgColor(const Color& c)
{
	selBgColor = c;
	Update();
}

BOOL CAveTextField::UpdateScrollbar()
{
	RECT r = GetRect();

	// get last character pos
	Rect lastCharRect = GetRectForCharacterIndex(GetText().length());
	int textHeight = lastCharRect.Height + lastCharRect.Y + scrollY;

	textHeight = (textHeight / lastCharRect.Height) * lastCharRect.Height;
	if(textHeight % lastCharRect.Height != 0)
		textHeight += lastCharRect.Height;

	int layoutHeight = r.bottom - r.top;
	int maxScroll = max(textHeight - layoutHeight, 0);
	int lines = maxScroll / lastCharRect.Height;
	if(maxScroll % lastCharRect.Height != 0)
		lines += 1;

	maxScroll = lines * lastCharRect.Height;

	BOOL dontCallUpdate = FALSE;
	int totalHeight = maxScroll + layoutHeight;
	if(scrollBar != NULL)
	{
		scrollBar->LockUpdate();

		if(onScrollbarNotify != NULL && scrollbarWasVisible != (maxScroll > 0))
		{
			dontCallUpdate = onScrollbarNotify(callbackDataScrollbar, this, maxScroll > 0, NULL);
			scrollbarWasVisible = maxScroll > 0;

			if(0 == maxScroll)
				scrollY = 0;
		}

		if(maxScroll > 0)
		{
			scrollBar->SetPos(scrollY);
			scrollBar->SetScrollScale(maxScroll);
			scrollBar->SetThumbSize(layoutHeight, totalHeight);
		}

		scrollBar->UnlockUpdate();
	}
	
	return dontCallUpdate;
}

// static
BOOL __stdcall CAveTextField::OnScrollNotify(void * data, CAveVerticalScrollBar* scrollBar)
{
	CAveTextField* textField = reinterpret_cast<CAveTextField*>(data);
	if(NULL == textField)
		return FALSE;

	textField->Lock();
	textField->noScrollSet = TRUE;
	BOOL bRes = textField->SetScrollY(scrollBar->GetPos());
	textField->noScrollSet = FALSE;
	textField->Unlock();

	return bRes;
}


BOOL CAveTextField::SetScrollY(int val)
{
	if(val < 0)
		val= 0;

	if(scrollY != val)
	{
		RECT r = GetRect();
		// get last character pos
		Rect lastCharRect = GetRectForCharacterIndex(GetText().length());
		int textHeight = lastCharRect.Height + lastCharRect.Y + scrollY;

		int h2 = (textHeight / lastCharRect.Height) * lastCharRect.Height;
		if(textHeight % lastCharRect.Height != 0)
			h2 += lastCharRect.Height;

		textHeight = h2;

		int layoutHeight = r.bottom - r.top;
		int maxScroll = max(textHeight - layoutHeight, 0);
		int lines = maxScroll / lastCharRect.Height;
		if(maxScroll % lastCharRect.Height != 0)
			lines += 1;

		maxScroll = lines * lastCharRect.Height;

		if(val > maxScroll)
			val = maxScroll;

		if(GetScrollOnlyWholeLines())
		{
			int v2 = (val / lastCharRect.Height) * lastCharRect.Height;
			if(val % lastCharRect.Height != 0)
				v2 += lastCharRect.Height;

			val = v2;
		}

		if(scrollY != val)
		{
			int diff = val - scrollY;
			mouseDownPoint.y -= diff;

			scrollY = val;

			BOOL dontCallUpdate = FALSE;
			int totalHeight = maxScroll + layoutHeight;
			if(scrollBar != NULL && !noScrollSet)
			{
				scrollBar->LockUpdate();

				if(onScrollbarNotify != NULL && scrollbarWasVisible != (maxScroll > 0))
				{
					dontCallUpdate = onScrollbarNotify(callbackDataScrollbar, this, maxScroll > 0, NULL);
					scrollbarWasVisible = maxScroll > 0;
				}

				if(maxScroll > 0)
				{
					scrollBar->SetScrollScale(maxScroll);
					scrollBar->SetPos(scrollY);
					scrollBar->SetThumbSize(layoutHeight, totalHeight);
				}

				scrollBar->UnlockUpdate();
			}

			if(!dontCallUpdate)
				Update();
			
			return TRUE;
		}
	}

	return FALSE;
}

int CAveTextField::GetScrollY()const
{
	return scrollY;
}

inline bool _AvePointInRect(Rect& r,const POINT& p)
{
	return (p.x > r.X && p.x < (r.X+r.Width) && p.y > (r.Y) && p.y < (r.Y + r.Height));
}

UINT CAveTextField::GetSelIndexFromPoint(const POINT* pt, bool noSideCorrections)
{
	std::wstring text = GetText();
	if(text.length () == 0)
		return 0;

	POINT p = *pt;

	RECT r = GetRect();
	RectF layoutRect(0.0f,0.0f, (REAL)(r.right - r.left), (REAL)AVE_TEXTBOXMAXHEIGHT);

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

	p.y += GetScrollY();

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
			bool isOnALineThatsHigher = p.y < r.Y;
			bool isOnLineThatsLower = p.y > r.Y + r.Height;
			bool isBefore = p.y < r.Y || (p.y <= r.Y + r.Height && p.x < r.X);
			if(isBefore)
				right = mid;
			else
				left = mid;

			done = (right - left) <= 1;
		}
	}

	if(pt->x > boundingRect.X + boundingRect.Width / 2)
		left += 1;

	return left;
}

void CAveTextField::NoSelection()
{
	SetSelRange(GetSelEnd(), GetSelEnd());
}

BOOL CAveTextField::MouseDown(const POINT* pt, const SIZE* s, DWORD keys)
{
	mouseIsDown = TRUE;
	movedCursor = FALSE;

	mouseDownPoint = *pt;

	if(hasFocus)
	{
		if(!movedCursor)
		{
			UINT newSel = GetSelIndexFromPoint(pt);
			caretVisible = TRUE;
			SetSelRange(newSel, newSel);
		}
	}

	DeskletLayerSetCapture(GetContainer()->GetDeskletHandle(), GetLayerId());

	return TRUE;
}

BOOL CAveTextField::MouseUp(const POINT* pt, const SIZE* s, DWORD keys)
{
	if(scrollTimerIsRunning)
	{
		scrollTimerIsRunning = FALSE;
		KillTimer(GetContainer()->GetDeskletHandle(), SCROLL_TIMER_ID);
	}

	DeskletLayerReleaseCapture(GetContainer()->GetDeskletHandle());

	mouseIsDown = FALSE;
	return TRUE;
}

BOOL CAveTextField::MouseWheelForward(const POINT* pt, const SIZE* s, DWORD keys, int numOfScrolls)
{
	if(GetScrollY() > 0)
	{
		SetScrollY(GetScrollY() - GetRectForCharacterIndex(GetText().length()).Height);
	}

	Update();

	return TRUE;
}

BOOL CAveTextField::MouseWheelBackward(const POINT* pt, const SIZE* s, DWORD keys, int numOfScrolls)
{
	SetScrollY(GetScrollY() + GetRectForCharacterIndex(GetText().length()).Height);

	return TRUE;
}


BOOL CAveTextField::MouseClick(const POINT* pt, const SIZE* s, DWORD keys)
{
	if(hasFocus)
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

BOOL CAveTextField::MouseOn(const POINT* pt, const SIZE* s)
{
	HCURSOR beam = LoadCursor(NULL, IDC_IBEAM);
	oldCursor = DeskletSetCursor(GetContainer()->GetDeskletHandle(), beam);

	return FALSE;
}

BOOL CAveTextField::MouseMove(const POINT* pt, const SIZE* s)
{
	if(mouseIsDown)
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
			scrollTimerIsRunning = TRUE;
			SetTimer(GetContainer()->GetDeskletHandle(), SCROLL_TIMER_ID, SCROLL_TIMER_INTERVAL, NULL);
		}
		else if(scrollTimerIsRunning)
		{
			scrollTimerIsRunning = FALSE;
			KillTimer(GetContainer()->GetDeskletHandle(), SCROLL_TIMER_ID);
		}
	}

	return FALSE;
}

BOOL CAveTextField::MouseOut()
{
	DeskletSetCursor(GetContainer()->GetDeskletHandle(), oldCursor);

	return FALSE;
}

void CAveTextField::DeleteSelection()
{
	SendMessage(editHwnd, WM_CLEAR, 0, 0);
	Update();
}

BOOL IsCtrlCharacter(CHAR ch)
{
	return ch <= 32;
}

int CAveTextField::GetCaretPos()const
{
	return GetSelStart();
}

void CAveTextField::SetOnTextFieldChangeCallback(void* data, OnTextFieldChangeCallback func)
{
	changeCallbackData = data;
	onChangeCallback = func;
}

BOOL CAveTextField::HandleCharMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
	int oldSelStart = GetSelStart();
	int oldSelEnd   = GetSelEnd();
	std::wstring oldText = GetText();

	SendMessageW(editHwnd, msg, wParam, lParam);
	DWORD strLen = SendMessageW(editHwnd, WM_GETTEXTLENGTH, 0, 0) + 1;
		
	/*
	WCHAR* str = new WCHAR[strLen];
	str[0] = L'\0';
	SendMessageW(editHwnd, WM_GETTEXT, strLen, (LPARAM)str);
	text = str;
	delete[] str;
	*/
	std::wstring text = GetText();

	UpdateScrollbar();
	bool needsUpdate = oldSelStart != GetSelStart() || oldSelEnd != GetSelEnd() || oldText != text;
	if(!MakeSureCaretIsVisible() && needsUpdate)
	{
		caretVisible = TRUE;
		Update();
	}

	return TRUE;
}


BOOL CAveTextField::Message(UINT msg, WPARAM wParam, LPARAM lParam)
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
	else if(WM_KEYDOWN == msg && HasFocus())
	{
		
		CHAR ch = (TCHAR) wParam;

		if(VK_UP == ch)
		{
			// semantics: 
			// - get the current line height
			// - go up 1/2 the current line height from the top
			// - the selection should be at that line
			int caretPos = GetCaretPos();
			if(caretPos > 0) // we can't go up at the first character neither
			{
				Rect r = GetRectForCharacterIndex(caretPos);

				POINT pt = {r.X, r.Y};

				int charAtUpPoint = 0;


				pt.y -= r.Height / 2;

				if(pt.y < 0)
				{
					pt.x = 0;
					pt.y = 0;
				}

				charAtUpPoint = GetSelIndexFromPoint(&pt, true);

				if(GetKeyState(VK_SHIFT) < 0)
				{
					// selection
					SetSelRange(GetSelEnd(), charAtUpPoint);
				}
				else
				{
					// cursor moving
					SetSelRange(charAtUpPoint, charAtUpPoint);
				}
			}

			caretVisible = TRUE;

			UpdateScrollbar();
			//if(!MakeSureCaretIsVisible())
				//Update();

			return TRUE;
		}
		else if(VK_DOWN == ch)
		{
			// semantics: 
			// - get the current line height
			// - go down 1/2 the current line height from the top
			// - the selection should be at that line
			int caretPos = GetCaretPos();
			if(caretPos < (int)GetText().length()) // we can't go up at the first character neither
			{
				Rect r = GetRectForCharacterIndex(caretPos);

				POINT pt = {r.X, r.Y};

				int charAtUpPoint = 0;


				pt.y += r.Height / 2 + r.Height;

				if(pt.y < 0)
				{
					pt.x = 0;
					pt.y = 0;
				}

				charAtUpPoint = GetSelIndexFromPoint(&pt, true);

				if(GetKeyState(VK_SHIFT) < 0)
				{
					// selection
					SetSelRange(GetSelEnd(), charAtUpPoint);
				}
				else
				{
					// cursor moving
					SetSelRange(charAtUpPoint, charAtUpPoint);
				}
			}

			UpdateScrollbar();
			caretVisible = TRUE;
			//if(!MakeSureCaretIsVisible())
				//Update();

			return TRUE;
		}

		
		HandleCharMessage(WM_KEYDOWN, wParam, lParam);

		return TRUE;
	}

	if(WM_CHAR == msg && HasFocus())
	{
		/*if(VK_UP == wParam || VK_DOWN == wParam)
		{
			return TRUE;
		}*/

		if(VK_RETURN == wParam && noReturns)
		{
			if(onEnterCallback != NULL)
				onEnterCallback(callbackData, this);

			return TRUE;
		}

		HandleCharMessage(WM_CHAR, wParam, lParam);

		return TRUE;
	}
	if(WM_KILLFOCUS == msg && GetOnlyLoseFocusOnDeskletOut())
	{
		if(HasFocus())
			KillFocus(NULL);
	}
	else if(WM_TIMER == msg && CARET_TIMER_ID == wParam && HasFocus())
	{
		caretVisible = !caretVisible;
		Update();

		return TRUE;
	}
	else if(WM_TIMER == msg && SCROLL_TIMER_ID == wParam && HasFocus())
	{
		if(mousePt.y < 0) // scroll up
		{
			int diff = mousePt.y / 2;
			SetScrollY(GetScrollY() + mousePt.y);
		}
		else
		{
			RECT r = GetRect();
			int height = r.bottom - r.top;
			if(mousePt.y > height)
			{
				int diff = (mousePt.y - height) / 2;
				SetScrollY(GetScrollY() + diff);
			}
		}
	}

	if(HasFocus())
	{
		switch(msg)
		{
			case 0x0109:
			case WM_DEADCHAR:
			//case WM_KEYUP:
			case WM_SYSKEYDOWN:
			case WM_SYSKEYUP:
			case WM_SYSCHAR:
			case WM_SYSDEADCHAR:
				return HandleCharMessage(msg, wParam, lParam);
		}
	}

	return FALSE;
}

BOOL CAveTextField::SetFocus()
{
	IAveControl::SetFocus();

	caretVisible = TRUE;
	Update();

	SetTimer(GetContainer()->GetDeskletHandle(), CARET_TIMER_ID, CARET_TIMER_INTERVAL, NULL);

	return TRUE;
}

void CAveTextField::KillFocus(const char* newLayer)
{
	if(!isInRightClickMenu && (!GetOnlyLoseFocusOnDeskletOut() || NULL == newLayer))
	{
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

Rect CAveTextField::GetRectForCharacterIndex(int index)
{
	RECT r = GetRect();
	RectF layoutRect(0.0f,(REAL)-GetScrollY(), (REAL)(r.right - r.left), (REAL)AVE_TEXTBOXMAXHEIGHT);

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

	std::wstring text = GetText();
	std::wstring txt = GetText();
	if(index >= (int)text.length())
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

void CAveTextField::Update()
{
	std::wstring text = GetText();

	RECT r = GetRect();
	RectF visibleRect(0.0f,0.0f, (REAL)(r.right - r.left), (REAL)(r.bottom - r.top));
	RectF layoutRect(0.0f, (REAL)-GetScrollY(), (REAL)(r.right - r.left), (REAL)AVE_TEXTBOXMAXHEIGHT);

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

			std::wstring text = GetText();
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

int CAveTextField::GetSelStart()const
{
	DWORD st;
	DWORD end;
	SendMessage(editHwnd, EM_GETSEL, (WPARAM)&st, (LPARAM)&end);
	return st;

}

int CAveTextField::GetSelEnd()const
{
	DWORD st;
	DWORD end;
	SendMessage(editHwnd, EM_GETSEL, (WPARAM)&st, (LPARAM)&end);

	return end;
}

void CAveTextField::SetSelRange(int start, int end)
{
	int selStart = GetSelStart();
	int selEnd   = GetSelEnd();

	if(selStart != start || selEnd != end)
	{
		selStart = start;
		selEnd = end;

		SendMessage(editHwnd, EM_SETSEL, selStart, selEnd);

		if(!MakeSureCaretIsVisible())
			Update();
	}
}

void CAveTextField::SetSelStart(int index)
{
	int selStart = GetSelStart();
	int selEnd   = GetSelEnd();

	if(selStart != index)
	{
		selStart = index;
		SendMessage(editHwnd, EM_SETSEL, selStart, selEnd);

		if(!MakeSureCaretIsVisible())
			Update();
	}	
}

void CAveTextField::SetSelEnd(int index)
{
	int selStart = GetSelStart();
	int selEnd   = GetSelEnd();

	if(selEnd != index)
	{
		selEnd = index;
		SendMessage(editHwnd, EM_SETSEL, selStart, selEnd);

		if(!MakeSureCaretIsVisible())
			Update();
	}	
}

bool CAveTextField::HasSelection()const
{
	int selStart = GetSelStart();
	int selEnd   = GetSelEnd();

	return selStart != selEnd;
}

std::wstring CAveTextField::GetText()
{
	DWORD strLen = SendMessageW(editHwnd, WM_GETTEXTLENGTH, 0, 0) + 1;
		
	WCHAR* str = new WCHAR[strLen];
	str[0] = L'\0';
	SendMessageW(editHwnd, WM_GETTEXT, strLen, (LPARAM)str);

	std::wstring text = str;
	delete[] str;

	return text;
}

void CAveTextField::SetText(const std::wstring& str)
{
	//text = str;
	/*
	while(text.find(L'\r', 0))
		text = text.replace(L'\r', L'');*/

	SendMessageW(editHwnd, WM_SETTEXT, 0, (WPARAM)str.c_str());
	Update();

	UpdateScrollbar();
}

std::string CAveTextField::GetTextA()
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

void CAveTextField::SetTextA(const std::string& str)
{
	SendMessage(editHwnd, WM_SETTEXT, 0, (WPARAM)str.c_str());
	//text = GetText();

	Update();

	UpdateScrollbar();
}
