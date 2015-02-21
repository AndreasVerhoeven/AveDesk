#include <windows.h>

#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib")

#include <commctrl.h>
#include "prsht.h"
#pragma comment(lib, "comctl32.lib")

#include "../deskletsdk.h"
#include "../drawpluginsdk.h"

#include "resource.h"

#include "colormatrixhelper.h"

struct PLUGIN_DATA
{
	public:
		HWND		hwndDesklet;
		HINSTANCE	hInstanceDll;

		Bitmap* bmp;

		char imgPath[MAX_PATH];

		bool useColorization;

		int saturation;
		int hue;
		int alpha;

		bool inPreview;
		ColorMatrix colorMatrix;
};


void __stdcall OnDrawPluginInfo(char* name, char* author, char* notes, int* version)
{
	// pre:  
	// post: 

	strcpy(name,"SelectionBox");
	strcpy(author,"Andreas Verhoeven");
	strcpy(notes,"Sample Drawplugin for AveDesk1.1");
	*version = 100;
}

PLUGIN_DATA* __stdcall OnCreate(HWND hwndDesklet, HINSTANCE hInstanceDll, char* ini, char* group)
{
	// pre:  
	// post: 

	PLUGIN_DATA* lpData = new PLUGIN_DATA;
	lpData->hwndDesklet = hwndDesklet;
	lpData->hInstanceDll = hInstanceDll;
	lpData->bmp = 0;

	lpData->inPreview = false;
	lpData->colorMatrix = ColorMatrixHelper::GetIdentityMatrix();

	GetDLLPath(hInstanceDll,lpData->imgPath,sizeof(lpData->imgPath)/sizeof(lpData->imgPath[0]));
	strcat(lpData->imgPath,"bg.png");
	
	lpData->useColorization = false;
	lpData->saturation = 100;
	lpData->hue = 0;
	lpData->alpha = 255;

	if(ini && group)
	{
		GetPrivateProfileString(group,"_EBG",lpData->imgPath,lpData->imgPath,sizeof(lpData->imgPath)/sizeof(lpData->imgPath[0]),ini);

		lpData->useColorization	= GetPrivateProfileInt(group,"_EUC",lpData->useColorization ? TRUE : FALSE,ini) == TRUE;
		lpData->saturation		= GetPrivateProfileInt(group,"_ESA",lpData->saturation,ini);
		lpData->hue				= GetPrivateProfileInt(group,"_EHU",lpData->hue,ini);
		lpData->alpha			= GetPrivateProfileInt(group,"_EAL",lpData->alpha,ini);
	}

	return lpData;
}
void __stdcall OnDestroy(PLUGIN_DATA* lpData)
{
	// pre:  
	// post: 

	if(lpData->bmp)
	{	
		delete lpData->bmp;
		lpData->bmp = 0;
	}

	delete lpData;
}

void __stdcall OnSave(PLUGIN_DATA* lpData, char* ini, char* group)
{
	// pre:  
	// post: 

	WritePrivateProfileString(group,"_EBG",lpData->imgPath,ini);
	WritePrivateProfileInt(group,"_EUC",lpData->useColorization ? TRUE : FALSE,ini);
	WritePrivateProfileInt(group,"_ESA",lpData->saturation,ini);
	WritePrivateProfileInt(group,"_EHU",lpData->hue,ini);
	WritePrivateProfileInt(group,"_EAL",lpData->alpha,ini);
}

