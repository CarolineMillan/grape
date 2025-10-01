#include <iostream>
#include <string>

#include "grape/compiler.h"
#include "grape/nfa.h"
#include "grape/parser.h"
#include "grape/token.h"

//throw std::runtime_error("Unhandled pattern " + pattern);

int main(int argc, char* argv[]) {
    // Flush after every std::cout / std::cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    // You can use print statements as follows for debugging, they'll be visible when running tests.
    // std::cerr << "Logs from your program will appear here" << std::endl;

    if (argc != 3) {
        std::cerr << "Expected two arguments" << std::endl;
        return 1;
    }

    std::string flag = argv[1];
    std::string pattern = argv[2];

    // do i really need this? I don't think I need a flag
    // what I do need is to figure out how to make this a cli tool that you can use by calling grape
    if (flag != "-E") {
        std::cerr << "Expected first argument to be '-E'" << std::endl;
        return 1;
    }

    std::string input_line;
    std::getline(std::cin, input_line);
   
    try {
        // create nfa here
        // don't need a parser and a compiler, it's a waste just have one engine to create the nfa
        Parser parser;
        Compiler compiler;
        vector<Token> tokens = parser.parse(pattern);
        NFA nfa = compiler.compile(tokens);

        if (nfa.run(input_line)) {
            std::cout << "true" << std::endl;
            return 0;
        } else {
            std::cout << "false" << std::endl;
            return 1;
        }
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
