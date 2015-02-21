#pragma once

#include "stdafx.h"
#include <vector>
#include <string>
#include <fstream>

inline void CHECKHRES(HRESULT hRes)
{
	if(FAILED(hRes))
		throw(hRes);
}

template<typename T, typename T2 = T> struct pair_t
{
	T key;
	T2 val;

	pair_t(){}
	pair_t(const T& k, const T2& v):key(k), val(v){}
};

struct ToolSettings
{
public:
	ToolSettings()
	{
		showTrayIcon = TRUE;
		showTrayIconMenu = TRUE;
		useControls = TRUE;

		loadFileOnBoot = TRUE;
		loadBootWhenEmpty = TRUE;
		saveTheme = TRUE;
		loadTheme = TRUE;
		saveSettings = TRUE;
		loadSettings = TRUE;
		useCloseButton = TRUE;
	}

	BOOL useCloseButton;

	std::wstring toolName;
	std::wstring settingsFolderName;
	std::wstring outputDir;
	std::vector<std::wstring> deskletFiles;
	BOOL useControls;
	BOOL showTrayIcon;
	BOOL showTrayIconMenu;
	std::wstring trayIconFile;
	std::wstring trayIconToolTip;

	std::wstring realIconFileName;

	BOOL loadFileOnBoot;
	BOOL loadBootWhenEmpty;
	BOOL saveTheme;
	BOOL loadTheme;
	BOOL saveSettings;
	BOOL loadSettings;
	std::wstring themeName;
	std::wstring settingsFile;
	std::wstring bootFile;

	std::vector<pair_t<std::wstring,std::wstring> > versionInfo;

	std::wstring lcase(const std::wstring& str)
	{
		std::wstring lc = str;
		for(size_t i = 0; i < str.length(); ++i)
			lc[i] = tolower(str[i]);

		return lc;
	}

	std::vector<pair_t<std::wstring,std::wstring> > GetDefaultVersionInfo()
	{
		std::vector<pair_t<std::wstring,std::wstring> > info;

		std::fstream file;
		file.open(WStringToString(GetDataFilePath(L"version.txt")).c_str(), std::ios_base::in);
		std::string line;
		while(std::getline(file,line))
		{
			std::wstring wLine = StringToSWtring(line);
			std::vector<std::wstring> keyval = splitString(wLine, L"=",1);
			if(keyval.size() > 1)
				info.push_back(pair_t<std::wstring,std::wstring>(keyval[0], keyval[1]));
		}
		file.close();

		return info;
	}

	std::vector<std::wstring> splitString(const std::wstring& str, const std::wstring& sep, int max=-1)
	{
		std::vector<std::wstring> parts;
		size_t beginPos = 0;
		size_t endPos = 0;

		int count = 0;

		while((-1==max || count<max) && (endPos = str.find(sep, beginPos)) != std::wstring::npos)
		{
			std::wstring part = str.substr(beginPos, endPos - beginPos);
			parts.push_back(part);
			beginPos = endPos+sep.length();
			count++;
		}

		if(beginPos < str.length())
		{
			std::wstring part = str.substr(beginPos, str.length() - beginPos);
			parts.push_back(part);
		}

		return parts;
	}

	std::wstring GetAveDeskPath()
	{
		WCHAR wAveDeskPath[MAX_PATH] = {0};
		GetModuleFileName(NULL, wAveDeskPath, MAX_PATH);
		PathRemoveFileSpec(wAveDeskPath);
		PathRemoveFileSpec(wAveDeskPath);

		return wAveDeskPath;
	}

	std::wstring GetDataFilePath(const WCHAR* file)
	{
		WCHAR wDataFilePath[MAX_PATH] = {0};
		GetModuleFileName(NULL, wDataFilePath, MAX_PATH);
		PathRemoveFileSpec(wDataFilePath);
		PathAppendW(wDataFilePath, L"data\\");
		PathAppendW(wDataFilePath, file);

		return wDataFilePath;
	}

	std::string WStringToString(const std::wstring& ws)
	{
		char* str = new char[ws.length() + 1];
		WideCharToMultiByte(GetACP(), 0, ws.c_str(), -1, str, ws.length() + 1, 0, 0);

		std::string stdstr = str;
		delete[] str;

		return stdstr;
	}

