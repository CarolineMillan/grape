#include <iostream>
#include <fstream>
#include <string>
#include "../../Core/Source/Core/nfa.h"
#include "../../Core/Source/Core/regex_compiler.h"
#include "../../Core/Source/Core/token.h"

//throw std::runtime_error("Unhandled pattern " + pattern);

int main(int argc, char* argv[]) {
    // Flush after every std::cout / std::cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    // You can use print statements as follows for debugging, they'll be visible when running tests.
    // std::cerr << "Logs from your program will appear here" << std::endl;

    if (argc != 4) {
        std::cerr << "Expected three arguments: -E <regex> <file.txt>" << std::endl;
        return 1;
    }


	// the tutorial had me create a flag for -E
	// if you don't add any other flags then get rid of the flag
    std::string flag = argv[1];
	// this is currently the regex expression to search for
    std::string pattern = argv[2];
	// input filepath
	std::string input_file = argv[3];

    // do i really need this? I don't think I need a flag
    // what I do need is to figure out how to make this a cli tool that you can use by calling grape

    if (flag != "-E") {
        std::cerr << "Expected first argument to be '-E'" << std::endl;
        return 1;
    }

	// here it just prompts you to give an input string
	// change it so that it reads from a third argument, an input file
	// read from input file, split it into lines, then just add a loop later on to run the nfa on each line
	

    //std::string input_line;
    //std::getline(std::cin, input_line);

    try {
        std::ifstream ifs {input_file};
        if (!ifs) {
            throw std::runtime_error("couldn't open file for reading: " + input_file);
        }
        // create nfa here
        // don't need a parser and a compiler, it's a waste just have one engine to create the nfa
        RegexCompiler compiler;
        vector<Token> tokens = compiler.parse(pattern);
        NFA nfa = compiler.compile(tokens);


        // loop through the file looking for the regex
        bool found = false;
		while (ifs) {
			std::string input_line;
			std::getline(ifs, input_line);
            if (nfa.run(input_line)) {
                std::cout << input_line << std::endl;
                found = true;
            }
		}
        if (found) {
            return 1;
        } else {
            std::cout << "No matches found" << std::endl;
            return 0;
        }
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
