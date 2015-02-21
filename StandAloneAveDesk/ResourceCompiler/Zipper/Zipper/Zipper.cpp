// Zipper.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ZipUnzip\Zipper.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#pragma comment(lib, "ZipUnzip/zlib/zlibstat.lib")

std::vector<std::string> readlines(std::string fname)
{
	std::vector<std::string> lines;

	std::fstream file;
	file.open(fname.c_str());
	std::string line;
	while(std::getline(file, line))
		lines.push_back(line);

	file.close();

	return lines;
}

int _tmain(int argc, _TCHAR* argv[])
{
	printf("zipper tool for AveTool\n");
	printf("=======================\n");
	if(argc > 2)
	{
		TCHAR* inputFile = argv[1];
		TCHAR* outputFile = argv[2];


		std::vector<std::string> lines = readlines(inputFile);
		if(lines.size() > 0)
		{

			char lowercaseBasePath[MAX_PATH] = {0};
			for(size_t x = 0; x < lines[0].length(); ++x)
				lowercaseBasePath[x] = tolower(lines[0][x]);

			CZipper zip;
			zip.OpenZip(outputFile, lines[0].c_str());
			for(size_t i = 1; i < lines.size(); ++i)
			{
				char path[MAX_PATH] = {0};
				PathCompactPathEx(path, lines[i].c_str(), 69, 0);
				printf("processing %s\n", path);
				if(PathIsDirectory(lines[i].c_str()))
					zip.AddFolderToZip(lines[i].c_str(), false, false);
				else
				{
					char lowercasePath[MAX_PATH] = {0};
					for(size_t x = 0; x < lines[i].length(); ++x)
							lowercasePath[x] = tolower(lines[i][x]);
					
					bool ignoreFilePath = strstr(lowercasePath, lowercaseBasePath) == NULL;
					if(lowercasePath[0] != '?')
						zip.AddFileToZip(lines[i].c_str(), ignoreFilePath);
					else
						zip.AddFileToZip(lowercasePath+1, true);
				}
			}
		}
	}
	else
	{
		printf("wrong arguments\n");
	}

	return 0;
}