	std::wstring StringToSWtring(const std::string& str)
	{
		WCHAR* wstr = new WCHAR[str.length() + 1];
		MultiByteToWideChar(GetACP(),0, str.c_str(), -1, wstr, str.length()+1);

		std::wstring wString = wstr;
		delete[] wstr;

		return wString;
	}

	void StartProject()
	{
		if(!PathFileExistsW(outputDir.c_str()))
			CreateDirectoryW(outputDir.c_str(), NULL);


		WCHAR wAveProjectFile[MAX_PATH] = {0};
		wcscpy_s(wAveProjectFile, MAX_PATH, outputDir.c_str());
		PathAppendW(wAveProjectFile, L"AveProject.avetool");
		WritePrivateProfileString(L"Project", L"Version", L"1", wAveProjectFile);

		WCHAR policiesFile[MAX_PATH] = {0};
		wcscpy_s(policiesFile, MAX_PATH, GetDataFilePath(L"policies.dat").c_str());

		WCHAR wPoliciesOutput[MAX_PATH] = {0};
		wcscpy_s(wPoliciesOutput, MAX_PATH, outputDir.c_str());
		PathAppendW(wPoliciesOutput, L"policies.xml");

		DeleteFileW(wPoliciesOutput);
		CopyFileW(policiesFile, wPoliciesOutput, FALSE);

		WCHAR versionFile[MAX_PATH] = {0};
		wcscpy_s(versionFile, MAX_PATH, GetDataFilePath(L"version.txt").c_str());

		WCHAR wVersionOutput[MAX_PATH] = {0};
		wcscpy_s(wVersionOutput, MAX_PATH, outputDir.c_str());
		PathAppendW(wVersionOutput, L"version.txt");

		DeleteFileW(wVersionOutput);
		CopyFileW(versionFile,wVersionOutput,FALSE);
	}

	void DoBuild()
	{
		if(!PathFileExistsW(outputDir.c_str()))
			CreateDirectoryW(outputDir.c_str(), NULL);

		DoIcon();

		WCHAR wFilesFile[MAX_PATH] = {0};
		wcscpy_s(wFilesFile, MAX_PATH, outputDir.c_str());
		PathAppendW(wFilesFile, L"buildpackage.txt");
		DeleteFileW(wFilesFile);

		std::fstream file;
		file.open(WStringToString(wFilesFile).c_str(), std::ios_base::out | std::ios_base::trunc);
		file << WStringToString(GetAveDeskPath()) << std::endl;

		file << "?" << WStringToString(realIconFileName) << std::endl;

		if(useCloseButton)
		{
			file << WStringToString(GetAveDeskPath()) << "\\images" << std::endl;
			file << WStringToString(GetAveDeskPath()) << "\\images\\close.png" << std::endl;
			file << WStringToString(GetAveDeskPath()) << "\\data" << std::endl;
			file << WStringToString(GetAveDeskPath()) << "\\data\\CloseEffect.data" << std::endl;
		}

		if(useControls)
			file << WStringToString(GetAveDeskPath()) << "\\avecontrols.dll" << std::endl;

		if(settingsFile.length() > 0)
			file << "?" << WStringToString(settingsFile) << std::endl;

		BOOL isAveScripter=FALSE;

		BOOL hasBootFile = FALSE;
		for(std::vector<std::wstring>::iterator iter = deskletFiles.begin(); iter != deskletFiles.end(); ++iter)
		{
			if(!isAveScripter && wcsstr(iter->c_str(), L"avescripter.dll") != 0)
				isAveScripter = TRUE;
			if(!hasBootFile)
			{
				std::wstring bootlc = lcase(bootFile);
				if(bootlc == lcase(*iter))
					hasBootFile = TRUE;
			}
			file << WStringToString(*iter) << std::endl;
		}

		if(settingsFile.length() > 0)
			file << WStringToString(settingsFile) << std::endl;
	
		if(!hasBootFile && bootFile.length() > 0)
			file << WStringToString(bootFile) << std::endl;

		//if(isAveScripter)
		file << WStringToString(GetAveDeskPath()) << "\\aveimg.dll" << std::endl;

		file.close();

		DoZip(wFilesFile);
		DoVersion();
		DoModifyPoliciesFile();
		DoResCompile();
	}

