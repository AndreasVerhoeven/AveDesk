# Microsoft Developer Studio Project File - Name="AveDesk" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=AveDesk - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "AveDesk.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "AveDesk.mak" CFG="AveDesk - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "AveDesk - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "AveDesk - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "AveDesk - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GR /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /win32
# SUBTRACT MTL /mktyplib203
# ADD BASE RSC /l 0x413 /d "NDEBUG"
# ADD RSC /l 0x413 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libc.lib" /out:"g:\avedesk\avedesk\AveDesk.exe" /opt:NOWIN98
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "AveDesk - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /Gi /GR /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x413 /d "_DEBUG"
# ADD RSC /l 0x413 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libc.lib" /out:"g:\avedesk\AveDesk\AveDesk.exe" /pdbtype:sept /warn:0
# SUBTRACT LINK32 /pdb:none
# Begin Custom Build - Performing registration
OutDir=.\Debug
TargetPath=g:\avedesk\avedesk\AveDesk.exe
InputPath=g:\avedesk\avedesk\AveDesk.exe
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	"$(TargetPath)" /RegServer 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	echo Server registration done! 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "AveDesk - Win32 Release"
# Name "AveDesk - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "GlobalSettings"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AveDeskGlobalSettings.cpp
# End Source File
# Begin Source File

SOURCE=.\GlobalSettingsClickSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\GlobalSettingsEffectSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\GlobalSettingsMoveSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\GlobalSettingsOtherSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\GlobalSettingsSelectSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\GlobalSettingsShowCaseSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\GlobalSettingsToolsSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\GlobalSettingsTraySheet.cpp
# End Source File
# End Group
# Begin Group "Dialogs "

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ConfigShadowWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfigSubShadowWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\DeskletAdvancedSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\DeskletAppearanceSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\DeskletLabelSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\ShadowPropertiesDlg.cpp
# End Source File
# End Group
# Begin Group "Unzip"

# PROP Default_Filter ""
# Begin Group "zlib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\unzip\zlib\ioapi.h
# End Source File
# Begin Source File

SOURCE=.\unzip\zlib\iowin32.h
# End Source File
# Begin Source File

SOURCE=.\unzip\zlib\unzip.h
# End Source File
# Begin Source File

SOURCE=.\unzip\zlib\zconf.h
# End Source File
# Begin Source File

SOURCE=.\unzip\zlib\zip.h
# End Source File
# Begin Source File

SOURCE=.\unzip\zlib\zlib.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\unzip\Unzipper.cpp
# End Source File
# Begin Source File

SOURCE=.\unzip\Unzipper.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\AboutDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\activdbg.idl

!IF  "$(CFG)" == "AveDesk - Win32 Release"

!ELSEIF  "$(CFG)" == "AveDesk - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ActivScp.Idl

!IF  "$(CFG)" == "AveDesk - Win32 Release"

!ELSEIF  "$(CFG)" == "AveDesk - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\AddDeskletDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Application.cpp
# End Source File
# Begin Source File

SOURCE=.\AvailableDeskletsAPI.cpp
# End Source File
# Begin Source File

SOURCE=.\Avedesk.cpp
# End Source File
# Begin Source File

SOURCE=.\AveDesk.def
# End Source File
# Begin Source File

SOURCE=.\AveDesk.idl
# ADD MTL /tlb ".\AveDesk.tlb" /h "AveDesk.h" /iid "AveDesk_i.c" /Oicf
# SUBTRACT MTL /nologo /mktyplib203
# End Source File
# Begin Source File

SOURCE=.\AveDeskFileAssociation.cpp
# End Source File
# Begin Source File

SOURCE=.\AveDeskletFile.cpp
# End Source File
# Begin Source File

SOURCE=.\AveEnvironment.cpp
# End Source File
# Begin Source File

SOURCE=.\AveImage.cpp
# End Source File
# Begin Source File

SOURCE=.\AveInstAbout.cpp
# End Source File
# Begin Source File

SOURCE=.\AveInstAction.cpp
# End Source File
# Begin Source File

SOURCE=.\AveInstActions.cpp
# End Source File
# Begin Source File

SOURCE=.\AveInstActionsFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\AveInstaller.cpp
# End Source File
# Begin Source File

