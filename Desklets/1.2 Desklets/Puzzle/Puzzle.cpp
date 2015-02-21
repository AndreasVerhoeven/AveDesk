#include <windows.h>
#include <gdiplus.h>
#pragma comment(lib,"gdiplus.lib")

#include "../DeskletSDK.h"
#include "../DeskletSDK.cpp"

#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")

#ifndef __ATLBASE_H__
#include <ATLBASE.h>
#endif

#ifndef __ATLCONV_H__
#include <ATLCONV.H>
#endif

#include "resource.h"

#include <vector>
#include <algorithm>

using namespace Gdiplus;

const LEFT = 0;
const DOWN = 1;
const RIGHT= 2;
const UP   = 3;

#include "../layers/osxpopupmenu.h"

#define WM_OSXMENU_NOTIFY_GRID WM_APP + 304


Rect GetPrivateProfileRect(const char* section, const char* ini)
{
	return Rect(	GetPrivateProfileInt(section,"Left",0,ini),
					GetPrivateProfileInt(section,"Top",0,ini),
					GetPrivateProfileInt(section,"Width",0,ini),
					GetPrivateProfileInt(section,"Height",0,ini)
				);
}

template<typename T> T RemapPoint(T& t, SIZE& s, float xScale=100, float yScale=100)
{
	T nT;
	nT.x = nT.y = 0;
	
	if(s.cx == 0 || s.cy == 0)
		return nT;

	nT.x = static_cast<long>(float(t.x) / float(s.cx) * xScale);
	nT.y = static_cast<long>(float(t.y) / float(s.cy) * yScale);

	return nT;
}

UINT SetTextLayer(HWND hwndDesklet, RECT* r, UINT xScale, UINT yScale, const char* name, const char* text, const char* font, UINT fontSize, BOOL redraw,
				  const Color& c)
{
	BOOL success = FALSE;
	UINT id = DeskletLayerGetLayer(hwndDesklet, name, &success);
	if(!success)
		id = DeskletLayerAdd(hwndDesklet, name);

	int w = r->right - r->left;
	int h = r->bottom - r->top;

	Bitmap* bmp = new Bitmap(w,h);
	Graphics g(bmp);

	OLECHAR wtext[10024] = {0};
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, font , -1, wtext, sizeof(wtext));

	Gdiplus::FontFamily  fontFamily(wtext);
	SolidBrush  brush(c);

	GraphicsPath path(Gdiplus::FillModeAlternate);
	StringFormat format(0, LANG_NEUTRAL);
	format.SetAlignment(StringAlignmentNear);
	format.SetLineAlignment(StringAlignmentNear);

	RectF rect(0.0f, 0.0f, w, h);

	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, text , -1, wtext, sizeof(wtext));
	path.AddString(wtext, -1, &fontFamily, FontStyleBold, (float)fontSize, rect, &format);

	g.SetSmoothingMode(SmoothingModeAntiAlias);
	g.SetInterpolationMode(InterpolationModeHighQualityBicubic);
	g.FillPath(&brush, &path);

	DeskletLayerSetImage(hwndDesklet, id, FALSE, bmp, TRUE);
	DeskletLayerSetPlacement(hwndDesklet, id, redraw, TRUE, r, xScale, yScale);

	return id;
}

UINT SetComboboxLayer(HWND hwndDesklet, RECT* r, UINT xScale, UINT yScale, const char* name, const char* text, BOOL redraw)
{
	// load images
	char relativePath[MAX_PATH] = {0};
	DeskletGetRelativeFolder(hwndDesklet, relativePath);

	char leftPath[MAX_PATH] = {0};
	strcpy(leftPath, relativePath);
	strcat(leftPath, "images\\controls\\combo_left.png");

	char midPath[MAX_PATH] = {0};
	strcpy(midPath, relativePath);
	strcat(midPath, "images\\controls\\combo_mid.png");

	char rightPath[MAX_PATH] = {0};
	strcpy(rightPath, relativePath);
	strcat(rightPath, "images\\controls\\combo_right.png");

	Bitmap* left  = DeskletLoadGDIPlusImage(leftPath);
	Bitmap* mid   = DeskletLoadGDIPlusImage(midPath);
	Bitmap* right = DeskletLoadGDIPlusImage(rightPath);


	BOOL success = FALSE;
	UINT id = DeskletLayerGetLayer(hwndDesklet, name, &success);
	if(!success)
		id = DeskletLayerAdd(hwndDesklet, name);

	int width  = r->right - r->left;
	int height = r->bottom - r->top;
	Bitmap* bmp = new Bitmap(width, height);
	Graphics g(bmp);
	g.SetSmoothingMode(SmoothingModeAntiAlias);
	g.SetInterpolationMode(InterpolationModeHighQualityBicubic);

	Rect midRect(left->GetWidth(), 0, width - left->GetWidth() - right->GetWidth(), height);

	//g.DrawImage(left, Rect(0,0, left->GetWidth(), height), 0,0, left->GetWidth(), left->GetHeight(), UnitPixel, 0, 0, 0);
	//SolidBrush red(Color(255, 255, 0, 0));
	//g.FillRectangle(&red, Rect(0,0, left->GetWidth(), height));
	g.DrawImage(left, Rect(0,0, left->GetWidth(), height), 0,0, left->GetWidth(), left->GetHeight(), UnitPixel, 0, 0, 0);
	g.DrawImage(right, Rect(width - right->GetWidth(),0 , right->GetWidth(), height), 0,0, right->GetWidth(), right->GetHeight(), UnitPixel, 0, 0, 0);


	Bitmap mid2(mid->GetWidth(), height);
	Graphics g2(&mid2);
	g2.SetSmoothingMode(SmoothingModeAntiAlias);
	g2.SetInterpolationMode(InterpolationModeHighQualityBicubic);
	g2.DrawImage(mid, Rect(0,0, mid->GetWidth(), height), 0, 0, mid->GetWidth(), mid->GetHeight(), UnitPixel, 0, 0, 0);


	int midWidth = mid->GetWidth();
	int drawsNeeded = midRect.Width / midWidth;
	int filled = 0;
	for(int i=0; i < drawsNeeded; ++i)
	{
		g.DrawImage(mid, Rect(midRect.X + filled,0, mid2.GetWidth(), midRect.Height), 0, 0, mid2.GetWidth(), mid2.GetHeight(), UnitPixel, 0, 0, 0);
		filled += midWidth;
	}
	g.DrawImage(mid, Rect(midRect.X + filled,0, midRect.Width - filled, midRect.Height), 0, 0, midRect.Width - filled, mid2.GetHeight(), UnitPixel, 0, 0, 0);
	int l = 0;

	OLECHAR wtext[10024] = {0};

	Gdiplus::FontFamily  fontFamily(L"Lucida Sans Unicode");
	SolidBrush  black(Gdiplus::Color(255, 0, 0, 0));

	GraphicsPath path(Gdiplus::FillModeAlternate);
	StringFormat format(0, LANG_NEUTRAL);
	format.SetAlignment(StringAlignmentCenter);
	format.SetLineAlignment(StringAlignmentCenter);

	RectF rect(midRect.X, midRect.Height / 2 - 14 / 2 + 1, midRect.Width, 14);

	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, text , -1, wtext, sizeof(wtext));
	path.AddString(wtext, -1, &fontFamily, FontStyleBold, (float)12, rect, &format);

	g.SetSmoothingMode(SmoothingModeAntiAlias);
	g.SetInterpolationMode(InterpolationModeHighQualityBicubic);
	g.FillPath(&black, &path);

	DeskletLayerSetImage(hwndDesklet, id, FALSE, bmp, TRUE);
	DeskletLayerSetPlacement(hwndDesklet, id, redraw, TRUE, r, xScale, yScale);

	delete left;
	delete mid;
	delete right;

	return id;
}


