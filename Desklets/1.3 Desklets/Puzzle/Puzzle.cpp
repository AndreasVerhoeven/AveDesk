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

// AveControls specific includes
#include "../AveControls/aveconstants.h"
#include "../ControlsDLL/AveControls.h"
#pragma comment(lib, "../AveControls.lib")



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
public:
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

	// control related handles.
	AVEHANDLE resources;
	AVEHANDLE container;
	AVEHANDLE comboSize;
	AVEHANDLE doneButton;

	BOOL overFlippy;
	HCURSOR oldCursor;

public:
	Puzzle(HWND hwnd, HINSTANCE dll) : 
	hwnd(hwnd), dll(dll), emptyPieceIndex(0),
		width(0),height(0), pw(0), ph(0),puzzleRect(16,13,160,160),
		isInBackMode(false), flipitIsVisible(false), gridSize(4),
		overFlippy(FALSE), oldCursor(NULL)
	{

		// get the path relative to this desklet and append it
		// with the folder in this desklet that holds the images for the controls.
		WCHAR resPath[MAX_PATH] = {0};
		DeskletGetRelativeFolderW(hwnd, resPath);
		wcscat(resPath, L"\\Images");
		wcscat(resPath, L"\\controls\\");

		// create some resources which we'll use to base the container on.
		// An container holds and manages the controls we create.
		// The resources are used to tell all the controls based on this container
		// where the necessary graphics are located. The resourceshandle could
		// also perform caching if necessary.
		resources = AveCreateResourcesW(resPath);
		container = AveCreateControlContainer(hwnd, resources);

		// empty
		GetDLLPath(dll,imagePath,MAX_PATH);
		strcat(imagePath,"default.jpg");

		bgWidth  = 172;
		bgHeight = 172;

		flipitRect.left   = 151;
		flipitRect.top    = 151;
		flipitRect.right  = flipitRect.left + 13;
		flipitRect.bottom = flipitRect.top + 13;

		gridComboRect.left   = 54;
		gridComboRect.top    = 75;
		gridComboRect.right  = gridComboRect.left + 70;
		gridComboRect.bottom = gridComboRect.top  + 25;

		doneRect.left        = 104;
		doneRect.top         = 130;
		doneRect.right       = doneRect.left + 55;
		doneRect.bottom      = doneRect.top  + 25;

		gridTextRect.left    = 30;
		gridTextRect.top     = 90 + (25 - 14) / 2 - 1;
		gridTextRect.right   = gridTextRect.left + 60;
		gridTextRect.bottom  = gridTextRect.top + 14;

		strcpy(bgPath, "orange.png");
	}

	Puzzle::~Puzzle()
	{
		ClearPieces();
		// let's not forget to clean up our own mess....
		AveFreeControlContainer(container);
		AveFreeResources(resources);
	}

	void SetGridSize(UINT newSize)
	{
		if(newSize != gridSize)
		{
			gridSize = newSize;
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

	static BOOL __stdcall OnSizeSelChange(Puzzle* w, AVEHANDLE control, INT* newSel)
	{
		// reacts to changes in the searchmode combo
		w->SetGridSize((*newSel) + 2);
		return TRUE;
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

		// create the backside controls
		comboSize = AveCreateControl(container, "back_combosearchmode", AVE_DROPDOWNLIST_CONTROL, 0L, NULL);
		// initially, we are not in backmode, so hide this control
		AveLockControl(comboSize);
		AveSendMessage(comboSize, AV_HIDE, 0L, 0L);
		AveSendMessage(comboSize, AV_SET_TEXTCOLOR, 0L, (LPARAM)Color(255,255,255,255).GetValue());
		SIZE backSideSize = {bgWidth, bgHeight};
		AveSendMessage(comboSize, AV_SET_RECT, (WPARAM)&gridComboRect, (LPARAM)&backSideSize);
		AveSendMessage(comboSize, AV_DROPLIST_ADDITEM, 0L, (LPARAM)L"2x2");
		AveSendMessage(comboSize, AV_DROPLIST_ADDITEM, 0L, (LPARAM)L"3x3");
		AveSendMessage(comboSize, AV_DROPLIST_ADDITEM, 0L, (LPARAM)L"4x4");
		AveSendMessage(comboSize, AV_DROPLIST_ADDITEM, 0L, (LPARAM)L"5x5");
		AveSendMessage(comboSize, AV_DROPLIST_ADDITEM, 0L, (LPARAM)L"6x6");
		AveSendMessage(comboSize, AV_DROPLIST_SET_SEL, (WPARAM)gridSize-2, 0L);
		AveSendMessage(comboSize, AV_DROPLIST_SET_ONSELCHANGECALLBACK, (WPARAM)this, (LPARAM)OnSizeSelChange);
		AveSendMessage(comboSize, AV_UPDATE, 0L, 0L);
		AveUnlockControl(comboSize);
		AveAddControlToContainer(container, comboSize);

		SetupDoneButton();
	}

	static BOOL __stdcall OnDoneButtonPress(Puzzle* w,AVEHANDLE button, const POINT* pt, const SIZE* s, DWORD keys)
	{
		// when the done button is pressed (backside), flip to frontside again
		w->Flip();
		return TRUE;
	}
		
	void SetupDoneButton()
	{
	/*	char relativePath[MAX_PATH] = {0};
		DeskletGetRelativeFolder(GetWindow(), relativePath);

		char donePath[MAX_PATH] = {0};
		strcpy(donePath, relativePath);
		strcat(donePath, "images\\done.png");

		UINT doneLayer       = DeskletLayerAdd(GetWindow(), "back_done");
		
		DeskletLayerSetVisibility(GetWindow(), doneLayer, FALSE, FALSE);
		DeskletLayerSetImageFile(GetWindow(), doneLayer, FALSE, donePath);
		DeskletLayerSetPlacement(GetWindow(), doneLayer, FALSE, TRUE, &doneRect, bgWidth, bgHeight);
		*/

		// we now lock the container so we can do a batch update of all the controls it holds.
		AveLockContainer(container);


		// done buttons
		doneButton = AveCreateControl(container, "back_done", AVE_IMAGEBUTTON_CONTROL, 0L, NULL);
		AveLockControl(doneButton);
		AveSendMessage(doneButton, AV_HIDE, 0L, 0L);
		SIZE backSideSize = {bgWidth, bgHeight};
		AveSendMessage(doneButton, AV_SET_RECT, (WPARAM)&doneRect, (LPARAM)&backSideSize);
		AveSendMessage(doneButton, AV_SET_NORMALIMAGEFILE, (WPARAM)FALSE, (LPARAM)L"done.png");
		AveSendMessage(doneButton, AV_SET_DOWNIMAGEFILE, FALSE, (LPARAM)L"done_pressed.png");
		AveSendMessage(doneButton, AV_SET_RETRIEVEIMAGESFROMRES, (WPARAM)TRUE, 0L);
		AveSendMessage(doneButton, AV_UPDATE, 0L, 0L);
		AveSendMessage(doneButton, AV_IMAGEBUTTON_SET_CLICKHANDLER, (WPARAM)this, (LPARAM)OnDoneButtonPress);
		AveUnlockControl(doneButton);
		AveAddControlToContainer(container, doneButton);

		
		// and finally, unlock the container.
		AveUnlockContainer(container);
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
								DeskletLayerGetLayer(hwnd, "back_combosearchmode", NULL),
								DeskletLayerGetLayer(hwnd, "back_done", NULL)
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

void __stdcall OnProcessMessage(Puzzle* t, HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(!t)
		return;

	if(t->container != NULL)
	{
		// we also forward windows messages to our container.
		// this way, the inputbox can respond to keys being pressed, for example.
		if(AveForwardMessage(t->container, msg, wParam, lParam))
			return;
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

	if(!puzzle->GetIsInBackMode())
	{
		if(PtInRect(&puzzle->GetFlipitRect(), rp))
			puzzle->Flip();
		else
			OnFrontClick(puzzle, p, s);
	}

	return 0;
}

BOOL __stdcall OnLayerClick(Puzzle* t, UINT layerId, const char* layerName, const POINT* pt, const SIZE* s, DWORD keys)
{
	// when we receive a layer click, we forward this mouse click to the container,
	// which, in turns, will forward it to the right control. If the container or its controlled
	// handled the mouse clikc, TRUE has been returned. Otherwise, FALSE.
	if(AveForwardMouseClick(t->container, layerId, layerName, pt, s, keys))
		return TRUE;

	// if we click the flippy, flip to the back, otherwise
	// when the user clicked any other part of the Puzzle besides any controls,
	// go back into mini mode if necessary.
	if(strcmp(layerName, "flipit") == 0)
		t->Flip();		

	// let's know we handle this.
	return TRUE;
}

BOOL __stdcall OnLayerRightClick(Puzzle* t, UINT layerId, const char* layerName, const POINT* pt, const SIZE* s, DWORD keys)
{
	// when we receive a layer right click, we forward this mouse click to the container,
	// which, in turns, will forward it to the right control. If the container or its controlled
	// handled the mouse rightclick, TRUE has been returned. Otherwise, FALSE.
	if(AveForwardMouseRightClick(t->container, layerId, layerName, pt, s, keys))
		return TRUE;		

	// let's know we don't handle this.
	return FALSE;
}

BOOL __stdcall OnLayerMouseDown(Puzzle* t, UINT layerId, const char* layerName, const POINT* pt, const SIZE* s, DWORD keys)
{
	// we'll also forward layer mouse down notifications.
	BOOL bRet = AveForwardMouseDown(t->container, layerId, layerName, pt, s, keys);

	// if any other layer is clicked, give control back to the input box.
	//if(!bRet)
		//AveSetFocusToControl(t->container, "input");

	return bRet;
}

BOOL __stdcall OnLayerMouseWheelForward(Puzzle* t, UINT layerId, const char* layerName, const POINT* pt, const SIZE* s, DWORD keys, int numOfScrolls)
{
	// forward mouse wheel notifications.
	if(AveForwardMouseWheelForward(t->container, layerId, layerName, pt, s, keys, numOfScrolls))
		return TRUE;	

	return FALSE;
}

BOOL __stdcall OnLayerMouseWheelBackward(Puzzle* t, UINT layerId, const char* layerName, const POINT* pt, const SIZE* s, DWORD keys, int numOfScrolls)
{
	if(AveForwardMouseWheelBackward(t->container, layerId, layerName, pt, s, keys, numOfScrolls))
		return TRUE;
	return FALSE;
}

BOOL __stdcall OnLayerMouseUp(Puzzle* t, UINT layerId, const char* layerName, const POINT* pt, const SIZE* s, DWORD keys)
{
	if(AveForwardMouseUp(t->container, layerId, layerName, pt, s, keys))
		return TRUE;

	return FALSE;
}

BOOL __stdcall OnLayerMouseOn(Puzzle* t, UINT layerId, const char* layerName, const POINT* pt, const SIZE* s)
{
	if(AveForwardMouseOn(t->container, layerId, layerName, pt, s))
		return TRUE;

	if(strcmp(layerName, "flipit") == 0)
	{
		if(!t->overFlippy)
		{
			t->overFlippy = TRUE;
			HCURSOR hand = LoadCursor(NULL, IDC_HAND);
			t->oldCursor = DeskletSetCursor(t->hwnd, hand);
		}
	}

	return FALSE;
}

BOOL __stdcall OnLayerMouseMove(Puzzle* t, UINT layerId, const char* layerName, const POINT* pt, const SIZE* s)
{
	if(AveForwardMouseMove(t->container, layerId, layerName, pt, s))
		return TRUE;

	return FALSE;
}


BOOL __stdcall OnLayerMouseOut(Puzzle* t, UINT layerId, const char* layerName)
{
	if(AveForwardMouseOut(t->container, layerId, layerName))
		return TRUE;

	if(strcmp(layerName, "flipit") == 0)
	{
		if(t->overFlippy)
		{
			t->overFlippy = FALSE;
			t->oldCursor = DeskletSetCursor(t->hwnd, t->oldCursor);
		}
	}

	return FALSE;
}