SOURCE=.\AveInstallerEnvironment.cpp
# End Source File
# Begin Source File

SOURCE=.\AveInstallFile.cpp
# End Source File
# Begin Source File

SOURCE=.\AveInstBitmapWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\AveInstCancelable.cpp
# End Source File
# Begin Source File

SOURCE=.\AveInstCopyInformation.cpp
# End Source File
# Begin Source File

SOURCE=.\AveInstCopyItem.cpp
# End Source File
# Begin Source File

SOURCE=.\AveInstCopyItemAllFilesAndSubFolders.cpp
# End Source File
# Begin Source File

SOURCE=.\AveInstCopyItemDirectory.cpp
# End Source File
# Begin Source File

SOURCE=.\AveInstCopyItemFile.cpp
# End Source File
# Begin Source File

SOURCE=.\AveInstDependencies.cpp
# End Source File
# Begin Source File

SOURCE=.\AveInstDeskletUsedRequirement.cpp
# End Source File
# Begin Source File

SOURCE=.\AveInstDoIfAction.cpp
# End Source File
# Begin Source File

SOURCE=.\AveInstDownloadAndInstallAction.cpp
# End Source File
# Begin Source File

SOURCE=.\AveInstError.cpp
# End Source File
# Begin Source File

SOURCE=.\AveInstFileDirOperation.cpp
# End Source File
# Begin Source File

SOURCE=.\AveInstFileFetcher.cpp
# End Source File
# Begin Source File

SOURCE=.\AveInstFontAction.cpp
# End Source File
# Begin Source File

SOURCE=.\AveInstInstallModuleAction.cpp
# End Source File
# Begin Source File

SOURCE=.\AveInstLoadDeskletAction.cpp
# End Source File
# Begin Source File

SOURCE=.\AveInstLoadTheme.cpp
# End Source File
# Begin Source File

SOURCE=.\AveInstNotRequired.cpp
# End Source File
# Begin Source File

SOURCE=.\AveInstPackageManager.cpp
# End Source File
# Begin Source File

SOURCE=.\AveInstProgressIndicator.cpp
# End Source File
# Begin Source File

SOURCE=.\AveInstProgressWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\AveInstRegisterCOMServerAction.cpp
# End Source File
# Begin Source File

SOURCE=.\AveInstReloadLanguage.cpp
# End Source File
# Begin Source File

SOURCE=.\AveInstReqItem.cpp
# End Source File
# Begin Source File

SOURCE=.\AveInstRequiredAveDeskVersion.cpp
# End Source File
# Begin Source File

SOURCE=.\AveInstRequiredDeskletVersion.cpp
# End Source File
# Begin Source File

SOURCE=.\AveInstRequiredFile.cpp
# End Source File
# Begin Source File

SOURCE=.\AveInstRequiredInstallerVersion.cpp
# End Source File
# Begin Source File

SOURCE=.\AveInstRequiredPackage.cpp
# End Source File
# Begin Source File

SOURCE=.\AveInstRequiredToAcceptText.cpp
# End Source File
# Begin Source File

SOURCE=.\AveInstRequirements.cpp
# End Source File
# Begin Source File

SOURCE=.\AveInstRequirementsFactory.cpp
# End Source File
# Begin Source File

SOURCE=.\AveInstRunScriptAction.cpp
# End Source File
# Begin Source File

SOURCE=.\AveInstShowBitmapAction.cpp
# End Source File
# Begin Source File

SOURCE=.\AveInstShowTextAction.cpp
# End Source File
# Begin Source File

SOURCE=.\AveInstSuspendDeskletAction.cpp
# End Source File
# Begin Source File

SOURCE=.\AveInstSuspendedDesklets.cpp
# End Source File
# Begin Source File

SOURCE=.\AveInstSuspendedDeskletsList.cpp
# End Source File
# Begin Source File

SOURCE=.\AveInstUnloadLanguage.cpp
# End Source File
# Begin Source File

SOURCE=.\AveInstUnloadModuleAction.cpp
# End Source File
# Begin Source File

SOURCE=.\AveThemeFile.cpp
# End Source File
# Begin Source File

SOURCE=.\CloseButton.cpp
# End Source File
# Begin Source File

SOURCE=.\CloseButtonMouseOverPolicy.cpp
# End Source File
# Begin Source File

