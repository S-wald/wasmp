#include <iostream>
#include <fstream>

#include "scanner.h"
#include "parser.h"

void print_usage() {
	std::cerr << "usage: wasmp infilename [-o outfilename]" << std::endl;
	std::cerr << "\tread midi pascal program, check it for syntax errors, and" << std::endl;
	std::cerr << "\tconvert it to the wasm text format (wat)" << std::endl;
}

bool parse_arguments(int argc, char *argv[], std::string &input_file_path,
		std::string &output_file_path) {
	if (argc < 2 || argc > 4) {
		return false;
	}
	input_file_path = argv[1];
	if (argc > 2) {
		std::string option(argv[2]);
		if (option != "-o") {
			return false;
		}
		output_file_path = argv[3];
	} else {
		output_file_path = "out.wat";
	}
	return true;
}

int main(int argc, char *argv[]) {
	std::string input_file_path;
	std::string output_file_path;
	if (!parse_arguments(argc, argv, input_file_path, output_file_path)) {
		print_usage();
		exit(EXIT_FAILURE);
	}
	std::ifstream input_file(input_file_path);
	std::ofstream output_file(output_file_path);
	if (!input_file.is_open()) {
		std::cerr << "Could not open file \"" << input_file_path << "\"" << std::endl;
		print_usage();
		exit(EXIT_FAILURE);
	}
	if (!output_file.is_open()) {
		std::cerr << "Could not open file \"" << output_file_path << "\"" << std::endl;
		print_usage();
		exit(EXIT_FAILURE);
	}
	scanner scanner(input_file);
	code_generator generator(output_file);
	parser parser(scanner, generator);
	parser.parse();
	input_file.close();
	output_file.close();
	return 0;
}


