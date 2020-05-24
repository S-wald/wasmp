#ifndef PARSER_H_
#define PARSER_H_

#include "scanner.h"
#include "symbol_table.h"
#include "code_generator.h"
#include "symbol.h"

class parser {
public:
	parser(scanner& scanner, code_generator& generator);
	void parse();
private:
	scanner& m_scanner;
	code_generator& m_gen;
	symbol m_cur_symbol;
	symbol m_la_symbol;
	symbol_table m_st;

	void program();
	void var_decl();
	void stat_seq();
	void stat();

	void expr();
	void term();
	void fact();

	void scan();
	void check(symbol_kind expected);
	void error(std::string msg);
};

#endif /* PARSER_H_ */
