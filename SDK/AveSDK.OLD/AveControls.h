//	This file and its contents is copyright © 2005 Andreas Verhoeven and
//	may only be distributed as part of the AveDesk SDK. 
//  This file and its contents may only be used for creating desklets or plug-ins
//  for AveDesk.

#ifndef AVE_CONTROLS_HEADER_GUARD_23939
#define AVE_CONTROLS_HEADER_GUARD_23939

#include <windows.h>

typedef DWORD_PTR* AVEHANDLE;

#pragma pack(push, 1)

struct AVE_VERSION
{
	DWORD major;					// major version number of the controlslibrary
	DWORD minor;					// minor version number of the controlslibrary
	DWORD revision;					// revision version number of the controlslibrary

	DWORD intendedAveDeskVersion;	// intended avedesk version (as returned 
									// by DeskletGetVersion()) for the controlslibrary
};

#pragma pack(pop)

// general functions

/**
 * Gets the version of this library.
 * @param version - a pointer to the AVE_VERSION* structure that needs to be filled with
 *					the version-information. Cannot be NULL.
 * @return TRUE iff sucessfull.
 */
BOOL __stdcall AveGetVersion(AVE_VERSION* version);


// resources functions
/// The AveControls library encapsules the notion of resources into a
///	RESOURCES handle. A resources handle can be created based on a folder
/// that holds the files for the resources.
/// Whenever a control needs an image/other resource, it doesn't access it
/// directly, but "asks" the RESOURCES handle to get the resource.
/// The resourceshandle can then decide how the access the resource,
/// thus allowing for future resourcesfunctions that take, for example, a ZIP file instead
/// of a directory. Also, the resources handle can transparently decide to cache certain
/// resources.

/**
 * Creates a RESOURCES-HANDLE baed on a folder (NON-UNICODE)
 * @param baseFolder - the absolute path to the folder where all resources are placed in. Cannot be NULL.
 * @remark NON-UNICODE version of AveCreateResourcesW()
 * @see AveCreateResourcesW
 * @return an AVEHANDLE for the RESOURCES iff successfull, otherwise NULL.
 */
AVEHANDLE __stdcall AveCreateResources(const CHAR* baseFolder);

/**
 * Creates a RESOURCES-HANDLE baed on a folder (UNICODE)
 * @param baseFolder - the absolute path to the folder where all resources are placed in. Cannot be NULL.
 * @remark UNICODE version of AveCreateResources()
 * @see AveCreateResources
 * @return an AVEHANDLE for the RESOURCES iff successfull, otherwise NULL.
 */
AVEHANDLE __stdcall AveCreateResourcesW(const WCHAR* baseFolder);

/**
 * Frees a RESOURCES-HANDLE and all the memory it allocated.
 * @param handle - a handle to the resources to free. Cannot be NULL.
 * @remark The caller must make sure no other object is using the resources associated
 *			with the handle that is going to be freed.
 * @remark after freeing, the handle becomes invalid and cannot be used again.
 * @return TRUE iff successfull.
 */
BOOL __stdcall AveFreeResources(AVEHANDLE handle);

/**
 * Changes the folder the resources are associated with to another folder (NON-UNICODE).
 * @param resourcesHandle - the handle to the resources to change the path of. Cannot be NULL.
 * @remark the caller must update all controls that are using resources
  * @param baseFolder - the absolute path to the folder where all resources are placed in. Cannot be NULL.
 *			associated with the resourcesHandle itself. It's not done automatically.
 * @remark NON-UNICODE version of AveSetResourcesW()
 * @see AveSetResourcesW
 * @return TRUE iff successfull.
 */
BOOL __stdcall AveSetResourcesPath(AVEHANDLE resourcesHandle, const CHAR* baseFolder);

