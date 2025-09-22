add_executable(test_nfa tests.cpp)
target_link_libraries(test_nfa PRIVATE grape_core)
add_test(NAME nfa_tests COMMAND test_nfa)