	void DoIcon()
	{
		WCHAR iconFile[MAX_PATH] = {0};
		wcscpy_s(iconFile, MAX_PATH, trayIconFile.c_str());
		const WCHAR* ext = PathFindExtension(iconFile);
		if(ext != NULL && _wcsicmp(ext, L".ico") == 0)
		{
			realIconFileName = iconFile;
		}
		else
		{
			WCHAR wIcoApp[MAX_PATH] = {0};
			GetModuleFileName(NULL, wIcoApp, MAX_PATH);
			PathRemoveFileSpec(wIcoApp);
			PathAppendW(wIcoApp, L"bin\\AveIco.exe");
			PathQuoteSpaces(wIcoApp);

			WCHAR outputFile[MAX_PATH] = {0};
			wcscpy_s(outputFile, MAX_PATH, outputDir.c_str());
			PathAppendW(outputFile, L"icon.ico");
			realIconFileName = outputFile;

			PathQuoteSpaces(outputFile);
		
			PathQuoteSpaces(iconFile);

			std::wstring cmdLine = wIcoApp;
			cmdLine += L" -i ";
			cmdLine += iconFile;
			cmdLine += L" -o ";
			cmdLine += outputFile;
			cmdLine += L" -s 32 32bpp -s 32 24bpp -s 16 32bpp -s 16 24bpp -s 48 32bpp";
			cmdLine += L" -noconfirm -silent";

			WCHAR* cmd = new WCHAR[cmdLine.length()+1];
			wcscpy_s(cmd, cmdLine.length()+1, cmdLine.c_str());

			STARTUPINFO si = {sizeof(STARTUPINFO)};
			si.dwFlags = STARTF_USESHOWWINDOW;
			si.wShowWindow = SW_HIDE;
			PROCESS_INFORMATION pi = {0};
			CreateProcess(NULL, cmd, NULL, NULL, FALSE, CREATE_DEFAULT_ERROR_MODE,0,0,&si, &pi);
			DWORD err = GetLastError();
			if(pi.hProcess != NULL)
				WaitForSingleObject(pi.hProcess, INFINITE);

			delete[] cmd;
		}
	}

	void DoVersion()
	{
		WCHAR versionFile[MAX_PATH] = {0};
		wcscpy_s(versionFile, MAX_PATH, outputDir.c_str());
		PathAppendW(versionFile, L"version.txt");
		PathQuoteSpaces(versionFile);

		std::fstream file;
		file.open(WStringToString(versionFile).c_str(), std::ios_base::out | std::ios_base::trunc);
		for(std::vector<pair_t<std::wstring, std::wstring> >::iterator iter = versionInfo.begin();
				iter != versionInfo.end();
				++iter)
		{
			file << WStringToString(iter->key) << "=" << WStringToString(iter->val) << std::endl;
		}

		file.close();
	}

	void DoZip(const WCHAR* input)
	{
		WCHAR wZipApp[MAX_PATH] = {0};
		GetModuleFileName(NULL, wZipApp, MAX_PATH);
		PathRemoveFileSpec(wZipApp);
		PathAppendW(wZipApp, L"bin\\AveZip.exe");
		WCHAR cmdLine[MAX_PATH*10] = {0};
		wcscpy_s(cmdLine, MAX_PATH*10, wZipApp);
		wcscat_s(cmdLine, MAX_PATH*10, L" ");
		wcscat_s(cmdLine, MAX_PATH*10, input);
		wcscat_s(cmdLine, MAX_PATH*10, L" ");
		wcscat_s(cmdLine, MAX_PATH, outputDir.c_str());
		wcscat_s(cmdLine, MAX_PATH*10, L"\\package.zip");
		STARTUPINFO si = {sizeof(STARTUPINFO)};
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;
		PROCESS_INFORMATION pi = {0};
		CreateProcess(NULL, cmdLine, NULL, NULL, FALSE, CREATE_DEFAULT_ERROR_MODE,0,0,&si, &pi);
		DWORD err = GetLastError();
		if(pi.hProcess != NULL)
			WaitForSingleObject(pi.hProcess, INFINITE);
	}

	CComPtr<IXMLDOMNode> GetNode(CComPtr<IXMLDOMDocument>& doc, const WCHAR* name)
	{	

		CComBSTR bstrName( name );
		CComPtr<IXMLDOMNode> node;
		if(FAILED( doc->selectSingleNode(bstrName,&node)))
			return node;

		if(NULL == node.p)
			return node;

		return node;
	}

