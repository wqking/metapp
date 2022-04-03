set(TARGET_TUTORIAL tutorial)

file(GLOB_RECURSE SRC_TUTORIAL "./*.cpp")
file(GLOB_RECURSE SRC_LIB "../../src/*.cpp")

add_executable(
	${TARGET_TUTORIAL}
	${SRC_TUTORIAL}
	${SRC_LIB}
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(${TARGET_TUTORIAL} Threads::Threads)


