#include "symbol.h"

symbol::symbol() : m_kind(symbol_kind::NONE), m_line(0), m_col(0), m_val("") {

}

symbol::symbol(const symbol_kind type, const std::string& str, const unsigned int line, const unsigned int col)
	: m_kind(type), m_line(line), m_col(col), m_val(str) {
}

symbol_kind symbol::get_kind() const {
	return m_kind;
}

unsigned int symbol::get_line() const {
	return m_line;
}

unsigned int symbol::get_col() const {
	return m_col;
}

const std::string& symbol::get_val() const {
	return m_val;
}
