//	AveDesk function headers with javadoc/doxygen style comments
//	(c) copyright Andreas Verhoeven 2005
//	Do not modify, reproduce or use this file without my permission!



/**
 *	Gets AveDesk's version.
 *	@return the version number of the current AveDesk host-application.
 *  @remark the version number is defined as major * 100 + minor * 10 + subversion;
 */
int CALLBACK DeskletGetAveDeskVersion();

/**
 * Sets the sublabel of a desklet.
 * @param hwndDesklet the handle of the desklet to set the sublabel for
 * @param subLabel    the text of the sublabel to set to. Cannot be NULL.
 * @remark the sublabel is not persisted when AveDesk or the current theme is closed.
 */
void CALLBACK DeskletSetSubLabelW(HWND hwndDesklet, const WCHAR* subLabel);


/**
 * Sets the sublabel of a desklet (NONUNICODE)
 * @deprecated
 * @see        DeskletSetSubLabelW
 * @remark     NON-UNICODE function of DeskletSetSubLabel
 */
void CALLBACK DeskletSetSubLabel(HWND hwndDesklet, char* subLabel);

/**
 * Adds a propertysheet to the desklets property-dialog.
 * @param  hwndDesklet the handle of the desklet to add the propertysheet for
 * @param  hpg         the propertysheet to add to dialog. Cannot be NULL.
 * @return the number of propertysheets present
 * @remark This function needs to be called only in the OnAddPropertySheet() callback function.
 *         Everytime the property-dialog is displayed, All old HPROPSHEETPAGE's are removed and
 *         OnAddPropertySheet() is called. In that function, the HPROPSHEETPAGE should be
 *         made again. In other words, HPROPSHEETPAGE are not persisted.
 */
int CALLBACK DeskletAddPropertySheet(HWND hwndDesklet, HPROPSHEETPAGE hpg);

/**
 * Gives the default menu for a desklet.
 * @param  hwndDesklet the handle of the desklet to get the default menu for 
 * @param  startId     the id that the first menuitem starts with.
 * @return An HMENU with the default-desklet-menu if successfull. Otherwise NULL.
 * @remark 1) The caller gets ownership of the HMENU and thus must destroy it with DestroyMenu()
 *         2) The default menu for a desklet is the menu a user gets when he right-clicks a desklet
 *            that has no rightclick-handler. The menu contains items to interact with avedesk and the
 *            desklet.
 */
HMENU CALLBACK DeskletGetDefaultMenu(HWND hwndDesklet, int startId);

/**
 * Performs the command associated with a menuitem for the desklets default-menu.
 * @param  hwndDesklet the handle of the desklet to perform the command for.
 * @param  startId     the id that the first menuitem starts with.
 * @param  cmd         the id of the menuitem to perform the command for.
 * @remark this function is typically used together with <function>DeskletGetDefaultMenu</function>
 */
void CALLBACK DeskletDoDefaultMenuCommand(HWND hwndDesklet,int startId,int cmd);

/**
 * Redraws a desklet completely.
 * @param hwndDesklet the handle of the desklet to redraw
 */
void CALLBACK DeskletRedraw(HWND hwndDesklet);
//--------------------------------------------------------------------------------------------
/**
 * Moves a desklet.
 * @param hwndDesklet          the handle of the desklet to move
 * @param newBoundingRectangle a pointer to the RECTangle that holds the coords to move the desklet to.
 * @param redraw               if TRUE, the desklet will be redrawn immediately. Otherwise, the desklet
 *                             won't be redrawn until A redraw command has been issued.
 * @remark This function can be used to scale and move the desklet in the same time, because
 *         the RECTANGLE holds the 4 bounding-points and thus also defines the new width and height.
 * @remark 1.3 changes: added a const-modifier to newBoundingRectangle
 */
void CALLBACK DeskletMove(HWND hwndDesklet, const RECT* newBoundingRectangle, BOOL redraw);

/**
 * Gets the visibility of the desklet
 * @param hwndDesklet the handle of the desklet to check the visibility of.
 * @return TRUE iff the desklet is visible.
 */
BOOL CALLBACK DeskletIsVisible(HWND hwndDesklet);

//--------------------------------------------------------------------------------------------
/**
 * Gets the rectangle for a desklet
 * @param hwndDesklet the handle of the desklet to get the rect for.
 * @param rcDesklet   A pointer to the rectangle that will receive the rectangle of this desklet.
 *                    cannot be NULL.
 * @return TRUE iff the desklet is visible.
 */
BOOL CALLBACK DeskletGetRect(HWND hwndDesklet,RECT *rcDesklet);

//--------------------------------------------------------------------------------------------
/**
 * Gets the label for a desklet.
 * @param hwndDesklet the handle of the desklet to get the label for.
 * @param wLabel      a pointer to the buffer that will receive the label forthis desklet. cannot be NULL.
 * @param maxSize     the maximum size of the buffer pointed to by wLabel.
 * @return -1 if not succesfull (the desklet does not exist). Otherwise, the length of the label + 1 (zero-terminator)
 *               will be returned. If the buffer is not large enough to hold the desklet's label. Nothing will be
 *               copied.
 * @remark Function differs from the NON-UNICODE version
 * @remark The caller should check if maxSize >= the return value to check if there was actually data copied
 *         into the wLabel buffer.
 * @see DeskletGetLabel
 */
int CALLBACK DeskletGetLabelW(HWND hwndDesklet,WCHAR* wLabel, int maxSize);

/**
 * Gets the label of a desklet.
 * @param hwndDesklet the handle of the desklet to get the label for.
 * @param szLabel the buffer that will receive the label. The buffer should be able to hold 1024 characters, + 1 zero-terminator.
 * @return -1 if not successfull (the desklet does not exist), otherwise the number of characters copied or the length of the label
 *         if szLabel is NULL.
 * @remark NON-UNICODE version of DeskletGetLabelW
 * @see DeskletGetLabelW
 */
int CALLBACK DeskletGetLabel(HWND hwndDesklet,char *szLabel);


//--------------------------------------------------------------------------------------------
/**
 * Sets the label of a desklet.
 * @param hwndDesklet the handle of the desklet to set the label for.
 * @param szLabel     the text to set the label to. Cannot be NULL.
 * @return If successfull, the number of copied characters has been returned.
 *         Otherwise -1 has been returned.
 * @remark UNICODE version of DeskletSetLabel
 * @see DeskletSetLabel
 */
