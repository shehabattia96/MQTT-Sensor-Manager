message("Building Manager") 
cmake_minimum_required(VERSION 3.11)
set( CMAKE_VERBOSE_MAKEFILE ON )
project( Manager C CXX )

get_filename_component( APP_PATH "${CMAKE_CURRENT_SOURCE_DIR}/" ABSOLUTE )

#### include paho-mqtt via VCPKG
find_package(OpenSSL REQUIRED)
find_package(eclipse-paho-mqtt-c CONFIG REQUIRED)
set(PAHO_INCLUDES_DIR "${eclipse-paho-mqtt-c_DIR}/../../include/paho-mqtt/")

#### include opencv via VCPKG
find_package(OpenCV 4.2 REQUIRED)
if ( OpenCV_FOUND )
	message( "OpenCV found." )
else()
    message( FATAL_ERROR "Could not find OpenCV." )
endif()


#### VCPKG include and link list
set(VCPKG_INCLUDES ${VCPKG_INCLUDES} ${OPENSSL_INCLUDE_DIR} ${PAHO_INCLUDES_DIR})
set(VCPKG_LIBRARIES 
	${VCPKG_LIBRARIES}
	# paho-related libraries
	${OPENSSL_LIBRARIES}
	eclipse-paho-mqtt-c::paho-mqtt3a eclipse-paho-mqtt-c::paho-mqtt3c eclipse-paho-mqtt-c::paho-mqtt3as eclipse-paho-mqtt-c::paho-mqtt3cs
	# opencv libraries
	${OpenCV_LIBS}
	)


	file(GLOB unit_sources CONFIGURE_DEPENDS "./Units/*.cpp")
	file(GLOB unit_headers CONFIGURE_DEPENDS "./Units/*.h")

set(
    sources
    ${APP_PATH}/main.cpp
	# ${unit_sources}
)

add_executable (Manager ${sources}  )
include_directories(${VCPKG_INCLUDES} )
target_link_libraries(Manager LINK_PUBLIC ${VCPKG_LIBRARIES})

#### copy resources directory to build dir:
add_custom_command(TARGET Manager POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory "${APP_PATH}/resources/" "$<TARGET_FILE_DIR:Manager>/resources")

#### tests	
include(CTest)
if(BUILD_TESTING)
  add_subdirectory(Test)
endif()


message("Done building Manager")  
