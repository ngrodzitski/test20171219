add_executable(_bench.restbed.single_handler main.cpp)
target_link_libraries(_bench.restbed.single_handler ${CMAKE_PROJECT_NAME})

install(TARGETS _bench.restbed.single_handler DESTINATION bin)
