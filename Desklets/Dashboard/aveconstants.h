//	This file and its contents is copyright © 2005 Andreas Verhoeven and
//	may only be distributed as part of the AveDesk SDK. 
//  This file and its contents may only be used for creating desklets or plug-ins
//  for AveDesk.

#ifndef AVECONSTANTS_INCLUDE_GUARD_394336467
#define AVECONSTANTS_INCLUDE_GUARD_394336467

#include <windows.h>

typedef DWORD_PTR* AVEHANDLE;

// possible controls (28 Aug 2005)
#define AVE_TEXTFIELD_CONTROL "TEXTFIELD" // multiline text field (font1)
#define AVE_VERTICAL_SCROLLBAR_CONTROL "VERTICAL_SCROLLBAR" // vertical scrollbar (no font)
#define AVE_EDITBOX_CONTROL "EDITBOX" // single line input box (font1)
#define AVE_DROPDOWNLIST_CONTROL "DROPDOWNLIST"// a dropdownlist control (font2)
#define AVE_IMAGEBUTTON_CONTROL "IMAGEBUTTON" // an image button control (no font)
#define AVE_LABEL_CONTROL "LABEL" // a static text label
#define AVE_HYPERLINK_CONTROL "HYPERLINK" // an hyperlink control


typedef BOOL (__stdcall *AveOnSetFocus)(void* callbackData, AVEHANDLE control);
typedef BOOL (__stdcall *AveOnKillFocus)(void* callbackData, AVEHANDLE control);
typedef BOOL (__stdcall *AveOnClick)(void* callbackData,AVEHANDLE button, const POINT* pt, const SIZE* s, DWORD keys);

// remarks: if no comment is made about the value of lParam or wParam, they always should be 0.

const UINT AV_SHOW = 1;		// shows a control
const UINT AV_HIDE = 2;		// hides a control
const UINT AV_ISVISIBLE = 3;// returns TRUE iff a control is visible
const UINT AV_SET_RECT = 4;  // sets the rect for this control, wParam = pointer to RECT, lParam = pointer to SIZE
const UINT AV_GET_RECT = 5;  // gets the rect for this control, wParam = receiving pointer to rect, lParam = receiving pointer to size
const UINT AV_GET_REDRAW = 6; // lParam = TRUE iff caller wants a redraw. Return value is TRUE the value that should be passed as bRedraw to AveDesk functions
const UINT AV_UPDATE = 7;    // updates a control.
const UINT AV_GET_NAME = 8;   // gets the name of a control. wParam = max buffer length (including zero-terminator), lParam = receiving CHAR* buffer [no WCHAR* !]
const UINT AV_GET_NAMELENGTH = 9; // returns the length of the name of a control (excluding zero-terminator).
const UINT AV_SET_ONSETFOCUSCALLBACK = 10; // sets the onsetfocus callback. wParam = callback data, lParam = AveOnSetFocus function pointer.
const UINT AV_SET_ONKILLFOCUSCALLBACK = 11; // sets the onkillfocus callback. wParam = callback data, lParam = AveOnKillFocus function pointer.
const UINT AV_SET_RESOURCES = 12; // overrides the default resources used by a control. lParam = AVEHANDLE to resources.
const UINT AV_SET_FONT2 = 13; // controls that support the font2 structure can use this message to set font information. lParam = pointer to AVE_FONT2 structure. the control isn't updated automatically.
const UINT AV_GET_FONT2 = 14; // controls that support the font2 structure can use this message to query for font information. lParam = pointer to receiving AVE_FONT2 structure.
const UINT AV_GET_LAYERID = 15; // returns the layerid that the control is using
const UINT AV_SET_SHADOWPROPERTIES = 16; // lParam = pointer to AVE_SHADOWPROPERTIES. wParam = TRUE iff use shadow on font.
const UINT AV_GET_SHADOWPROPERTIES = 17; // lParam = optional pointer to receiving AVE_SHADOWPROPERTIES. wParam = optional pointer to BOOL receiving TRUE iff shadow should be drawn on font.
const UINT AV_SET_ALIGNMENT = 18; // wParam = vertical aligment, lParam = horizontal aligment
const UINT AV_GET_VERTALIGNMENT = 19; // returns vertical alignment.
const UINT AV_GET_HORALIGMENT = 20; // returns horizontal alignment.
const UINT AV_SET_ISDISABLED = 21; // wParam = TRUE iff the control should be disabled.
const UINT AV_GET_ISDISABLED = 22; // returns TRUE iff the control is disabled.

