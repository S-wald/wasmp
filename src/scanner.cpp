#include "scanner.h"

scanner::scanner(std::ifstream& text) : m_text(text), m_char_line(1), m_char_col(0), m_cur_char(ENDOFFILE) {
	get_next_char();
}

void scanner::get_next_char() {
	m_cur_char = m_text.get();
	if (m_cur_char == CR || m_cur_char == LF) {
		m_char_line++;
		m_cur_char = BLANK;
		m_char_col = 0;
	}
	else {
		m_char_col++;
	}
}

const symbol scanner::get_next_symbol() {
	while (m_cur_char == BLANK || m_cur_char == TAB) {
		get_next_char();
	}

	int symbol_line = m_char_line;
	int symbol_col = m_char_col;

	if (m_cur_char == '+') {
		get_next_char();
		return symbol(symbol_kind::PLUS, "+", symbol_line, symbol_col);
	}

	if (m_cur_char == '-') {
		get_next_char();
		return symbol(symbol_kind::MINUS, "-", symbol_line, symbol_col);
	}

	if (m_cur_char == '*') {
		get_next_char();
		return symbol(symbol_kind::MUL, "*", symbol_line, symbol_col);
	}

	if (m_cur_char == '/') {
		get_next_char();
		return symbol(symbol_kind::DIV, "/", symbol_line, symbol_col);
	}

	if (m_cur_char == '(') {
		get_next_char();
		return symbol(symbol_kind::LEFT_PAR, "(", symbol_line, symbol_col);
	}

	if (m_cur_char == ')') {
		get_next_char();
		return symbol(symbol_kind::RIGHT_PAR, ")", symbol_line, symbol_col);
	}

	if (m_cur_char == '.') {
		get_next_char();
		return symbol(symbol_kind::PERIOD, ".", symbol_line, symbol_col);
	}

	if (m_cur_char == ',') {
		get_next_char();
		return symbol(symbol_kind::COMMA, ",", symbol_line, symbol_col);
	}

	if (m_cur_char == ';') {
		get_next_char();
		return symbol(symbol_kind::SEMICOLON, ";", symbol_line, symbol_col);
	}

	if (m_cur_char == ':') {
		get_next_char();
		if (m_cur_char == '=') {
			get_next_char();
			return symbol(symbol_kind::ASSIGN, ":=", symbol_line, symbol_col);
		}
		return symbol(symbol_kind::COLON, ":", symbol_line, symbol_col);
	}

	if (std::isalpha(m_cur_char) || m_cur_char == '_') {
		std::string cur_ident_name = "";
		while (std::isalnum(m_cur_char) || m_cur_char == '_') {
			cur_ident_name.append(1, toupper(m_cur_char));
			get_next_char();
		}
		if (cur_ident_name == "BEGIN") {
			return symbol(symbol_kind::BEGIN, cur_ident_name, symbol_line, symbol_col);
		}
		if (cur_ident_name == "END") {
			return symbol(symbol_kind::END, cur_ident_name, symbol_line, symbol_col);
		}
		if (cur_ident_name == "INTEGER") {
			return symbol(symbol_kind::INTEGER, cur_ident_name, symbol_line, symbol_col);
		}
		if (cur_ident_name == "PROGRAM") {
			return symbol(symbol_kind::PROGRAM, cur_ident_name, symbol_line, symbol_col);
		}
		if (cur_ident_name == "VAR") {
			return symbol(symbol_kind::VAR, cur_ident_name, symbol_line, symbol_col);
		}
		if (cur_ident_name == "WRITE") {
			return symbol(symbol_kind::WRITE, cur_ident_name, symbol_line, symbol_col);
		}
		if (cur_ident_name == "READ") {
			return symbol(symbol_kind::READ, cur_ident_name, symbol_line, symbol_col);
		}
		if (cur_ident_name == "IF") {
			return symbol(symbol_kind::IF, cur_ident_name, symbol_line, symbol_col);
		}
		if (cur_ident_name == "THEN") {
			return symbol(symbol_kind::THEN, cur_ident_name, symbol_line, symbol_col);
		}
		if (cur_ident_name == "ELSE") {
			return symbol(symbol_kind::ELSE, cur_ident_name, symbol_line, symbol_col);
		}
		if (cur_ident_name == "WHILE") {
			return symbol(symbol_kind::WHILE, cur_ident_name, symbol_line, symbol_col);
		}
		if (cur_ident_name == "DO") {
			return symbol(symbol_kind::DO, cur_ident_name, symbol_line, symbol_col);
		}
		return symbol(symbol_kind::IDENT, cur_ident_name, symbol_line, symbol_col);
	}

	if (std::isdigit(m_cur_char)) {
		std::string cur_number = "";
		while (isdigit(m_cur_char)) {
			cur_number.append(1, m_cur_char);
			get_next_char();
		}
		return symbol(symbol_kind::NUMBER, cur_number, symbol_line, symbol_col);
	}

	return symbol(symbol_kind::NONE, "\0", symbol_line, symbol_col);
}
