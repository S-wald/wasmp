#ifndef CODE_GENERATOR_H_
#define CODE_GENERATOR_H_
#include <string>
#include <fstream>

enum class node_label {
	IMPORT,
	EXPORT,
	MODULE,
	FUNC,
	PARAM,
	LOCAL,
	RESULT
};

static const std::string node_label_strings[] = {
			"import",
			"export",
			"module",
			"func",
			"param",
			"local",
			"result"
};

enum class instruction {
	i32_CONST,
	LOCAL_SET,
	LOCAL_GET,
	i32_ADD,
	i32_SUB,
	i32_MUL,
	i32_DIV,
	CALL,
	IF,
	ELSE,
	END,
	BLOCK,
	LOOP,
	BRANCH,
	BRANCH_IF,
	i32_EQZ
};

static const std::string instruction_strings[] = {
			"i32.const",
			"local.set",
			"local.get",
			"i32.add",
			"i32.sub",
			"i32.mul",
			"i32.div_u",
			"call",
			"if",
			"else",
			"end",
			"block",
			"loop",
			"br",
			"br_if",
			"i32.eqz"
};

static const std::string NEWLINE = "\n";
static const std::string INDENT_STR = "  ";

class code_generator {
public:
	code_generator(std::ofstream& output);
	void emit_begin_complex_node(const node_label label, const std::initializer_list<const std::string> attributes);
	void emit_simple_node(const node_label label, const std::initializer_list<const std::string> attributes);
	void emit_instruction(const instruction instr, const std::initializer_list<const std::string> parameters);
	void emit_end_complex_node();
	void write_to_output();
private:
	std::string m_buffer;
	std::ofstream& m_output;
	unsigned int m_indent_level{0};
};

#endif /* CODE_GENERATOR_H_ */