// text structures
#pragma pack(push, 1)
struct AVE_FONT
{
	WCHAR faceName[LF_FACESIZE];
	UINT size;
	UINT style;
};

struct AVE_FONT2
{
	WCHAR faceName[LF_FACESIZE];
	FLOAT size;
	UINT style;
};

const INT ALIGNMENT_NEAR   = 0x01;
const INT ALIGNMENT_CENTRE = 0x02;
const INT ALIGNMENT_FAR    = 0x04;

typedef DWORD ALPHA_RGB;
struct AVE_SHADOWPROPERTIES
{
	DWORD dwSize;		// sizeof(AVE_SHADOWPROPERTIES)

	INT xOffset;		// 
	INT yOffset;
	INT extraWidth;
	INT extraHeight;
	ALPHA_RGB color;
	FLOAT blurRatio;
	UINT intensity;
};

#pragma pack(pop, 1)


typedef BOOL (__stdcall *AveOnScrollbarNotifyCallback)(void* callbackData, AVEHANDLE textField, BOOL becomesVisible, DWORD* data);
typedef BOOL (__stdcall *AveOnEnterCallback)(void* callbackData, AVEHANDLE control);
typedef BOOL (__stdcall *AveOnTextChangeCallback)(void* callbackData, AVEHANDLE control);

// text messages
const UINT AV_SET_SELRANGE = 104; // wParam = selstart, lParam = selend
const UINT AV_GET_SETRANGE = 105; // wlParam = pointer to selstart, lParam = pointer to selend
const UINT AV_HAS_SELECTION= 106; // returns TRUE iff the text controls has a selection
const UINT AV_NO_SELECTION = 107; // makes sure the textfield has no selection
const UINT AV_SET_TEXTCOLOR= 108; // sets the text color, lParam = ARGB value with the color
const UINT AV_GET_TEXTCOLOR= 109; // returns the text color as ARGB
const UINT AV_SET_SELBGCOLOR=110; // sets the selection background color, lParam = ARGB value with the color
const UINT AV_GET_SELBGCOLOR=111; // returns the selection background color as ARGB
const UINT AV_SET_FONT      =112; // sets the font. lParam = pointer to AVE_FONT struct
const UINT AV_GET_FONT      =113; // gets the font. lParam = pointer to receiving AVE_FONT struct
const UINT AV_SET_TEXT      =114; // sets the text.lParam = WCHAR* string to set.
const UINT AV_GET_TEXT      =115; // gets the text. wParam = maximum buffer length (including zero-terminator). lParam = receiving WCHAR* buffer.
const UINT AV_GET_TEXT_LEN  = 116; // returns the text length (excluding zero terminator).
const UINT AV_GET_TEXT_LENGTH = 116; // alias for AV_GET_TEXT_LEN
const UINT AV_DELETE_SELECTION=117; // deletes the current selection
const UINT AV_SET_SCROLLBAR  = 118; // sets a scrollbar. lParam = AVEHANDLE to a scrollbar control.
const UINT AV_SET_SCROLLBARCALLBACK=119; // sets a scrollbar notify callback. wParam = callbackData, lParam = AveOnScrollbarNotifyCallback function pointer. 
const UINT AV_SET_ONENTERCALLBACK=120; // sets an on enter callback notify. wParam = callbackData, lParam = AveOnEnterCallback function pointer.
const UINT AV_SET_SMOOTHSCROLLING=121; // sets smoothscrolling. if wParam = TRUE, then it's enabled, otherwise disabled.
const UINT AV_GET_SMOOTHSCROLLING=122;// returns TRUE iff smoothscrolling is enabled
const UINT AV_SET_ONLY_LOSE_FOCUS_ON_DESKLET_OUT=123; // makes the textfield only lose focus on desklet out if wParam = TRUE.
const UINT AV_GET_ONLY_LOSE_FOCUS_ON_DESKLET_OUT=124; // returns TRUE if OnlyLoseFocusOnDesklet out style is enabled
const UINT AV_SET_READONLY = 125; // iff wParam is TRUE, then read-only is set, otherwise it's unset.
const UINT AV_GET_READONLY = 126; // returns TRUE iff read-only is set.
const UINT AV_SET_ISPASSWORD = 127; // iff wParam is TRUE then is-password is set, otherwise it's unset.
const UINT AV_GET_ISPASSWORD = 128; // returns TRUE iff is-password is set.
const UINT AV_SET_CHANGECALLBACK  = 129; // sets the callback function for the OnChange [hen text changes] event. wParam = callbackData, lParam = AveOnTextChangeCallback function pointer.


