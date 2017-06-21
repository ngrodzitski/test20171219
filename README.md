[TOC]

# RESTinio benchmark

This is a benchmark test suite for
[RESTinio](https://bitbucket.org/sobjectizerteam/restinio-0.2) -- a c++ REST framework.
As a developers of RESTinio we want to understand what is the performance of
our framework and how new changes influence it and, of course it is
interesting to compare RESTinio with other frameworks.

## Abstract

Assuming you have a piece of software that does REST API
and you using one of C++ REST frameworks providing request handler for it,
so what is the cost of the framework itself?
And that is the kind of question I am trying to answer with this benchmark.

Frameworks to test:

* [Beast](https://github.com/vinniefalco/Beast);
* [Cpprestsdk](https://github.com/Microsoft/cpprestsdk);
* [Restbed](https://github.com/Corvusoft/restbed);
* [RESTinio](https://bitbucket.org/sobjectizerteam/restinio-0.2).

More frameworks may be added later.

All benches are compared to a very straight ad-hock server implementation
with [ASIO](http://think-async.com/).

## Choosing a starting point

Well, there are too many conditions that benchmarks depend on:
hardware, OS, compiler and compiler options,
so having raw numbers gives you not to much information.
So before starting with any framework bench,
lets implement a very quick server that pretends to be an http-server
and serves all request with the same response:
```
HTTP/1.1 200 OK
Connection: keep-alive
Content-Length: 12
Server: Pure asio bench server
Content-Type: text/plain; charset=utf-8

Hello world!
```

I've implemented such server using only [ASIO](http://think-async.com/).
It receives connections and starts reading data until it finds "\r\n\r\n" sequence,
treats it as an end of request and sends prepared response back and then starts
reading again. And that logic is ok for GET requests.
So the performance of that server I assume to be a pivot point for estimating
the performance of other frameworks.
See full implementation [here](./dev/pure_asio/benches/single_handler/main.cpp).

## Note on thread pool

Although pure-asio bench implementation and all framework-base implementations
(except cpprestsdk) have an option to set the number of threads that asio runs on,
for all benchmarks a single thread for asio is used.

## Benchmark tool and parameters

For performing tests a [wrk](https://github.com/wg/wrk) util was used
for generating a traffic of incoming requests.

Arguments for test were the following:
```
wrk -t N -c C -d 5 http://127.0.0.1:8080/
```
Where N - the number of threads used by wkr, and C the number of running connections.

To choose a reasonable values for N and C, I've tested a pure-asio benchmark
under (N,C) params in the following ranges:

* N in {1, 2, 3, 4 };
* C in {10, 50, 100, 200, ... 1000 }.

And the maximum performance i've got for:
```
wrk -t 2 -c 400 -d 5 http://127.0.0.1:8080/
```

So all the tests are don with that command.

## Environment

The following environment was used for benchmarks:

* CPU: 8x Intel(R) Core(TM) i7-6700K CPU @ 4.00GHz;
* Memory: 16343MB;
* Operating System: Ubuntu 16.04.2 LTS.
* Compiler: gcc version 5.4.1 20160904 (Ubuntu 5.4.1-2ubuntu1~16.04)

## RESULTS

|       bench\res      |  req/sec  | compared to pure-asio impl |
|:--------------------:|:---------:|:--------------------------:|
| pure-asio            | 517737.34 |            100%            |
| RESTinio (no timers) | 152683.06 |           29.49%           |
| Beast                | 131503.64 |           25.40%           |
| RESTinio (timers)    | 115309.33 |           22.27%           |
| Restbed              |  58223.26 |           11.25%           |
| Cpprestsdk           |  9487.37  |            1.83%           |

### Pure asio implementation

[Benchmark implementation](./dev/pure_asio/benches/single_handler/main.cpp).

Results: 517737.34 req/seq. And that is a 100% result.

### Beast implementation

[Benchmark implementation](./dev/beast/benches/single_handler/main.cpp).

Results: 131503.64 req/seq. And that is a 25.40% result.

### cpprestsdk implementation

[Benchmark implementation](./dev/cpprestsdk/benches/single_handler/main.cpp).

Results: 9487.37 req/seq. And that is a 1.83% result.

### Restbed implementation

[Benchmark implementation](./dev/restbed/benches/single_handler/main.cpp).

Results: 58223.26 req/seq. And that is a 11.25% result.

### RESTinio implementations

*With no timeout guards for io operations*

[Benchmark implementation](./dev/restinio/benches/single_handler_no_timer/main.cpp).

Results: 152683.06 req/seq. And that is a 29.49% result.

*With timeout guards for io operations*

[Benchmark implementation](./dev/restinio/benches/single_handler/main.cpp).

Results: 115309.33 req/seq. And that is a 22.27% result.

## Build

For building benchmarks cmake is used. There are separate cmake scripts
for each framework benchmark, and it is a modification of the original cmake script
that comes with the framework.

First you need to obtain sources of all frameworks and their dependencies.
Ones it is done you can use a helper script (`build_benches.sh`)
that will build all the benchmarks and put all the binaries in *_bench_run/bin* dir.

Note that for frameworks that are as shared libraries you need to
point an library path to run their benchmarks, for example the relative path to
bin directory with benchmarks executables:
```
export LD_LIBRARY_PATH=../lib
```

For sample instructions on how to build benchmarks refer to the following two section.

### Obtain and build from archive

```
wget https://bitbucket.org/sobjectizerteam/restinio-benchmark-jun2017/downloads/restinio-benchmark-jun2017-0.1.0.tar.bz2
tar xjvf restinio-benchmark-jun2017-0.1.0.tar.bz2
cd restinio-benchmark-jun2017-0.1.0/dev
./build_benches.sh
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:`pwd`/_bench_run/lib
cd _bench_run/bin
```

### Obtain and build from repository

To build benchmarks from repository, you sould have Ruby and Mxx_ru (gem) installed.

```
hg clone https://bitbucket.org/sobjectizerteam/restinio-benchmark-jun2017
cd restinio-benchmark-jun2017
mxxruexternals
cd dev
./build_benches.sh
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:`pwd`/_bench_run/lib
cd _bench_run/bin
```