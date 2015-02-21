//	ave::tokenizer
//	A simple tokenizer that works with c-style strings and is able to handle
//	collections of tokens
//
//	(c) Andreas Verhoeven 2004
//
//	Do not distribute, use or modify this file without my explicit permission!
//

//namespace ave
//{
	#include "avetokenizer.h"

	char* ave::tokenizer::findNextToken() const
	{
		// pre:  
		// post: the place of the start of the next token has been returned

		char* _retstr = curChar;
		while(*_retstr != 0 && isDelimiter(*_retstr))_retstr++;
		return _retstr;
	}
	
	char* ave::tokenizer::findNextDelimiter(bool startWithCollChar) const
	{
		// pre:  
		// post: the place where the first delimiter was found has been returned
		//		 note that when startWithColChar is true, only collectionChar is seen as 
		//		 as delimiter

		char* _retstr = curChar;
		while(*_retstr != 0 && (startWithCollChar ? *_retstr != collectionChar : !isDelimiter(*_retstr)) ) _retstr++;
		return _retstr; 
	}

	ave::tokenizer::tokenizer(char* str, char* delims,
		bool reconizeCollections, char collectionChar) :

	curChar(str) ,
	delimiters(delims) ,
	reconizeCollections(reconizeCollections),
	collectionChar(collectionChar)

	{
		// pre:	 str is not 0
		// post: A tokenizer-object has been created
	}
	
	ave::tokenizer::~tokenizer()
	{
		// post: this tokenizer-object has been destroyed
	}

	void ave::tokenizer::setString(char* str)
	{
		// post: curChar has been set to str and therefor str will be used
		//		 as the source to tokenize

		// note: if str == 0, hasToken() always returns false and nextToken() always
		//		 returns 0

		curChar = str;
	}

	void ave::tokenizer::setDelimiters(const char* delim)
	{
		// post: delimiters has been set to delim and delim will therefore
		//		 be used as the set of delimiters

		// note: if delim == 0, there are no delimiters used

		delimiters = delim;
	}

	bool ave::tokenizer::isDelimiter(char c) const
	{
		// pre:  delimiters is not 0
		// post: true has been returned if c is a delimiter, otherwise
		//		 false has been returned

		if(!delimiters)return false;

		int i=0;
		while(delimiters[i] != 0 && delimiters[i] != c)i++;
		return delimiters[i] == c;		
	}

	bool ave::tokenizer::getReconizeCollection() const
	{
		// post: reconizeCollections has been returned

		return reconizeCollections;
	}
	char ave::tokenizer::getCollectionChar() const
	{
		// post: collectionChar has been returned

		return collectionChar;
	}
	void ave::tokenizer::setCollections(bool useCollections, char collChar)
	{
		// post: reconizeCollections has been set to useCollections and
		//		 if reconizeCollections == true, collectionChar has been 
		//		 set to collChar

		reconizeCollections = useCollections;
		if(reconizeCollections) collectionChar = collChar;
	}

	char* ave::tokenizer::strbecpy(char* dest, char* beg, const char* end)
	{
		// pre:  dest, beg and end are not 0
		// post: all characters from beg to end has been copied into dest
	
		// note: there is no 0-terminator added

		if(!dest || !beg || !end)return 0;

		char* _str = beg;
		char* _desstr = dest;

		while(_str != end)*_desstr++ = *_str++;
		return dest;
	}

	char* ave::tokenizer::nextToken()
	{
		// pre:  curChar is not 0
		// post: the next token has been returned as a newly created
		//		 c-style string, otherwise 0 has been returned

		if(!curChar)return false;
	
		// get the start of the first token
		char* tokenStart	= curChar = findNextToken();

		// determine if this token is the beginning of a collection, if
		// reconizeCollections is used
		bool isCollection = reconizeCollections && tokenStart!=0 && *tokenStart==collectionChar;

		// if so, we advance tokenStart by one, because we do not want to include
		// the collectionChar in the token
		if(isCollection)tokenStart=++curChar;

		// find the end of the token
		char* tokenEnd		= curChar = findNextDelimiter(isCollection);
		
		// if we are using a collection, we advance curChar one, so we
		// skip the closing collectionChar. If we don't do this, this will be seen
		// as the start of the next token. Also note that we only do this, if 
		// curChar is not 0, otherwise we will go beyond the end of the string
		// that we are tokenizing
		if(isCollection && curChar!=0)curChar++;

		// get the length of the token
		int tokenLength = tokenEnd - tokenStart;

		if(tokenLength < 1)
		{
			// we do not see tokens that have no length as a ""-string
			// so we return 0 instead. Also, we return 0 if there are no
			// tokens left
			return 0;
		}
		else
		{
			// create a new c-style string that contains
			// the found token and return it
			char* token = new char[tokenLength + 1];
			strbecpy(token,tokenStart,tokenEnd);
			token[tokenLength] = 0;
			return token;
		}

	}

	bool ave::tokenizer::hasToken() const
	{
		// pre:  curChar is not 0
		// post: true has been returned if there is a token in the string
		//		 otherwise, false has been returned

		if(!curChar)return false;

		return *findNextToken() != 0;
	}


	bool ave::tokenizer::checkAndGotoNextToken()
	{
		// pre: curChar is not 0
		// post: true has been returned if there is a next token
		//		 and curChar points to the start of this token,
		//		 otherwise false has been returned

		// note: this can be used for operations that do not
		//		 change the set of delimiters when tokenizing, because
		//		 this method should be faster than hasToken() and nextToken()
		//		 ,because that way, the next token will be searched for twice.
		//		 This method will automatically let curChar point to the next token
		//		 that is found, so nextToken() won't have to perform this again.

		if(!curChar)return false;

		curChar = findNextToken();
		return *curChar != 0;
	}

	void ave::tokenizer:skipToken()
	{
		// pre: curChar is not 0
		// post: the current token is skipped and we head to the next token 

		if(!curChar)return;
 
		curChar = findNextDelimiter(reconizeCollections && curChar!=0 && *curChar==collectionChar);
		curChar = findNextToken();
	}
//};
