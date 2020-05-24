#include <iostream>
#include "parser.h"


parser::parser(scanner& scanner, code_generator& generator)
	: m_scanner(scanner), m_gen(generator), m_cur_symbol(), m_la_symbol(), m_st() {
}

void parser::parse() {
	scan();
	program();
	check(symbol_kind::NONE);
	m_gen.write_to_output();
}

void parser::program() {
	check(symbol_kind::PROGRAM);
	check(symbol_kind::IDENT);
	std::string program_ident = m_cur_symbol.get_val();
	check(symbol_kind::SEMICOLON);
	/* SEM */
		m_gen.emit_begin_complex_node(node_label::MODULE, {});
		/* imports */
		m_gen.emit_begin_complex_node(node_label::IMPORT, { "\"host\"", "\"write\"" });
		m_gen.emit_begin_complex_node(node_label::FUNC, { "$write" });
		m_gen.emit_simple_node(node_label::PARAM, { "i32" });
		m_gen.emit_end_complex_node();
		m_gen.emit_end_complex_node();
		m_gen.emit_begin_complex_node(node_label::IMPORT, { "\"host\"", "\"read\"" });
		m_gen.emit_begin_complex_node(node_label::FUNC, { "$read" });
		m_gen.emit_simple_node(node_label::RESULT, { "i32" });
		m_gen.emit_end_complex_node();
		m_gen.emit_end_complex_node();
		/* end imports */
		/* exports */
		m_gen.emit_begin_complex_node(node_label::EXPORT, { "\"" + program_ident + "\""});
		m_gen.emit_simple_node(node_label::FUNC, { "$" + program_ident });
		m_gen.emit_end_complex_node();
		/* end exports */
		m_gen.emit_begin_complex_node(node_label::FUNC, { "$" + program_ident });
	/* ENDSEM */
	if (m_la_symbol.get_kind() == symbol_kind::VAR) {
		var_decl();
	}
	check(symbol_kind::BEGIN);
	stat_seq();
	check(symbol_kind::END);
	check(symbol_kind::PERIOD);
	/* SEM */
		m_gen.emit_end_complex_node();
		m_gen.emit_end_complex_node();
	/* ENDSEM */
}

void parser::var_decl() {
	check(symbol_kind::VAR);
	check(symbol_kind::IDENT);
	/* SEM */
		if (!m_st.declare(m_cur_symbol.get_val(), attribute_kind::VAR, attribute_type::INTEGER)) {
			error("Identifier " + m_cur_symbol.get_val() + " already declared.");
		}
		m_gen.emit_simple_node(node_label::LOCAL, { "i32" });
	/* ENDSEM */
	while (m_la_symbol.get_kind() == symbol_kind::COMMA) {
		scan();
		check(symbol_kind::IDENT);
		/* SEM */
			if (!m_st.declare(m_cur_symbol.get_val(), attribute_kind::VAR, attribute_type::INTEGER)) {
				error("Identifier " + m_cur_symbol.get_val() + " already declared.");
			}
			m_gen.emit_simple_node(node_label::LOCAL, { "i32" });
		/* ENDSEM */
	}
	check(symbol_kind::COLON);
	check(symbol_kind::INTEGER);
	check(symbol_kind::SEMICOLON);
}

void parser::stat_seq() {
	stat();
	while (m_la_symbol.get_kind() == symbol_kind::SEMICOLON) {
		scan();
		stat();
	}
}

