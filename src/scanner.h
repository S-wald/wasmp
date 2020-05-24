#ifndef SCANNER_H_
#define SCANNER_H_

#include <fstream>
#include "symbol.h"

const char BLANK = ' ';
const char TAB = '\t';
const char LF = '\n';
const char CR = '\r';
const char ENDOFFILE = '\0';


class scanner {
public:
	scanner(std::ifstream& text);
	const symbol get_next_symbol();
private:
	std::ifstream& m_text;
	unsigned int m_char_line, m_char_col;
	char m_cur_char;

	void get_next_char();
};

#endif /* SCANNER_H_ */