class PuzzlePiece
{
private:
	UINT animstage;
	UINT animdirection;
	UINT index;
	Bitmap* bmp;

public:
	PuzzlePiece(UINT index, Bitmap* bmp) :
	index(index), bmp(bmp), animstage(0),animdirection(0)
	{
		// empty
	}

	~PuzzlePiece()
	{
		delete bmp;
		bmp = 0;
	}

	UINT GetAnimStage()const{return animstage;};
	void SetAnimStage(UINT v){animstage=v;};

	UINT GetAnimDirection()const{return animdirection;};
	void SetAnimDirection(UINT v){animdirection=v;};

	Bitmap* GetBitmap()const{return bmp;};
	UINT GetIndex()const{return index;};
	void SetIndex(UINT i){index=i;};
};

char NumberToHex(int num)
{
	char ret[10] = {0};
	itoa(num,ret,36); // MAX!
	return ret[0];
}

int HexToNumber(char c)
{
	c = toupper(c);
	if(c >= 'A' && c <= 'Z')
	{
		return (c - 'A') + 10;
	}
	else
	{
		char num[2] = {0};
		num[0] = c;
		return atoi(num);
	}
}

class Puzzle
{
private:
	HWND hwnd;
	HINSTANCE dll;

	UINT emptyPieceIndex;
	UINT width, height, pw, ph;

	std::vector<PuzzlePiece*> pieces;

	Rect puzzleRect;
	RECT flipitRect;
	RECT gridComboRect;
	RECT doneRect;
	RECT gridTextRect;

	UINT bgWidth;
	UINT bgHeight;

	char imagePath[MAX_PATH];

	bool isInBackMode;
	bool flipitIsVisible;

	char bgPath[MAX_PATH];

	UINT gridSize;

public:
	Puzzle(HWND hwnd, HINSTANCE dll) : 
	hwnd(hwnd), dll(dll), emptyPieceIndex(0),
		width(0),height(0), pw(0), ph(0),puzzleRect(16,13,160,160),
		isInBackMode(false), flipitIsVisible(false), gridSize(4)
	{
		// empty
		GetDLLPath(dll,imagePath,MAX_PATH);
		strcat(imagePath,"default.jpg");

		bgWidth  = 172;
		bgHeight = 172;

		flipitRect.left   = 157;
		flipitRect.top    = 155;
		flipitRect.right  = flipitRect.left + 10;
		flipitRect.bottom = flipitRect.top + 10;

		gridComboRect.left   = 90;
		gridComboRect.top    = 90;
		gridComboRect.right  = gridComboRect.left + 70;
		gridComboRect.bottom = gridComboRect.top  + 25;

		doneRect.left        = 90;
		doneRect.top         = 130;
		doneRect.right       = doneRect.left + 58;
		doneRect.bottom      = doneRect.top  + 23;

		gridTextRect.left    = 30;
		gridTextRect.top     = 90 + (25 - 14) / 2 - 1;
		gridTextRect.right   = gridTextRect.left + 60;
		gridTextRect.bottom  = gridTextRect.top + 14;

		strcpy(bgPath, "orange.png");
	}

	Puzzle::~Puzzle()
	{
		ClearPieces();
	}

