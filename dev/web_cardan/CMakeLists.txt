project( web_cardan )
cmake_minimum_required( VERSION 3.1 )

SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory( ext/alterstack )

add_subdirectory( benches )