SOURCE=.\CloseButtonPolicy.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorButton.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorMatrix.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorMatrixHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\ContainerLayer.cpp
# End Source File
# Begin Source File

SOURCE=.\ControlDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CSpecializedShowCaseTrackBar.cpp
# End Source File
# Begin Source File

SOURCE=.\Debug.cpp
# End Source File
# Begin Source File

SOURCE=.\Desklet.cpp
# End Source File
# Begin Source File

SOURCE=.\DeskletBrowserDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DeskletCollection.cpp
# End Source File
# Begin Source File

SOURCE=.\DeskletFilter.cpp
# End Source File
# Begin Source File

SOURCE=.\DeskletFinder.cpp
# End Source File
# Begin Source File

SOURCE=.\DeskletInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\DeskletInfos.cpp
# End Source File
# Begin Source File

SOURCE=.\DeskletItem.cpp
# End Source File
# Begin Source File

SOURCE=.\DeskletProvider.cpp
# End Source File
# Begin Source File

SOURCE=.\Desklets.cpp
# End Source File
# Begin Source File

SOURCE=.\DeskletSkin.cpp
# End Source File
# Begin Source File

SOURCE=.\DeskletSkinEnumerator.cpp
# End Source File
# Begin Source File

SOURCE=.\DeskletSkinInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\DeskletSkins.cpp
# End Source File
# Begin Source File

SOURCE=.\DeskletsView.cpp
# End Source File
# Begin Source File

SOURCE=.\docklet.cpp
# End Source File
# Begin Source File

SOURCE=.\dockletdll.cpp
# End Source File
# Begin Source File

SOURCE=.\DownloadableFileDescription.cpp
# End Source File
# Begin Source File

SOURCE=.\DragAnimationWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\drawplugin.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawPluginInstance.cpp
# End Source File
# Begin Source File

SOURCE=.\DropSettingsImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\EffectLet.cpp
# End Source File
# Begin Source File

SOURCE=.\EnumDeskletDragDropFormats.cpp
# End Source File
# Begin Source File

SOURCE=.\FlipAnimation.cpp
# End Source File
# Begin Source File

SOURCE=.\FlipAnimWrapper.cpp
# End Source File
# Begin Source File

SOURCE=.\FoundDeskletEntry.cpp
# End Source File
# Begin Source File

SOURCE=.\helper_funcs.cpp
# End Source File
# Begin Source File

SOURCE=.\IconAnimator.cpp
# End Source File
# Begin Source File

SOURCE=.\IconWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\IIconAnimatorCallback.cpp
# End Source File
# Begin Source File

SOURCE=.\image_funcs.cpp
# End Source File
# Begin Source File

SOURCE=.\Images.cpp
# End Source File
# Begin Source File

SOURCE=.\INI_API_Interceptor.cpp
# End Source File
# Begin Source File

SOURCE=.\InstallErrorDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\IObservable.cpp
# End Source File
# Begin Source File

SOURCE=.\IObserver.cpp
# End Source File
# Begin Source File

SOURCE=.\IPersistable.cpp
# End Source File
# Begin Source File

SOURCE=.\IProgressIndicator.cpp
# End Source File
# Begin Source File

SOURCE=.\label.cpp
# End Source File
# Begin Source File

SOURCE=.\labelbackground.cpp
# End Source File
# Begin Source File

SOURCE=.\LabelProperties.cpp
# End Source File
# Begin Source File

SOURCE=.\Layer.cpp

!IF  "$(CFG)" == "AveDesk - Win32 Release"

# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "AveDesk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LayerFuncs.cpp

!IF  "$(CFG)" == "AveDesk - Win32 Release"

# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "AveDesk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Matrix.cpp

!IF  "$(CFG)" == "AveDesk - Win32 Release"

# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "AveDesk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Module.cpp
# End Source File
# Begin Source File

SOURCE=.\Modules.cpp
# End Source File
# Begin Source File

SOURCE=.\ModulesDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\NewFeedDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Od_docklet.cpp

!IF  "$(CFG)" == "AveDesk - Win32 Release"

# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "AveDesk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\od_docklet_instance.cpp

!IF  "$(CFG)" == "AveDesk - Win32 Release"

# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "AveDesk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\oLayer.cpp

!IF  "$(CFG)" == "AveDesk - Win32 Release"

# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "AveDesk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\oLayers.cpp

!IF  "$(CFG)" == "AveDesk - Win32 Release"

# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "AveDesk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Pidl.cpp
# End Source File
# Begin Source File

SOURCE=.\PidlDropBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Policies.cpp
# End Source File
# Begin Source File

SOURCE=.\Preference.cpp

!IF  "$(CFG)" == "AveDesk - Win32 Release"

# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "AveDesk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Preferences.cpp

!IF  "$(CFG)" == "AveDesk - Win32 Release"

# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "AveDesk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\resources.rc
# End Source File
# Begin Source File

SOURCE=.\shadowProperties.cpp
# End Source File
# Begin Source File

SOURCE=.\ShellHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\ShowCase1.cpp
# End Source File
# Begin Source File

SOURCE=.\SkinEnumerator.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "AveDesk - Win32 Release"

# ADD CPP /Yc"stdafx.h"

!ELSEIF  "$(CFG)" == "AveDesk - Win32 Debug"

# ADD CPP /Yc

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SuckyAnimation.cpp
# End Source File
# Begin Source File

SOURCE=.\ThemeBrowserDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ThemeIniToXMLMapper.cpp
# End Source File
# Begin Source File

SOURCE=.\ThemePackager.cpp
# End Source File
# Begin Source File

SOURCE=.\Timer.cpp

!IF  "$(CFG)" == "AveDesk - Win32 Release"

# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "AveDesk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Tools.cpp

!IF  "$(CFG)" == "AveDesk - Win32 Release"

# ADD CPP /Yu"stdafx.h"

!ELSEIF  "$(CFG)" == "AveDesk - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\TrayAnimationData.cpp
# End Source File
# Begin Source File

SOURCE=.\TrayIconAnimation.cpp
# End Source File
# Begin Source File

SOURCE=.\UpdateBounds.cpp
# End Source File
# Begin Source File

SOURCE=.\WaterAnimation.cpp
# End Source File
# Begin Source File

SOURCE=.\XMLLoader.cpp
# End Source File
# Begin Source File

SOURCE=.\XMLSaver.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "Global Settings"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AveDeskGlobalSettings.hpp
# End Source File
# Begin Source File

SOURCE=.\GlobalSettingsClickSheet.hpp
# End Source File
# Begin Source File

SOURCE=.\GlobalSettingsEffectSheet.h
# End Source File
# Begin Source File

SOURCE=.\GlobalSettingsMoveSheet.hpp
# End Source File
# Begin Source File

SOURCE=.\GlobalSettingsOtherSheet.hpp
# End Source File
# Begin Source File

SOURCE=.\GlobalSettingsSelectSheet.hpp
# End Source File
# Begin Source File

SOURCE=.\GlobalSettingsShowCaseSheet.hpp
# End Source File
# Begin Source File

SOURCE=.\GlobalSettingsToolsSheet.hpp
# End Source File
# Begin Source File

SOURCE=.\GlobalSettingsTraySheet.hpp
# End Source File
# End Group
# Begin Group "Dialogs"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ConfigShadowWindow.hpp
# End Source File
# Begin Source File

SOURCE=.\ConfigSubShadowWindow.hpp
# End Source File
# Begin Source File

SOURCE=.\DeskletAdvancedSheet.hpp
# End Source File
# Begin Source File

SOURCE=.\DeskletlabelSheet.hpp
# End Source File
# Begin Source File

SOURCE=.\ShadowPropertiesDlg.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\AboutDlg.h
# End Source File
# Begin Source File

SOURCE=.\AddDeskletDlg.h
# End Source File
# Begin Source File

SOURCE=.\Application.h
# End Source File
# Begin Source File

SOURCE=.\AvailableDeskletsAPI.h
# End Source File
# Begin Source File

SOURCE=.\AveDeskFileAssociation.h
# End Source File
# Begin Source File

SOURCE=.\AveDeskletFile.h
# End Source File
# Begin Source File

SOURCE=.\AveEnvironment.h
# End Source File
# Begin Source File

SOURCE=.\AveImage.h
# End Source File
# Begin Source File

SOURCE=.\AveInstAbout.h
# End Source File
# Begin Source File

SOURCE=.\AveInstAction.h
# End Source File
# Begin Source File

