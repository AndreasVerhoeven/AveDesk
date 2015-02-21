//	ave::tokenizer
//	A simple tokenizer that works with c-style strings and is able to handle
//	collections of tokens
//
//	(c) Andreas Verhoeven 2004
//
//	Do not distribute, use or modify this file without my explicit permission!
//

#ifndef AVETOKENIZER_H_34
#define AVETOKENIZER_H_34

namespace ave
{
	class tokenizer
	{
		private:
			char* curChar;
			const char* delimiters;

			bool reconizeCollections;
			char collectionChar;

			char* findNextToken() const;
			char* findNextDelimiter(bool startWithCollChar=false) const;

			char* strbecpy(char* dest, char* beg, const char* end);

		public:
			// creates a tokenizer object
			tokenizer(char* str, char* delims,
				bool reconizeCollections = false, char collectionChar='"');

			// destructs a tokenizer object
			~tokenizer();

			// sets a new source string to tokenize
			void setString(char* str);

			// sets a new set of delimiters to use
			void setDelimiters(const char* delim);

			// checks if character c is a delimter
			bool isDelimiter(char c) const;

			// returns if collections are being reconized
			bool getReconizeCollection() const;

			//returns the character used to indentify tokens
			char getCollectionChar() const;

			// set the state and the character used for collections of tokens
			void setCollections(bool useCollections, char collChar = '"');

			// Gives back a newly allocated string containing the next token if available, otherwise 0 is returned
			char* nextToken();

			// true if there is a next token, otherwise false
			bool hasToken() const;
			
			// check if there is a next token and automatically forward to that one
			bool checkAndGotoNextToken();
			
			// skip the current token and go to the next token
			void skipToken();
	};

};

#endif//AVETOKENIZER_H_34