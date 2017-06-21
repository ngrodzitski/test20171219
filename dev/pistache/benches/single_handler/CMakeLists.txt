add_executable(_bench.pistache.single_handler main.cpp)
target_link_libraries(_bench.pistache.single_handler net_static)

install(TARGETS _bench.pistache.single_handler DESTINATION bin)
