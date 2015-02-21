//	AveDesk event callback definitions with javadoc/doxygen style comments
//	(c) copyright Andreas Verhoeven 2005
//	Do not modify, reproduce or use this file without my permission!



///	All the following functions CAN be exported by a desklets dll. The functions
///	need to be exported unmangled (use a .DEF file in VC).
///
/// Some functions are mandatory, like OnCreate(), OnDestroy() and OnGetInformation().
/// All callback functions - except OnCreate() - have as first parameter a pointer
/// that the call to OnCreate() returned. This pointer could be a pointer struct or a
/// class keeping instance variables. This way, desklets can be instantiated multiple 
//  times. The pointertype, which is void*, is called DESKLET_DATA for clarity in the
/// documentation.
///
///	General cyclus in AveDesk for the desklets lifetime:
///
///		pointer = OnCreate(hwnd, hInstance, ini, group);
///		
///			// during the lifetime of the desklet, it receives many event calls
///			OnLefButtonClick(pointer, ...); // a certain call
///			OnSave(pointer, ...); // a certain call
///
///		OnDestroy(pointer, hwnd);
///

#define DESKLET_DATA void

/**
 * Called when AveDesk queries for information about the desklet - not a particular instance. Mandatory function.
 * @remark this function is not associated with a particular instance.
 * @param szName - a non-unicode buffer that should receive the name of the desklet. Max 1023 characters.
 * @param szAuthor - a non-unicode buffer that should receive the author of the desklet. Max 1023 characters.
 * @param version - a pointer to an int that should receive the version of this desklet.
 * a non-unicode buffer that should receive the notes for the desklet. Max 1023 characters.
 */
void CALLBACK OnGetInformation(char *szName,char *szAuthor,int *version,char *szNotes);

/**
 * Called when the desklet is instantiated. Mandatory function.
 * @param hwndDesklet - the handle of the desklets window. Must be used in all Desklet*() API functions to
 *						identify the desklet.
 * @param hInstance - the handle of the dll of the desklet loaded into avedesk's processspace
 * @param szIni - if the desklet is created from a saved instance, this non-unicode string
 *					holds the filepath of the ini-file where the settings are saved.
 *					If the desklet is a totally new instance, szIni will be NULL.
 *	@param szIniGroup - if the desklet is created from a saved instance, this non-unicode string
 *					holds the group/section name of the section in the ini-file where the settings are saved.
 *					If the desklet is a totally new instance, szIni will be NULL.
 * @return a DESKLET_DATA* pointer that will be used in all other callback events calls - see docs.
 */
DESKLET_DATA* CALLBACK OnCreate (HWND hwndDesklet,HINSTANCE hInstance,const char *szIni,const char *szIniGroup);

/**
 * Called when a desklet instance is destroyed. Mandatory function.
 * @param lpData - see DESKLET_DATA.
 * @param hwndDesklet - the handle of the desklet to be saved.
 * @remark the desklet should destroy itself and cleanup everything
 *         allocated structs, classes, close threads it used, etc..
 */
void CALLBACK OnDestroy(DESKLET_DATA *lpData,HWND hwndDesklet);

/**
 * Called when a desklet instance is saved.
 * @param lpData - see DESKLET_DATA.
 * @param szIni - a non-unicode string that holds the filepath to the ini-file where the settings
 *					for this desklet can be saved.
 * @param szIniGroup - a non-unicode string that holds the group/section name of the section
 *						where this desklet should saves it settings in.
 * @param bIsForExport - not used, for Object Dock compatibility only.
 */
void CALLBACK OnSave(DESKLET_DATA *lpData,const char *szIni,const char *szIniGroup,BOOL bIsForExport);


/**
 * Called when the desklets window processes a message.
 * @param lpData - DESKLET_DATA see remarks
 * @param hwnd - the window handle of the desklet
 * @param msg - see win32 sdk
 * @param wParam - see win32 sdk
 * @param lParam - see win32 sdk
 */
void CALLBACK OnProcessMessage(DESKLET_DATA *lpData,HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);


