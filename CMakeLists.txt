PROJECT (MoGS_Joystick)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

set(LIBRARY_OUTPUT_PATH lib/${CMAKE_BUILD_TYPE})

foreach(p LIB INCLUDE)
	set(var CMAKE_INSTALL_${p}DIR)
	if(NOT IS_ABSOLUTE "${${var}}")
		set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
	endif()
endforeach()

if (MoGS_GLOBAL_COMPILED)
  set(MoGS_Joystick_MODULE_COMPILED "TRUE" CACHE INTERNAL "")
  set(MoGS_Joystick_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/include/" CACHE INTERNAL "" )
  set(MoGS_Joystick_LIBRARY_DIRS "${CMAKE_BINARY_DIR}/${PROJECT_NAME}/lib" CACHE INTERNAL "" )
  set(MoGS_Joystick_LIBRARIES  -lMoGS_Joystick CACHE INTERNAL "")
  set(MoGS_Joystick_GLOBAL_COMPILED "TRUE" CACHE INTERNAL "")
else (MoGS_GLOBAL_COMPILED)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse2")
  set(CMAKE_VERBOSE_MAKEFILE OFF)
  set(PROJECT_CONTACT lengagne@gmail.com)
endif (MoGS_GLOBAL_COMPILED)

INCLUDE_DIRECTORIES ( 
	${CMAKE_CURRENT_BINARY_DIR}/src
	include
	src
)

# Perform the proper linking
SET (CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

file(	GLOB_RECURSE
	MoGS_Joystick_SOURCES
	src/* )

FILE ( GLOB_RECURSE
        header_files_MoGS_Joystick
	include/*.h
	)

ADD_LIBRARY(MoGS_Joystick SHARED 
    ${MoGS_Joystick_SOURCES}
  
)

target_link_libraries(MoGS_Joystick
      -lpthread
)

ADD_EXECUTABLE(test_pad
		"${CMAKE_CURRENT_SOURCE_DIR}/test/test_pad.cpp")

target_link_libraries(test_pad      
      MoGS_Joystick)

ADD_EXECUTABLE(test_generic
		"${CMAKE_CURRENT_SOURCE_DIR}/test/test_generic.cpp")

target_link_libraries(test_generic      
      MoGS_Joystick)

ADD_EXECUTABLE(test_MoGS_pad
		"${CMAKE_CURRENT_SOURCE_DIR}/test/test_MoGS_pad.cpp")

target_link_libraries(test_MoGS_pad      
      MoGS_Joystick)


configure_file(MoGS_Joystick.pc.in MoGS_Joystick.pc @ONLY)

# install the pkgconfig files
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/MoGS_Joystick.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/lib/pkgconfig)
# install the include files
install(FILES ${header_files_MoGS_Joystick} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/include/MoGS/MoGS_Joystick)
# install the library files
install( TARGETS "MoGS_Joystick" LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/lib  COMPONENT main)
