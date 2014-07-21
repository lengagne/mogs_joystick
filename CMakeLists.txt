PROJECT (MoGS_Joystick)
set(PROJECT_CONTACT lengagne@gmail.com)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
SET( CMAKE_BUILD_TYPE Release ) 

set (MOGS_JOYSTICK_VERSION_MAJOR 2)
set (MOGS_JOYSTICK_VERSION_MINOR 0)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse2 -fpic")
set(CMAKE_VERBOSE_MAKEFILE OFF)

set( LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE})
SET( EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin )

foreach(p LIB INCLUDE)
	set(var CMAKE_INSTALL_${p}DIR)
	if(NOT IS_ABSOLUTE "${${var}}")
		set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
	endif()
endforeach()


FIND_PACKAGE( Boost COMPONENTS filesystem system REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIRS} )
if (NOT MoGS_Tinyxml_FOUND)
	FIND_PACKAGE(MoGS_Tinyxml REQUIRED)
endif (NOT MoGS_Tinyxml_FOUND)

set(MoGS_Joystick_MODULE_COMPILED "TRUE" CACHE INTERNAL "")
set(MoGS_Joystick_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include/" CACHE INTERNAL "" )
set(MoGS_Joystick_LIBRARY_DIRS "${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}" CACHE INTERNAL "" )
set(MoGS_Joystick_LIBRARIES  -lMoGS_Joystick CACHE INTERNAL "")
set(MoGS_Joystick_FOUND "TRUE" CACHE INTERNAL "")
set(MoGS_Joystick_FLAGS "-DMoGS_Joystick_FOUND" CACHE INTERNAL "")

add_definitions(${MoGS_Joystick_FLAGS})

configure_file ( "${PROJECT_SOURCE_DIR}/config_Joystick.h.in"
		 "${PROJECT_SOURCE_DIR}/include/config_Joystick.h" )
		 
IF(NOT MoGS_GLOBAL_COMPILED)
	add_custom_target(test)
ENDIF(NOT MoGS_GLOBAL_COMPILED)

INCLUDE_DIRECTORIES ( 
	include
	${MoGS_Tinyxml_INCLUDE_DIRS}
)

LINK_DIRECTORIES( ${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}
		  ${MoGS_Tinyxml_LIBRARY_DIRS})
		  
# Perform the proper linking
SET (CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

file(	GLOB_RECURSE
	MoGS_Joystick_SOURCES
	src/* )

FILE ( GLOB_RECURSE
        header_files_MoGS_Joystick
	include/*.h)

ADD_LIBRARY(	MoGS_Joystick SHARED 
		${MoGS_Joystick_SOURCES}
)

link_directories( ${MoGS_LIBRARY_DIRS})

target_link_libraries( MoGS_Joystick
    -lpthread
    ${Boost_LIBRARIES}
    ${MoGS_Tinyxml_LIBRARIES}
)

ADD_SUBDIRECTORY( ${PROJECT_SOURCE_DIR}/apps )
ADD_SUBDIRECTORY( ${PROJECT_SOURCE_DIR}/test )

# Perform the proper linking
SET (CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

# install the include files
SET(INSTALL_INCLUDE_DIRS ${CMAKE_INSTALL_INCLUDEDIR}/include/MoGS/MoGS_Joystick)
SET(INSTALL_LIB_DIRS ${CMAKE_INSTALL_LIBDIR}/lib )

# install the include files
install(FILES ${header_files_MoGS_Joystick} DESTINATION ${INSTALL_INCLUDE_DIRS})

# install the library files
install( TARGETS MoGS_Joystick LIBRARY DESTINATION ${INSTALL_LIB_DIRS}  COMPONENT main)

### Configure file
configure_file(MoGS_JoystickConfig.cmake.in "${PROJECT_BINARY_DIR}/MoGS_JoystickConfig.cmake" @ONLY)

install ( 	FILES "${PROJECT_BINARY_DIR}/MoGS_JoystickConfig.cmake"
		DESTINATION "${INSTALL_LIB_PATH}/MoGS_Joystick" COMPONENT dev ) 

install(DIRECTORY 	${CMAKE_CURRENT_SOURCE_DIR}/data/DTD
	DESTINATION 	${CMAKE_INSTALL_LIBDIR}/share/MoGS)