/**
 * Called when the user clicked with the left button on a desklet.
 * @param lpData - see DESKLET_DATA.
 * @param ptCursor - a pointer to a structure holding the point the user clicked on.
 * @param sizeDesklet - a pointer describing holding the size of the desklet when the user
 *						clicked on it.
 * @remark a leftbuttonclick can be defined as a double- or single-click 
 *			depending on the users settings.
 * @return TRUE iff the callee handled the leftbuttonclick.
 */
BOOL CALLBACK OnLeftButtonClick(DESKLET_DATA *lpData,POINT *ptCursor,SIZE *sizeDesklet);


/**
 * Called when the user clicked with the right button on a desklet.
 * @param lpData - see DESKLET_DATA.
 * @param ptCursor - a pointer to a structure holding the point the user clicked on.
 * @param sizeDesklet - a pointer describing holding the size of the desklet when the user
 *						clicked on it.
 * @remark if returned !TRUE or not implemented, the default desklet right-click menu
 *				will be displayed.
 * @return TRUE iff the callee handled the rightbuttonclick.
 */
BOOL CALLBACK OnRightButtonClick(DESKLET_DATA *lpData,POINT *ptCursor,SIZE *sizeDesklet);


/**
 * Called when the users wants to configure the desklet.
 * @deprecated
 * @remark use propertysheets instead
 * @see OnAddPropertySheets
 */
void CALLBACK OnConfigure(DESKLET_DATA *lpData);

/**
 * Called after creation to query if the desklet wants to accept drops.
 * @param lpData - see DESKLET_DATA.
 * @remark consider using OLE drag-'n-drop with the OnDropEvent().
 * @return TRUE iff the desklet should accept dropped files not
 *			dropped by OLE drag-'n-drop.
 */
BOOL (CALLBACK *OnAcceptDropFiles)(DESKLET_DATA *lpData);

/**
 * Called when files are dropped onto the desklet.
 * @param lpData - see DESKLET_DATA.
 * @param hDrop - the HDROP dropstructures, also see WM_DROPFILES win32 message.
 * @remark consider using OLE drag-'n-drop with the OnDropEvent().
 */
void CALLBACK OnDropFiles(DESKLET_DATA *lpData,HDROP hDrop);

// v1.1 only

/**
 * Called when the mouse enters the desklet.
 * @param lpData - see DESKLET_DATA.
 * @version minimal 1.1
 * @return TRUE iff the callee handles this call.
 */
BOOL CALLBACK OnMouseOn(DESKLET_DATA* lpData);


/**
 * Called when the mouse exits the desklet.
 * @param lpData - see DESKLET_DATA.
 * @version minimal 1.1
 * @return TRUE iff the callee handles this call.
 */
BOOL CALLBACK OnMouseOut(DESKLET_DATA* lpData);

/**
 * Called when the mouse moves over the desklet.
 * @param lpData - see DESKLET_DATA.
 * @param x - the x coordinate of the mouse-position, scaled on the current desklet size.
 * @param y - the y coordinate of the mouse position, scaled on the current desklet size.
 * @version minimal 1.1
 * @return TRUE iff the callee handles this call.
 */
BOOL CALLBACK OnMouseMove(DESKLET_DATA* lpData, int x, int y);


/**
 * Called when the mouse scrollwheel is scrolled forward.
 * @param lpData - see DESKLET_DATA.
 * @param fKeysDown - the keys held down when the scroll message occured.
 * @param numScrolls - the number of scrolls the wheel made.
 * @param x - the x coordinate of the mouse-position, scaled on the current desklet size.
 * @param y - the y coordinate of the mouse position, scaled on the current desklet size.
 * @version minimal 1.1
 * @return TRUE iff the callee handles this call.
 */
BOOL CALLBACK OnMouseWheelForward(DESKLET_DATA* lpData,WORD fKeysDown, int numScrolls, int x,int y);

/**
 * Called when the mouse scrollwheel is scrolled backwards.
 * @param lpData - see DESKLET_DATA.
 * @param fKeysDown - the keys held down when the scroll message occured.
 * @param numScrolls - the number of scrolls the wheel made.
 * @param x - the x coordinate of the mouse-position, scaled on the current desklet size.
 * @param y - the y coordinate of the mouse position, scaled on the current desklet size.
 * @version minimal 1.1
 * @return TRUE iff the callee handles this call.
 */