int CALLBACK ConfigDlgProc(HWND hDlg,UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_INITDIALOG:
		{
			SetProp(hDlg,"lpData",(HANDLE)lParam);

			PLUGIN_DATA* lpData = (PLUGIN_DATA*) GetProp(hDlg,"lpData");

			if(!lpData)return TRUE;

			SendDlgItemMessage(hDlg,IDC_SLIDESATURATION,TBM_SETRANGEMIN,(WPARAM)FALSE,(LPARAM)0);
			SendDlgItemMessage(hDlg,IDC_SLIDESATURATION,TBM_SETRANGEMAX,(WPARAM)TRUE,(LPARAM)800);
			SendDlgItemMessage(hDlg,IDC_SLIDESATURATION,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)lpData->saturation);

			SendDlgItemMessage(hDlg,IDC_SLIDEHUE,TBM_SETRANGEMIN,(WPARAM)FALSE,(LPARAM)-180);
			SendDlgItemMessage(hDlg,IDC_SLIDEHUE,TBM_SETRANGEMAX,(WPARAM)TRUE,(LPARAM)180);
			SendDlgItemMessage(hDlg,IDC_SLIDEHUE,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)lpData->hue);

			SendDlgItemMessage(hDlg,IDC_SLIDEALPHA,TBM_SETRANGEMIN,(WPARAM)FALSE,(LPARAM)0);
			SendDlgItemMessage(hDlg,IDC_SLIDEALPHA,TBM_SETRANGEMAX,(WPARAM)TRUE,(LPARAM)255);
			SendDlgItemMessage(hDlg,IDC_SLIDEALPHA,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)lpData->alpha);

			CheckDlgButton(hDlg,IDC_USECOLORIZATION,lpData->useColorization ? TRUE : FALSE);

		}
		break;

		case WM_HSCROLL:
			{
					PLUGIN_DATA* lpData = (PLUGIN_DATA*) GetProp(hDlg,"lpData");
					if(!lpData)return TRUE;

					lpData->saturation	= SendDlgItemMessage(hDlg,IDC_SLIDESATURATION,TBM_GETPOS,0,0);
					lpData->hue			= SendDlgItemMessage(hDlg,IDC_SLIDEHUE,TBM_GETPOS,0,0);
					lpData->alpha		= SendDlgItemMessage(hDlg,IDC_SLIDEALPHA,TBM_GETPOS,0,0);

					lpData->useColorization = IsDlgButtonChecked(hDlg,IDC_USECOLORIZATION) == BST_CHECKED;

					if(lpData->useColorization)
					{
							lpData->colorMatrix = ColorMatrixHelper::GetIdentityMatrix();
							lpData->colorMatrix = ColorMatrixHelper::SetSaturation(lpData->colorMatrix,float(lpData->saturation)/100.0f);
							lpData->colorMatrix = ColorMatrixHelper::SetHue(lpData->colorMatrix,float(lpData->hue));
							lpData->colorMatrix.m[3][3] = float(lpData->alpha) / 255.0f;

					}

					DeskletRedraw(lpData->hwndDesklet);
			}
		break;

		case WM_PAINT:
		{
			PLUGIN_DATA* lpData = (PLUGIN_DATA*) GetProp(hDlg,"lpData");
			if(!lpData)return FALSE;

			PAINTSTRUCT ps = {0};
			HDC hdc = BeginPaint(hDlg,&ps);

			RECT r= {0};
			GetClientRect(GetDlgItem(hDlg,IDC_IMG),&r);

			Bitmap buf(r.right - r.left-4, r.bottom - r.top-4);
			Graphics bufG(&buf);
			bufG.Clear(Color(255,255,255,255));

			Bitmap* bmp = DeskletLoadGDIPlusImage(lpData->imgPath);
			bufG.DrawImage(bmp,Rect(2,2,buf.GetWidth() - 4, buf.GetHeight() - 4),0,0,bmp->GetWidth(),bmp->GetHeight(),UnitPixel,0,0,0);
			delete bmp;

			Graphics g(GetDlgItem(hDlg,IDC_IMG));
			g.DrawImage(&buf,2,2);

			ReleaseDC(hDlg,hdc);
			EndPaint(hDlg,&ps);

			return TRUE;
		}

		break;

		case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
				case IDRESET:
					SendDlgItemMessage(hDlg,IDC_SLIDESATURATION,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)100);
					SendDlgItemMessage(hDlg,IDC_SLIDEHUE,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)0);
					SendDlgItemMessage(hDlg,IDC_SLIDEALPHA,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)255);
					SendMessage(hDlg,WM_HSCROLL,0,0);
				break;

				case IDC_PREVIEW:
				{
					PLUGIN_DATA* lpData = (PLUGIN_DATA*) GetProp(hDlg,"lpData");
					if(!lpData)return TRUE;

					lpData->inPreview = !lpData->inPreview;

					if(lpData->bmp)
					{
						delete lpData->bmp;
					}
					lpData->bmp = DeskletLoadGDIPlusImage(lpData->imgPath);
					

					lpData->saturation	= SendDlgItemMessage(hDlg,IDC_SLIDESATURATION,TBM_GETPOS,0,0);
					lpData->hue			= SendDlgItemMessage(hDlg,IDC_SLIDEHUE,TBM_GETPOS,0,0);
					lpData->alpha		= SendDlgItemMessage(hDlg,IDC_SLIDEALPHA,TBM_GETPOS,0,0);

					lpData->useColorization = IsDlgButtonChecked(hDlg,IDC_USECOLORIZATION) == BST_CHECKED;

					if(lpData->useColorization)
					{
							lpData->colorMatrix = ColorMatrixHelper::GetIdentityMatrix();
							lpData->colorMatrix = ColorMatrixHelper::SetSaturation(lpData->colorMatrix,float(lpData->saturation)/100.0f);
							lpData->colorMatrix = ColorMatrixHelper::SetHue(lpData->colorMatrix,float(lpData->hue));
							lpData->colorMatrix.m[3][3] = float(lpData->alpha) / 255.0f;

					}

					DeskletRedraw(lpData->hwndDesklet);

				}
				break;

				case IDSETIMAGE:
				{
					PLUGIN_DATA* lpData = (PLUGIN_DATA*) GetProp(hDlg,"lpData");
					if(!lpData)return TRUE;

					if(DeskletBrowseForImage(hDlg,lpData->imgPath))
					{
						if(lpData->inPreview)
						{
							delete lpData->bmp;
							lpData->bmp = DeskletLoadGDIPlusImage(lpData->imgPath);
							DeskletRedraw(lpData->hwndDesklet);
						}
					}
				}
				break;

				case IDOK:
				{
					PLUGIN_DATA* lpData = (PLUGIN_DATA*) GetProp(hDlg,"lpData");
					if(!lpData)return TRUE;

					if(lpData->bmp)
					{
						delete lpData->bmp;
						lpData->bmp = DeskletLoadGDIPlusImage(lpData->imgPath);
					}

					lpData->saturation	= SendDlgItemMessage(hDlg,IDC_SLIDESATURATION,TBM_GETPOS,0,0);
					lpData->hue			= SendDlgItemMessage(hDlg,IDC_SLIDEHUE,TBM_GETPOS,0,0);
					lpData->alpha		= SendDlgItemMessage(hDlg,IDC_SLIDEALPHA,TBM_GETPOS,0,0);

					lpData->useColorization = IsDlgButtonChecked(hDlg,IDC_USECOLORIZATION) == BST_CHECKED;

					if(lpData->useColorization)
					{
							lpData->colorMatrix = ColorMatrixHelper::GetIdentityMatrix();
							lpData->colorMatrix = ColorMatrixHelper::SetSaturation(lpData->colorMatrix,float(lpData->saturation)/100.0f);
							lpData->colorMatrix = ColorMatrixHelper::SetHue(lpData->colorMatrix,float(lpData->hue));
							lpData->colorMatrix.m[3][3] = float(lpData->alpha) / 255.0f;

					}

					DeskletRedraw(lpData->hwndDesklet);

				}

				EndDialog(hDlg,TRUE);

				break;
			}

		}
		break;

		case WM_CLOSE:
			EndDialog(hDlg,FALSE);
		break;
		case WM_DESTROY:
		{
			PLUGIN_DATA* lpData = (PLUGIN_DATA*) GetProp(hDlg,"lpData");
			if(!lpData)return TRUE;

			lpData->inPreview = false;

			RemoveProp(hDlg,"lpData");
		}
		break;
	}
	return FALSE;
}