int CALLBACK DeskletSetLabelW(HWND hwndDocklet,const WCHAR *szLabel);

/**
 * Sets the label of a desklet.
 * @param hwndDesklet the handle of the desklet to set the label for.
 * @param szLabel     the text to set the label to. Cannot be NULL.
 * @return If successfull, the number of copied characters has been returned.
 *         Otherwise -1 has been returned.
 * @remark NON-UNICODE version of DeskletSetLabelW
 * @see DeskletSetLabelW
 */
int CALLBACK DeskletSetLabel(HWND hwndDesklet, const char* szLabel);


/**
 * Loads an Image File (png, jpg, bmp, ico) as a GDI+ Bitmap objects
 * @param szImage the path of the image file to load. Can be relative to the AveDesk folder.
 *                cannot be NULL.
 * @return if successfull, a pointer to an GDI+ Bitmap object will be returned. Otherwise, NULL
 *         will be returned.
 * @remark The caller gets ownership of the returned GDI+ Bitmap object and is thus responsible
 *         for releasing it with C++'s delete function. 
 * @remark ONLY SUPPORTS VISUAL STUDIO 6, see DeskletLoadHBITMAPImageW for a language independant version.
 * @see DeskletLoadHBITMAPImageW
 * @remark UNICODE version of DeskletLoadGDIPlusImage
 * @see DeskletLoadGDIPlusImage 
 */
Bitmap* CALLBACK DeskletLoadGDIPlusImageW(const WCHAR *szImage);

/**
 * Loads an Image File (png, jpg, bmp, ico) as a GDI+ Bitmap objects
 * @param szImage the path of the image file to load. Can be relative to the AveDesk folder.
 *                cannot be NULL.
 * @return if successfull, a pointer to an GDI+ Bitmap object will be returned. Otherwise, NULL
 *         will be returned.
 * @remark The caller gets ownership of the returned GDI+ Bitmap object and is thus responsible
 *         for releasing it with C++'s delete function. 
 * @remark ONLY SUPPORTS VISUAL STUDIO 6, see DeskletLoadHBITMAPImageW for a language independant version.
 * @see DeskletLoadHBITMAPImageW
 * @remark NON-UNICODE version of DeskletLoadGDIPlusImageW
 * @see DeskletLoadGDIPlusImageW
 */
Bitmap* CALLBACK DeskletLoadGDIPlusImage(const char *szImage);

//-----------------------------------------------------------------------------

/**
 * Sets the image for this desklet.
 * @param hwndDesklet the handle of the desklet to set the image for.
 * @param lpImageNew the image to set.
 * @param bAutomaticallyDeleteImage, if TRUE, AveDesk Host Application gets ownership of
 *        lpImageNew and is responsible for freeing its memory.
 * @remark ONLY COMPATIBLE WITH VISUAL STUDIO 6. USE DeskletSetHBITMAP for a language-
 *         independent function.
 * @see DeskletSetHBITMAP
 */
void CALLBACK DeskletSetImage(HWND hwndDesklet,Image *lpImageNew,BOOL bAutomaticallyDeleteImage);


//--------------------------------------------------------------------------------------------
/**
 * Adds a FORMATETC to the list of `dragformats` that are accepted exported
 * by this desklet.
 * @param hwndDesklet the handle of the desklet to add the dragformat to
 * @param format the FORMATETC describing the dragformat to add.
 * @remark for more info on FORMATETC and OLE drag-'n-drop, see the MSDN.
 */
void CALLBACK DeskletAddDragDropFormat(HWND hwndDesklet,FORMATETC* format);
//--------------------------------------------------------------------------------------------
/**
 * Removes a FORMATETC from the list of `dragformats` that are accepted exported
 * by this desklet.
 * @param hwndDesklet the handle of the desklet to remove the dragformat from
 * @param format      the FORMATETC describing the dragformat to remove.
 * @remark for more info on FORMATETC and OLE drag-'n-drop, see the MSDN.
 */
void CALLBACK DeskletRemoveDragDropFormat(HWND hwndDesklet,const FORMATETC* format);
//--------------------------------------------------------------------------------------------
/**
 * Sets an image for a desklet by using a filename.
 * @param hwndDesklet   the handle of the desklet to set the image for.
 * @param imageFilename the filename of the image to set. If imageFilename is NULL,
 *                      then the current image will be removed.
 * @remark UNICODE version of DeskletSetImageFile
 * @see DeskletSetImageFile
 */
void CALLBACK DeskletSetImageFileW(HWND hwndDesklet, const WCHAR *imageFilename);

/**
 * Sets an image for a desklet by using a filename.
 * @param hwndDesklet   the handle of the desklet to set the image for.
 * @param imageFilename the filename of the image to set. If imageFilename is NULL,
 *                      then the current image will be removed.
 * @remark NON-UNICODE version of DeskletSetImageFileW
 * @see DeskletSetImageFileW
 */
void CALLBACK DeskletSetImageFile(HWND hwndDesklet,const char *imageFilename);

/*
 * Sets an overlay image for a desklet.
 * @remark Programmers are encouraged to use the DeskletLayer* set of functions.
 * @deprecated
 */
void CALLBACK DeskletSetImageOverlay(HWND hwndDesklet, Image *lpImageNew,BOOL bAutomaticallyDeleteImage);
//--------------------------------------------------------------------------------------------
/**
 * Sets an overlay imagefile for a desklet.
 * @remark Programmers are encouraged to use the DeskletLayer* set of functions.
 * @deprecated
 */
void CALLBACK DeskletSetImageOverlayFile(HWND hwndDesklet,char *szImage);
//--------------------------------------------------------------------------------------------
/**
 * Shows a browse-for-image dialog to the user.
 * @param HWND hwndParent - the parent for the browse-for-image-dialog.
 * @param szImage the buffer to receive the relative path to the selected image file. 
 *        szImage cannot be NULL and must be at least MAX_PATH characters in length.
 *		  The default selected file will also be based on szImage.
 * @return TRUE iff the user selected a new imagefile, otherwise false.
 * @remark UNICODE version of DeskletBrowseForImage
 * @see DeskletBrowseForImage
 */