/**
 * Changes the folder the resources are associated with to another folder (UNICODE).
 * @param resourcesHandle - the handle to the resources to change the path of. Cannot be NULL.
 * @remark the caller must update all controls that are using resources
  * @param baseFolder - the absolute path to the folder where all resources are placed in. Cannot be NULL.
 *			associated with the resourcesHandle itself. It's not done automatically.
 * @remark UNICODE version of AveSetResources()
 * @see AveSetResources
 * @return TRUE iff successfull.
 */
BOOL __stdcall AveSetResourcesPathW(AVEHANDLE resourcesHandle, const WCHAR* baseFolder);



// control container functions
///
/// In the AveControls library, all controls must be part of a container.
///	A container holds additional data that is shared between all controls
/// that are part of the particular container. For example, since controls
/// are associated with a desklet, the container holds the methods internally
/// to communicate with the desklet.
///
/// RESOURCES AND CONTAINERS
/// Also, a container holds a handle to the resources. When a control (eg. a combobox)
/// wants to draw itself, it needs some image resources to paint certain parts.
/// The control then requests the resourceshandle  - to access these images - from the 
/// container. Notice that all controls can also be set to use a different resourceshandle
///	than their container, but, BY DEFAULT, the resourceshandle of the container is used.
///
/// FORWARDING EVENTS AND MESSAGES
/// Another task of the controlscontainer is to route and delegate all mouse-events and
/// messages to the right control. This works globally as follows:
///	The desklet should "forward" all OnLayer*On() messages to the container
/// by calling the respectively AveForward function with the containerhandle
/// and the parameters to the OnLayer*On() call. The container then checks
/// if the layer that generated the event is a control. If that's the 
/// case, the event is forwarded to the control.
/// NOTE: A special  case is that also window messages for the desklet ( OnProcessMessage() )
///       should be forwared to the container. This is to allow timers and keyboard input to work.
///
/// FREEING A CONTAINER
/// When a container is free'd, all controls that it contained are also automatically free'd.
///
/// LOCKING
/// A container can be locked. When a container is locked, any update performed on a control
/// that is owned by the container is not causing a redraw of the desklet. 
/// When doing a lot of updates of a container or the controls in a container, it's
/// a good idea to lock the container so no unnecessary desklet-redraws are done.
/// Locking is done by counting then number of times AveLockContainer() is called.
/// Each call to AveLockContainer() increases the lock-count with one and each
/// call to AveUnlockContainer() decreases the lock-count with one. When the lock-count
/// is 0, the container is not locked.
///

/**
 * Creates a control-container.
 * @param deskletHwnd - the handle of the desklet this container should be associated with
 * @param resources - the resourceshandle for this container. Cannot be NULL.
 * @return An AVEHANDLE to the controlcontainer if succcessfull, otherwise NULL.
 */
AVEHANDLE __stdcall AveCreateControlContainer(HWND deskletHwnd, AVEHANDLE resources);

/**
 * Frees a control-container.
 * @param container -the handle of the container to free. Cannot be NULL.
 * @remark any controls that this container owned when it is free'd are also free'd automatically.
 * @return TRUE iff successfull.
 */
BOOL __stdcall AveFreeControlContainer(AVEHANDLE container);

/**
 * Adds a control to a control-container.
 * @param containerHandle - the handle of the container to add a control to. Cannot be NULL.
 * @param controlHandle - the handle of the control to add to the container. Cannot be NULL.
 * @remark A control should also be added to the container with this function after it has
 *			been created. This is not done automatically.
 * @return TRUE iff successfull.
 */
BOOL __stdcall AveAddControlToContainer(AVEHANDLE containerHandle, AVEHANDLE controlHandle);

/**
 * Removes a control from a container.
 * @param containerHandle - the handle of the container to remove a control from. Cannot be NULL.
 * @param name - the name of the control to remove from the container. Cannot be NULL.
 * @remark the name of the control to remove is a NON-UNICODE name, because names
 *			are internally and map directly to an AveDesk layer.
 * @return TRUE iff successfull.
 */
