set(TARGET_BENCHMARK benchmark)

file(GLOB_RECURSE HEADERS "../../include/metapp/*.h")
file(GLOB_RECURSE SRC_LIB "../../src/*.cpp")

set(SRC_BENCHMARK
	${HEADERS}
	${SRC_LIB}

	testmain.cpp
	benchmark_accessible.cpp
	benchmark_callable.cpp
	benchmark_variant.cpp
)

add_executable(
	${TARGET_BENCHMARK}
	${SRC_BENCHMARK}
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(${TARGET_BENCHMARK} Threads::Threads)

