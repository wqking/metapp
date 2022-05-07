set(TARGET_TEST dlibtest)

file(GLOB_RECURSE SRC_TEST "./*.cpp")
file(GLOB_RECURSE SRC_LIB "../../../src/*.cpp")

add_executable(
	${TARGET_TEST}
	${SRC_TEST}
	${SRC_LIB}
)

target_include_directories(
	${TARGET_TEST}
	PUBLIC
	./
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(${TARGET_TEST} Threads::Threads)

set_target_properties(${TARGET_TEST} PROPERTIES CXX_STANDARD 11)

