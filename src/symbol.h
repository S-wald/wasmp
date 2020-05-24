#ifndef SYMBOL_H_
#define SYMBOL_H_

#include <string>

enum class symbol_kind { NONE,
					BEGIN, END,
					INTEGER,
					PROGRAM, VAR,
					PLUS, MINUS, MUL, DIV,
					LEFT_PAR, RIGHT_PAR,
					COMMA, COLON, ASSIGN, SEMICOLON, PERIOD,
					NUMBER, IDENT, WRITE, READ, IF, THEN, ELSE, WHILE, DO};

static std::string symbol_names[] = { "NONE", "BEGIN", "END", "INTEGER", "PROGRAM", "VAR", "PLUS", "MINUS", "MUL", "DIV",
									"LEFT_PAR", "RIGHT_PAR", "COMMA", "COLON", "ASSIGN", "SEMICOLON", "PERIOD", "NUMBER", "IDENT", "WRITE",
									"READ", "IF", "THEN", "ELSE", "WHILE", "DO"};

class symbol {
public:
	symbol();
	symbol(const symbol_kind type, const std::string& val, const unsigned int line, const unsigned int col);
	symbol_kind get_kind() const;
	unsigned int get_line() const;
	unsigned int get_col() const;
	const std::string& get_val() const;
private:
	symbol_kind m_kind;
	unsigned int m_line, m_col;
	std::string m_val;
};

#endif /* SYMBOL_H_ */
