//	file:			Constants for AveDesk 
//	author:			Andreas Verhoeven
//	version:		1.0
//	last changed:	8 januari 2004
//	description:	<none>
//	comments:		<none>


#ifndef CONSTANTS_H_50403932091__
#define CONSTANTS_H_50403932091__

#include<windows.h>

// for CModules
#define WM_AVELOADMODULE	WM_USER + 192
#define WM_AVEREMOVEMODULE	WM_USER + 193
#define	WM_AVECLEARMODULES	WM_USER + 194

// constants for desklets style
const int STYLE_NORMAL	= 0;
const int STYLE_BOTTOM	= 1;
const int STYLE_TOP		= 2;
const int STYLE_ICON	= 3;

//!1.2
const int STYLE_SHOWCASE= 4;

// constants for label position
const int LABEL_BOTTOM	= 0;
const int LABEL_LEFT	= 1;
const int LABEL_TOP		= 2;
const int LABEL_RIGHT	= 3;	

// constants for label type
const int LABEL_REGULAR	= 0;
const int LABEL_MASTER	= 2;
const int LABEL_NONE	= 1;

// constants for clicking type
const int CLICKMODE_SINGLE = 0;
const int CLICKMODE_DOUBLE = 1;

// constants for desklet clicking mode
const int CLICKMODE_DESKLET_DEFAULT = 0;
const int CLICKMODE_DESKLET_SINGLE  = 1;
const int CLICKMODE_DESKLET_DOUBLE  = 2;

// constants for quality
const int QUALITY_LOWEST	= 0;
const int QUALITY_LOW		= 1;
const int QUALITY_AVERAGE	= 2;
const int QUALITY_GOOD		= 3;
const int QUALITY_BEST		= 4;

// timer constants
const int CLEANER_TIMER		= 439119292; // random number
const int FOCUS_TIMER		= 302191015; // random number
const int FADER_TIMER		= 202191984; // random number

const int labelFaderSpeed = 33; //ms for fader timer

// WM_USER+ constants
const int ICON_CALLBACK = WM_USER + 612;

// event constants
const int EVENT_ONMOUSEON   = 0;
const int EVENT_ONMOUSEOUT  = 1;
const int EVENT_ONSELECT	= 2;
const int EVENT_ONDESELECT	= 3;
const int EVENT_ONSHOW		= 4;
const int EVENT_ONHIDE		= 5;
const int EVENT_ONCONFIGURE = 6;
const int EVENT_ONSTARTMOVE	= 7;
const int EVENT_ONENDMOVE	= 8;
const int EVENT_ONLEFTCLICK	= 9;
const int EVENT_ONRIGHTCLICK= 10;

// herd - script effectlets only
const int EVENT_ONDESTROY   = 11;
const int EVENT_ONSAVE      = 12;
const int EVENT_ONLOAD      = 13;
const int EVENT_ONTIMER     = 14;


// desklet flags
const int DESKLET_NO_LABEL					= 0x01;
const int DESKLET_ONLY_INSTANTIATE_SKINS	= 0x02;
const int DESKLET_SUPPORTS_ASYNC_PAINTING	= 0x04;
const int DESKLET_NO_ROTATION				= 0x08;
const int DESKLET_NO_CHANGE_IMAGE			= 0x16;
#define SUPPORTS_FLAG(f, i) ((f) & (i))

#endif//CONSTANTS_H_50403932091__