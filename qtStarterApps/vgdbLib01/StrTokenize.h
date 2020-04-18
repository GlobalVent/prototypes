#ifndef __TOKENIZE__H__
#define __TOKENIZE__H__

#include <vector>
#include <string>

class StrTokenize {
public:	
	StrTokenize() = delete;			// no default constructor
	/**
	 * @brief StrTokenize
	 * @details tokenize a string
	 * 
	 * @param str string to tokenize
	 * @param delim delimiter to tokenize with...
	 */
	StrTokenize(std::string const &str, 
		const char delim) {
		// the owner can remove the constness...
	std::vector<std::string> *t = const_cast<std::vector<std::string>*>(&tokens);
		size_t start;
		size_t end = 0;

		while ((start = str.find_first_not_of(delim, end)) != std::string::npos)
		{
			end = str.find(delim, start);
			t->push_back(str.substr(start, end - start));
		}


	}
	;
	// public immutable variable...
	const std::vector<std::string> tokens;


};


#endif