	void SetGridSize(UINT newSize)
	{
		if(newSize != gridSize)
		{
			gridSize = newSize;
			char gridComboString[100] = {0};
			CreateGridString(gridComboString);
			SetComboboxLayer(GetWindow(), &gridComboRect, bgWidth, bgHeight, "back_gridcombo", gridComboString, isInBackMode ? TRUE : FALSE);

			ClearPieces();

			MakePuzzle(imagePath);

			UINT lid = GetLayerID();
			Bitmap* bmp = Render();
			DeskletLayerSetImage(hwnd,lid,FALSE,bmp,TRUE);

		}
	}

	void SetIsInBackMode(bool val){isInBackMode = val;};
	bool GetIsInBackMode()const{return isInBackMode;};

	void SetFlipitIsVisible(bool val){flipitIsVisible = val;};
	bool GetFlipitIsVisible()const{return flipitIsVisible;};

	UINT GetBgWidth()const{ return bgWidth;};
	UINT GetBgHeight()const{ return  bgHeight;};

	void SetupFlippingButton()
	{
		char relativePath[MAX_PATH] = {0};
		DeskletGetRelativeFolder(GetWindow(), relativePath);

		char flipitPath[MAX_PATH] = {0};
		strcpy(flipitPath, relativePath);
		strcat(flipitPath, "images\\flipit.png");

		UINT flipitLayer       = DeskletLayerAdd(GetWindow(), "flipit");
		
		DeskletLayerSetVisibility(GetWindow(), flipitLayer, FALSE, FALSE);
		DeskletLayerSetImageFile(GetWindow(), flipitLayer, FALSE, flipitPath);
		DeskletLayerSetPlacement(GetWindow(), flipitLayer, FALSE, TRUE, &flipitRect, bgWidth, bgHeight);
	}

	void SetupBackSide()
	{
		char relativePath[MAX_PATH] = {0};
		DeskletGetRelativeFolder(GetWindow(), relativePath);

		char backPath[MAX_PATH] = {0};
		strcpy(backPath, relativePath);
		strcat(backPath, "images\\back.png");

		UINT backLayer       = DeskletLayerAdd(GetWindow(), "back");
		
		DeskletLayerSetVisibility(GetWindow(), backLayer, FALSE, FALSE);
		DeskletLayerSetImageFile(GetWindow(), backLayer, FALSE, backPath);
		//DeskletLayerSetPlacement(GetWindow(), flipitLayer, FALSE, TRUE, &flipitRect, bgWidth, bgHeight);


		char gridComboString[100] = {0};
		CreateGridString(gridComboString);
		SetTextLayer(GetWindow(), &gridTextRect, bgWidth, bgHeight, "back_gridtext", "Grid Size", "Lucida Sans Unicode", 12, FALSE, Color(255,0,0,0));
		SetComboboxLayer(GetWindow(), &gridComboRect, bgWidth, bgHeight, "back_gridcombo", gridComboString, FALSE);
		DeskletLayerSetVisibility(GetWindow(), GetBackSideGridComboLayerID(), FALSE, FALSE);
		DeskletLayerSetVisibility(GetWindow(), GetBackSideGridTextLayerID(), FALSE, FALSE);

		SetupDoneButton();
	}

	void SetupDoneButton()
	{
		char relativePath[MAX_PATH] = {0};
		DeskletGetRelativeFolder(GetWindow(), relativePath);

		char donePath[MAX_PATH] = {0};
		strcpy(donePath, relativePath);
		strcat(donePath, "images\\done.png");

		UINT doneLayer       = DeskletLayerAdd(GetWindow(), "back_done");
		
		DeskletLayerSetVisibility(GetWindow(), doneLayer, FALSE, FALSE);
		DeskletLayerSetImageFile(GetWindow(), doneLayer, FALSE, donePath);
		DeskletLayerSetPlacement(GetWindow(), doneLayer, FALSE, TRUE, &doneRect, bgWidth, bgHeight);
	}

	HWND GetWindow()const{return hwnd;};
	HINSTANCE GetDLL()const{return dll;};

	void ClearPieces();
	void MakePuzzle(char* filename);

	UINT GetWidth()const{return width;};
	UINT GetHeight()const{return height;};

	RECT GetFlipitRect()const{return flipitRect;};
	RECT GetDoneRect()const{return doneRect;};
	RECT GetGridComboRect()const{return gridComboRect;};


	UINT GetFlipitLayerID()const
	{
		return DeskletLayerGetLayer(GetWindow(), "flipit", NULL);
	}

	UINT GetBackSideLayerID()const
	{
		return DeskletLayerGetLayer(GetWindow(), "back", NULL);
	}

	UINT GetBackSideDoneLayerID()const
	{
		return DeskletLayerGetLayer(GetWindow(), "back_done", NULL);
	}

	UINT GetBackSideGridComboLayerID()const
	{
		return DeskletLayerGetLayer(GetWindow(), "back_gridcombo", NULL);
	}

	UINT GetBackSideGridTextLayerID()const
	{
		return DeskletLayerGetLayer(GetWindow(), "back_gridtext", NULL);
	}

	

	void SetBackgroundLayer()
	{
		char relativePath[MAX_PATH] = {0};
		DeskletGetRelativeFolder(GetWindow(), relativePath);

		char imgPath[MAX_PATH] = {0};
		strcpy(imgPath, relativePath);
		strcat(imgPath, "images\\");	
		strcat(imgPath, bgPath);

		DeskletSetImageFile(GetWindow(), imgPath);
	}

	Bitmap* Render();

	std::vector<PuzzlePiece*>& GetPieces(){ return pieces;};

	UINT GetIndex(POINT& p)
	{
			return (p.x / (GetPuzzleRect().Width/ gridSize )) + (p.y / (GetPuzzleRect().Height/gridSize) ) * gridSize;
	};