BOOL CALLBACK OnMouseWheelBackward(DESKLET_DATA* lpData,WORD fKeysDown, int numScrolls, int x,int y);


/**
 * Called when a drag-operation enters the desklet.
 * @param lpData - see DESKLET_DATA.
 * @param obj - an IDataObject that holds the data to be dropped
 * @param keys - the keys held down.
 * @param pnt - a LONG point structure holding the point the mouse is over the desklet,
 *				scaled on the current desklet size.
 * @param effect - a pointer to a DWORD that should receive the a OR'd combination
 *					of the DROPEFFECT's that are possible see win32 SDK.
 * @remark see win32 IDropTarget::DragEnter() for behaviour
 * @remark will only be called when DeskletActivateExtendedDragDrop() was called.
 * @version minimal 1.1
 * @return TRUE iff the callee handles this call.
 */
BOOL CALLBACK OnDragEnter(DESKLET_DATA* lpData,IDataObject* obj,DWORD fKeys  ,POINTL pnt ,DWORD* effect);


/**
 * Called when the mouse moves while a drag-operation is over the desklet.
 * @param lpData - see DESKLET_DATA.
 * @param keys - the keys held down.
 * @param pnt - a LONG point structure holding the point the mouse is over the desklet,
 *				scaled on the current desklet size.
 * @param effect - a pointer to a DWORD that should receive the a OR'd combination
 *					of the DROPEFFECT's that are possible see win32 SDK.
 * @remark see win32 IDropTarget::DragOver() for behaviour
 * @remark will only be called when DeskletActivateExtendedDragDrop() was called.
 * @version minimal 1.1
 * @return TRUE iff the callee handles this call.
 */
BOOL CALLBACK OnDragOver(DESKLET_DATA* lpData,DWORD keys, POINTL pnt ,DWORD* effect);


/**
 * Called when the mouse out of the desklet when a drag-operation was over the desklet.
 * @param lpData - see DESKLET_DATA.
 * @remark see win32 IDropTarget::DragLeave() for behaviour
 * @remark will only be called when DeskletActivateExtendedDragDrop() was called.
 * @version minimal 1.1
 * @return TRUE iff the callee handles this call.
 */
BOOL CALLBACK OnDragLeave(DESKLET_DATA* lpData);


/**
 * Called when something is dropped onto the desklet.
 * @param lpData - see DESKLET_DATA.
 * @param keys - the keys held down.
 * @param pnt - a LONG point structure holding the point the mouse is over the desklet,
 *				scaled on the current desklet size.
 * @param effect - a pointer to a DWORD that should receive the a OR'd combination
 *					of the DROPEFFECT's that are possible see win32 SDK.
 * @remark see win32 IDropTarget::Drop() for behaviour
 * @remark will only be called when DeskletActivateExtendedDragDrop() was called.
 * @version minimal 1.1
 * @return TRUE iff the callee handles this call.
 */
BOOL CALLBACK OnDrop(DESKLET_DATA* lpData ,IDataObject* obj, DWORD keys, POINTL pnt, DWORD* effect);


/**
 * Called when the desklet is requested for OLE drag-'n-drop data. Only
 * FORMATETC's added to the list of possible formats by the desklet calling 
 * DeskletAddDragFormat() will be requested.
 * @param lpData - see DESKLET_DATA.
 * @param etc - a pointer to a FORMATETC structure describing
 * @param medium - a pointer to the STGMEDIUM that is used to transfer the requested
 *					data.
 * @remark will only be called when DeskletAddDragFormat() for at least one FORMATETC was called
 *			by the desklet.
 * @version minimal 1.1
 * @return TRUE iff the callee handles this call successfully.
 */
BOOL CALLBACK OnDragDropDataRequest(DESKLET_DATA* lpData,FORMATETC* etc ,STGMEDIUM* medium);


/**
 * Called when the desklet is selected.
 * @param lpData - see DESKLET_DATA.
 * @version minimal 1.1
 * @return TRUE iff the callee handles this call.
 */