SOURCE=.\AveInstActions.h
# End Source File
# Begin Source File

SOURCE=.\AveInstActionsFactory.h
# End Source File
# Begin Source File

SOURCE=.\AveInstaller.h
# End Source File
# Begin Source File

SOURCE=.\AveInstallerEnvironment.h
# End Source File
# Begin Source File

SOURCE=.\AveInstallFile.h
# End Source File
# Begin Source File

SOURCE=.\AveInstBitmapWindow.h
# End Source File
# Begin Source File

SOURCE=.\AveInstCancelable.h
# End Source File
# Begin Source File

SOURCE=.\AveInstCopyInformation.h
# End Source File
# Begin Source File

SOURCE=.\AveInstCopyItem.h
# End Source File
# Begin Source File

SOURCE=.\AveInstCopyItemAllFilesAndSubFolders.h
# End Source File
# Begin Source File

SOURCE=.\AveInstCopyItemDirectory.h
# End Source File
# Begin Source File

SOURCE=.\AveInstCopyItemFile.h
# End Source File
# Begin Source File

SOURCE=.\AveInstDependencies.h
# End Source File
# Begin Source File

SOURCE=.\AveInstDeskletUsedRequirement.h
# End Source File
# Begin Source File

SOURCE=.\AveInstDoIfAction.h
# End Source File
# Begin Source File

SOURCE=.\AveInstDownloadAndInstallAction.h
# End Source File
# Begin Source File

SOURCE=.\AveInstError.h
# End Source File
# Begin Source File

SOURCE=.\AveInstFileDirOperation.h
# End Source File
# Begin Source File

SOURCE=.\AveInstFileFetcher.h
# End Source File
# Begin Source File

SOURCE=.\AveInstFontAction.h
# End Source File
# Begin Source File

SOURCE=.\AveInstInstallModuleAction.h
# End Source File
# Begin Source File

SOURCE=.\AveInstLoadDeskletAction.h
# End Source File
# Begin Source File

SOURCE=.\AveInstLoadTheme.h
# End Source File
# Begin Source File

SOURCE=.\AveInstNotRequired.h
# End Source File
# Begin Source File

SOURCE=.\AveInstPackageManager.h
# End Source File
# Begin Source File

SOURCE=.\AveInstProgressIndicator.h
# End Source File
# Begin Source File

SOURCE=.\AveInstProgressWindow.h
# End Source File
# Begin Source File

SOURCE=.\AveInstRegisterCOMServerAction.h
# End Source File
# Begin Source File

SOURCE=.\AveInstReloadLanguage.h
# End Source File
# Begin Source File

SOURCE=.\AveInstReqItem.h
# End Source File
# Begin Source File

SOURCE=.\AveInstRequiredAveDeskVersion.h
# End Source File
# Begin Source File

SOURCE=.\AveInstRequiredDeskletVersion.h
# End Source File
# Begin Source File

SOURCE=.\AveInstRequiredFile.h
# End Source File
# Begin Source File

SOURCE=.\AveInstRequiredInstallerVersion.h
# End Source File
# Begin Source File

SOURCE=.\AveInstRequiredPackage.h
# End Source File
# Begin Source File

SOURCE=.\AveInstRequiredToAcceptText.h
# End Source File
# Begin Source File

SOURCE=.\AveInstRequirements.h
# End Source File
# Begin Source File

SOURCE=.\AveInstRequirementsFactory.h
# End Source File
# Begin Source File

SOURCE=.\AveInstRunScriptAction.h
# End Source File
# Begin Source File

SOURCE=.\AveInstShowBitmapAction.h
# End Source File
# Begin Source File

SOURCE=.\AveInstShowTextAction.h
# End Source File
# Begin Source File

SOURCE=.\AveInstSuspendDeskletAction.h
# End Source File
# Begin Source File

SOURCE=.\AveInstSuspendedDesklets.h
# End Source File
# Begin Source File

SOURCE=.\AveInstSuspendedDeskletsList.h
# End Source File
# Begin Source File

SOURCE=.\AveInstUnloadLanguage.h
# End Source File
# Begin Source File

SOURCE=.\AveInstUnloadModuleAction.h
# End Source File
# Begin Source File

SOURCE=.\AveThemeFile.h
# End Source File
# Begin Source File