	PuzzlePiece* GetPieceFromIndex(int index)
	{
		int numPieces = gridSize * gridSize;
		for(int i=0;i< numPieces;++i)
		{
			if(pieces[i]->GetIndex() == index)
				return pieces[i];
		}
		return NULL;
	};

	bool SolutionIsFound()
	{
		int numPieces = gridSize * gridSize;
		for(int i=0;i<numPieces;i++)
		{
			if(pieces[i]->GetIndex() != i)
				return false;
		}
		return true;
	}

	UINT GetGridSize()const { return gridSize;};



	void ReadFromIni(const char* ini, const char* group)
	{
		puzzleRect.X = GetPrivateProfileInt(group,"PRX",puzzleRect.X,ini);
		puzzleRect.Y = GetPrivateProfileInt(group,"PRY",puzzleRect.Y,ini);
		puzzleRect.Width = GetPrivateProfileInt(group,"PRW",puzzleRect.Width,ini);
		puzzleRect.Height = GetPrivateProfileInt(group,"PRH",puzzleRect.Height,ini);

		gridSize = GetPrivateProfileInt(group,"PuzzleGridSize", gridSize, ini);

		char order[37] = {0}; // max is 6 x 6 = 36 (+ zero-term)

		strcpy(order,"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");
		GetPrivateProfileString(group,"PuzzleOrder",order,order,37,ini);

		GetPrivateProfileString(group,"PuzzleImage",imagePath,imagePath,MAX_PATH,ini);
		MakePuzzle(imagePath);


		int numPieces = gridSize* gridSize;
		for(int i=0;i<numPieces;++i)
		{
			pieces[i]->SetIndex(HexToNumber(order[i]));
		}
	}

	void SaveToIni(const char* ini, const char* group)
	{
		int numPieces = gridSize * gridSize;
		char order[37] = {0}; // max is 6 x 6 = 36 (+ zero-terminator)
		for(int i=0;i<numPieces;++i)
		{
			order[i] = NumberToHex(pieces[i]->GetIndex() );
		}
		order[numPieces] = '\0';
		WritePrivateProfileString(group,"PuzzleOrder",order,ini);

		WritePrivateProfileString(group,"PuzzleImage",imagePath,ini);

		WritePrivateProfileInt(group, "PuzzleGridSize", gridSize, ini);

		WritePrivateProfileInt(group,"PRX",puzzleRect.X,ini);
		WritePrivateProfileInt(group,"PRY",puzzleRect.Y,ini);
		WritePrivateProfileInt(group,"PRW",puzzleRect.Width,ini);
		WritePrivateProfileInt(group,"PRH",puzzleRect.Height,ini);
	}

	UINT GetLayerID()
	{
		BOOL success(FALSE);
		UINT lid = DeskletLayerGetLayer(GetWindow(),"Puzzle.ShuffleArea",&success);
		if(!success)
		{
			RECT r = {0};
			r.left = puzzleRect.X;
			r.top = puzzleRect.Y;
			r.right = puzzleRect.Width + puzzleRect.X;
			r.bottom = puzzleRect.Height + puzzleRect.Y;
			lid = DeskletLayerAdd(GetWindow(),"Puzzle.ShuffleArea");
			DeskletLayerSetPlacement(GetWindow(),lid,FALSE,TRUE,&r,192,192);
		}
		return lid;
	}

	void SetImagePath(const char* path){strcpy(imagePath,path);};
	const char* GetImagePath()const{return imagePath;};

	Rect GetPuzzleRect()const{return puzzleRect;};
	void SetPuzzleRect(const Rect& r){puzzleRect=r;};

	void CreateGridString(char* str)
	{
		char num[2] = {0};
		itoa(gridSize, num, 10);
		strcpy(str, num);
		strcat(str, " x ");
		strcat(str, num);
	}

	void Flip()
	{
		FlipParams params = {0};
		params.flipToBack = !isInBackMode;
		params.useLayers = TRUE;

		HWND hwndDesklet = GetWindow();

		UINT frontLayers[] = {
								0, // front layer
								GetLayerID()
								//GetFlipitLayerID()
							};

		UINT backLayers[]  = { 
								GetBackSideLayerID(),
								GetBackSideGridTextLayerID(),
								GetBackSideGridComboLayerID(),
								GetBackSideDoneLayerID()
							};


		params.oldLayersVisible = frontLayers;
		params.newLayersVisible = backLayers;
		params.oldLayersVisibleCount = sizeof(frontLayers) / sizeof(frontLayers[0]);
		params.newLayersVisibleCount = sizeof(backLayers) / sizeof(backLayers[0]);
		if(isInBackMode)
		{
			params.oldLayersVisible = backLayers;
			params.newLayersVisible = frontLayers;

			params.oldLayersVisibleCount = sizeof(backLayers) / sizeof(backLayers[0]);
			params.newLayersVisibleCount = sizeof(frontLayers) / sizeof(frontLayers[0]);
		}

		DeskletFlip(GetWindow(), &params);

		isInBackMode = !isInBackMode;
	}
	
};

void __stdcall OnProcessMessage(Puzzle* puzzle, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(WM_OSXMENU_NOTIFY_GRID == msg)
	{
		UINT selItem = static_cast<UINT>(wParam);
		puzzle->SetGridSize(selItem + 2);
	}
}

void Puzzle::ClearPieces()
{

	for(std::vector<PuzzlePiece*>::iterator iter = pieces.begin(); iter!=pieces.end(); ++iter)
	{
		PuzzlePiece* piece = *iter;
		delete piece;
	}
	pieces.clear();
}