BOOL CALLBACK OnSelect(DESKLET_DATA* lpData);

/**
 * Called when the desklet is deselected.
 * @param lpData - see DESKLET_DATA.
 * @version minimal 1.1
 * @return TRUE iff the callee handles this call.
 */
BOOL CALLBACK OnDeselect(DESKLET_DATA* lpData);


/**
 * Called when AveDesk builds the desklets configuration dialog, so
 * desklets will be able to attach additional propertysheets to the dialog
 * by calling DeskletAddPropertySheet().
 * @param lpData - see DESKLET_DATA.
 * @param numSheetsPresent - the number of propertysheets already present
 * @version minimal 1.1
 * @return the number of propertysheets after the desklet added its own sheets.
 */
int CALLBACK OnAddPropertySheets(DESKLET_DATA* lpData,int numSheetsPresent);


/**
 * Called when AveDesk queries the desklet for an IDispatch pointer.
 * If a desklet has an COM IDispatch interface, it could be used thru scripting
 * by means of the IDesklet.Model property if this function is implemented, so
 * there could be interplay between the AveDesk COM interface and the desklet's.
 * @param lpData - see DESKLET_DATA.
 * @param pVal - the pointer to the IDispatch pointer that should be set
 *					the the pointer to the IDispatch implementation.
 * @version minimal 1.2
 * @return TRUE iff the callee handles this function.
 */
BOOL CALLBACK OnGetModel(DESKLET_DATA* lpData, IDispatch **pVal);


/**
 * Called when there is a window message for the desklets window.
 * This function is called before OnProceessMessage() and allows
 * the desklet to have AveDesk not handle the message anymore.
 * @param lpData - DESKLET_DATA see remarks
 * @param hwnd - the window handle of the desklet
 * @param msg - see win32 sdk
 * @param wParam - see win32 sdk
 * @param lParam - see win32 sdk
 * @param isHandled - pointer to a BOOL value that should be set to
 *						TRUE iff AveDesk shouldn't handle this
 *						message anymore.
 * @see OnProcessMessage
 * @return the return value for the WNDPROC if *isHandled is TRUE, otherwise any value is valid.
 */
LRESULT CALLBACK OnProcessMessageEx(DESKLET_DATA* lpData, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, BOOL* isHandled);


/**
 * Called when the desklets REAL visibility is changed.
 * The desklets status could be VISIBLE, but the desklet
 * could still be hidden - for example in case if its on the ShowCase layer.
 * This function notifies the desklet about changes for the real visibility,
 * so the desklet could only update itself when it is REALLY visible, to
 * save CPU time.
 * @param lpData - see DESKLET_DATA.
 * @param becomesVisible - TRUE iff the desklet will become visible again, FALSE if it will be hidden.
 * @param flags - unused currently.
 * @version minimal 1.2
 * @return TRUE iff the callee handles this call.
 */
BOOL CALLBACK OnVisibilityNotify(DESKLET_DATA *lpData, BOOL becomesVisible, DWORD flags);