BOOL __stdcall AveRemoveControlFromContainer(AVEHANDLE containerHandle, const CHAR* name);


/**
 * Gets a handle to a control from a container.
 * @param containerHandle - the handle of the container to remove a control from. Cannot be NULL.
 * @param name - the name of the control to get a handle of from the container. Cannot be NULL.
 * @remark the name of the control to get a handle of is a NON-UNICODE name, because names
 *			are internally and map directly to an AveDesk layer.
 * @return The AVEHANDLE to the control if successfull, otherwise NULL.
 */
AVEHANDLE __stdcall AveGetControlFromContainer(AVEHANDLE containerHandle, const CHAR* name);

/**
 * Removes and frees all controls from a container.
 * @param containerHandle - the handle of the container to remove all controls from. Cannot be NULL.
 * @return TRUE iff successfull.
 */
BOOL __stdcall AveEmptyContainer(AVEHANDLE containerHandle);

/**
 * Sets the active focus to another control in the container.
 * @param containerHandle - the handle of the container to set the focus a control from. Cannot be NULL.
 * @param name - the name of the control to get a handle of from the container. Can be NULL to indicate
 *					that no control should have the focus.
 * @remark the name of the control to set the focus of is a NON-UNICODE name, because names
 *			are internally and map directly to an AveDesk layer.
 * @return The AVEHANDLE to the control if successfull, otherwise NULL.
 */
BOOL __stdcall AveSetFocusToControl(AVEHANDLE containerHandle, const CHAR* name);

/**
 * Increases the lock-count of a container
 * @param containerHandle - the handle of the container to increase the lock-count of.
 * @return the new lock-count of the container.
 */
INT __stdcall AveLockContainer(AVEHANDLE containerHandle);

/**
 * Decreases the lock-count of a container
 * @param containerHandle - the handle of the container to decrease the lock-count of.
 * @return the new lock-count of the container.
 */
INT __stdcall AveUnlockContainer(AVEHANDLE containerHandle);

/**
 * Gets the current lock-status of a container.
 * @param containerHandle - the handle of the container to get the lock-status of.
 * @return FALSE iff the container's lock-count == 0 (the container is unlocked). 
 *			TRUE iff the container is locked (lock-count != 0).
 */
BOOL __stdcall AveIsContainerLocked(AVEHANDLE containerHandle);


// container event forwarders
/**
 * Forwards windows message from the desklet (OnPrcoessMessage) to the control-container.
 * @params see desklet-SDK OnProcessMessage
 * @return TRUE iff a control in the container handled this call.
 */
BOOL __stdcall AveForwardMessage(AVEHANDLE containerHandle, UINT msg, WPARAM wParam, LPARAM lParam);

/**
 * Forwards mousewheelforward events from the desklet (OnLayerMouseWheelForward) to the control-container.
 * @params see desklet-SDK OnLayerMouseWheelForward
 * @return TRUE iff a control in the container handled this call.
 */
BOOL __stdcall AveForwardMouseWheelForward(AVEHANDLE containerHandle, UINT id, const CHAR* layerName, const POINT* pt, const SIZE* s, DWORD keys, int scrolls);

/**
 * Forwards mousewheelbackward events from the desklet (OnLayerMouseWheelBackward) to the control-container.
 * @params see desklet-SDK OnLayerMouseWheelBackward
 * @return TRUE iff a control in the container handled this call.
 */
BOOL __stdcall AveForwardMouseWheelBackward(AVEHANDLE containerHandle, UINT id, const CHAR* layerName, const POINT* pt, const SIZE* s, DWORD keys, int scrolls);

/**
 * Forwards mousedown events from the desklet (OnLayerMouseDown) to the control-container.
 * @params see desklet-SDK OnLayerMouseDown
 * @return TRUE iff a control in the container handled this call.
 */
BOOL __stdcall AveForwardMouseDown(AVEHANDLE containerHandle, UINT id, const CHAR* layerName, const POINT* pt, const SIZE* s, DWORD keys);

