add_executable(nonrecursive main.c stack.c)
target_link_libraries(nonrecursive io err atomic)