void parser::stat() {
	std::string dst = "";
	if (m_la_symbol.get_kind() == symbol_kind::IDENT) {
		scan();
		/* SEM */
			dst = m_cur_symbol.get_val();
			if (!m_st.is_declared(dst)) {
				error("Identifier " + m_cur_symbol.get_val() + " not declared." );
			}
		/* ENDSEM */
		check(symbol_kind::ASSIGN);
		expr();
		/* SEM */
			m_gen.emit_instruction(instruction::LOCAL_SET, { std::to_string(m_st.retrieve(dst).get_address()) });
		/* ENDSEM */
	}
	else if (m_la_symbol.get_kind() == symbol_kind::WRITE) {
		scan();
		check(symbol_kind::LEFT_PAR);
		expr();
		check(symbol_kind::RIGHT_PAR);
		/* SEM */
			m_gen.emit_instruction(instruction::CALL, { "$write" });
		/* ENDSEM */
	}
	else if (m_la_symbol.get_kind() == symbol_kind::READ) {
		scan();
		check(symbol_kind::LEFT_PAR);
		check(symbol_kind::IDENT);
		/* SEM */
			dst = m_cur_symbol.get_val();
			if (!m_st.is_declared(dst)) {
				error("Identifier " + m_cur_symbol.get_val() + " not declared." );
			}
		/* ENDSEM */
		check(symbol_kind::RIGHT_PAR);
		/* SEM */
			m_gen.emit_instruction(instruction::CALL, { "$read" });
			m_gen.emit_instruction(instruction::LOCAL_SET, { std::to_string(m_st.retrieve(dst).get_address()) });
		/* ENDSEM */
	}
	else if (m_la_symbol.get_kind() == symbol_kind::BEGIN) {
		scan();
		stat_seq();
		check(symbol_kind::END);
	}
	else if (m_la_symbol.get_kind() == symbol_kind::IF) {
		scan();
		expr();
		/* SEM */
			m_gen.emit_instruction(instruction::IF, { });
		/* ENDSEM */
		check(symbol_kind::THEN);
		stat();
		if (m_la_symbol.get_kind() == symbol_kind::ELSE) {
			scan();
			/* SEM */
				m_gen.emit_instruction(instruction::ELSE, { });
			/* ENDSEM */
			stat();
			/* SEM */
				m_gen.emit_instruction(instruction::END, { });
			/* ENDSEM */
		}
		else {
			/* SEM */
				m_gen.emit_instruction(instruction::END, { });
			/* ENDSEM */
		}
	}
	else if (m_la_symbol.get_kind() == symbol_kind::WHILE) {
		scan();
		/* SEM */
			m_gen.emit_instruction(instruction::BLOCK, { });
			m_gen.emit_instruction(instruction::LOOP, { });
		/* ENDSEM */
		expr();
		/* SEM */
			m_gen.emit_instruction(instruction::i32_EQZ, { });
			m_gen.emit_instruction(instruction::BRANCH_IF, { "1" });
		/* ENDSEM */
		check(symbol_kind::DO);
		stat();
		/* SEM */
			m_gen.emit_instruction(instruction::BRANCH, { "0" });
			m_gen.emit_instruction(instruction::END, { });
			m_gen.emit_instruction(instruction::END, { });
		/* ENDSEM */
	}
}

void parser::expr() {
	term();
	for(;;) {
		if (m_la_symbol.get_kind() == symbol_kind::PLUS) {
			scan();
			term();
			/* SEM */
				m_gen.emit_instruction(instruction::i32_ADD, {});
			/* ENDSEM */
		}
		else if (m_la_symbol.get_kind() == symbol_kind::MINUS) {
			scan();
			term();
			/* SEM */
				m_gen.emit_instruction(instruction::i32_SUB, {});
			/* ENDSEM */
		}
		else {
			break;
		}
	}
}

void parser::term() {
	fact();
	for(;;) {
		if (m_la_symbol.get_kind() == symbol_kind::MUL) {
			scan();
			fact();
			/* SEM */
				m_gen.emit_instruction(instruction::i32_MUL, {});
			/* ENDSEM */
		} else if (m_la_symbol.get_kind() == symbol_kind::DIV) {
			scan();
			fact();
			/* SEM */
				m_gen.emit_instruction(instruction::i32_DIV, {});
			/* ENDSEM */
		}
		else {
			break;
		}
	}
}

void parser::fact() {
	if (m_la_symbol.get_kind() == symbol_kind::IDENT) {
		scan();
		/* SEM */
			if (!m_st.is_declared(m_cur_symbol.get_val())) {
				error("Identifier " + m_cur_symbol.get_val() + " not declared.");
			}
			m_gen.emit_instruction(instruction::LOCAL_GET, { std::to_string(m_st.retrieve(m_cur_symbol.get_val()).get_address()) });
		/* ENDSEM */
	}
	else if (m_la_symbol.get_kind() == symbol_kind::NUMBER) {
		scan();
		/* SEM */
			m_gen.emit_instruction(instruction::i32_CONST, { m_cur_symbol.get_val() });
		/* ENDSEM */
	}
	else if (m_la_symbol.get_kind() == symbol_kind::LEFT_PAR) {
		scan();
		expr();
		check(symbol_kind::RIGHT_PAR);
	}
	else {
		error("Invalid 'fact' - Variable, integer number or arithmetic expression expected.");
	}

}

void parser::scan() {
	m_cur_symbol = m_la_symbol;
	m_la_symbol = m_scanner.get_next_symbol();
}

void parser::check(symbol_kind expected) {
	if (m_la_symbol.get_kind() == expected) {
		scan();
	}
	else {
		error("Expected " + symbol_names[static_cast<unsigned int>(expected)]);
	}
}

void parser::error(std::string msg) {
	std::cerr << "[" << m_cur_symbol.get_line() << ", " << m_cur_symbol.get_col() << "]: " << msg << std::endl;
	exit(EXIT_FAILURE);
}


