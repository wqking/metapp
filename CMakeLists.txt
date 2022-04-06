cmake_minimum_required(VERSION 3.2)
project(metapp VERSION 0.1.0)

add_library(${PROJECT_NAME} STATIC)
set(CMAKE_CXX_STANDARD 11)

target_include_directories(
	${PROJECT_NAME}
	PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

file(GLOB_RECURSE SRC_LIB "src/*.cpp")
target_sources(${PROJECT_NAME}
    PRIVATE
        ${SRC_LIB}
)

add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

# Installation
# ------------
include(GNUInstallDirs)

# Install the library
install(
	TARGETS ${PROJECT_NAME}
	EXPORT ${PROJECT_NAME}Targets
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

# Install the headers
install(
	DIRECTORY include/
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# (Generate and) install the target import file
install(
	EXPORT ${PROJECT_NAME}Targets
	NAMESPACE metapp::
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/metapp
)

# Generate the package version file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
	${CMAKE_CURRENT_BINARY_DIR}/metappConfigVersion.cmake
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY AnyNewerVersion
)

# Generate the package configuration file, that allows other
# CMake projects to find the library with find_package()
configure_package_config_file(
	cmake/metappConfig.cmake.in
	${CMAKE_CURRENT_BINARY_DIR}/metappConfig.cmake
	INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/metapp
)

# Install the package version and configuration files
install(
	FILES
	${CMAKE_CURRENT_BINARY_DIR}/metappConfig.cmake
	${CMAKE_CURRENT_BINARY_DIR}/metappConfigVersion.cmake
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/metapp
)

# Install readme and license
install(
	FILES
	readme.md
	license
	DESTINATION ${CMAKE_INSTALL_DATADIR}/metapp
)