/**
 * Forwards mouseup events from the desklet (OnLayerMouseUp) to the control-container.
 * @params see desklet-SDK OnLayerMouseUp
 * @return TRUE iff a control in the container handled this call.
 */
BOOL __stdcall AveForwardMouseUp(AVEHANDLE containerHandle, UINT id, const CHAR* layerName, const POINT* pt, const SIZE* s, DWORD keys);

/**
 * Forwards mouseon events from the desklet (OnLayerMouseOn) to the control-container.
 * @params see desklet-SDK OnLayerMouseOn
 * @return TRUE iff a control in the container handled this call.
 */
BOOL __stdcall AveForwardMouseOn(AVEHANDLE containerHandle, UINT id, const CHAR* layerName, const POINT* pt, const SIZE* s);

/**
 * Forwards mousemove events from the desklet (OnLayerMouseMove) to the control-container.
 * @params see desklet-SDK OnLayerMouseMove
 * @return TRUE iff a control in the container handled this call.
 */
BOOL __stdcall AveForwardMouseMove(AVEHANDLE containerHandle, UINT id, const CHAR* layerName, const POINT* pt, const SIZE* s);

/**
 * Forwards mouseclick events from the desklet (OnLayerMouseClick) to the control-container.
 * @params see desklet-SDK OnLayerMouseClick
 * @return TRUE iff a control in the container handled this call.
 */
BOOL __stdcall AveForwardMouseClick(AVEHANDLE containerHandle, UINT id, const CHAR* layerName, const POINT* pt, const SIZE* s, DWORD keys);

/**
 * Forwards mouserightclick events from the desklet (OnLayerMouseRightClick) to the control-container.
 * @params see desklet-SDK OnLayerMouseRightClick
 * @return TRUE iff a control in the container handled this call.
 */
BOOL __stdcall AveForwardMouseRightClick(AVEHANDLE containerHandle, UINT id, const CHAR* layerName, const POINT* pt, const SIZE* s, DWORD keys);

/**
 * Forwards mouseout events from the desklet (OnLayerMouseOut) to the control-container.
 * @params see desklet-SDK OnLayerMouseOut
 * @return TRUE iff a control in the container handled this call.
 */
BOOL __stdcall AveForwardMouseOut(AVEHANDLE containerHandle, UINT id, const CHAR* layerName);




// control functions
///
/// The AveConrols library is build around controls. Controls are UI-controls such as
/// buttons, textboxes, textfields, comboboxes etc that are commonly used in user-interfaces
/// to communicate with the user. Since desklets cannot use default Windows UI-controls,
/// AveDesk now provides a library of SKINNABLE, ALPHA-BLENDED UI-CONTROLS.
///
/// AVEDESK AND CONTROLS
/// To AveDesk, a control is simply a layer. The AveControls  library uses the AveDesk's layers function
///	to draw the control and interact with the user. The name of a control maps directly to a
//	layer name in a desklet-instance.
///
/// CONTAINERS AND CONTROLS
/// Each control is based upon a layer and then must be added to a container with AveAddControlToContainer().
/// The container makes sure that all events and messages are being forwared to the controls.
///
/// PROPERTIES AND ACTIONS
/// A control can do actions or properties can be get/set'ed by communication
/// with the control with the AveSendMessage() function. Just like the Win32 SDK,
/// this uses constant "messages" (numerical values) to define the action to take 
/// when the message is send to the control. Also, 2 generic typed parameters are used
//// to pass all kinds of data-structures and values.
/// For example, to send some pointer value to a control thru the LPARAM parameter, 
/// the pointer value should be casted to this (LPARAM) value.
/// Example:
///  AVE_FONT2 font2 = {0};
/// AvSendMessage(controlHandle, AV_SET_FONT2, (WPARAM)TRUE, (LPARAM)&font2);
///
/// LOCKING
/// A control can be locked. When a control is locked, any update performed on the control
/// is not causing a redraw of the desklet. 
/// When doing a lot of updates on a control, it's a good idea to lock the control so 
/// no unnecessary desklet-redraws are done.
/// Locking is done by counting then number of times AveLockControl() is called.
/// Each call to AveLockControl() increases the lock-count with one and each
/// call to AveUnlockControl() decreases the lock-count with one. When the lock-count
/// is 0, the container is not locked.
/// AveLockContainer() and AveLockControl() can be used together. A control is locked
/// when its container is locked OR the control is locked.
///

