#include <iostream>
#include <fstream>
#include <string>
#include "../../Core/Source/Core/nfa.h"
#include "../../Core/Source/Core/regex_compiler.h"
#include "../../Core/Source/Core/token.h"

//throw std::runtime_error("Unhandled pattern " + pattern);

bool run_nfa(std::istream* input, NFA &nfa, bool found, string filename = "") {
    // loop through the file looking for the regex
    std::string input_line;
    while (std::getline(*input, input_line)) {
        if (nfa.run(input_line)) {
            if (filename != "") {
                std::cout << filename << ": " << input_line << std::endl;
            } else {
                std::cout << input_line << std::endl;
            }
            found = true;
        }
    }
    return found;
}

int main(int argc, char* argv[]) {
    // Flush after every std::cout / std::cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    // You can use print statements as follows for debugging, they'll be visible when running tests.
    // std::cerr << "Logs from your program will appear here" << std::endl;
    if (argc < 3) {
        std::cerr << "Expected at least three arguments: -E <regex> [file]" << std::endl;
        return 2;
    }

	// the tutorial had me create a flag for -E
	// if you don't add any other flags then get rid of the flag
    std::string flag = argv[1];
	// this is currently the regex expression to search for
    std::string pattern = argv[2];

    // do i really need this? I don't think I need a flag
    if (flag != "-E") {
        std::cerr << "Expected first argument to be '-E'" << std::endl;
        return 2;
    }

    try {
        // create nfa here
        // don't need a parser and a compiler, it's a waste just have one engine to create the nfa
        RegexCompiler compiler;
        vector<Token> tokens = compiler.parse(pattern);
        NFA nfa = compiler.compile(tokens);



        // accept both file input stream and cin input stream by making the input a pointer to an istream
        std::istream* input;
        std::ifstream ifs;

        bool found = false;
        if (argc > 3) {

            for (int i = 3; i < argc; i++) {
                // make a helper function that runs the nfa 
                // input filepath
                std::string input_file = argv[i];
                ifs.open(input_file);
                if (!ifs) {
                    throw std::runtime_error("couldn't open file for reading: " + input_file);
                    return 2;
                }
                input = &ifs;
                found = run_nfa(input, nfa, found, input_file);
                ifs.close();
                ifs.clear();
            }
        } else if (argc == 3) {
            // we have an input stirng
            input = &std::cin;
            found = run_nfa(input, nfa, found);
        }


        if (found) {
            return 0;
        } else {
            std::cout << "No matches found" << std::endl;
            return 1;
        }
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return 2;
    }
}


