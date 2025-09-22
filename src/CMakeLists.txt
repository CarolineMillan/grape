add_library(grape_core
        grape/compiler.cpp
        grape/compiler.h
        grape/nfa.cpp
        grape/nfa.h
        grape/nfa_fragment.h
        grape/parser.cpp
        grape/parser.h
        grape/state.h
        grape/token.h
        grape/transition.h
)
target_include_directories(grape_core PUBLIC ${CMAKE_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(grape_cli main.cpp)
target_link_libraries(grape_cli PRIVATE grape_core)
