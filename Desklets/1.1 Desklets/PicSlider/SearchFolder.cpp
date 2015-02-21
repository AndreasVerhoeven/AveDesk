#include <windows.h>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>

#include "SearchFolder.hpp"

/*std::vector<SearchFolder*> SearchFolder::Stack;
std::string SearchFolder::baseFolder;*/

std::string GetExtension(std::string fname)
{
	std::string::reverse_iterator dotPosition = std::find(fname.rbegin(),fname.rend(),'.');

	std::string ext;

	if(dotPosition != fname.rend() )
		std::reverse_copy(fname.rbegin(),dotPosition,std::insert_iterator<std::string>(ext,ext.begin()) );

	return ext;
}

void Search::PushOnStack(SearchFolder* sf)
{
	// post: sf has been pushed on the stack
	Stack.push_back(sf);
}

SearchFolder* Search::PopFromStack()
{
	// pre:  the stack holds at least one element
	// post: the last added item has been removed from the stack and has
	//		 been returned

	std::vector<SearchFolder*>::iterator iter = Stack.end() - 1;
	SearchFolder* sf = *iter;
	Stack.erase(iter);
	return sf;
}

Search::Search(const std::string& folder)
{
	// post: an Search object has been created
	SetBaseFolder(folder);
}

Search::~Search()
{
	// post: this search object has been destroyed
	while(Stack.size() > 0)
		delete PopFromStack();
}

void Search::SetBaseFolder(const std::string& folder)
{
	// post: baseFolder has been set to folder
	//		 and the Search object has been reset
	baseFolder = folder;
	Reset();
}

std::string Search::GetBaseFolder()
{
	// post: baseFolder has been returned
	return baseFolder;
}

void Search::Reset()
{
	// post: this Search object has been reset

	while(Stack.size() > 0)
		delete PopFromStack();

	PushOnStack(new SearchFolder(baseFolder,*this) );
}

std::string Search::NextFile()
{
	// post: the next file has been returned if it was there, otherwise
	//       an empty string has been returned
	//		 also, the Search object has been advanced by one
	if(Stack.size() == 0)
		PushOnStack(new SearchFolder(baseFolder,*this) );

	std::string retFile;

	int cur = Stack.size() - 1;
	while (cur >= 0)
	{
		if( !Stack[cur] -> RetrieveNextFile(retFile) )
			delete PopFromStack();
		else
			return retFile;

		cur = Stack.size() - 1;
	}

	return retFile;
}

bool Search::HasNextFile()
{
	// post: true has been returned iff the next file is valid
	return Stack.size() > 0 && Stack[Stack.size() - 1 ] ->IsCurrentFileValid();
}

/*std::string SearchFolder::GetNextFile()
{
	// post: the next file has been returned if it was there, otherwise
	//       an empty string has been returned

	if(Stack.size() == 0)
	{
		SearchFolder* sf = new SearchFolder(baseFolder);
		Stack.push_back(sf);
	}

	std::string retFile ("");

	int cur = Stack.size() - 1;
	while (cur >= 0)
	{
		if ( !Stack[cur] -> RetrieveNextFile(retFile) )
		{
			cur = Stack.size() - 1;
			delete Stack[cur];
			std::vector<SearchFolder*>::iterator iter = std::find(Stack.begin(),Stack.end(),Stack[cur]);
			if( iter != Stack.end() )
				Stack.erase(iter);
		}
		else
		{	
			return retFile;
		}
		cur = Stack.size() - 1;
	}
	return retFile;
}

void SearchFolder::SetBaseFolder(std::string folder)
{
	// post: the base folder for this Search has been set
	baseFolder = folder;
}

void SearchFolder::ReleaseAll()
{
	// post: all items on the stack has been released
	for ( std::vector<SearchFolder*>::iterator iter = Stack.begin(); iter != Stack.end(); ++iter)
	{
		delete (*iter);
	}
	Stack.clear();
}*/

SearchFolder::SearchFolder(std::string folder,Search& parentSearch) :
	currentFolder(folder),
	searchHandle(0),
	parentSearch(parentSearch)
{
	// post: a SearchFolder object has been created

	std::string s ( currentFolder + "*.*");

	searchHandle = FindFirstFile(s.c_str(),&data);
	if (!IsCurrentFileValid() )
	{
		if (!GoToNextValidFile() )
		{
			FindClose(searchHandle);
			searchHandle = 0;
		}
	}
}

SearchFolder::~SearchFolder()
{
	// post: this searchfolder object has been destructed
	if(searchHandle)FindClose(searchHandle);
}

bool SearchFolder::IsCurrentFileValid()
{
	// post: true has been returned iff the current file is a valid file
	//		 for this search
	std::string fname ( data.cFileName);
	if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	{
		return (fname != "." && fname != "..");
		}
	else
	{
		std::string ext ( GetExtension(fname) );
		return    (ext == "jpg" || ext == "jpeg" || ext=="png"   ||
				   ext =="bmp"  || ext=="gif"    || ext == "tif" ||
			       ext == "tiff");
	}
}

bool SearchFolder::GoToNextValidFile()
{
	// post: true has been returned if this object has been advanced 
	//		 to the next valid file, otherwise false has been returned
	do
	{
		if(!FindNextFile(searchHandle,&data))
			return false;

	}while(!IsCurrentFileValid());

	return true;
}

bool SearchFolder::RetrieveNextFile(std::string& nextFile)
{
	// post: true has been returned if the current file is valid and
	//		 the object advanced one file, otherwise false has been returned
	if(!searchHandle)
	{
		return false;
	}
	else
	{
		if(data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			std::string f ( currentFolder + data.cFileName + "\\" );
			SearchFolder* sf = new SearchFolder(f,parentSearch);

			parentSearch.PushOnStack(sf);

			if (!GoToNextValidFile())
			{
				FindClose(searchHandle);
				searchHandle = 0;
			}
			return sf->RetrieveNextFile(nextFile);
		}
		else
		{
			nextFile = currentFolder + data.cFileName;
			if (!GoToNextValidFile())
			{
				FindClose(searchHandle);
				searchHandle = 0;
			}
			return true;
		}
	}
}