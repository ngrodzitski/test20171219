#!/bin/bash
echo -- Beast --
cd beast
mkdir cmake_build
cd cmake_build
cmake -DCMAKE_INSTALL_PREFIX=../../_bench_run -DCMAKE_BUILD_TYPE=Release ..
make && make install
cd ../../

echo -- cpprestsdk --
cd cpprestsdk
mkdir cmake_build
cd cmake_build
cmake -DCMAKE_INSTALL_PREFIX=../../_bench_run -DCMAKE_BUILD_TYPE=Release ..
make && make install
cd ../../

echo -- restbed --
cd restbed
mkdir cmake_build
cd cmake_build
cmake -DCMAKE_INSTALL_PREFIX=../../_bench_run -DCMAKE_BUILD_TYPE=Release ..
make && make install
cd ../../

echo -- pistache --
cd pistache
mkdir cmake_build
cd cmake_build
cmake -DCMAKE_INSTALL_PREFIX=../../_bench_run -DCMAKE_BUILD_TYPE=Release ..
make && make install
cd ../../

echo -- restinio --
cd restinio
mkdir cmake_build
cd cmake_build
cmake -DCMAKE_INSTALL_PREFIX=../../_bench_run -DCMAKE_BUILD_TYPE=Release ..
make && make install
cd ../../

echo -- pure_asio --
cd pure_asio
mkdir cmake_build
cd cmake_build
cmake -DCMAKE_INSTALL_PREFIX=../../_bench_run -DCMAKE_BUILD_TYPE=Release ..
make && make install
cd ../../