/// AveDesk supports desklet with skins. For example, an iTunes remote controller could
/// have different appearances/looks. Those appereances are called 'skins'. 
/// By not hardcoding any value into the desklet, but rather let a special skin-file 
/// define those, a desklet will become more versatile and a greater seperation of
///	function and look is created.
///
/// To get a list of skins, AveDesk let the desklet enumerates the skins. This way,
/// desklets are free to choose their favorite skinformat.
///	When AveDesk wants to get a list of skins, it tries to call the OnCreateSkinEnumeratorW()
///	function. This function should return a pointer to a structure/class holding all the
/// data for the enumeration. The pointer type of the return value, is void*, but this
///	type is called ENUMERATOR_DATA* for clarity in the docs.
///
/// Next, AveDesk calls OnGetSkinW() with as the first parameter the pointer returned by 
/// the OnCreateSkinEnumeratorW() function. As long as there are skins to enumerate, this
/// function will be called. The second parameter to this function is a SkinInfo
/// pointer. This structure should be filled with information by the desklet; one of 
/// the fields of this structure is a string to hold a key.
///
/// If the user wants to instantiate a skin for the desklet, the OnCreateFromSkinW()
/// function is called with as parameters the ENUMERATOR_DATA* pointer returned
/// by OnCreateSkinEnumeratorW() and a SkinInfo structure that has at least
//  the key the same as the skin that should be instantiated.
///
/// When the enumeration data is no longer needed, OnDestroySkinEnumeratorW() is called,
/// which allows the desklet to free the structure associated with the ENUMERATOR_DATA*
/// pointer returned by OnCreateFromSkinW().
///
///
/// General skin-enumerating-cyclus:
///
///		// start enumerating:
///		pointer = OnCreateSkinEnumerator(dllHandle, pathToDeskletsFolder);
///
///			// as long as there are skins, call:
///			OnGetSkinW(pointer, info);
///
///		// if the user picked a skin to instantiate:
///		OnCreateFromSkinW(pointer, info); // info has at least the same key-field as 
///										  // the call to OnGetSkinW() for the particular skin gave.
///
///		// done with the enumerator:
///		OnDestroySkinEnumeratorW(pointer);
///
///
///	NOTE: OnCreateFromSkinW() can also be called with a ENUMERATOR_DATA* pointer that's NULL.
///			In that case, the function should try to instantiate the desklet with the 
///			skin that matches the SkinInfo best.

#define ENUMERATOR_DATA void

/**
 * Called when AveDesk wants to get a list of skins for the desklet.
 * This function is not associated with a particular instance of the desklet.
 * 
 * @param hDll - the HINSTANCE handle of the desklets dll that is currently loaded into the AveDesk process.
 * @param deskletDirectory - UNICODE string holding the path to the folder the desklets DLL is located in.
 * @version minimal 1.2
 * @return a pointer to a structure/class that will be passed in all skin-enumerating callback functions
 *			for this particular enumeration.
 */
ENUMERATOR_DATA* CALLBACK OnCreateSkinEnumeratorW(HINSTANCE hDll,const WCHAR* deskletDirectory);

/**
 * Called when the skin-enumerator is destroyed.
 * @param lpEnumData - the pointer returned by OnCreateSkinEnumeratorW() that should be freed.
 * @version minimal 1.2
 */
void CALLBACK OnDestroySkinEnumeratorW(ENUMERATOR_DATA* lpEnumData);

/**
 * Called for an skin-enumeration to get information about a skin.
 * @param lpEnumData - the pointer returned by OnCreateSkinEnumeratorW() for which information about a skin
 *						is requested.
 * @param info - info->num holds the number of the skin for which data is requested. The other fields
 *					should be filled accordingly with the requested information about the skin.
 *					Each string-buffer of the structure has a maxXLength (with X the name of the field).
 *					If one of the buffers is too small, this function should set the length that
 *					is needed for field X in maxXLength and return AVEDESK_BUF_TOO_SMALL.
 *					AveDesk then allocate enough space for the strings fields so that they can
 *					hold at least maxXLength-1 characters.
 * @version minimal 1.2
 * @return the calle should return one of the following:
 *			. AVEDESK_NO_MORE_SKINS if there is no skin for info->num to supply data about.
 *			. AVEDESK_MORE_SKINS if there are still skins to enumerate.
 *			. AVEDESK_BUF_TOO_SMALL if one of the string buffer fields is too small too hold
 *									 the data for that particular field.
 *			
 */
INT CALLBACK OnGetSkinW(ENUMERATOR_DATA* lpEnumData , SkinInfo* info);

/**
 * Called when a particular skin of a desklet is instantiated.
 * @param hwndDesklet - the handle of the desklets window. Must be used in all Desklet*() API functions to
 *						identify the desklet.
 * @param hInstance - the handle of the dll of the desklet loaded into avedesk's processspace
 * @param lpEnumData - a pointer to the ENUMERATOR_DATA structure used to collect data
 *						about a skin.
 *						NOTE: lpEnumData can be NULL, in which case the callee
 *							should try to match its skins as good as possible with
 *							the SkinInfo* info parameter.
 * @param info - a pointer to a SkinInfo structure describing the skin that needs to be instantiated.
 *				 If lpEnumData != NULL, at least info->key is the same as was set by OnGetSkinW() for the
 *				 particular skin to instantiate.
 *				 If lpEnumData == NULL, info->key could be anything and the desklet
 * @version minimal 1.2
 * @remark - the desklet should call DeskletSetSkinInfoW() itself to tell AveDesk
 *				about the skin used for the newly instantiated desklet. AveDesk
 *				does not do this automatically.
 * @remark - this function cannot fail. If the desklet cannot find a particular skin,
 *				it's recommended to delegate this call to OnCreate(hwndDesklet, hInstance, NULL, NULL)
 *				instead.
 * @return a DESKLET_DATA* pointer that will be used in all other callback events calls - see docs and OnCreate().
 * @see OnCreate
 */