BOOL CALLBACK DeskletBrowseForImageW(HWND hwndParent, WCHAR *szImage);
/**
 * Shows a browse-for-image dialog to the user.
 * @param HWND hwndParent - the parent for the browse-for-image-dialog.
 * @param szImage the buffer to receive the relative path to the selected image file. 
 *        szImage cannot be NULL and must be at least MAX_PATH characters in length.
 *		  The default selected file will also be based on szImage.
 * @return TRUE iff the user selected a new imagefile, otherwise false.
 * @remark NON-UNICODE version of DeskletBrowseForImageW
 * @see DeskletBrowseForImageW
 */
BOOL CALLBACK DeskletBrowseForImage(HWND hwndParent,char *szImage);


//--------------------------------------------------------------------------------------------
/**
 * Gets the relative folder for a desklet.
 * @param hwndDesklet the desklet to retrieve the relative folder for.
 * @param folder the buffer to receive the relative folder of the desklet.
 *			folder cannot be NULL and much be at least MAX_PATH characters long.
 * @remark UNICODE version of DeskletGetRelativeFolder.
 * @see DeskletGetRelativeFolder
 */
void CALLBACK DeskletGetRelativeFolderW(HWND hwndDesklet,WCHAR* folder);

/**
 * Gets the relative folder for a desklet.
 * @param hwndDesklet the desklet to retrieve the relative folder for.
 * @param folder the buffer to receive the relative folder of the desklet.
 *			folder cannot be NULL and much be at least MAX_PATH characters long.
 * @remark NON-UNICODE version of DeskletGetRelativeFolderW
 * @see DeskletGetRelativeFolderW
 */
void CALLBACK DeskletGetRelativeFolder(HWND hwndDocklet,char* szFolder);

//--------------------------------------------------------------------------------------------
/**
 * Gets the root folder for avedesk.
 * @param hwndDesklet the desklet to retrieve the root folder for.
 * @param folder the buffer to receive the root folder of the desklet.
 *			folder cannot be NULL and much be at least MAX_PATH characters long.
 * @remark UNICODE version of DeskletGetRootFolder.
 * @see DeskletGetRootFolder
 */
void CALLBACK DeskletGetRootFolderW(HWND hwndDesklet,WCHAR* folder);
/**
 * Gets the root folder for avedesk.
 * @param hwndDesklet the desklet to retrieve the root folder for.
 * @param folder the buffer to receive the root folder of the desklet.
 *			folder cannot be NULL and much be at least MAX_PATH characters long.
 * @remark NON-UNICODE version of DeskletGetRootFolderW.
 * @see DeskletGetRootFolderW
 */

void CALLBACK DeskletGetRootFolder(HWND hwndDesklet,char *szFolder);


//--------------------------------------------------------------------------------------------
/**
 * Shows the default property-dialog for a desklet.
 * @deprecated
 * @remark Use the OnAddPropertySheet functionality
 */
void CALLBACK DeskletDefaultConfigDialog(HWND hwndDesklet,PropertyDialogStruct* pds);


/**
 * Removes an desklet instance.
 * @param hwndDesklet the handle of the desklet instance to remove.
 * @param Ask iff TRUE the user will be asked if the desklet should be removed. If FALSE, the
 *				the desklet will be removed without asking the user.
 * @return TRUE iff the desklet will be removed, otherwise FALSE.
 *
 */
BOOL CALLBACK DeskletRemove(HWND hwndDesklet, BOOL Ask);
//--------------------------------------------------------------------------------------------
/**
 * Loads and creates a desklet from an INI file.
 * @deprecated
 * @see DeskletLoadFromConfigW
 */
HWND CALLBACK DeskletLoadFromIniW(const WCHAR* filename, const WCHAR* ini, const WCHAR* iniGroup, void** p, BOOL makeVisible);

/**
 * Loads and creates a desklet.
 * @see DeskletLoadFromDLLW
 */
HWND CALLBACK DeskletLoad(char* filename,char* ini, char* iniGroup,void** p,BOOL makeVisible);
//--------------------------------------------------------------------------------------------
/**
 * Hides a desklet.
 * @param hwndDesklet the handle of the desklet to hide.
 */
void CALLBACK DeskletHide(HWND hwndDesklet);
//--------------------------------------------------------------------------------------------
/**
 * Shows a desklet.
 * @param hwndDesklet the handle of the desklet to show.
 */
void CALLBACK DeskletShow(HWND hwndDesklet);
//--------------------------------------------------------------------------------------------
/**
 * Sets the position of desklet.
 * @param hwndDesklet the handle of the desklet to set the position of.
 * @param p a pointer to the POINT structure describing the new position of the desklet.
 */
void CALLBACK DeskletSetPosition(HWND hwndDesklet,POINT *p);
//--------------------------------------------------------------------------------------------
/**
 * Sets the size of desklet.
 * @param hwndDesklet the handle of the desklet to set the size of.
 * @param s a pointer to the SIZE structure describing the new size of the desklet.
 * @remark this function will always cause a total redraw immediately
 */
void CALLBACK DeskletSetSize(HWND hwndDesklet,SIZE *s);
//--------------------------------------------------------------------------------------------
/**
 * Sets the alpha-level of desklet.
 * @param hwndDesklet the handle of the desklet to set the alpha of.
 * @param alpha the new alpha level for the desklet range [0,255].
 */
void CALLBACK DeskletSetAlpha(HWND hwndDesklet,BYTE alpha);
//--------------------------------------------------------------------------------------------
/**
 * Sets the rotation of desklet.
 * @param hwndDesklet the handle of the desklet to set the rotation of.
 * @param p rotation the new rotation value for the desklet
 * @remark this function will cause a total redraw immediately.
 */
void CALLBACK DeskletSetRotation(HWND hwndDesklet,INT rotation);
//--------------------------------------------------------------------------------------------
/**
 * Gets the position of a desklet.
 * @param hwndDesklet the handle of the desklet to get the position of.
 * @param p a pointer to a POINT structure receiving the position of the desklet. Cannot be NULL.
 */
void CALLBACK DeskletGetPosition(HWND hwndDesklet,POINT *p);
//--------------------------------------------------------------------------------------------
/**
 * Gets the size of a desklet.
 * @param hwndDesklet the handle of the desklet to get the size of.
 * @param s a pointer to a SIZE structure receiving the size of the desklet. Cannot be NULL.
 */
void CALLBACK DeskletGetSize(HWND hwndDesklet,SIZE *s);
//--------------------------------------------------------------------------------------------
/**
 * Gets the alpha-level of a desklet.
 * @param hwndDesklet the handle of the desklet to get the alpha of.
 * @return the current alpha-level of the desklet, range [0,255].
 */
