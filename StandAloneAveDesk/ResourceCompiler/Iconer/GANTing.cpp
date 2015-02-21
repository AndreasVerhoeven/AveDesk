#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <windows.h>
#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")
#include <gdiplus.h>
#pragma comment(lib,"gdiplus.lib")
using namespace Gdiplus;


#include "iconlib.h"

struct SizeAndBpp
{
	int size;
	int bpp;
};

void printHeader(void)
{
	std::cout << "AveIcon Tool" << std::endl;
	std::cout << "(c) Andreas Verhoeven 2007" << std::endl;
	std::cout << "------------------------------------------------\n" << std::endl;
}

void showHelp(void)
{
	std::cout << "Commandline options:" << std::endl;
	std::cout << "-? for help" << std::endl;
	std::cout << "-i <file>, specifies the input file that contain the PNG files to convert" << std::endl;
	std::cout << "-o <file>, specifies the output file for the new ICON files" << std::endl;
	std::cout << "-s <size,bpp>, specifies a size of the ICON files. Use multiple -s <size><bpp>bpp for multiple sizes and bits per pixels" << std::endl;
	std::cout << "-noconfirm, don't ask the user for confirmation" << std::endl;
	std::cout << "-silent, no progress output" << std::endl;

	std::cout << "\nExample: png2ico.exe -s 16 32bpp -s 32 32bpp -s 48 32bpp -i c:\\pngs\\bla.png -o c:\\icons\\bla.ico" << std::endl;
	std::cout << "\nThis will convert all PNG files in c:\\pngs\\ to icons with 16x16, 32x32 and 48x48 (all in 32bits per pixel) icons and output them to c:\\icons\\" << std::endl;
}

void retrieveArguments(int argc, char** argv, std::vector<SizeAndBpp>& sizes, std::string& outputDir, std::string& inputDir, bool& showHelp, bool& noConfirm, bool& silently)
{
	for (int i=0; i < argc; ++i)
	{
		char* c = argv[i];

		if(stricmp(c,"-noconfirm") == 0)
		{
			noConfirm = true;
		}
		if(stricmp(c,"-silent") == 0)
		{
			silently = true;
		}
		else if (i < argc -1)
		{
			if (stricmp(c,"-s") == 0 && i < argc - 2)
			{
				int size(0);
				std::stringstream ss(argv[i+1]);
				ss >> size;

				int bpp(32);

				//std::string str(argv[i+2]);
				//if(str.length() > 3)
				//	str = str.substr(0,str.length() -3);

				std::stringstream ss2(argv[i+2]);

				ss2 >> bpp;
				
				SizeAndBpp sb;
				sb.size = size;
				sb.bpp = bpp;

				if (size > 0 && (32 == bpp || 24 == bpp || 16 == bpp || 8 == bpp || 4 ==bpp))
					sizes.push_back(sb);

				i+=2;
			}
			else if (stricmp(c,"-o") == 0)
			{
				outputDir = argv[i+1];
				i++;
			}
			else if (stricmp(c,"-i") == 0)
			{
				inputDir = argv[i+1];
				i++;
			}
			else if(strcmp(c,"-?") == 0)
			{
				showHelp = true;
			}
		}		
	}

}

void showArgumentsInfo(std::vector<SizeAndBpp>& sizes, std::string& outputDir, std::string& inputDir)
{
	std::cout << "Input  Directory:\t" << inputDir << std::endl;
	std::cout << "Output Directory:\t" << outputDir << std::endl;
	std::cout << "Output sizes:\t\t";

	for (std::vector<SizeAndBpp>::iterator iter = sizes.begin(); iter != sizes.end(); ++iter)
	{
		SizeAndBpp& size = *iter;
		std::cout << size.size << "x" << size.size << " " << size.bpp << "bpp";
		if (iter != sizes.end()-1)
			std::cout << ", ";
	}

	std::cout << std::endl;
}



