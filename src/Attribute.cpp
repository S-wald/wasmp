#include "attribute.h"

attribute::attribute(const attribute_kind kind, const attribute_type type, const unsigned int address)
	: m_kind(kind), m_type(type), m_address(address) {
}

attribute_kind attribute::get_kind() const {
	return m_kind;
}

attribute_type attribute::get_type() const {
	return m_type;
}

unsigned int attribute::get_address() const {
	return m_address;
}
