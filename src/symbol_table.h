#ifndef SYMBOL_TABLE_H_
#define SYMBOL_TABLE_H_

#include <map>

#include "attribute.h"

class symbol_table {
public:
	bool declare(const std::string& ident, const attribute_kind kind, const attribute_type type);
	const attribute retrieve(const std::string& ident) const;
	bool is_declared(const std::string& ident) const;
private:
	std::map<std::string, attribute> m_idents{};
	unsigned int m_next_address{0};
};

#endif /* SYMBOL_TABLE_H_ */