Bitmap* Puzzle::Render()
{
	pw = GetPuzzleRect().Width/ gridSize;
	ph = GetPuzzleRect().Height/ gridSize;
	Bitmap* bmp = new Bitmap(pw* gridSize,ph* gridSize);
	Graphics g(bmp);
	//g.Clear(Color(255,255,255,255));
	int numPieces = gridSize * gridSize;
	for(int ind = 1;ind<numPieces;ind++)
	{
		PuzzlePiece* piece = pieces[ind];
		int i = piece->GetIndex();
		if( piece->GetBitmap() != 0)
		{
			UINT animStage = piece->GetAnimStage();
			if(animStage != 0)
			{
				
				UINT dir = piece->GetAnimDirection();
				if(dir == LEFT)
				{
					g.DrawImage(piece->GetBitmap(),(int)((i%gridSize)*pw - (pw/(gridSize * 2) * animStage)),(int)((i/gridSize)*ph),pw,ph);
				}
				else if(dir == DOWN)
				{
					g.DrawImage(piece->GetBitmap(),(int)((i%gridSize)*pw),(int)((i/gridSize)*ph + (ph/(gridSize * 2) * animStage)),pw,ph);
				}
				else if(dir == RIGHT)
				{
					g.DrawImage(piece->GetBitmap(),(int)((i%gridSize)*pw+(pw/(gridSize * 2) * animStage)),(int)((i/gridSize)*ph),pw,ph);
				}
				else if(dir == UP)
				{
					g.DrawImage(piece->GetBitmap(),(int)((i%gridSize)*pw),(int)((i/gridSize)*ph - (ph/(gridSize * 2) * animStage)),pw,ph);
				}
			}
			else
			{
				g.DrawImage(piece->GetBitmap(),(int)((i%gridSize)*pw),(int)((i/gridSize)*ph),pw,ph);	
			}
		}
	}

	return bmp;
}

void Puzzle::MakePuzzle(char* filename)
{
	ClearPieces();

	Bitmap* orig = DeskletLoadGDIPlusImage(filename);

	SIZE s = {0};
	//s.cx = (puzzle->GetWidth()/4 )* 4;
	//s.cy = (puzzle->GetHeight()/4) * 4;
	//DeskletGetSize(hwnd,&s);

	s.cx = GetPuzzleRect().Width;
	s.cy = GetPuzzleRect().Height;


	Bitmap* orig2 = new Bitmap(s.cx,s.cy);
	Graphics g2(orig2);
	g2.SetSmoothingMode(SmoothingModeAntiAlias);
	g2.SetInterpolationMode(InterpolationModeHighQualityBicubic);
	g2.DrawImage(orig,0,0,s.cx,s.cy);
	delete orig;
	orig = 0;
	orig = orig2;
	

	strcpy(imagePath,filename);
	if(orig != NULL)
	{
		width = orig->GetWidth();
		height= orig->GetHeight();
		pw = width / gridSize;
		ph =  height / gridSize;
		pieces.push_back(new PuzzlePiece(0,NULL));
		int numPieces = gridSize * gridSize;
		for(int i =1; i < numPieces;++i)
		{
			Bitmap* pbmp = new Bitmap(pw,ph);
			Graphics g(pbmp);
			g.DrawImage(orig,Rect(0,0,pw,ph),(int)(i%gridSize)*pw,(int)(i/gridSize)*ph,(int)pw,(int)ph,UnitPixel);
			Pen p(Color(100,100,100,100),1);
			g.DrawLine(&p,0,0,pw,0);
			g.DrawLine(&p,ph-1,0,pw,ph-1);
			g.DrawLine(&p,0,0,0,ph);
			g.DrawLine(&p,pw-1,0,pw-1,ph);
		
			pieces.push_back(new PuzzlePiece(i,pbmp));
		}
	}
	delete orig;
}

void __stdcall OnGetInformation(char *szName, char *szAuthor, int *iVersion, char *szNotes)
{
	strcpy(szName,"Puzzle");
	strcpy(szAuthor,"Andreas Verhoeven");
	strcpy(szNotes,":)");
	*iVersion = 130;
}

Puzzle* __stdcall OnCreate(HWND hwnd, HINSTANCE dll, const char* ini, const char* group)
{
	DeskletActivateExtendedDragDrop(hwnd);

	Puzzle* puzzle = new Puzzle(hwnd,dll);

	if(!ini || !group)
	{
		SIZE s = {puzzle->GetBgWidth(), puzzle->GetBgHeight() };
		DeskletSetSize(hwnd, &s);

		char bg[MAX_PATH] = {0};
		DeskletGetRelativeFolder(hwnd,bg);
		strcat(bg,"bg.png");
		DeskletSetImageFile(hwnd,bg);

		char imgP[MAX_PATH] = {0};
		strcpy(imgP,puzzle->GetImagePath());

		puzzle->MakePuzzle(imgP);
	}
	else
	{
		puzzle->ReadFromIni(ini,group);
	}
	UINT lid = puzzle->GetLayerID();
	Bitmap* bmp = puzzle->Render();
	DeskletLayerSetImage(hwnd,lid,FALSE,bmp,TRUE );

	puzzle->SetBackgroundLayer();
	puzzle->SetupFlippingButton();
	puzzle->SetupBackSide();

	return puzzle;
}

void __stdcall OnSave(Puzzle* puzzle, const char* ini, const char* group, BOOL bUnused)
{
	puzzle->SaveToIni(ini,group);
}

void __stdcall OnDestroy(Puzzle* puzzle, HWND hwnd)
{
	delete puzzle;
	puzzle = 0;
}