BYTE CALLBACK DeskletGetAlpha(HWND hwndDesklet);
//--------------------------------------------------------------------------------------------
/**
 * Gets the rotation of a desklet.
 * @param hwndDesklet the handle of the desklet to get the rotation of.
 * @return the current rotation of the desklet.
 */
INT CALLBACK DeskletGetRotation(HWND hwndDesklet);
//--------------------------------------------------------------------------------------------
/**
 * Gets the ID of the OK button for a desklet propertysheet.
 * @deprecated
 * @remark programmers are encouraged to use the AddOnPropertySheet functionality
 */
UINT CALLBACK DeskletGetParentOK();
//--------------------------------------------------------------------------------------------
/**
 * Performs an avedesk manager function.
 * @param action, the action to perform. must be one of the following:
 *					DM_CONTROLPANEL, opens the control panel
 *					DM_SAVE, saves the current theme
 *					DM_ABOUT, opens the about dialog
 *					DM_HELP, opens the help
 *					DM_EXIT, closes avedesk
 *					DM_GLOBALSETTINGS, opens the global settings dialog 
 *					DM_DEFAULTDESKLETPROPERTIES, opens the default desklet properties dialog
 *					DM_ADDDESKLET, opens the add-desklet dialog.
 */
void CALLBACK DeskletManager(UINT action);
//--------------------------------------------------------------------------------------------
/**
 * Activates Extended (OLE) drag&drop for a desklet.
 * @param hwndDesklet the handle of the desklet to activate drag and drop for
 * @remark call this function if you want your desklet to be able to receive drags
 */
void CALLBACK DeskletActivateExtendedDragDrop(HWND hwndDesklet);
//--------------------------------------------------------------------------------------------
/**
 * Deactivates Extended (OLE) drag&drop for a desklet.
 * @param hwndDesklet the handle of the desklet to deactivate drag and drop for.
 */
void CALLBACK DeskletDeactivateExtendedDragDrop(HWND hwndDesklet);
//--------------------------------------------------------------------------------------------
/**
 * Gets the COM IDesklet pointer wrapper for a desklet.
 * @param hwndDesklet the handle of the desklet to retrieve the wrapper COM IDesklet pointer for.
 * @param ptr, A pointer to the pointer receiving the IDesklet object.
 * @remark caller must call AddRef()
 * @return TRUE iff successfull, otherwise FALSE.
 */
BOOL CALLBACK DeskletGetWrapper(HWND hwndDesklet, /*out, retval*/ IDesklet **ptr);
//--------------------------------------------------------------------------------------------
/**
 * Fades a desklet in.
 * @param hwndDesklet the handle of the desklet to fade in.
 * @param async iff TRUE, the function will return before the fade finished, otherwise
 *				the function will return if the fade is completed.
 * @param durationInMs the time the fade in animation should take, in MS. DEFAULT_DURATION
 *						can be passed to use the default duration.
 * @remark makes a desklet visible if it's hidden, so this function is nice for showing a 
 *			a desklet when it's hidden.
 */
void CALLBACK DeskletFadeIn(HWND hwndDesklet, BOOL async, DWORD durationInMs);
//--------------------------------------------------------------------------------------------
/**
 * Fades a desklet out.
 * @param hwndDesklet the handle of the desklet to fade out.
 * @param async iff TRUE, the function will return before the fade finished, otherwise
 *				the function will return if the fade is completed.
 * @param durationInMs the time the fade out animation should take, in MS. DEFAULT_DURATION
 *						can be passed to use the default duration.
 * @remark makes a desklet hidden if it's visible, so this function is nice for hiding a 
 *			a desklet when it's visible.
 */
void CALLBACK DeskletFadeOut(HWND hwndDesklet, BOOL async, DWORD durationInMs);
//--------------------------------------------------------------------------------------------
/**
 * Saves a desklet instance to a file.
 * @param hwndDesklet the handle of the desklet to save.
 * @param fileName the name of the file to save this desklet instance to. Cannot be NULL.
 * @return TRUE iff successfull, FALSE otherwise.
 * @remark only a UNICODE version of this function is available.
 */
BOOL CALLBACK DeskletSaveW(HWND hwndDesklet, const WCHAR* fileName);
//--------------------------------------------------------------------------------------------
/**
 * Loads and creates a desklet from a DLL.
 * @param dllFileName the filename of the dll of the desklet to create an instance from. Cannot be NULL.
 * @param configFileName optional filename of the configuration file for the newly created desklet. Can be NULL,
 *							in which case no configuration file will be used.
 * @param pointer optional, a pointer to a receiving pointer that is returned by OnCreate(), if successfull. Can
 *				  be NULL if not used.
 * @param makeVisible if TRUE, the desklet will be made visible directly, otherwise not.
 * @return the handle of the new desklet iff successfull, otherwise NULL.
 * @remark only a UNICODE version of this function is available.
 */
HWND CALLBACK DeskletLoadFromDLLW(const WCHAR* dllFileName, const WCHAR* configFileName, void** pointer,
								  BOOL makeVisible);
//--------------------------------------------------------------------------------------------
/**
 * Loads and creates a desklet from a config file.
 * @param fileName the filename of a configuration file to create a desklet from. Cannot be NULL.
 * @param pointer optional, a pointer to a receiving pointer that is returned by OnCreate(), if successfull. Can
 *				  be NULL if not used.
 * @param makeVisible if TRUE, the desklet will be made visible directly, otherwise not.
 * @return the handle of the new desklet iff successfull, otherwise NULL.
 * @remark only a UNICODE version of this function is available.
 */
HWND CALLBACK DeskletLoadFromConfigW(const WCHAR* fileName, void** pointer, BOOL makeVisible);
//--------------------------------------------------------------------------------------------
typedef BOOL (CALLBACK *DESKLETENUMPROC)(HWND,LPARAM);
/**
 * Enumerates the handles of all running desklet instances.
 * @param enumProc a function pointer to a DESKLETENUMPROC function that will be called for each desklet handle.
 *					cannot be NULL.
 * @param lParam a value that will be passes to each call of enumProc.
 * @remark enumProc will be called as long as there are desklet handles AND enumProc returns TRUE.
 * @return TRUE iff successfull, otherwise FALSE.
 */