// scrollbar structures

typedef BOOL (__stdcall *AveOnScroll)(void * data, AVEHANDLE scrollBar);

// scrollbar messages
const UINT AV_SCROLLBAR_SET_THUMBSIZE = 201; // sets the thumbsize. wParam = visible area, lParam = total area
const UINT AV_SCROLLBAR_SET_POS = 202;       // set the scrollbar position. wParam = new position
const UINT AV_SCROLLBAR_GET_POS = 203;       // returns the current scrollbar position.
const UINT AV_SCROLLLBAR_SET_SCALE = 204;    // set the scrollbar scale (max). wParam = new scale
const UINT AV_SCROLLBAR_GET_SCALE  = 205;    // returns the current scrollbar scale(max)
const UINT AV_SCROLLBAR_SET_ONSCROLL_CALLBACK = 206; // set the scrollcallback. wParam = callbackData, lParam = AveOnScroll function pointer.


// dropdownlist structures

typedef BOOL (__stdcall* AveOnSelChange)(void* data, AVEHANDLE control, INT* newSel);

// other droplist constants
const UINT DROPLIST_ERR = -1;

//dropdownlist messages
// NOTE: dropdownlist supports all the AVE_FONT2 messages (AV_SET_FONT2, AV_GET_FONT2, AV_SET_TEXTCOLOR, AV_GET_TEXTCOLOR).
const UINT AV_DROPLIST_INSERTITEM= 301; // inserts item after index. wParam = index, lParam = WCHAR* of string to insert (cannot be NULL). Returns new index.
const UINT AV_DROPLIST_ADDITEM   = 302; // adds an item at the end of the list. lParam = WCHAR* of string to insert (cannot be NULL). Returns new index.
const UINT AV_DROPLIST_REMOVEITEM= 303; // removes an item at position index. wParam = index. returns TRUE iff succesfull.
const UINT AV_DROPLIST_FINDITEM  = 304; // finds an item in the list while beginning the search at index. wParam = index. lParam = WCHAR* of string to find. Returns the found index if succesfull, otherwise DROPLIST_ERR.
const UINT AV_DROPLIST_CLEAR     = 305; // removes all items.
const UINT AV_DROPLIST_COUNT     = 306; // returns the number of items.
const UINT AV_DROPLIST_SET_SEL   = 307; // sets the current selection to index. wParam = index.
const UINT AV_DROPLIST_GET_SEL   = 308; // returns the current selection.
const UINT AV_DROPLIST_DROPDOWN  = 309; // drops the menu down from the box.
const UINT AV_DROPLIST_ISDROPPEDDOWN=310;//returns TRUE iff the menu is dropped down from the box.
const UINT AV_DROPLIST_SET_ONSELCHANGECALLBACK = 311; // sets the onselchange callback. wParam = callback data, lParam = AveOnSelChange function pointer.
const UINT AV_DROPLIST_GET_ITEMSTRING = 312; // gets the string of an item. wParam = index of the item, lParam = WCHAR* buffer that receives the string of the item. The buffer must be large enough (including NULL-terminator).
const UINT AV_DROPLIST_GET_ITEMSTRING_LENGTH = 313; // returns the length of an the string of an item (excluding null terminator). wParam = index.



// image button messages
// NOTE: an image button can use 4 images:
//       - normal state (applies when no other state applies)
//       - pressed state (applies when the ISPRESSED flag has been set)
//       - down state (applies when the mouse is down on the button)
//       - disabled state (applies when the button is disabled)
//
// the ISPRESSED flag can be used to make a toggle button that toggles
// between two states (normal and isPressed).
//
// the RETRIEVEIMAGESFROMRES specifies the way imagefiles are retrieved.
// If this flag is set, images will be loaded from the supplied AveResources
// handle, and thus their file-path is relative to the resources-path.
// if this flag is not set, the file-path is directly passed to the host
// application (AveDesk).

