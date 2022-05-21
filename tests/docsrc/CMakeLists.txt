set(TARGET_DOCSRC docsrc)

file(GLOB_RECURSE SRC_DOCSRC "./*.cpp")
file(GLOB_RECURSE SRC_LIB "../../src/*.cpp")

add_executable(
	${TARGET_DOCSRC}
	${SRC_DOCSRC}
	${SRC_LIB}
)

target_include_directories(
	${TARGET_DOCSRC}
	PUBLIC
	./
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(${TARGET_DOCSRC} Threads::Threads)

if(MSVC)
	target_link_options(${TARGET_DOCSRC} PRIVATE "/INCREMENTAL")
endif()

