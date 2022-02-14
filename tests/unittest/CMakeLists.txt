set(TARGET_TEST unittest)

set(SRC_TEST
	testmain.cpp
	test.cpp
	test_basic.cpp
	test_typekind.cpp
	test_usertype.cpp
	test_metatype.cpp
	test_variant_cast.cpp
	test_metatyperepo.cpp
)

file(GLOB_RECURSE HEADERS "../../include/metapp/*.h")

add_executable(
	${TARGET_TEST}
	${SRC_TEST}
	${HEADERS}
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(${TARGET_TEST} Threads::Threads)

set_target_properties(${TARGET_TEST} PROPERTIES CXX_STANDARD 11)

if(CMAKE_COMPILER_IS_GNUCXX)
if(coverage)
	set(CMAKE_CXX_FLAGS "-g -O0 -Wall -fprofile-arcs -ftest-coverage")
endif()
endif()

add_test(NAME ${TARGET_TEST} COMMAND ${TARGET_TEST})