/**
 * Creates a control.
 * @param container - the controlcontainer to base the control on.
 * @param name - an unique (NON-UNICODE) name of the control. This name maps directly to the layer
 *					name of the desklet's layer that is used by the control.
 * @param type: the type of the control. one of the following:
 *					. AVE_TEXTFIELD_CONTROL - a multiline textfield
 *					. AVE_VERTICAL_SCROLLBAR_CONTROL - a vertical scrollbar
 *					. AVE_EDITBOX_CONTROL - a single-line editbox
 *					. AVE_DROPDOWNLIST_CONTROL - a combobox-style dropdownlist
 *					. AVE_IMAGEBUTTON_CONTROL - a button that uses images.
 *					. AVE_LABEL_CONTROL - a textlabel.
 *					. AVE_HYPERLINK_CONTROL - a hyperlink label.
 * @param flags: type-specific flags, one of the following combinations:
 *					AVE_TEXTFIELD_CONTROL:
 *						.TEXTFIELD_NORETURNS - no newlines are added to the textfield if the <enter> key
 *												is pressed.
 * @param reserved - reserved, MUST BE NULL.
 * @remark the control is NOT automatically added to the container that is passed as the first parameter.
 *		   Rather, this container is only used to initialize the control (create the avedesk layer, etc).
 *		   The control must still be added to the container with AveAddControlToContainer() in order
 *		   to receive events and messages.
 * @remark constants defined in AveConstants.h and AveFlags.h
 * @return An AVEHANDLE to the control if successfull, otherwise NULL.
 */
AVEHANDLE __stdcall AveCreateControl(AVEHANDLE container, const CHAR* name, const CHAR* type,
			DWORD flags, DWORD_PTR* reserved);

/**
 * Frees a control.
 * @param control - the handle of the control to free.
 * @remark the control may not be used actively by any
 * @return TRUE iff successfull.
 */
BOOL __stdcall AveFreeControl(AVEHANDLE control);


/**
 * Increases the lock-count of a control
 * @param containerHandle - the handle of the control to increase the lock-count of.
 * @return the new lock-count of the control.
 */
INT __stdcall AveLockControl(AVEHANDLE controlHandle);

/**
 * Increases the lock-count of a control
 * @param containerHandle - the handle of the control to increase the lock-count of.
 * @return the new lock-count of the control.
 */
INT __stdcall AveUnlockControl(AVEHANDLE controlHandle);


/**
 * Gets the current lock-status of a control.
 * @param containerHandle - the handle of the control to get the lock-status of.
 * @return TRUE iff the control's lock-count != 0 OR the control's container's lock-count != 0.
 *				Otherwise, FALSE is returned.
 */
BOOL __stdcall AveIsControlLocked(AVEHANDLE controlHandle);

// control set/get function
/**
 * Sends a message to a control.
 * @param controlHandle - the handle of the control to send the message too. Cannot be NULL.
 * @param msg - the message to send to the control.
 * @param wParam - message specific value
 * @param lParam - message specific value
 * @return message specific value
 * @remark see AveConstants.h for a description of all possible messages and their parameters.
 */
DWORD_PTR __stdcall AveSendMessage(AVEHANDLE controlHandle, UINT msg, WPARAM wParam, LPARAM lParam);


#endif//AVE_CONTROLS_HEADER_GUARD_23939