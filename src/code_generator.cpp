#include "code_generator.h"

code_generator::code_generator(std::ofstream& output) : m_output(output) {

}

void code_generator::emit_begin_complex_node(const node_label n, const std::initializer_list<const std::string> list) {
	std::string complex_node = node_label_strings[static_cast<unsigned int>(n)];
	for (auto& para : list) {
		complex_node.append(" " + para);
	}
	if (n != node_label::MODULE) {
		std::string indent = NEWLINE;
		for (unsigned int i = 0; i < m_indent_level; i++) {
			indent += INDENT_STR;
		}
		m_buffer.append(indent);
	}
	m_buffer.append("(");
	m_buffer.append(complex_node);
	m_indent_level++;
}

void code_generator::emit_end_complex_node() {
	m_buffer.append(")");
	m_indent_level--;
}

void code_generator::emit_simple_node(const node_label n, const std::initializer_list<const std::string> list) {
	std::string simple_node = node_label_strings[static_cast<unsigned int>(n)];
	for (auto& para : list) {
		simple_node.append(" " + para);
	}
	m_buffer.append(" (" + simple_node + ")");
}

void code_generator::emit_instruction(const instruction i, const std::initializer_list<const std::string> list) {
	std::string instruction = instruction_strings[static_cast<unsigned int>(i)];
	for (auto& para : list) {
		instruction.append(" " + para);
	}
	if (i == instruction::ELSE || i == instruction::END) {
		m_indent_level--;
	}
	std::string indent = NEWLINE;
	for (unsigned int i = 0; i < m_indent_level; i++) {
		indent += INDENT_STR;
	}
	m_buffer.append(indent + instruction);
	if (i == instruction::IF || i == instruction::ELSE || i == instruction::BLOCK || i == instruction::LOOP) {
		m_indent_level++;
	}
}

void code_generator::write_to_output() {
	for (auto& n : m_buffer) {
		m_output << n;
	}
}