#include <math.h>

	BOOL addIconFromBitmap(PICONFILE iconFile,int entryNum, Bitmap* src, int size, int bits)
	{
		PixelFormat pf = PixelFormat32bppARGB;
		int lColors = 0xFFFFFFFF;
		if(24 == bits)
		{
			//pf = PixelFormat24bppRGB;
			lColors = 0xFFFFFF;
		}
		else if( 16 == bits)
		{
			//pf = PixelFormat16bppRGB565;
			lColors = 0xFFFF;
		}
		else if( 8 == bits)
		{
			//pf = PixelFormat8bppIndexed;
			lColors = 0xFF;
		}
		else if( 4 == bits)
		{
			//pf = PixelFormat4bppIndexed;
			lColors = 0xF;
		}


		Bitmap bmp(size,size,pf );
		Bitmap bmpMask(size,size);
		Graphics g(&bmp);
		Graphics g2(&bmpMask);

		//if(bits != 32)
		g.Clear(Color(0,255,255,255));

		g.SetInterpolationMode(InterpolationModeHighQualityBicubic);
        //g.SetSmoothingMode(SmoothingModeAntialias);
		g.DrawImage(src,0,0,size,size);

		g2.SetInterpolationMode(InterpolationModeHighQualityBicubic);
        //g.SetSmoothingMode(SmoothingModeAntialias);
		g2.DrawImage(src,0,0,size,size);

		HBITMAP hColor = {0};
		HBITMAP hMask = {0};

		//int oldbits = bits;
		//if(bits == 24)
		//	bits = 32;

		//if(bits == 32)
		{
			bmp.GetHBITMAP(0,&hColor);
			if(hColor == NULL)
				return FALSE;

			hMask = CreateBitmap(size,size,1,1,NULL);
			if(!hMask)
			{	
				DeleteObject((HGDIOBJ)hColor);
				return FALSE;
			}

			HDC dc = CreateCompatibleDC(NULL);
			if(!dc)
			{	
				DeleteObject((HGDIOBJ)hColor);
				DeleteObject((HGDIOBJ)hMask);
				return FALSE;
			}

			HBITMAP oldHBMP = (HBITMAP) SelectObject(dc,hMask);
			for(int x=0;x<size;++x)
			{
				for(int y=0;y<size;++y)
				{
					Color c;
					bmpMask.GetPixel(x,y,&c);
					if(32 == bits)
					{
					
						if(c.GetAlpha() == 0)
							SetPixel(dc,x,y,RGB(255,255,255));
						else
							SetPixel(dc,x,y,RGB(0,0,0));	
					}
					else
					{
						
						if(c.GetAlpha() < 127)
							SetPixel(dc,x,y,RGB(255,255,255));
						else
							SetPixel(dc,x,y,RGB(0,0,0));
					}
				}
			}

			SelectObject(dc,oldHBMP);
			DeleteDC(dc);
			
		}


		CREATEICONENTRY entry = {0};
		entry.entryNum = entryNum;
		entry.hColor = hColor;
		entry.hMask = hMask;
		if(lColors <= 0xFFFFFFFF)
			lColors += 1;
		entry.lColors = lColors;

		BOOL res = WriteIconEntry(iconFile,&entry);

		DeleteObject((HGDIOBJ)hColor);
		DeleteObject((HGDIOBJ)hMask);

		return res;

	}

void convertIcons(std::string inputDir, std::string outputDir, std::vector<SizeAndBpp>& sizes, bool silently)
{
	WCHAR wFile[MAX_PATH] = {0};
	MultiByteToWideChar(GetACP(),0,inputDir.c_str(),-1,wFile,MAX_PATH);
	Bitmap bmp(wFile);

	bool error = bmp.GetLastStatus() !=  Ok;

	if(!error)
	{
		ICONFILE iconFile = {0};
		strcpy(iconFile.filename,outputDir.c_str());
		char* ext = PathFindExtension(iconFile.filename);
		iconFile.numberOfEntries = sizes.size();

		if(OpenIconFile(&iconFile))
		{
			bool error = false;
			int entryNum(0);
			for(std::vector<SizeAndBpp>::iterator sizes_iter = sizes.begin(); sizes_iter != sizes.end() && !error; ++sizes_iter)
			{
				SizeAndBpp& size = *sizes_iter;
				if(!addIconFromBitmap(&iconFile,entryNum,&bmp,size.size,size.bpp))
					error = true;
				entryNum++;
			}

			if(!CloseIconFile(&iconFile))
				error = true;

		}
		else
		{
			error = true;
		}
	}


	if(!silently)
	{
		if(error)
			std::cout <<"[ERROR]";
		else
			std::cout <<"[OK]";
	
		std::cout << std::endl;
	}
}

bool confirm()
{
	std::cout << "Continue [yes/no]? ";

	std::string answer;
	std::cin >> answer;

	return stricmp(answer.c_str(),"ja") == 0 || stricmp(answer.c_str(),"yes") == 0 || stricmp(answer.c_str(),"y") == 0 || stricmp(answer.c_str(),"true") == 0 || stricmp(answer.c_str(),"ok") == 0 || answer == "1";
}

int main(int argc, char** argv)
{
	// start up GDI+
	GdiplusStartupInput g_gdiplusStartupInput;
	ULONG_PTR g_gdiplusToken;
	GdiplusStartup(&g_gdiplusToken, &g_gdiplusStartupInput, NULL);


	std::vector<SizeAndBpp> sizes;
	std::string outputDir;
	std::string inputDir;
	bool shouldShowHelp(false);
	bool shouldNotConfirm(false);
	bool silently(false);

	retrieveArguments(argc,argv,sizes,outputDir, inputDir,shouldShowHelp,shouldNotConfirm, silently);
	if(!silently)
		printHeader();

	if (sizes.size() > 0 && !outputDir.empty() && !shouldShowHelp)
	{
		if(!silently)
		{
			showArgumentsInfo(sizes,outputDir,inputDir);
			std::cout <<"\nConverting PNGS to ICOs:" << std::endl;
		}

		if(shouldNotConfirm || confirm())
		{
			convertIcons(inputDir,outputDir,sizes, silently);

			if(!silently)
				std::cout << "\nDONE! The PNGs are converted to ICOns." << std::endl;
		}
	}
	else
	{
		showHelp();
	}

	// shut down GDI+
	GdiplusShutdown(g_gdiplusToken);

	return 0;
}