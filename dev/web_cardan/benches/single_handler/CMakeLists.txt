project( sample_web_server )
cmake_minimum_required( VERSION 3.1 )

# web server
add_executable( _bench.web_cardan.single_handler main.cpp )
target_link_libraries( _bench.web_cardan.single_handler alterstack )

install(TARGETS _bench.web_cardan.single_handler DESTINATION bin)