BOOL CALLBACK DeskletEnumRunningDesklets(DESKLETENUMPROC enumProc, LPARAM lParam);
//--------------------------------------------------------------------------------------------
/**
 * Sets the pointer of a desklet asynchronically, before OnCreate() returns.
 * @param hwndDesklet the handle of the desklet to set the pointer of.
 * @param pointer the value of the pointer to set.
 * @remark should be used if OnCreate() will trigger callback On* functions. For example,
 *			a desklet could remove itself in OnCreate() by calling DeskletRemove(). It needs
 *			to set the desklets pointer async then, because OnDestroy will be called with a NULL pointer,
 *			leaving a (possible) memory leak.
 * @return TRUE iff successfull, otherwise FALSE.
 */
BOOL CALLBACK DeskletAsyncSetPointer(HWND hwndDesklet, void* pointer);
}
//--------------------------------------------------------------------------------------------
#define AVEDESK_SETTING_SETTINGSPATH	1
#define AVEDESK_SETTING_THEMESPATH		2

/**
 * Gets an AveDesk directory path.
 * @param directory the directory to get the path of, can be one of the following
 *					AVEDESK_SETTING_SETTINGSPATH, the directory where all avedesk settings are saved in.
 *					AVEDESK_SETTING_THEMESPATH, the directory where all avedesk themes are saved in.
 * @param wBuf a pointer to a buffer receiving the path. Cannot be NULL.
 * @param maxBufSize a pointer to an UINT giving the length (in characters) of the wBuf buffer. If
 *						the buffer is not large enough to hold the path, maxBufSize will receive
 *						the number of characters the buffer should be able to hold. Cannot be NULL.
 * @param TRUE iff a path was successfully copied into wBuf, otherwise FALSE.
 */
BOOL CALLBACK DeskletGetDirectoryW(DWORD directory, WCHAR* wBuf, UINT* maxBufSize);
//--------------------------------------------------------------------------------------------
/**
 * Sets the SkinInfo for a desklet.
 * @param hwndDesklet the handle of the desklet to set the skininfo for.
 * @param info a pointer to a SkinInfo structure that holds the skininfo data. Can be NULL, in case of 
 *				no skin. 
 * @return TRUE iff successfull, otherwise FALSE.
 */
BOOL CALLBACK DeskletSetSkinInfoW(HWND hwndDesklet, SkinInfo* info);
//--------------------------------------------------------------------------------------------
/**
 * Sets the Image Status for a desklet.
 * @param hwndDesklet the handle of the deskle to set the image status of.
 * @param imageCanBeSet iff TRUE, the image can be set by the user, otherwise not.
 * @remark if imageCanBeSet, the user can set the image of the desklet on the appearance tab
 *          if !imageCanBeSet, the user cannot set the image of the desklet manually.
 * @return TRUE iff successfull, otherwise FALSE.
 */
BOOL CALLBACK DeskletSetImageSetStatus(HWND hwndDesklet, BOOL imageCanBeSet);
//--------------------------------------------------------------------------------------------
/**
 * Flips a desklet with a visual effect to another set of active layers.
 * @param hwndDesklet the handle of the desklet to flip.
 * @param params a pointer to a FlipParams structure giving the parameters for the flip. Cannot be NULL.
 * @return TRUE iff successfull, otherwise FALSE.
 * @remark this function returns immediately, it doesn't wait 'till the flip is done. To be notified when
 *			the flip is completed, use the callback field of the FlipParams structure.
 */
BOOL CALLBACK DeskletFlip(HWND hwndDesklet, const FlipParams* params);
//--------------------------------------------------------------------------------------------
/**
 * Sets the close point for a desklet.
 * @param hwndDesklet the handle of the desklet to set the closepoint for.
 * @param closePoint a pointer to a POINT structure giving the closepoint coordinates. Can be NULL
 *					 to disable the usage of a closepoint for this desklet.
 * @param xScale the scale on which closePoint->x is based.
 * @param yScale the scale on which closePoint->y is based.
 * @remark the closepoint is the point on which avedesk will render a closebutton under certain
 *			circumstances.
 * @remark the POINT structure is relative to the xScale and yScale parameters. For example, if xScale and yScale
 *			are both 100, then the POINT structure will give the coordinates in percentages of the desklet.
 * @return TRUE iff successfull, otherwise FALSE.
 */
BOOL CALLBACK DeskletSetClosePoint(HWND hwndDesklet, const POINT* closePoint, int xScale, int yScale);
//--------------------------------------------------------------------------------------------
/**
 * Hit tests a desklet for a certain POINT.
 * @param hwndDesklet the handle of the desklet to hittest.
 * @param pt the point to hittest the desklet on. Cannot be NULL.
 * @param s the scale the pt parameter is based on. Cannot be NULL. See remarks.
 * @param onlyTestVisibleLayers if TRUE, only layers that are visible will be hittested, otherwise
 *								all layers will be hittested.
 * @remark the POINT structure is relative to the s->cx, s->cy, s parameter. For example, if s->cx and s->cy
 *			are both 100, then the POINT structure will give the coordinates in percentages of the desklet.
 * @return the id of the layer that is under pt (with scale s) if successfull. Otherwise, (UINT)-1.
 */
unsigned int CALLBACK DeskletHitTest(HWND hwndDesklet, const POINT* pt, const SIZE* s, BOOL onlyTestVisibleLayers);
//--------------------------------------------------------------------------------------------
/**
 * Locks invalidating for a desklet. If invalidating is locked for
 * a desklet, updating a desklet may be more efficient since no update rectangle
 * is added to the desklet.
 * @param HWND hwndDesklet the handle to the deskle to lock
 * @return the lock-count (can be negative too).
 * @remark must be called together with DeskletUnlockInvalidating in pairs
 */
INT CALLBACK DeskletLockInvalidating(HWND hwndDesklet);
//--------------------------------------------------------------------------------------------
/**
 * Unlocks invalidating for a desklet. If invalidating is locked for
 * a desklet, updating a desklet may be more efficient since no update rectangle
 * is added to the desklet.
 * @param HWND hwndDesklet the handle to the deskle to unlock
 * @param BOOL redraw iff TRUE, the desklet will be redrawn
 * @return the lock-count (can be negative too).
 * @remark must be called totogether with DeskletLockInvalidating in pairs
 */