void ShufflePuzzlePieces(std::vector<PuzzlePiece*>& pieces, int gridSize )
{
	int numPieces = gridSize * gridSize;
	std::vector<int>numbers;
	for(int i=0;i<numPieces;++i)
		numbers.push_back(i);

	std::random_shuffle(numbers.begin(),numbers.end());

	for(int ii=0;ii<numPieces;++ii)
	{
		//pieces[ii]->SetIndex(ii);
		pieces[ii]->SetIndex(numbers[ii]);
	}

}


int shuffle(Puzzle* puzzle,int prevMode, bool doShuffle)
{
	int gridSize = puzzle->GetGridSize();
	int missingPieceIndex = puzzle->GetPieces()[0]->GetIndex();

	std::vector<int>validMoves;

	if(missingPieceIndex % gridSize != 0 && prevMode != RIGHT)
		validMoves.push_back(LEFT);

	if(missingPieceIndex % gridSize < (gridSize - 1) && prevMode != LEFT)
		validMoves.push_back(RIGHT);


	if(missingPieceIndex> gridSize && prevMode != DOWN)
		validMoves.push_back(UP);
	
	if(missingPieceIndex<((gridSize -1) * gridSize) && prevMode != UP )
		validMoves.push_back(DOWN);

	if(doShuffle)
		std::random_shuffle(validMoves.begin(),validMoves.end());

	int swapIndex = missingPieceIndex;
	int move = validMoves[0];
	if(move == UP)swapIndex-=gridSize;
	else if(move==DOWN)swapIndex+=gridSize;
	else if(move==LEFT)swapIndex--;
	else if(move==RIGHT)swapIndex++;


	PuzzlePiece* p = puzzle->GetPieceFromIndex(swapIndex);
	p->SetIndex(missingPieceIndex);
	puzzle->GetPieces()[0]->SetIndex(swapIndex);

	return move;
}


BOOL __stdcall OnFrontClick(Puzzle* puzzle, POINT* p, SIZE* s)
{
	POINT p2 = {0};
	p2.x = (float)p->x / (float)s->cx * 192;
	p2.y = (float)p->y / (float)s->cy * 192;

	Rect pr = puzzle->GetPuzzleRect();
	p2.x -= pr.X;
	p2.y -= pr.Y;

	std::vector<PuzzlePiece*>& pieces = puzzle->GetPieces();
	UINT lid = puzzle->GetLayerID();

	if(puzzle->SolutionIsFound())
	{
		if(MessageBox(puzzle->GetWindow(),"This puzzle is \'unpuzzled\'. Do you want to shuffle it again?","PuzzleDesklet",MB_ICONINFORMATION | MB_YESNO)==IDYES)
		{
			int move = -1;
			for(int i=0; i < 331;i++)
			{
				bool doShuffle(true);
				if(i%20==1 || i%20==2 || i%20==3 || i%20==4)
					doShuffle=false;

				move = shuffle(puzzle,move, doShuffle);
				//ShufflePuzzlePieces(pieces);
				//DeskletLayerSetAlpha(puzzle->GetWindow(),lid,FALSE,(int)(   ((float)i)* 2.5 ) );
				DeskletLayerSetImage(puzzle->GetWindow(),lid,TRUE,puzzle->Render(),TRUE );
			}
		}
		DeskletLayerSetAlpha(puzzle->GetWindow(),lid,TRUE,255);
		return TRUE;
	}

	UINT clickedOn = puzzle->GetIndex(p2);
	int moveTo = -1;
	UINT dir = 5;
	PuzzlePiece* piece = puzzle->GetPieceFromIndex(clickedOn);
	if(piece == NULL || piece->GetBitmap() == 0)
	{
		return TRUE;

	}
	int gridSize = puzzle->GetGridSize();
	int emptyIndex = pieces[0]->GetIndex();

	if(clickedOn >= gridSize && pieces[0]->GetIndex() == clickedOn-gridSize)
	{
		moveTo = clickedOn - gridSize;
		dir = UP;
	}
	else if(clickedOn < ((gridSize -1) * gridSize) && pieces[0]->GetIndex() == clickedOn+gridSize)
	{
		moveTo = clickedOn + gridSize;
		dir = DOWN;
	}
	else if(clickedOn % gridSize > 0 && pieces[0]->GetIndex() == clickedOn-1)
	{
		moveTo = clickedOn - 1;
		dir = LEFT;
	}
	else if(clickedOn % gridSize < (gridSize - 1) && pieces[0]->GetIndex() == clickedOn+1)
	{
		moveTo = clickedOn + 1;
		dir = RIGHT;
	}
	if(moveTo==-1)
		return TRUE;

	piece->SetAnimDirection(dir);
	for(int i=0;i<8;i++)
	{
		piece->SetAnimStage(i);
		DeskletLayerSetImage(puzzle->GetWindow(),lid,TRUE,puzzle->Render(),TRUE );
		Sleep(5);
	}
	piece->SetAnimStage(0);
	piece->SetIndex(moveTo);
	pieces[0]->SetIndex(clickedOn);
	DeskletLayerSetImage(puzzle->GetWindow(),lid,TRUE,puzzle->Render(),TRUE );

	if(puzzle->SolutionIsFound())
	{
		BOOL success(FALSE);
		UINT id = DeskletLayerGetLayer(puzzle->GetWindow(),"Puzzle.Solution",&success);
		if(!success)
		{
			Rect puzzleRect = puzzle->GetPuzzleRect();
			RECT r = {0};
			r.left = puzzleRect.X;
			r.top = puzzleRect.Y;
			r.right = puzzleRect.Width + puzzleRect.X;
			r.bottom = puzzleRect.Height + puzzleRect.Y;
			id = DeskletLayerAdd(puzzle->GetWindow(),"Puzzle.Solution");
			DeskletLayerSetPlacement(puzzle->GetWindow(),id,FALSE,TRUE,&r,192,192);
		}
		DeskletLayerSetImageFile(puzzle->GetWindow(),id,FALSE,const_cast<char*>(puzzle->GetImagePath()));
		for(int i=0;i<25;++i)
		{
			DeskletLayerSetAlpha(puzzle->GetWindow(),id,TRUE,i*10);
		}
		DeskletLayerRemove(puzzle->GetWindow(),id,FALSE);
		DeskletLayerSetImageFile(puzzle->GetWindow(),lid,TRUE,const_cast<char*>(puzzle->GetImagePath()));
		return TRUE;
	}

	return TRUE;
}


