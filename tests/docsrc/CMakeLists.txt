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
	../thirdparty/include
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(${TARGET_DOCSRC} Threads::Threads)

set_target_properties(${TARGET_DOCSRC} PROPERTIES CXX_STANDARD 20)

if(MSVC)
	target_link_options(${TARGET_DOCSRC} PRIVATE "/INCREMENTAL")
endif()

add_test(NAME ${TARGET_DOCSRC} COMMAND ${TARGET_DOCSRC})