INT CALLBACK DeskletUnlockInvalidating(HWND hwndDesklet, BOOL redraw);
//--------------------------------------------------------------------------------------------
/**
 * Invalidates a portion of the desklet.
 * @param HWND hwndDesklet the handle to to the desklet to invalidate a rect of
 * @param RECT* r, a pointer to rectangle to erase. Cannot be NULL.
 * @param SIZE* scale, a pointer to a SIZE structure defining the scale of r. Cannot be NULL
 * @param BOOL redraw iff TRUE, the desklet will be redrawn
  * @remark the RECT structure is relative to the s->cx, s->cy, s parameter. For example, if s->cx and s->cy
 *			are both 100, then the RECT structure will give the coordinates in percentages of the desklet.
 * @return TRUE iff successfull
 */
BOOL CALLBACK DeskletInvalidateRect(HWND hwndDesklet, RECT* rect, SIZE* scale, BOOL redraw);
//--------------------------------------------------------------------------------------------
/**
 * Sets the cursor for a desklet.
 * @param HWND hwndDesklet the handle of the desklet to set the cursor of
 * @param HCURSOR handle of the new cursor to set
 * @return the handle of the previous cursor
 */
HCURSOR CALLBACK DeskletSetCursor(HWND hwndDesklet, HCURSOR hCursor);
//--------------------------------------------------------------------------------------------



/**
 * Captures the mouse to a layer.
 * @param hwndDesklet the handle of the desklet.
 * @param id the id of the layer that gets the capture. (UINT)-1 means no layer.
 * @remark 1.3 function uses (UINT)-1 for no-layer, rather than the ambgious 0.
 * @return returns the layer's id that HAD the capture.
 */
UINT CALLBACK DeskletLayerSetCapture(HWND hwndDesklet, unsigned int id);

/**
 * Releases the mouse-capture.
 * @param hwndDesklet the handle of the desklet to release the mousecapture for.
 * @remark 1.3 function uses (UINT)-1 for no-layer, rather than the ambgious 0.
 * @return returns the layer's id that HAD the capture.
 */
UINT CALLBACK DeskletLayerReleaseCapture(HWND hwndDesklet);

/**
 * Gets the id of the layer that has the capture.
 * @param hwndDesklet the handle of the desklet.
 * @remark 1.3 function uses (UINT)-1 for no-layer, rather than the ambgious 0.
 * @return returns the layer's id that HAS the capture.
 */
UINT CALLBACK DeskletLayerGetCapture(HWND hwndDesklet);

/**
 * Hit-tests a layer for a certain point.
 * @param hwndDesklet the handle of the desklet
 * @param id the id of the layer to test
 * @param pt a pointer to a POINT structure describing the point on desklet-scale to test. 
 *				this x and y-values are relative to s. Cannot be NULL.
 * @param s the scale of pt. Cannot be NULL. See remarks.
 * @remark the POINT structure is relative to the s->cx, s->cy, s parameter. For example, if s->cx and s->cy
 *			are both 100, then the POINT structure will give the coordinates in percentages of the desklet.
 * @return TRUE iff pt would hit-test on id, otherwise FALSE.
 */
BOOL CALLBACK DeskletLayerHitTest(HWND hwndDesklet, unsigned int id, const POINT* pt, const SIZE* s);

/**
 * Sets the Image Clipping Rectangle for a layer.
 * @param hwndDesklet the handle of the desklet.
 * @param id the id of the layer to set the image clipping rectangle for.
 * @param redraw iff TRUE, the desklet will be immediately redrawn after the properties have been applied.
 * @param useClipping iff TRUE, clipping will be used on the layers image.
 * @param clipRect the relative clipping rectangle to be used for the image. Can be NULL if useClipping = FALSE, otherwise must be not NULL.
 * @param xScale the scale on which all x coordinates in clipRect are based
 * @param yScale the scale on which all y coordinates in clipRect are based
 * @remark if xScale and yScale are both 100, clipRect will describe percentages of the images that should be visible.
 *			e.g. if clipRect->left = 0, clipRect->right = 50. 50% of the layers image will be used for drawing.
 *			If you make xScale == image.Width and yScale == image.Height. clipRect maps directly onto the
 *			pixels that should be visible for the layers image.
 * @return TRUE iff sucessfull.
 */
BOOL CALLBACK DeskletLayerSetImageClipRect(HWND hwndDesklet, unsigned int id, BOOL redraw, BOOL useClipping, RECT* clipRect, 
										   int xScale, int yScale);

/**
 * Sets the placement for this layer.
 * @param hwndDesklet the handle of the desklet.
 * @param id the id of the layer to set the placement of.
 * @param redraw iff TRUE, the desklet will be immediately redrawn after the properties have been applied.
 * @param usePlacement iff TRUE, the layer will be placed at a specific point on the desklet. Otherwise, 
 *						the layer will be stretched over the whole desklet.
 * @param placementRect the placement rectangle to use. Cannot be NULL iff usePlacement is TRUE.
 * @param xScale the scale on which all x coordinates in placementRect are based
 * @param yScale the scale on which all y coordinates in placementRect are based
 * @remark if no placement is used, the layer will be stretched over the whole desklet (0,0, deskletWidth, deskletHeight)
 *			when drawn. If placement is used, the layer will be drawn at a specific place.
 *			The coordinates passed to this function are on a relative scale. if xScale and yScale are both 100, 
 *			then placementRect coordinates are in percentages. This means that if the desklet is resized, the layers
 *			will scale accordingly. For example, when xScale = 100, yScale = 100 and
 *			placementRect = ;, the layer will always be drawn in the left upper quarter of the desklet.
 *			If you make sure xScale = deskletWidth, yScale = deskletHeight, then you map the desklets
 *			pixels directly on the layers placementRect.
 * @return TRUE iff sucessfull.
 */
void CALLBACK DeskletLayerSetPlacement(HWND hwndDesklet, unsigned int id,BOOL redraw, BOOL usePlacement, RECT* s, int xScale, int yScale);


/**
 * Sets the icon state of a desklet.
 * @param hwndDesklet the handle of the desklet.
 * @param id the id of the layer to set the iconstate of.
 * @param redraw iff TRUE, the desklet will be immediately redrawn after the properties have been applied.
 * @param canBeIcon iff FALSE, the layer cannot be an icon (cannot be changed thru setimage, no dropshadow applied).
 */