BOOL __stdcall OnDragEnter(Puzzle* puzzle,IDataObject* obj,DWORD keys ,POINTL pnt ,DWORD* effect)
{
	return TRUE;
}

BOOL __stdcall OnDragLeave(Puzzle* puzzle)
{
	return TRUE;
}

BOOL __stdcall OnDrop(Puzzle* puzzle,IDataObject* obj,DWORD keys ,POINTL pnt ,DWORD* effect)
{
	if(IsDataObjectHDROP(obj) )
	{

		DropStruct* ds = LockHDROP(obj);
	
		int num = DragQueryFile(ds->hDrop, -1, 0, 0);
		if(num > 0)
		{
			char path[MAX_PATH] = {0};
			DragQueryFile(ds->hDrop,0,path,MAX_PATH);
			puzzle->MakePuzzle(path);
			DeskletLayerSetImage(puzzle->GetWindow(),puzzle->GetLayerID(),TRUE,puzzle->Render(),TRUE );
		}

		UnlockHDROP(ds);

	}
	return TRUE;
}

int __stdcall overlayConfigProc(HWND hDlg,UINT msg, WPARAM wParam, LPARAM lParam)
{
	static bool init;

	Puzzle* puzzle = (Puzzle*)GetProp(hDlg,"lpData");

	switch(msg)
	{
		case WM_INITDIALOG:
			init=true;
			PROPSHEETPAGE* psp;
			psp = (PROPSHEETPAGE*)lParam;
			if(!psp)return FALSE;
			SetProp(hDlg,"lpData",(HANDLE)psp->lParam);
			puzzle = (Puzzle*)GetProp(hDlg,"lpData");
			if(!puzzle)return TRUE;
		
			init = false;
			{
				Rect r = puzzle->GetPuzzleRect();
				SetDlgItemInt(hDlg,IDC_X,r.X,FALSE);
				SetDlgItemInt(hDlg,IDC_Y,r.Y,FALSE);
				SetDlgItemInt(hDlg,IDC_WIDTH,r.Width,FALSE);
				SetDlgItemInt(hDlg,IDC_HEIGHT,r.Height,FALSE);
			}
			
		break;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDC_X: case IDC_Y: case IDC_WIDTH: case IDC_HEIGHT:
					if(HIWORD(wParam)==EN_CHANGE)
						PropSheet_Changed(GetParent(hDlg),hDlg);
				break;
			}
		break;

		case WM_NOTIFY:
			NMHDR* nmhdr;
			nmhdr = (NMHDR*) lParam;
			if(!nmhdr)break;
			switch(nmhdr->code)
			{
				case PSN_APPLY:
					if(puzzle)
					{
						Rect r = puzzle->GetPuzzleRect();
						r.X = GetDlgItemInt(hDlg,IDC_X,NULL,FALSE);
						r.Y = GetDlgItemInt(hDlg,IDC_Y,NULL,FALSE);
						r.Width = GetDlgItemInt(hDlg,IDC_WIDTH,NULL,FALSE);
						r.Height = GetDlgItemInt(hDlg,IDC_HEIGHT,NULL,FALSE);

						puzzle->SetPuzzleRect(r);
						UINT lid = puzzle->GetLayerID();
						RECT r2 = {0};
						r2.left = r.X;
						r2.top = r.Y;
						r2.right = r.Width + r.X;
						r2.bottom = r.Height + r.Y;
						DeskletLayerSetPlacement(puzzle->GetWindow(),lid,FALSE,TRUE,&r2,192,192);
						std::vector<int> indeces;
						int numPieces = puzzle->GetGridSize() * puzzle->GetGridSize();
						for(int i=0;i<numPieces;++i)
						{
							indeces.push_back(puzzle->GetPieces()[i]->GetIndex());
						}
						puzzle->MakePuzzle(const_cast<char*>(puzzle->GetImagePath()));
						for(int ii=0;ii<numPieces;++ii)
						{
							puzzle->GetPieces()[ii]->SetIndex(indeces[ii]);
						}
						DeskletLayerSetImage(puzzle->GetWindow(),lid,TRUE,puzzle->Render(),TRUE );


					}
				break;
			}
		break;
	}
	return FALSE;
}

INT __stdcall OnAddPropertySheets(Puzzle* puzzle, int numPresent)
{
	PROPSHEETPAGE psp = {0};

	psp.dwSize = sizeof(psp);
	psp.dwFlags = PSP_USETITLE;
	psp.hInstance = puzzle->GetDLL();
	psp.pszTitle = "Overlay Settings";
	psp.pszTemplate = MAKEINTRESOURCE(IDD_OVERLAY);
	psp.pfnDlgProc = overlayConfigProc;
	psp.lParam = (LPARAM)puzzle;

	return DeskletAddPropertySheet(puzzle->GetWindow(),CreatePropertySheetPage(&psp));
}



