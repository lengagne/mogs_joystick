PROJECT (MoGS_Joystick)
set(PROJECT_CONTACT lengagne@gmail.com)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

set (MOGS_JOYSTICK_VERSION_MAJOR 2)
set (MOGS_JOYSTICK_VERSION_MINOR 0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse2")
set(CMAKE_VERBOSE_MAKEFILE OFF)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE})

foreach(p LIB INCLUDE)
	set(var CMAKE_INSTALL_${p}DIR)
	if(NOT IS_ABSOLUTE "${${var}}")
		set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
	endif()
endforeach()

FIND_PACKAGE(MoGS_Tinyxml REQUIRED)
LINK_DIRECTORIES( ${MoGS_Tinyxml_LIBRARY_DIRS} )

set(MoGS_Joystick_MODULE_COMPILED "TRUE" CACHE INTERNAL "")
set(MoGS_Joystick_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include/" CACHE INTERNAL "" )
set(MoGS_Joystick_LIBRARY_DIRS "${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}" CACHE INTERNAL "" )
set(MoGS_Joystick_LIBRARIES  -lMoGS_Joystick CACHE INTERNAL "")
set(MoGS_Joystick_FOUND "TRUE" CACHE INTERNAL "")
set(MoGS_Joystick_FLAGS "-DMoGS_Joystick_FOUND" CACHE INTERNAL "")

add_definitions(${MoGS_Joystick_FLAGS})

configure_file ( "${PROJECT_SOURCE_DIR}/config_Joystick.h.in"
		 "${PROJECT_SOURCE_DIR}/include/config_Joystick.h" )

INCLUDE_DIRECTORIES ( 
	include
	${MoGS_Tinyxml_INCLUDE_DIRS}
)

# Perform the proper linking
SET (CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

file(	GLOB_RECURSE
	MoGS_Joystick_SOURCES
	src/* )
	
LIST(REMOVE_ITEM MoGS_Joystick_SOURCES  ${CMAKE_CURRENT_SOURCE_DIR}/src/Config_pad.cpp)

FILE ( GLOB_RECURSE
        header_files_MoGS_Joystick
	include/*.h
	)

ADD_LIBRARY(	MoGS_Joystick SHARED 
		${MoGS_Joystick_SOURCES}
)

link_directories( ${MoGS_LIBRARY_DIRS})

target_link_libraries(MoGS_Joystick
			-lpthread
			${MoGS_Tinyxml_LIBRARIES}
)

ADD_EXECUTABLE(test_pad
		"${CMAKE_CURRENT_SOURCE_DIR}/test/test_pad.cpp")

target_link_libraries(test_pad      
      MoGS_Joystick)

ADD_EXECUTABLE(test_MoGS_pad
		"${CMAKE_CURRENT_SOURCE_DIR}/test/test_MoGS_pad.cpp")

target_link_libraries(test_MoGS_pad      
      MoGS_Joystick)

ADD_EXECUTABLE(MoGS_Config_Joystick
		"${CMAKE_CURRENT_SOURCE_DIR}/src/Config_pad.cpp")

target_link_libraries(MoGS_Config_Joystick      
      MoGS_Joystick)
      

ADD_SUBDIRECTORY( ${PROJECT_SOURCE_DIR}/apps )

# Perform the proper linking
SET (CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

# install the include files
SET(INSTALL_INCLUDE_DIRS ${CMAKE_INSTALL_INCLUDEDIR}/include/MoGS/MoGS_Joystick)
SET(INSTALL_LIB_DIRS ${CMAKE_INSTALL_LIBDIR}/lib )

# install the include files
install(FILES ${header_files_MoGS_Joystick} DESTINATION ${INSTALL_INCLUDE_DIRS})

# install the library files
install( TARGETS MoGS_Joystick LIBRARY DESTINATION ${INSTALL_LIB_DIRS}  COMPONENT main)

# install the binary files
install( TARGETS MoGS_Config_Joystick RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin  COMPONENT main)

### Configure file
configure_file(MoGS_JoystickConfig.cmake.in "${PROJECT_BINARY_DIR}/MoGS_JoystickConfig.cmake" @ONLY)



install ( 	FILES "${PROJECT_BINARY_DIR}/MoGS_JoystickConfig.cmake"
		DESTINATION "${INSTALL_LIB_PATH}/MoGS_Joystick" COMPONENT dev ) 

install(DIRECTORY 	${CMAKE_CURRENT_SOURCE_DIR}/data/DTD
	DESTINATION 	${CMAKE_INSTALL_LIBDIR}/share/MoGS)
