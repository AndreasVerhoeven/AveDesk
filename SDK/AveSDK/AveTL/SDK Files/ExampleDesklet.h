#pragma once

#include <windows.h>
#include "../DeskletImpl.hpp"

class CExampleDesklet : 
	public CDeskletImpl<CExampleDesklet>
{
public:
	//USE_DEFAULT_DESKLET_CONSTRUCTOR;
	// uncomment the above line if don't want to provide code for the constructor.

	DEFAULT_DESKLET_CONSTRUCTOR_PROTOTYPE(CExampleDesklet);

	// implemented events handlers
	BOOL LeftButtonClick(POINT* p, SIZE* s);
	BOOL RightButtonClick(POINT* p, SIZE* s);

	// implemented drop handlers
	BOOL FilesDrop(std::vector<std::string>& files,DWORD /*keys*/,POINTL /*pnt*/, DWORD* /*effect*/);
	BOOL StringDrop(std::string& s,DWORD /*keys*/,POINTL /*pnt*/, DWORD* /*effect*/);
};

// this line is always required, otherwise the host application (AveDesk) won't reconize
// this as a valid desklet. Required information (in order): name, author, comments, version.
DESKLET_INFORMATION("Example Desklet","Ave","An example Desklet written using the templated wrapper for AveDesk desklets.",100)

// these events are always required. They make sure the desklet is created, destroyed and
// identified by the host application (AveDesk).
REQUIRED_EVENT_ONCREATE(CExampleDesklet)
REQUIRED_EVENT_ONDESTROY(CExampleDesklet)

// these events are optional, uncomment the ones you want to use and replace t with your class name.
// Also, you have to remove the the semicolon in front of the events-name in the .DEF file.
// If you want to respond to an event in your class, you should override the function tha
// matches it.

EVENT_ONLEFTBUTTONCLICK(CExampleDesklet)		// invokes LeftButtonClick
EVENT_ONRIGHTBUTTONCLICK(CExampleDesklet)		// invokes RightButtonClick
//EVENT_ONMOUSEMOVE(CExampleDesklet)			// invokes MouseMove
//EVENT_ONMOUSEON(CExampleDesklet)				// invokes MouseOn
//EVENT_ONMOUSEOUT(CExampleDesklet)				// invokes MouseOut
//EVENT_ONDRAGENTER(CExampleDesklet)			// invokes DragEnter
//EVENT_ONDRAGLEAVE(CExampleDesklet)			// invokes DragLeaver
EVENT_ONDROP(CExampleDesklet)					// invokes 3 drop handlers: FilesDrop(), StringDrop(), Drop()
//EVENT_ONDRAGOVER(CExampleDesklet)				// invokes DragOver
//EVENT_ONSAVE(CExampleDesklet)					// invokes SaveToIni
//EVENT_ONPROCESSMESSAGE(CExampleDesklet)		// invokes ProcessMessage
//EVENT_ONADDPROPERTYSHEETS(CExampleDesklet)	// invokes AddPropertySheets
//EVENT_ONDRAGDROPDATAREQUEST(CExampleDesklet)	// invokes DragDropDataRequest iff extended drag drop is enabled
//EVENT_ONSELECT(CExampleDesklet)				// invokes Select
//EVENT_ONDESELECT(CExampleDesklet)				// invokes Deselect
//EVENT_ONMOUSEWHEELFORWARD(CExampleDesklet)	// invokes MouseWheelForward
//EVENT_ONMOUSEWHEELBACKWARD(CExampleDesklet)	// invokes MouseWheelBackward