	CComPtr<IXMLDOMNode> CreateNode(CComPtr<IXMLDOMDocument>& doc, const WCHAR* name)
	{
		std::vector<std::wstring> parts = splitString(name, L"//");
		std::wstring tmpName;
		std::vector<CComPtr<IXMLDOMNode> > nodes;
		CComPtr<IXMLDOMNode> finaleNode;
		for(std::vector<std::wstring>::iterator iter = parts.begin(); iter != parts.end(); ++iter)
		{
			const std::wstring& part = *iter;
			tmpName += L"//";
			tmpName += part;
			CComPtr<IXMLDOMNode> node = GetNode(doc, tmpName.c_str());
			if(node != NULL)
				nodes.push_back(node);
			else
			{
				CComBSTR bstrName( part.c_str() );
				CComBSTR bstrNamespace( L"" );

				CComPtr<IXMLDOMNode> newNode;

				CComVariant varNodeType(NODE_ELEMENT);
				if(FAILED(doc->createNode(varNodeType,bstrName,bstrNamespace,&newNode)))
					return finaleNode;

				CComPtr<IXMLDOMNode> retNode;
				if( FAILED(nodes[nodes.size()-1]->appendChild(newNode,&retNode)) )
					return finaleNode;

				nodes.push_back(retNode);
			}
		}

		finaleNode = nodes[nodes.size()-1];
		return finaleNode;
	}

	void WritePolicyBool(CComPtr<IXMLDOMDocument>& doc, const WCHAR* name, BOOL val)
	{
		if(val)
			WritePolicy(doc, name, L"true");
		else
			WritePolicy(doc, name, L"false");
	}

	void WritePolicy(CComPtr<IXMLDOMDocument>& doc, const WCHAR* name, const WCHAR* val)
	{
		std::wstring polName = L"policies//policy//";
		polName += name;
		std::wstring valName = polName;
		valName += L"//@value";

		CComPtr<IXMLDOMNode> node = GetNode(doc, valName.c_str());
		if(NULL == node.p)
			node = GetNode(doc, polName.c_str());
		if(node.p != NULL)
			node->put_text(CComBSTR(val));
		else
		{
			CComPtr<IXMLDOMNode> node = CreateNode(doc, polName.c_str());
			if(node.p != NULL)
				node->put_text(CComBSTR(val));
		}
	}

	void DoModifyPoliciesFile()
	{
		WCHAR policiesFile[MAX_PATH] = {0};
		wcscpy_s(policiesFile, MAX_PATH, outputDir.c_str());
		PathAppendW(policiesFile, L"policies.xml");	

		CComPtr<IXMLDOMDocument> doc;
		try
		{
			CHECKHRES( doc.CoCreateInstance(CLSID_DOMDocument) );
			doc->put_preserveWhiteSpace(VARIANT_TRUE);


			CComBSTR bstrFileName( policiesFile );
			CComVariant varFileName( bstrFileName );
			VARIANT_BOOL vbSuccess = { VARIANT_FALSE };

			CHECKHRES ( doc->load(varFileName,&vbSuccess) );

			WritePolicy(doc, L"generator//name", L"AveTool");
			WritePolicy(doc, L"paths//settingsFolderName", settingsFolderName.c_str());
			WritePolicyBool(doc, L"ui//trayicon//show", showTrayIcon);
			WritePolicyBool(doc, L"ui//trayicon//menu//allow", showTrayIconMenu);
			

			WCHAR trayFileName[MAX_PATH] = {0};
			wcscpy_s(trayFileName, MAX_PATH, realIconFileName.c_str());
			WritePolicy(doc, L"ui//trayicon//iconfile", PathFindFileNameW(trayFileName));
			WritePolicy(doc, L"ui//trayicon//tooltip", trayIconToolTip.c_str());

			WritePolicyBool(doc, L"loading//doFirstBootIntro", loadFileOnBoot);
			WritePolicyBool(doc, L"loading//firstBootWhenEmptyTheme", loadBootWhenEmpty);
			WritePolicyBool(doc, L"closing//saveTheme", saveTheme);
			WritePolicyBool(doc, L"loading//loadTheme", loadTheme);
			WritePolicyBool(doc, L"closing//saveSettings", saveSettings);
			WritePolicyBool(doc, L"loading//loadSettings", loadSettings);

			WritePolicy(doc, L"files//themeFileName", themeName.c_str());
			
			WCHAR settingsFileName[MAX_PATH] = {0};
			wcscpy_s(settingsFileName, MAX_PATH, settingsFile.c_str());
			WritePolicy(doc, L"loading//initialSettingsFile", PathFindFileNameW(settingsFileName));

			WCHAR bootFileName[MAX_PATH] = {0};
			wcscpy_s(bootFileName, MAX_PATH, lcase(bootFile).c_str());
			if(wcsstr(bootFileName, lcase(GetAveDeskPath()).c_str()) == 0)
				WritePolicy(doc, L"loading//deskletForFirstBoot", PathFindFileNameW(bootFileName));
			else
			{
				WCHAR relPath[MAX_PATH] = {0};
				PathRelativePathToW(relPath, GetAveDeskPath().c_str(),FILE_ATTRIBUTE_DIRECTORY,bootFileName, 0); 
				std::wstring wsRelPath = relPath;
				if(wsRelPath[0] == L'.'  && (wsRelPath[1] == L'\\' || wsRelPath[1] == L'/'))
					wsRelPath = wsRelPath.substr(2, wsRelPath.length() - 2);
				WritePolicy(doc, L"loading//deskletForFirstBoot", wsRelPath.c_str());
			}


			doc->save(varFileName);
		}
		catch(HRESULT&)
		{
		}
	}