DESKLET_DATA* CALLBACK OnCreateFromSkinW(HWND hwndDesklet, HINSTANCE hInstance,
							ENUMERATOR_DATA* lpEnumData, const SkinInfo* info);





/**
 * Called when the leftmousebutton is held down on a layer.
 * @param lpData - see DESKLET_DATA.
 * @param layerId - the id of the layer.
 * @param layerName - the name of the layer (ANSI instead of UNICODE, because it's an internal name only).
 * @param pt - a pointer to a POINT structure describing the point the mouse is down on, relative
 *				to the layers size (layerSize param).
 * @param layerSize - a pointer to a SIZE structure describing the size of the layer.
 * @param keys - the keys held down.
 * @remark if the layer is using placement (see DeskletLayerSetPlacement()), then layerSize
 *			and will always be equal to the size of the layer as it was set with DeskletLayerSetPlacement().
 *			Also, the pt parameter will then be simply a 1:1 mapping. This effectively means that if
 *			placement is used, it doesn't matter what size the desklet is, the coordinates will
 *			always be passed on the same scale as they were passed by DeskletLayerSetPlacement().
 *			If the layer doesn't use placement, the coordinates are passed in the same style
 *			as OnLeftButtonClick().
 * @return TRUE iff the user should not be able to move the desklet when the user is holding down the mouse
 *			on this layer.
 * @version minimal 1.3
 */
BOOL CALLBACK OnLayerMouseDown(DESKLET_DATA* lpData, UINT layerId, const char* layerName,
								const POINT* pt, const SIZE* layerSize, DWORD keys);

/**
 * Called when the leftmousebutton was held down on a layer and is released again.
 * @param lpData - see DESKLET_DATA.
 * @param layerId - the id of the layer.
 * @param layerName - the name of the layer (ANSI instead of UNICODE, because it's an internal name only).
 * @param pt - a pointer to a POINT structure describing the point the mouse is released on, relative
 *				to the layers size (layerSize param).
 * @param layerSize - a pointer to a SIZE structure describing the size of the layer.
 * @param keys - the keys held down.
 * @remark if the layer is using placement (see DeskletLayerSetPlacement()), then layerSize
 *			and will always be equal to the size of the layer as it was set with DeskletLayerSetPlacement().
 *			Also, the pt parameter will then be simply a 1:1 mapping. This effectively means that if
 *			placement is used, it doesn't matter what size the desklet is, the coordinates will
 *			always be passed on the same scale as they were passed by DeskletLayerSetPlacement().
 *			If the layer doesn't use placement, the coordinates are passed in the same style
 *			as OnLeftButtonClick().
 * @return TRUE iff OnLayerClick() should not be called for this layer when the down and up 
 *				coordinates were inside the layers area.
 * @version minimal 1.3
 */
BOOL CALLBACK OnLayerMouseUp(DESKLET_DATA* lpData, UINT layerId, const char* layerName,
								const POINT* pt, const SIZE* layerSize, DWORD keys);