SOURCE=.\callback_funcs.h
# End Source File
# Begin Source File

SOURCE=.\CDropTarget.hpp
# End Source File
# Begin Source File

SOURCE=.\CloseButton.h
# End Source File
# Begin Source File

SOURCE=.\CloseButtonMouseOverPolicy.h
# End Source File
# Begin Source File

SOURCE=.\CloseButtonPolicy.h
# End Source File
# Begin Source File

SOURCE=.\ColorButton.h
# End Source File
# Begin Source File

SOURCE=.\ColorMatrix.h
# End Source File
# Begin Source File

SOURCE=.\ColorMatrixHelper.h
# End Source File
# Begin Source File

SOURCE=.\constants.h
# End Source File
# Begin Source File

SOURCE=.\ContainerLayer.h
# End Source File
# Begin Source File

SOURCE=.\ControlDlg.h
# End Source File
# Begin Source File

SOURCE=.\DataObjectImpl.hpp
# End Source File
# Begin Source File

SOURCE=.\Debug.h
# End Source File
# Begin Source File

SOURCE=.\Desklet.h
# End Source File
# Begin Source File

SOURCE=.\DeskletAppearanceSheet.h
# End Source File
# Begin Source File

SOURCE=.\DeskletBrowserDlg.h
# End Source File
# Begin Source File

SOURCE=.\DeskletCollection.h
# End Source File
# Begin Source File

SOURCE=.\DeskletFilter.h
# End Source File
# Begin Source File

SOURCE=.\DeskletFinder.h
# End Source File
# Begin Source File

SOURCE=.\DeskletInfo.h
# End Source File
# Begin Source File

SOURCE=.\DeskletInfos.h
# End Source File
# Begin Source File

SOURCE=.\DeskletItem.h
# End Source File
# Begin Source File

SOURCE=.\DeskletListBox.h
# End Source File
# Begin Source File

SOURCE=.\DeskletProvider.h
# End Source File
# Begin Source File

SOURCE=.\Desklets.h
# End Source File
# Begin Source File

SOURCE=.\DeskletSkin.h
# End Source File
# Begin Source File

SOURCE=.\DeskletSkinEnumerator.h
# End Source File
# Begin Source File

SOURCE=.\DeskletSkinInfo.h
# End Source File
# Begin Source File

SOURCE=.\DeskletSkins.h
# End Source File
# Begin Source File

SOURCE=.\DeskletsView.h
# End Source File
# Begin Source File

SOURCE=.\docklet.h
# End Source File
# Begin Source File

SOURCE=.\dockletdll.h
# End Source File
# Begin Source File

SOURCE=.\DoubleBufferedWindowImpl.h
# End Source File
# Begin Source File

SOURCE=.\DownloadableFileDescription.h
# End Source File
# Begin Source File

SOURCE=.\DragAnimationWindow.h
# End Source File
# Begin Source File

SOURCE=.\draginterface.h
# End Source File
# Begin Source File

SOURCE=.\DragTargetImpl.hpp
# End Source File
# Begin Source File

SOURCE=.\drawplugin.h
# End Source File
# Begin Source File

SOURCE=.\DrawPluginInstance.h
# End Source File
# Begin Source File

SOURCE=.\DropSettingsImpl.h
# End Source File
# Begin Source File

SOURCE=.\DropSourceImpl.hpp
# End Source File
# Begin Source File

SOURCE=.\DropTargetImpl.hpp
# End Source File
# Begin Source File

SOURCE=.\EffectLet.h
# End Source File
# Begin Source File

SOURCE=.\EnumDeskletDragDropFormats.h
# End Source File
# Begin Source File

SOURCE=.\FlipAnimation.h
# End Source File
# Begin Source File

SOURCE=.\FlipAnimWrapper.h
# End Source File
# Begin Source File

SOURCE=.\FoundDeskletEntry.h
# End Source File
# Begin Source File

SOURCE=.\helper_funcs.h
# End Source File
# Begin Source File

SOURCE=.\IconAnimator.h
# End Source File
# Begin Source File

SOURCE=.\IconWindow.h
# End Source File
# Begin Source File

SOURCE=.\IDataObjImpl.hpp
# End Source File
# Begin Source File

SOURCE=.\IIconAnimatorCallback.h
# End Source File
# Begin Source File