BOOL __stdcall OnMouseMove(Puzzle* puzzle, int x, int y)
{
	SIZE s = {0};
	DeskletGetSize(puzzle->GetWindow(), &s);

	POINT p = {x,y};
	POINT rp = RemapPoint(p,s,(float)puzzle->GetBgWidth(), (float)puzzle->GetBgHeight() );

	if(!puzzle->GetIsInBackMode())
	{
		if(PtInRect(&puzzle->GetFlipitRect(), rp))
		{
			if(!puzzle->GetFlipitIsVisible())
				DeskletLayerSetVisibility(puzzle->GetWindow(), puzzle->GetFlipitLayerID(), TRUE, TRUE);

			puzzle->SetFlipitIsVisible(true);
		}
		else
		{
			if(puzzle->GetFlipitIsVisible())
				DeskletLayerSetVisibility(puzzle->GetWindow(), puzzle->GetFlipitLayerID(), TRUE, FALSE);

			puzzle->SetFlipitIsVisible(false);
		}
	}

	return TRUE;
}

BOOL __stdcall OnMouseOut(Puzzle* puzzle)
{
	if(puzzle->GetFlipitIsVisible())
		DeskletLayerSetVisibility(puzzle->GetWindow(), puzzle->GetFlipitLayerID(), TRUE, FALSE);

	puzzle->SetFlipitIsVisible(false);

	return TRUE;
}

BOOL __stdcall OnLeftButtonClick(Puzzle* puzzle, POINT* p, SIZE *s)
{
	POINT rp = RemapPoint(*p,*s,(float)puzzle->GetBgWidth(), (float)puzzle->GetBgHeight() );

	if(puzzle->GetIsInBackMode())
	{
		if(PtInRect(&puzzle->GetDoneRect(), rp))
			puzzle->Flip();
		else if(PtInRect(&puzzle->GetGridComboRect(), rp))
		{
			char** fonts = new char*[5];
			fonts[0] = "2 x 2";
			fonts[1] = "3 x 3";
			fonts[2] = "4 x 4";
			fonts[3] = "5 x 5";
			fonts[4] = "6 x 6";
				
				MenuData data = {0};
				data.selected = 1;
				data.count = 5;
				data.items = fonts;

				data.selected = puzzle->GetGridSize() - 2;

				char resourcesPath[MAX_PATH] = {0};
				DeskletGetRelativeFolder(puzzle->GetWindow(), resourcesPath);
				strcat(resourcesPath, "images\\controls\\menu\\");

				POINT pt = {0};
				RECT r = {0};
				GetWindowRect(puzzle->GetWindow(), &r);
				pt.x = r.left + puzzle->GetGridComboRect().left;
				pt.y = r.top  + puzzle->GetGridComboRect().top;


				DoOSXPopupMenu(puzzle->GetWindow(), WM_OSXMENU_NOTIFY_GRID, &data, resourcesPath, &pt);

				delete[] fonts;
			}

		/*
		if(!CheckBackColorsForHit(sticky, rp))
		{
			if(PtInRect(&doneRect, rp))
				FlipSticky(sticky);
			else if(PtInRect(&fontRect, rp))
				{
				char** fonts = new char*[6];
				fonts[0] = "Arial";
				fonts[1] = "Comic Sans MS";
				fonts[2] = "Hyena";
				fonts[3] = "Impact";
				fonts[4] = "Lucida Sans";
				fonts[5] = "Verdana";
				
				MenuData data = {0};
				data.selected = 1;
				data.count = 6;
				data.items = fonts;

				for(int i = 0; i < data.count; ++i)
				{
					if(stricmp(data.items[i], sticky->font) == 0)
						data.selected = i;
				}

				char resourcesPath[MAX_PATH] = {0};
				DeskletGetRelativeFolder(sticky->hwndDesklet, resourcesPath);
				strcat(resourcesPath, "images\\controls\\menu\\");

				POINT pt = {0};
				RECT r = {0};
				GetWindowRect(sticky->hwndDesklet, &r);
				pt.x = r.left + fontRect.left;
				pt.y = r.top  + fontRect.top;


				DoOSXPopupMenu(sticky->hwndDesklet, WM_OSXMENU_NOTIFY_FONT, &data, resourcesPath, &pt);

				delete[] fonts;
			}
			else if(PtInRect(&sizeRect, rp))
				{
				char** sizes = new char*[7];
				sizes[0] = "14";
				sizes[1] = "16";
				sizes[2] = "18";
				sizes[3] = "20";
				sizes[4] = "22";
				sizes[5] = "24";
				sizes[6] = "30";
				
				MenuData data = {0};
				data.selected = 1;
				data.count = 7;
				data.items = sizes;

				char curSizeAsString[10] = {0};
				itoa(sticky->fontSize, curSizeAsString, 10);

				for(int i = 0; i < data.count; ++i)
				{
					if(stricmp(data.items[i], curSizeAsString) == 0)
						data.selected = i;
				}

				char resourcesPath[MAX_PATH] = {0};
				DeskletGetRelativeFolder(sticky->hwndDesklet, resourcesPath);
				strcat(resourcesPath, "images\\controls\\menu\\");

				POINT pt = {0};
				RECT r = {0};
				GetWindowRect(sticky->hwndDesklet, &r);
				pt.x = r.left + sizeRect.left;
				pt.y = r.top  + sizeRect.top;


				DoOSXPopupMenu(sticky->hwndDesklet, WM_OSXMENU_NOTIFY_SIZE, &data, resourcesPath, &pt);

				delete[] sizes;
			}
		}
		*/
	}
	else
	{
		if(PtInRect(&puzzle->GetFlipitRect(), rp))
			puzzle->Flip();
		else
			OnFrontClick(puzzle, p, s);
	}

	return 0;
}