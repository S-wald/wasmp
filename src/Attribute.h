#ifndef ATTRIBUTE_H_
#define ATTRIBUTE_H_

#include <ostream>

enum class attribute_kind { VAR };
enum class attribute_type { INTEGER };

class attribute {
public:
	attribute(const attribute_kind kind, const attribute_type type, const unsigned int address);
	attribute_kind get_kind() const;
	attribute_type get_type() const;
	unsigned int get_address() const;
private:
	attribute_kind m_kind;
	attribute_type m_type;
	unsigned int m_address;
};

#endif /* ATTRIBUTE_H_ */