const UINT AV_IMAGEBUTTON_SET_ISPRESSED = 401; // sets the ISPRESSED flag. wParam = TRUE iff the flag should be set.
const UINT AV_IMAGEBUTTON_GET_ISPRESSED = 402; // returns TRUE iff the ISPRESSED flag is set.
const UINT AV_IMAGEBUTTON_SET_CLICKHANDLER=403;// sets the onclick handler. wParam = callback data, lParam = AveOnClick function pointer.
const UINT AV_SET_NORMALIMAGEFILE       = 404; // sets the normal image. wParam = TRUE iff the button must be updated immediately, lParam = WCHAR* string to the image file.
const UINT AV_SET_DOWNIMAGEFILE         = 405; // sets the down image. wParam = TRUE iff the button must be updated immediately, lParam = WCHAR* string to the image file.
const UINT AV_SET_PRESSEDIMAGEFILE      = 406; // sets the pressed image. wParam = TRUE iff the button must be updated immediately, lParam = WCHAR* string to the image file.
const UINT AV_SET_DISABLEDIMAGEFILE     = 407; // sets the disabled image. wParam = TRUE iff the button must be updated immediately, lParam = WCHAR* string to the image file.
const UINT AV_SET_RETRIEVEIMAGESFROMRES = 408; // sets the RETRIEVEIMAGESFROMRES flag if wParam = TRUE.
const UINT AV_GET_RETRIEVEIMAGESFROMRES = 409; // returns TRUE if the RETRIEVEIMAGESFROMRES flag has been set.

// LABEL messages
// labels are static
/*
const UINT AV_SET_TEXT      =114; // sets the text.lParam = WCHAR* string to set.
const UINT AV_GET_TEXT      =115; // gets the text. wParam = maximum buffer length (including zero-terminator). lParam = receiving WCHAR* buffer.
const UINT AV_GET_TEXT_LEN  = 116; // returns the text length (excluding zero terminator).
const UINT AV_SET_FONT2 = 13; // controls that support the font2 structure can use this message to set font information. lParam = pointer to AVE_FONT2 structure. the control isn't updated automatically.
const UINT AV_GET_FONT2 = 14; // controls that support the font2 structure can use this message to query for font information. lParam = pointer to receiving AVE_FONT2 structure.
const UINT AV_GET_LAYERID = 15; // returns the layerid that the control is using
const UINT AV_SET_SHADOWPROPERTIES = 16; // lParam = pointer to AVE_SHADOWPROPERTIES. wParam = TRUE iff use shadow on font.
const UINT AV_GET_SHADOWPROPERTIES = 17; // lParam = optional pointer to receiving AVE_SHADOWPROPERTIES. wParam = optional pointer to BOOL receiving TRUE iff shadow should be drawn on font.
const UINT AV_SET_ALIGNMENT = 18; // wParam = vertical aligment, lParam = horizontal aligment
const UINT AV_GET_VERTALIGNMENT = 19; // returns vertical alignment.
const UINT AV_GET_HORALIGMENT = 20; // returns horizontal alignment.
const UINT AV_SET_TEXTCOLOR= 108; // sets the text color, lParam = ARGB value with the color
const UINT AV_GET_TEXTCOLOR= 109; // returns the text color as ARGB
*/


// HYPERLINK messages
// hyperlinks are labels, thus they use also the same messages
// as labels
const UINT AV_HYPERLINK_SET_CLICK_CALLBACK = 501; // sets the onclick callback. wParam = callback data, lParam = function pointer to AveOnClick function.
const UINT AV_SET_MOUSEOVER_FONT2 = 502; // controls that support the font2 structure can use this message to set font information. lParam = pointer to AVE_FONT2 structure. the control isn't updated automatically.
const UINT AV_GET_MOUSEOVER_FONT2 = 503; // controls that support the font2 structure can use this message to query for font information. lParam = pointer to receiving AVE_FONT2 structure.
const UINT AV_SET_MOUSEOVER_SHADOWPROPERTIES = 504; // lParam = pointer to AVE_SHADOWPROPERTIES. wParam = TRUE iff use shadow on font.
const UINT AV_GET_MOUSEOVER_SHADOWPROPERTIES = 505; // lParam = optional pointer to receiving AVE_SHADOWPROPERTIES. wParam = optional pointer to BOOL receiving TRUE iff shadow should be drawn on font.
const UINT AV_SET_MOUSEOVER_ALIGNMENT = 506; // wParam = vertical aligment, lParam = horizontal aligment
const UINT AV_GET_MOUSEOVER_VERTALIGNMENT = 507; // returns vertical alignment.
const UINT AV_GET_MOUSEOVER_HORALIGMENT = 508; // returns horizontal alignment.
const UINT AV_SET_MOUSEOVER_TEXTCOLOR= 509; // sets the text color, lParam = ARGB value with the color
const UINT AV_GET_MOUSEOVER_TEXTCOLOR= 510; // returns the text color as ARGB


#endif//AVECONSTANTS_INCLUDE_GUARD_394336467