SOURCE=.\image_funcs.h
# End Source File
# Begin Source File

SOURCE=.\ImageListBox.h
# End Source File
# Begin Source File

SOURCE=.\Images.h
# End Source File
# Begin Source File

SOURCE=.\INI_API_Interceptor.h
# End Source File
# Begin Source File

SOURCE=.\InstallErrorDlg.h
# End Source File
# Begin Source File

SOURCE=.\IObservable.h
# End Source File
# Begin Source File

SOURCE=.\IObserver.h
# End Source File
# Begin Source File

SOURCE=.\IPersistable.h
# End Source File
# Begin Source File

SOURCE=.\IProgressIndicator.h
# End Source File
# Begin Source File

SOURCE=.\label.h
# End Source File
# Begin Source File

SOURCE=.\LabelBackground.h
# End Source File
# Begin Source File

SOURCE=.\LabelProperties.h
# End Source File
# Begin Source File

SOURCE=.\layer.h
# End Source File
# Begin Source File

SOURCE=.\LayeredWindowImpl.hpp
# End Source File
# Begin Source File

SOURCE=.\ListBoxCategories.h
# End Source File
# Begin Source File

SOURCE=.\Matrix.h
# End Source File
# Begin Source File

SOURCE=.\Module.h
# End Source File
# Begin Source File

SOURCE=.\Modules.h
# End Source File
# Begin Source File

SOURCE=.\ModulesDlg.h
# End Source File
# Begin Source File

SOURCE=.\NewFeedDlg.h
# End Source File
# Begin Source File

SOURCE=.\od_docklet.hpp
# End Source File
# Begin Source File

SOURCE=.\od_docklet_instance.hpp
# End Source File
# Begin Source File

SOURCE=.\oLayer.h
# End Source File
# Begin Source File

SOURCE=.\oLayers.h
# End Source File
# Begin Source File

SOURCE=.\Pidl.h
# End Source File
# Begin Source File

SOURCE=.\PidlDropBox.h
# End Source File
# Begin Source File

SOURCE=.\Policies.h
# End Source File
# Begin Source File

SOURCE=.\Preference.h
# End Source File
# Begin Source File

SOURCE=.\Preferences.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\ShadowProperties.h
# End Source File
# Begin Source File

SOURCE=.\ShellHelper.h
# End Source File
# Begin Source File

SOURCE=.\ShowCase1.h
# End Source File
# Begin Source File

SOURCE=.\SkinEnumerator.h
# End Source File
# Begin Source File

SOURCE=.\SpecializedShowCaseTrackBar.hpp
# End Source File
# Begin Source File

SOURCE=.\SplitterBar.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\SuckyAnimation.h
# End Source File
# Begin Source File

SOURCE=.\ThemeBrowserDlg.h
# End Source File
# Begin Source File

SOURCE=.\ThemeIniToXMLMapper.h
# End Source File
# Begin Source File

SOURCE=.\ThemeListBox.h
# End Source File
# Begin Source File

SOURCE=.\ThemePackager.h
# End Source File
# Begin Source File

SOURCE=.\Timer.h
# End Source File
# Begin Source File

SOURCE=.\Tools.hpp
# End Source File
# Begin Source File

SOURCE=.\ToolTipImpl.h
# End Source File
# Begin Source File

SOURCE=.\TrayAnimationData.h
# End Source File
# Begin Source File

SOURCE=.\TrayIconAnimation.h
# End Source File
# Begin Source File

SOURCE=.\UpdateBounds.h
# End Source File
# Begin Source File

SOURCE=.\WaterAnimation.h
# End Source File
# Begin Source File

SOURCE=.\XMLLoader.h
# End Source File
# Begin Source File

SOURCE=.\XMLSaver.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\graphics\about.bmp
# End Source File
# Begin Source File

SOURCE=.\Application.rgs
# End Source File
# Begin Source File

SOURCE=.\graphics\Application.rgs
# End Source File
# Begin Source File

SOURCE=.\graphics\ave_deskicon.ico
# End Source File
# Begin Source File

SOURCE=.\graphics\ave_deskiconsmall.bmp
# End Source File
# Begin Source File

SOURCE=".\graphics\AveDesk Icon basic.ico"
# End Source File
# Begin Source File

