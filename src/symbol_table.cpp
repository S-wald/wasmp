#include "symbol_table.h"

bool symbol_table::declare(const std::string& ident, const attribute_kind kind, const attribute_type type) {
	if (m_idents.find(ident) != std::end(m_idents)) {
		return false;
	}
	m_idents.insert(std::make_pair(ident, attribute(kind, type, m_next_address)));
	m_next_address++;
	return true;
}

const attribute symbol_table::retrieve(const std::string& ident) const {
	return m_idents.at(ident);
}

bool symbol_table::is_declared(const std::string& ident) const {
	return m_idents.find(ident) != std::end(m_idents);
}