/**
 * Called when the leftmousebutton was clicked upon a layer.
 * @param lpData - see DESKLET_DATA.
 * @param layerId - the id of the layer.
 * @param layerName - the name of the layer (ANSI instead of UNICODE, because it's an internal name only).
 * @param pt - a pointer to a POINT structure describing the point the mouse is released on, relative
 *				to the layers size (layerSize param).
 * @param layerSize - a pointer to a SIZE structure describing the size of the layer.
 * @param keys - the keys held down.
 * @remark if the layer is using placement (see DeskletLayerSetPlacement()), then layerSize
 *			and will always be equal to the size of the layer as it was set with DeskletLayerSetPlacement().
 *			Also, the pt parameter will then be simply a 1:1 mapping. This effectively means that if
 *			placement is used, it doesn't matter what size the desklet is, the coordinates will
 *			always be passed on the same scale as they were passed by DeskletLayerSetPlacement().
 *			If the layer doesn't use placement, the coordinates are passed in the same style
 *			as OnLeftButtonClick().
 * @return TRUE iff OnLeftButtonClick() should not be called.
 * @version minimal 1.3
 */
BOOL CALLBACK OnLayerClick(DESKLET_DATA* lpData, UINT layerId, const char* layerName,
								const POINT* pt, const SIZE* layerSize, DWORD keys);

/**
 * Called when the rightmousebutton was clicked upon a layer.
 * @param lpData - see DESKLET_DATA.
 * @param layerId - the id of the layer.
 * @param layerName - the name of the layer (ANSI instead of UNICODE, because it's an internal name only).
 * @param pt - a pointer to a POINT structure describing the point the mouse is released on, relative
 *				to the layers size (layerSize param).
 * @param layerSize - a pointer to a SIZE structure describing the size of the layer.
 * @param keys - the keys held down.
 * @remark if the layer is using placement (see DeskletLayerSetPlacement()), then layerSize
 *			and will always be equal to the size of the layer as it was set with DeskletLayerSetPlacement().
 *			Also, the pt parameter will then be simply a 1:1 mapping. This effectively means that if
 *			placement is used, it doesn't matter what size the desklet is, the coordinates will
 *			always be passed on the same scale as they were passed by DeskletLayerSetPlacement().
 *			If the layer doesn't use placement, the coordinates are passed in the same style
 *			as OnLeftButtonClick().
 * @return TRUE iff OnRightButtonClick() should not be called.
 * @version minimal 1.3
 */
BOOL CALLBACK OnLayerRightClick(DESKLET_DATA* lpData, UINT layerId, const char* layerName,
								const POINT* pt, const SIZE* layerSize, DWORD keys);


/**
 * Called when the mouse enters a layer.
 * @param lpData - see DESKLET_DATA.
 * @param layerId - the id of the layer.
 * @param layerName - the name of the layer (ANSI instead of UNICODE, because it's an internal name only).
 * @param pt - a pointer to a POINT structure describing the point the mouse is over, relative
 *				to the layers size (layerSize param).
 * @param layerSize - a pointer to a SIZE structure describing the size of the layer.
 * @remark if the layer is using placement (see DeskletLayerSetPlacement()), then layerSize
 *			and will always be equal to the size of the layer as it was set with DeskletLayerSetPlacement().
 *			Also, the pt parameter will then be simply a 1:1 mapping. This effectively means that if
 *			placement is used, it doesn't matter what size the desklet is, the coordinates will
 *			always be passed on the same scale as they were passed by DeskletLayerSetPlacement().
 *			If the layer doesn't use placement, the coordinates are passed in the same style
 *			as OnLeftButtonClick().
 * @return TRUE iff OnMouseOn() should not be called.
 * @version minimal 1.3
 */
BOOL CALLBACK OnLayerMouseOn(DESKLET_DATA* lpData, UINT layerId, const char* layerName,
								const POINT* pt, const SIZE* layerSize);


/**
 * Called when the mouse moves over a layer.
 * @param lpData - see DESKLET_DATA.
 * @param layerId - the id of the layer.
 * @param layerName - the name of the layer (ANSI instead of UNICODE, because it's an internal name only).
 * @param pt - a pointer to a POINT structure describing the point the mouse is over, relative
 *				to the layers size (layerSize param).
 * @param layerSize - a pointer to a SIZE structure describing the size of the layer.
 * @remark if the layer is using placement (see DeskletLayerSetPlacement()), then layerSize
 *			and will always be equal to the size of the layer as it was set with DeskletLayerSetPlacement().
 *			Also, the pt parameter will then be simply a 1:1 mapping. This effectively means that if
 *			placement is used, it doesn't matter what size the desklet is, the coordinates will
 *			always be passed on the same scale as they were passed by DeskletLayerSetPlacement().
 *			If the layer doesn't use placement, the coordinates are passed in the same style
 *			as OnLeftButtonClick().
 * @return TRUE iff OnMouseMove() should not be called.
 * @version minimal 1.3
 */
