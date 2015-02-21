#pragma once

#include <windows.h>
#include <string>
#include <vector>
#include <algorithm>

class Search;

class SearchFolder
{
	private:
		friend class Search;

		Search& parentSearch;

		/*static std::vector<SearchFolder*>Stack;
		static std::string baseFolder;*/

		SearchFolder(std::string folder,Search& parentSearch);
		~SearchFolder();

		bool RetrieveNextFile(std::string& nextFile);
		bool GoToNextValidFile();

		bool IsCurrentFileValid();
		
		std::string currentFolder;
		HANDLE searchHandle;
		WIN32_FIND_DATA data;

	public:

		/*static void SetBaseFolder(std::string folder);
		static std::string GetNextFile();

		static void ReleaseAll();*/
};

class Search
{
	private:
		friend class SearchFolder;

		std::vector<SearchFolder*>Stack;
		std::string baseFolder;

		void PushOnStack(SearchFolder* sf);
		SearchFolder* PopFromStack();

	public:
		Search(const std::string& folder);
		~Search();

		void SetBaseFolder(const std::string& folder);
		std::string GetBaseFolder();

		void Reset();

		std::string NextFile();
		bool HasNextFile();
};