	void DoResCompile()
	{
		WCHAR wResCApp[MAX_PATH] = {0};
		GetModuleFileName(NULL, wResCApp, MAX_PATH);
		PathRemoveFileSpec(wResCApp);
		PathAppendW(wResCApp, L"bin\\AveResC.exe");
		PathQuoteSpaces(wResCApp);

		WCHAR templateFile[MAX_PATH] = {0};
		wcscpy_s(templateFile, MAX_PATH, GetDataFilePath(L"avedesk.template").c_str());
		PathQuoteSpaces(templateFile);

		WCHAR outputFile[MAX_PATH] = {0};
		wcscpy_s(outputFile, MAX_PATH, outputDir.c_str());
		PathAppendW(outputFile, L"desklet.exe");
		PathQuoteSpaces(outputFile);

		WCHAR policiesFile[MAX_PATH] = {0};
		wcscpy_s(policiesFile, MAX_PATH, outputDir.c_str());
		PathAppendW(policiesFile, L"policies.xml");
		PathQuoteSpaces(policiesFile);

		WCHAR versionFile[MAX_PATH] = {0};
		wcscpy_s(versionFile, MAX_PATH, outputDir.c_str());
		PathAppendW(versionFile, L"version.txt");
		PathQuoteSpaces(versionFile);

		WCHAR packageFile[MAX_PATH] = {0};
		wcscpy_s(packageFile, MAX_PATH, outputDir.c_str());
		PathAppendW(packageFile, L"package.zip");
		PathQuoteSpaces(packageFile);

		WCHAR iconFile[MAX_PATH] = {0};
		wcscpy_s(iconFile, MAX_PATH, realIconFileName.c_str());
		PathQuoteSpaces(iconFile);

		std::wstring cmdLine;
		cmdLine += wResCApp;
		cmdLine += L" ";
		cmdLine += templateFile;
		cmdLine += L" ";
		cmdLine += outputFile;
		cmdLine += L" ";
		cmdLine += packageFile;
		cmdLine += L" ";
		cmdLine += policiesFile;
		cmdLine += L" ";
		cmdLine += iconFile;
		cmdLine += L" ";
		cmdLine += versionFile;

		WCHAR* cmd = new WCHAR[cmdLine.length()+1];
		wcscpy_s(cmd, cmdLine.length()+1, cmdLine.c_str());

		STARTUPINFO si = {sizeof(STARTUPINFO)};
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;
		PROCESS_INFORMATION pi = {0};
		CreateProcess(NULL, cmd, NULL, NULL, FALSE, CREATE_DEFAULT_ERROR_MODE,0,0,&si, &pi);
		DWORD err = GetLastError();
		if(pi.hProcess != NULL)
			WaitForSingleObject(pi.hProcess, INFINITE);

		delete[] cmd;

		std::wstring msg;
		msg = L"Your Desklet has been compiled!\nPath:";
		msg += outputFile;

		MessageBox(NULL, msg.c_str(), L"AveTool", MB_ICONINFORMATION);

	}
};