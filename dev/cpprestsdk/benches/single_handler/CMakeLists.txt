add_executable(_bench.cpprest.single_handler main.cpp)
target_link_libraries(_bench.cpprest.single_handler cpprest)

install(TARGETS _bench.cpprest.single_handler DESTINATION bin)