void CALLBACK DeskletLayerSetIconState(HWND hwndDesklet, unsigned int id, BOOL redraw, BOOL canBeIcon);
/**
 * Sets the isNotMovable state of this layer.
 * @remark if the isNotMovable state for a desklet is TRUE, the whole desklet cannot be moved when
 *				the mouse is being held down on this particular layer.
 * @param hwndDesklet the handle of the desklet.
 * @param id the id of the layer to set the isNotMovable state of.
 * @param isNotMovable the new value of the isNotMovable state for this layer.
 */
void CALLBACK DeskletLayerSetIsNotMovable(HWND hwndDesklet, unsigned int id, BOOL isNotMovable);

/**
 * Sets the isClickThrough state of this layer.
 * @remark if the isClickThrough state for a desklet is TRUE, hittesting on the desklet will always
 *			result in a "no-hit". The layer cannot be click upon thus and will not receive any mouse-messages.
 * @param hwndDesklet the handle of the desklet.
 * @param id the id of the layer to set the isClickThrough state of.
 * @param isClickThrough the new value of the isClickThrough state for this layer.
 */
void CALLBACK DeskletLayerSetIsClickThrough(HWND hwndDesklet, unsigned int id, BOOL isClickThrough);
/**
 * Sets the noAlphaBlendedHitTesting state of this layer.
 * @remark by default, layers use the alpha-blended information in their images when hit-testing. Thus,
 *				if the mouse is over a pixel with alpha=0, the layer will hit-test "no-hit". By setting 
 *				the noAlphaBlendedHitTesting to TRUE, alpha information will not be taken into
 *				account when hit-testing; layers will then have rectangular hit-testing areas.
 * @param hwndDesklet the handle of the desklet.
 * @param id the id of the layer to set the noAlphaBlendedHitTesting state of.
 * @param isClickThrough the new value of the noAlphaBlendedHitTesting state for this layer.
 */
void CALLBACK DeskletLayerSetNoAlphaBlendedHitTesting(HWND hwndDesklet, unsigned int id, BOOL noAlphaBlendedHitTesting);

/**
 * Gets the id by the name of a layer.
 * @param hwndDesklet the handle of the desklet.
 * @param name the name of the layer to get the id for. Cannot be NULL.
 * @param success optional pointer to a BOOL which value will be set to TRUE iff the a layer with name 'name' was found,
 *			otherwise it's set to FALSE. Can be NULL.
 * @remark layer names are ANSI, since they are only internal to a desklet and avedesk.
 * @return the id of the layer with name 'name' if successfull, or 0 otherwise. use the success parameter for error-checking.
 * @remark layer id's are dynamical: the id corresponds to the place in the layer-stack/order of the layer.
 */
unsigned int CALLBACK DeskletLayerGetLayer(HWND hwndDesklet, char* name, BOOL* success);

/**
 * Gets the number of layers in a desklet.
 * @param hwndDesklet the handle of the desklet.
 * @return the number of layers this desklet has.
 */
unsigned int CALLBACK DeskletLayerCount(HWND hwndDesklet);

/**
 * Adds a layer to a desklet.
 * @param hwndDesklet the handle of the desklet.
 * @param name the name for the new layer. Cannot be NULL.
 * @remark layer names are ANSI, since they are only internal to a desklet and avedesk.
 * @return the id of the layer just added, or the id of the layer that had the name 'name'
 *			already.
 * @remark layer id's are dynamical: the id corresponds to the place in the layer-stack/order of the layer.
 */
unsigned int CALLBACK DeskletLayerAdd(HWND hwndDesklet, char* name);

/**
 * Removes a layer from a desklet.
 * @param hwndDesklet the handle of the desklet.
 * @param id the id of the layer to remove.
 * @param redraw iff TRUE, the desklet will be immediately redrawn after the properties have been applied.
 * @effect: layer ids will be re-assigned
 */
void  CALLBACK DeskletLayerRemove(HWND hwndDesklet, unsigned int id, BOOL redraw);


/**
 * Merges all layers into one new layer with id 0.
 * @param hwndDesklet the handle of the desklet.
 * @param redraw iff TRUE, the desklet will be immediately redrawn after the properties have been applied.
 * @remark layer id's are dynamical: the id corresponds to the place in the layer-stack/order of the layer.
 */
void  CALLBACK DeskletLayerMergeAll(HWND hwndDesklet, BOOL redraw);


/**
 * Merges only the visible layers into one new layer with the lowest id possible.
 * @param hwndDesklet the handle of the desklet.
 * @param redraw iff TRUE, the desklet will be immediately redrawn after the properties have been applied.
 * @effect layer ids will be re-assigned
 * @remark layer id's are dynamical: the id corresponds to the place in the layer-stack/order of the layer.
 */
void  CALLBACK DeskletLayerMergeVisible(HWND hwndDesklet,  BOOL redraw);
/**
 * Moves a layer to the bottom.
 * @param hwndDesklet the handle of the desklet.
 * @param id the id of the layer to move.
 * @param redraw iff TRUE, the desklet will be immediately redrawn after the properties have been applied.
 * @effect: layers ids will be re-assigned. This layer will get an id of 0.
 * @remark layer id's are dynamical: the id corresponds to the place in the layer-stack/order of the layer.
 */
void  CALLBACK DeskletLayerMoveBottom(HWND hwndDesklet, unsigned int id, BOOL redraw);

/**
 * Moves a layer down in the layer-order.
 * @param hwndDesklet the handle of the desklet.
 * @param id the id of the layer to move.
 * @param redraw iff TRUE, the desklet will be immediately redrawn after the properties have been applied.
 * @effect: layer ids will be re-assigned.
 * @remark layer id's are dynamical: the id corresponds to the place in the layer-stack/order of the layer.
 */
void  CALLBACK DeskletLayerMoveDown(HWND hwndDesklet, unsigned int id, BOOL redraw);

/**
 * Moves a layer to the top in the layer-order, thus getting an id of DeskletLayerGetCount() - 1.
 * @param hwndDesklet the handle of the desklet.
 * @param id the id of the layer to move.
 * @param redraw iff TRUE, the desklet will be immediately redrawn after the properties have been applied.
 * @effect: layer ids will be re-assigned.
 * @remark layer id's are dynamical: the id corresponds to the place in the layer-stack/order of the layer.
 */
void  CALLBACK DeskletLayerMoveTop(HWND hwndDesklet, unsigned int id, BOOL redraw);