SOURCE=".\graphics\AveDesk Icon final.ico"
# End Source File
# Begin Source File

SOURCE=.\AveDesk.rgs
# End Source File
# Begin Source File

SOURCE=.\graphics\AveDesk.rgs
# End Source File
# Begin Source File

SOURCE=.\graphics\avedesk_icon.ico
# End Source File
# Begin Source File

SOURCE=.\AveDeskletFile.rgs
# End Source File
# Begin Source File

SOURCE=.\graphics\AveDeskletFile.rgs
# End Source File
# Begin Source File

SOURCE=.\AveInstallFile.rgs
# End Source File
# Begin Source File

SOURCE=.\graphics\AveInstallFile.rgs
# End Source File
# Begin Source File

SOURCE=.\AveThemeFile.rgs
# End Source File
# Begin Source File

SOURCE=.\graphics\AveThemeFile.rgs
# End Source File
# Begin Source File

SOURCE=.\graphics\bitmap5.bmp
# End Source File
# Begin Source File

SOURCE=.\graphics\ColorMatrix.rgs
# End Source File
# Begin Source File

SOURCE=.\graphics\Debug.rgs
# End Source File
# Begin Source File

SOURCE=.\Desklet.rgs
# End Source File
# Begin Source File

SOURCE=.\graphics\Desklet.rgs
# End Source File
# Begin Source File

SOURCE=.\DeskletInfo.rgs
# End Source File
# Begin Source File

SOURCE=.\DeskletInfos.rgs
# End Source File
# Begin Source File

SOURCE=.\Desklets.rgs
# End Source File
# Begin Source File

SOURCE=.\graphics\Desklets.rgs
# End Source File
# Begin Source File

SOURCE=.\DeskletSkin.rgs
# End Source File
# Begin Source File

SOURCE=.\graphics\DeskletSkin.rgs
# End Source File
# Begin Source File

SOURCE=.\DeskletSkins.rgs
# End Source File
# Begin Source File

SOURCE=.\graphics\DeskletSkins.rgs
# End Source File
# Begin Source File

SOURCE=.\Docklets.rgs
# End Source File
# Begin Source File

SOURCE=.\graphics\down.bmp
# End Source File
# Begin Source File

SOURCE=.\EffectLet.rgs
# End Source File
# Begin Source File

SOURCE=.\graphics\EffectLet.rgs
# End Source File
# Begin Source File

SOURCE=.\graphics\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\graphics\icon_desklet.ico
# End Source File
# Begin Source File

SOURCE=.\graphics\icon_install.ico
# End Source File
# Begin Source File

SOURCE=.\graphics\icon_theme.ico
# End Source File
# Begin Source File

SOURCE=.\Image.rgs
# End Source File
# Begin Source File

SOURCE=.\Images.rgs
# End Source File
# Begin Source File

SOURCE=.\graphics\left.bmp
# End Source File
# Begin Source File

SOURCE=.\graphics\logo.bmp
# End Source File
# Begin Source File

SOURCE=.\graphics\Matrix.rgs
# End Source File
# Begin Source File

SOURCE=.\oLayer.rgs
# End Source File
# Begin Source File

SOURCE=.\oLayers.rgs
# End Source File
# Begin Source File

SOURCE=.\graphics\Preference.rgs
# End Source File
# Begin Source File

SOURCE=.\graphics\Preferences.rgs
# End Source File
# Begin Source File

SOURCE=.\graphics\right.bmp
# End Source File
# Begin Source File

SOURCE=.\graphics\ShowCase.rgs
# End Source File
# Begin Source File

SOURCE=.\ShowCase.rgs
# End Source File
# Begin Source File

SOURCE=.\graphics\SkinEnumerator.rgs
# End Source File
# Begin Source File

SOURCE=.\SkinEnumerator.rgs
# End Source File
# Begin Source File

SOURCE=.\Timer.rgs
# End Source File
# Begin Source File

SOURCE=.\graphics\up.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\graphics\AveDesk.exe.manifest
# End Source File
# Begin Source File

SOURCE=.\manifest.txt
# End Source File
# End Target
# End Project
# Section AveDesk : {00000000-0000-0000-0000-800000800000}
# 	1:29:IDD_CSHADOWPROPERTIESDIALOGUE:123
# End Section