void __stdcall OnConfigure(PLUGIN_DATA* lpData, HWND parent)
{
	// pre:  
	// post:
	DialogBoxParam(lpData->hInstanceDll,MAKEINTRESOURCE(IDD_CONFIG),parent,ConfigDlgProc,(LPARAM)lpData);
}


void __stdcall OnEvent(PLUGIN_DATA* lpData, UINT event)
{
	// pre:  
	// post:
	if(event == EVENT_ONSELECT)
	{
		if(!lpData->bmp)
		{
			lpData->bmp = DeskletLoadGDIPlusImage(lpData->imgPath);
			if(lpData->useColorization)
			{
				lpData->colorMatrix = ColorMatrixHelper::GetIdentityMatrix();
				lpData->colorMatrix = ColorMatrixHelper::SetSaturation(lpData->colorMatrix,float(lpData->saturation)/100.0f);
				lpData->colorMatrix = ColorMatrixHelper::SetHue(lpData->colorMatrix,float(lpData->hue));
				lpData->colorMatrix.m[3][3] = float(lpData->alpha) / 255.0f;
			}
		}
		DeskletRedraw(lpData->hwndDesklet);
	}
	else if(event == EVENT_ONDESELECT)
	{
		CRITICAL_SECTION cs = {0};
		InitializeCriticalSection(&cs);
		EnterCriticalSection(&cs);
		delete lpData->bmp;
		lpData->bmp = 0;
		LeaveCriticalSection(&cs);
		DeleteCriticalSection(&cs);
		DeskletRedraw(lpData->hwndDesklet);
	}
}

void __stdcall BeforeDraw(PLUGIN_DATA* lpData, HWND hwndDesklet,
	Gdiplus::Graphics* g,UINT left, UINT top, UINT width, UINT height, Gdiplus::ColorMatrix* m, Gdiplus::Matrix* matrix,bool* modified,
		bool isSelected, bool isMouseOn)
{
	if(isSelected || lpData->inPreview)
	{
		if(lpData->bmp)
		{
			if(lpData->useColorization)
			{
				ImageAttributes ia;
				ia.SetColorMatrix(&lpData->colorMatrix);
				g->DrawImage(lpData->bmp,Rect(left,top,width,height),0,0,lpData->bmp->GetWidth(),lpData->bmp->GetHeight(),UnitPixel,&ia,0,0);
			}
			else
			{
				ImageAttributes ia;
				g->DrawImage(lpData->bmp,Rect(left,top,width,height),0,0,lpData->bmp->GetWidth(),lpData->bmp->GetHeight(),UnitPixel,&ia,0,0);
			}
		}
	}
}