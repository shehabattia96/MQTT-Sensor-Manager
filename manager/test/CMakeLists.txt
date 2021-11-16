message("Building Manager_Tests")
cmake_minimum_required(VERSION 3.11)
set( CMAKE_VERBOSE_MAKEFILE ON )

project( Manager_Tests C CXX)

get_filename_component( TEST_PATH "${CMAKE_CURRENT_SOURCE_DIR}/" ABSOLUTE )


#### Catch2
find_package(Catch2 CONFIG REQUIRED) # uses vcpkg toolchain
set(VCPKG_LIBRARIES ${VCPKG_LIBRARIES} Catch2::Catch2)

#### Test sources
set(
    sources
    ${TEST_PATH}/test_main.cpp
    ${TEST_PATH}/TestEventLoop.cpp
)


add_executable (Manager_Tests ${sources})
include_directories(${VCPKG_INCLUDES})
target_link_libraries(Manager_Tests LINK_PUBLIC ${VCPKG_LIBRARIES})

#### copy resources directory to build dir:
add_custom_command(TARGET Manager_Tests POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory "${TEST_PATH}/resources/" "$<TARGET_FILE_DIR:Manager_Tests>/resources")


message("Done building Manager_Tests")   