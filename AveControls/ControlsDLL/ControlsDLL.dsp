# Microsoft Developer Studio Project File - Name="ControlsDLL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=ControlsDLL - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ControlsDLL.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ControlsDLL.mak" CFG="ControlsDLL - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ControlsDLL - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ControlsDLL - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ControlsDLL - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CONTROLSDLL_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CONTROLSDLL_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x413 /d "NDEBUG"
# ADD RSC /l 0x413 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"g:\avedesk\avedesk\AveControls.dll"

!ELSEIF  "$(CFG)" == "ControlsDLL - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CONTROLSDLL_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CONTROLSDLL_EXPORTS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x413 /d "_DEBUG"
# ADD RSC /l 0x413 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"g:\avedesk\avedesk\AveControls.dll" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "ControlsDLL - Win32 Release"
# Name "ControlsDLL - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Controls"

# PROP Default_Filter ""
# Begin Group "graphics_funcs"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\AveControls\graphics_funcs\ColorMatrixHelper.cpp
# End Source File
# Begin Source File

SOURCE=..\AveControls\graphics_funcs\ColorMatrixHelper.h
# End Source File
# Begin Source File

SOURCE=..\AveControls\graphics_funcs\image_funcs.cpp
# End Source File
# Begin Source File

SOURCE=..\AveControls\graphics_funcs\image_funcs.h
# End Source File
# Begin Source File

SOURCE=..\AveControls\graphics_funcs\shadowProperties.cpp
# End Source File
# Begin Source File

SOURCE=..\AveControls\graphics_funcs\ShadowProperties.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\AveControls\AveButton.cpp
# End Source File
# Begin Source File

SOURCE=..\AveControls\AveButton.h
# End Source File
# Begin Source File

SOURCE=..\AveControls\aveconstants.h
# End Source File
# Begin Source File

SOURCE=..\AveControls\AveControlHelper.cpp
# End Source File
# Begin Source File

SOURCE=..\AveControls\AveControlHelper.h
# End Source File
# Begin Source File

SOURCE=..\AveControls\AveDropDownList.cpp
# End Source File
# Begin Source File

SOURCE=..\AveControls\AveDropDownList.h
# End Source File
# Begin Source File

SOURCE=..\AveControls\AveEditBox.cpp
# End Source File
# Begin Source File

SOURCE=..\AveControls\AveEditBox.h
# End Source File
# Begin Source File

SOURCE=..\AveControls\AveFont.cpp
# End Source File
# Begin Source File

SOURCE=..\AveControls\AveFont.h
# End Source File
# Begin Source File

SOURCE=..\AveControls\AveImageButton.cpp
# End Source File
# Begin Source File

SOURCE=..\AveControls\AveImageButton.h
# End Source File
# Begin Source File

SOURCE=..\AveControls\AveLabel.cpp
# End Source File
# Begin Source File

SOURCE=..\AveControls\AveLabel.h
# End Source File
# Begin Source File

SOURCE=..\AveControls\AveStaticText.cpp
# End Source File
# Begin Source File

SOURCE=..\AveControls\AveStaticText.h
# End Source File
# Begin Source File

SOURCE=..\AveControls\AveTextField.cpp
# End Source File
# Begin Source File

SOURCE=..\AveControls\AveTextField.h
# End Source File
# Begin Source File

SOURCE=..\AveControls\AveVerticalScrollBar.cpp
# End Source File
# Begin Source File

SOURCE=..\AveControls\AveVerticalScrollBar.h
# End Source File
# Begin Source File

SOURCE=..\AveControls\controls_header.h
# End Source File
# Begin Source File

SOURCE=..\DeskletSDK.cpp
# End Source File
# Begin Source File

SOURCE=..\DeskletSDK.h
# End Source File
# Begin Source File

SOURCE=..\AveControls\IAveCallback.cpp
# End Source File
# Begin Source File

SOURCE=..\AveControls\IAveCallback.h
# End Source File
# Begin Source File

SOURCE=..\AveControls\IAveCallbackClick.cpp
# End Source File
# Begin Source File

SOURCE=..\AveControls\IAveCallbackClick.h
# End Source File
# Begin Source File

SOURCE=..\AveControls\IAveControl.cpp
# End Source File
# Begin Source File

SOURCE=..\AveControls\IAveControl.h
# End Source File
# Begin Source File

SOURCE=..\AveControls\IAveControlContainer.cpp
# End Source File
# Begin Source File

SOURCE=..\AveControls\IAveControlContainer.h
# End Source File
# Begin Source File

SOURCE=..\AveControls\IAveLockable.cpp
# End Source File
# Begin Source File

SOURCE=..\AveControls\IAveLockable.h
# End Source File
# Begin Source File

SOURCE=..\AveControls\IAveResources.cpp
# End Source File
# Begin Source File

SOURCE=..\AveControls\IAveResources.h
# End Source File
# Begin Source File

SOURCE=..\AveControls\IAveScrollbar.cpp
# End Source File
# Begin Source File

SOURCE=..\AveControls\IAveScrollbar.h
# End Source File
# Begin Source File

SOURCE=..\AveControls\IAveUsingFontImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\AveControls\IAveUsingFontImpl.h
# End Source File
# Begin Source File

SOURCE=..\AveControls\IAveVisibilityControl.cpp
# End Source File
# Begin Source File

SOURCE=..\AveControls\IAveVisibilityControl.h
# End Source File
# Begin Source File

SOURCE=..\AveControls\OSXPopupMenu.cpp
# End Source File
# Begin Source File

SOURCE=..\AveControls\OSXPopupMenu.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\AveControls.cpp
# End Source File
# Begin Source File

SOURCE=.\AveControls.def
# End Source File
# Begin Source File

SOURCE=..\AveControls\AveHyperLink.cpp
# End Source File
# Begin Source File

SOURCE=.\Script1.rc
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AveControls.h
# End Source File
# Begin Source File

SOURCE=..\AveControls\AveFlags.h
# End Source File
# Begin Source File

SOURCE=..\AveControls\AveHyperLink.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