/**
 * Moves a layer one up in the layer-order.
 * @param hwndDesklet the handle of the desklet.
 * @param id the id of the layer to move.
 * @param redraw iff TRUE, the desklet will be immediately redrawn after the properties have been applied.
 * @effect: layer ids will be re-assigned.
 * @remark layer id's are dynamical: the id corresponds to the place in the layer-stack/order of the layer.
 */
void  CALLBACK DeskletLayerMoveUp(HWND hwndDesklet, unsigned int id, BOOL redraw);

/**
 * Resets all properties of a layer
 * @param hwndDesklet the handle of the desklet.
 * @param id the id of the layer to reset.
 * @param redraw iff TRUE, the desklet will be immediately redrawn after the properties have been applied.
 */
void  CALLBACK DeskletLayerReset(HWND hwndDesklet, unsigned int id, BOOL redraw);

/**
 * Sets the alpha level of a layer.
 * @param hwndDesklet the handle of the desklet.
 * @param id the id of the layer to set the alpha-value of.
 * @param redraw iff TRUE, the desklet will be immediately redrawn after the properties have been applied.
 * @param alpha the new alpha value of the layer. range [0,255] with 0 is invisible, 255 fully visible.
 */
void  CALLBACK DeskletLayerSetAlpha(HWND hwndDesklet, unsigned int id, BOOL redraw, BYTE alpha);
/**
 * Sets a colormatrix for a layer.
 * @param hwndDesklet the handle of the desklet.
 * @param id the id of the layer to set the colormatrix for.
 * @param redraw iff TRUE, the desklet will be immediately redrawn after the properties have been applied.
 * @param matrix the colormatrix for the layer. Can be NULL to un-assign it.
 * @remark might get deprecated in the next release
 */
void  CALLBACK DeskletLayerSetColorMatrix(HWND hwndDesklet, unsigned int id, BOOL redraw, ColorMatrix* matrix);


/**
 * Sets the hue level of a layer.
 * @param hwndDesklet the handle of the desklet.
 * @param id the id of the layer to set the hue-value of.
 * @param redraw iff TRUE, the desklet will be immediately redrawn after the properties have been applied.
 * @param hue the new hue value of the layer.
 */
void CALLBACK DeskletLayerSetHue(HWND hwndDesklet, unsigned int id, BOOL redraw, int hue);

/**
 * Sets a GDI+ image for a layer.
 * @param hwndDesklet the handle of the desklet.
 * @param id the id of the layer to set the image for.
 * @param redraw iff TRUE, the desklet will be immediately redrawn after the properties have been applied.
 * @param img. the image to use for this layer. Can be NULL to specify no image.
 * @param if AutoDel is TRUE, AveDesk takes ownership of this object lifetime and thus the caller
 *						may not free/delete it. If FALSE, AveDesk will directly use img
 *						when redrawing is done, so changes made to the image will result in an
 *						updated desklet when the desklet is redrawn. Threading issues must be taken
 *						into account by the caller then.
 * @remark For performance, AutoDel can be set to FALSE so updating might be less expensive.
 * @remark NON-UNICODE version of DeskletLayerSetImageFileW
 * @see DeskletLayerSetImageFileW
 */
void  CALLBACK DeskletLayerSetImage(HWND hwndDesklet, unsigned int id, BOOL redraw, Image* img, BOOL AutoDel);


/**
 * Sets an image file for a layer.
 * @param hwndDesklet the handle of the desklet.
 * @param id the id of the layer to set the image file for.
 * @param redraw iff TRUE, the desklet will be immediately redrawn after the properties have been applied.
 * @param filename the filename of the image file to use for this layer. Can be NULL to specify no image.
 * @remark NON-UNICODE version of DeskletLayerSetImageFileW
 * @see DeskletLayerSetImageFileW
 */
void  CALLBACK DeskletLayerSetImageFile(HWND hwndDesklet, unsigned int id, BOOL redraw, const char* filename);
/**
 * Sets an image file for a layer.
 * @param hwndDesklet the handle of the desklet.
 * @param id the id of the layer to set the image file for.
 * @param redraw iff TRUE, the desklet will be immediately redrawn after the properties have been applied.
 * @param filename the filename of the image file to use for this layer. Can be NULL to specify no image.
 * @remark UNICODE version of DeskletLayerSetImageFile
 * @see DeskletLayerSetImageFile
 */
void  CALLBACK DeskletLayerSetImageFileW(HWND hwndDesklet, unsigned int id, BOOL redraw, const WCHAR* filename);

/**
 * Sets the rotation level of a layer.
 * @param hwndDesklet the handle of the desklet.
 * @param id the id of the layer to set the rotation-value of.
 * @param redraw iff TRUE, the desklet will be immediately redrawn after the properties have been applied.
 * @param rotation the new rotation value in degrees for the layer.
 */
void  CALLBACK DeskletLayerSetRotation(HWND hwndDesklet, unsigned int id, BOOL redraw, int rotation);

/**
 * Sets the saturation level of a layer.
 * @param hwndDesklet the handle of the desklet.
 * @param id the id of the layer to set saturation hue-value of.
 * @param redraw iff TRUE, the desklet will be immediately redrawn after the properties have been applied.
 * @param saturation the new saturation value of the layer.
 */
void  CALLBACK DeskletLayerSetSaturation(HWND hwndDesklet, unsigned int id, BOOL redraw, unsigned int saturation);
/**
 * Sets the visibility of a layer.
 * @param hwndDesklet the handle of the desklet.
 * @param id the id of the layer to set visibility of.
 * @param redraw iff TRUE, the desklet will be immediately redrawn after the properties have been applied.
 * @param visibility. iff TRUE the layer will be visible, otherwise not.
 */
void  CALLBACK DeskletLayerSetVisibility(HWND hwndDesklet, unsigned int id, BOOL redraw, BOOL visibility);


/**
 * Adds a layer to a desklet (extended function).
 * @param hwndDesklet the handle of the desklet.
 * @param name the name for the new layer. Cannot be NULL.
 * @param flags the flags for this layer: valid are LAYER_IS_CONTAINER and LAYER_IS_BUFFERED
 * @remark layer names are ANSI, since they are only internal to a desklet and avedesk.
 * @return the id of the layer just added, or the id of the layer that had the name 'name'
 *			already.
 * @version minimum avedesk 2.0
 * @remark layer id's are dynamical: the id corresponds to the place in the layer-stack/order of the layer.
 */

unsigned int CALLBACK DeskletLayerAddEx(HWND hwndDesklet, const char* name, const char* nameOfParent, DWORD flags);