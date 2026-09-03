cmake_minimum_required(VERSION 3.28)
project(EstruturasEmArvore CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-stdlib=libc++)
add_link_options(-stdlib=libc++)

add_executable(app src/main.cpp)

target_sources(app
        PUBLIC
        FILE_SET CXX_MODULES FILES
        src/arvores/trie.cppm
        src/arvores/patricia.cppm
        src/arvores/treap.cppm
        src/arvores/splay.cppm
        src/arvores/kdtree.cppm
        utils/generator.cppm
        utils/benchmark.cppm
)