BOOL CALLBACK OnLayerMouseMove(DESKLET_DATA* lpData, UINT layerId, const char* layerName,
								const POINT* pt, const SIZE* layerSize);


/**
 * Called when the mouse exits a layer.
 * @param lpData - see DESKLET_DATA.
 * @param layerId - the id of the layer.
 * @param layerName - the name of the layer (ANSI instead of UNICODE, because it's an internal name only).
 * @return TRUE iff OnMouseOut() should not be called.
 * @version minimal 1.3
 */
BOOL CALLBACK OnLayerMouseOut(DESKLET_DATA* lpData, UINT layerId, const char* layerName);


/**
 * Called when the mousewheel is scrolled forward when the mouse is over a layer.
 * @param lpData - see DESKLET_DATA.
 * @param layerId - the id of the layer.
 * @param layerName - the name of the layer (ANSI instead of UNICODE, because it's an internal name only).
 * @param pt - a pointer to a POINT structure describing the point the mouse is over, relative
 *				to the layers size (layerSize param).
 * @param layerSize - a pointer to a SIZE structure describing the size of the layer.
 * @param keys - the keys held down.
 * @param numScrolls - the number of scrolls the mousewheel made.
 * @remark if the layer is using placement (see DeskletLayerSetPlacement()), then layerSize
 *			and will always be equal to the size of the layer as it was set with DeskletLayerSetPlacement().
 *			Also, the pt parameter will then be simply a 1:1 mapping. This effectively means that if
 *			placement is used, it doesn't matter what size the desklet is, the coordinates will
 *			always be passed on the same scale as they were passed by DeskletLayerSetPlacement().
 *			If the layer doesn't use placement, the coordinates are passed in the same style
 *			as OnLeftButtonClick().
 * @return TRUE iff the callee handles this call.
 * @version minimal 1.3
 */
BOOL CALLBACK OnLayerMouseWheelForward(DESKLET_DATA* lpData, UINT layerId, const char* layerName,
								const POINT* pt, const SIZE* layerSize, DWORD keys, int numScrolls);

/**
 * Called when the mousewheel is scrolled backwards when the mouse is over a layer.
 * @param lpData - see DESKLET_DATA.
 * @param layerId - the id of the layer.
 * @param layerName - the name of the layer (ANSI instead of UNICODE, because it's an internal name only).
 * @param pt - a pointer to a POINT structure describing the point the mouse is over, relative
 *				to the layers size (layerSize param).
 * @param layerSize - a pointer to a SIZE structure describing the size of the layer.
 * @param keys - the keys held down.
 * @param numScrolls - the number of scrolls the mousewheel made.
 * @remark if the layer is using placement (see DeskletLayerSetPlacement()), then layerSize
 *			and will always be equal to the size of the layer as it was set with DeskletLayerSetPlacement().
 *			Also, the pt parameter will then be simply a 1:1 mapping. This effectively means that if
 *			placement is used, it doesn't matter what size the desklet is, the coordinates will
 *			always be passed on the same scale as they were passed by DeskletLayerSetPlacement().
 *			If the layer doesn't use placement, the coordinates are passed in the same style
 *			as OnLeftButtonClick().
 * @return TRUE iff the callee handles this call.
 * @version minimal 1.3
 */
BOOL CALLBACK OnLayerMouseWheelBackward(DESKLET_DATA* lpData, UINT layerId, const char* layerName,
								const POINT* pt, const SIZE* layerSize, DWORD keys, int numScrolls);;


/**
 * Called before a desklet instance is removed from a theme (not when the theme is closed).
 * @param lpData - see DESKLET_DATA.
 * @return TRUE iff the callee handles this call.
 * @version minimal 1.3
 */
BOOL CALLBACK OnBeforeRemoval(DESKLET_DATA* lpData);