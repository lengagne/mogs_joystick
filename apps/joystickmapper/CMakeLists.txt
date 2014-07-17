PROJECT( MoGS_Joystick_Mapper )

#
# Qt config
#
SET( CMAKE_INCLUDE_CURRENT_DIR ON )
SET( CMAKE_AUTOMOC ON )
SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall" )

FIND_PACKAGE( Qt4 4.8 COMPONENTS QTCORE QTGUI )
INCLUDE_DIRECTORIES( ${QT_INCLUDE_DIRS} )
INCLUDE( ${QT_USE_FILE} )
ADD_DEFINITIONS( ${QT_DEFINITIONS} )

# 
# Project source 
#
INCLUDE_DIRECTORIES( ${PROJECT_SOURCE_DIR}/src )

SET( Mapper_SRCS 
    ${PROJECT_SOURCE_DIR}/main.cpp
    ${PROJECT_SOURCE_DIR}/src/JoystickMapper.cpp 
    ${PROJECT_SOURCE_DIR}/src/JoystickSelector.cpp 
    ${PROJECT_SOURCE_DIR}/src/JoystickWrapper.cpp
    ${PROJECT_SOURCE_DIR}/src/view/ActionConfigurationView.cpp
    ${PROJECT_SOURCE_DIR}/src/view/JoystickVisualizer.cpp  
    ${PROJECT_SOURCE_DIR}/src/delegate/JoystickActionDelegate.cpp
    ${PROJECT_SOURCE_DIR}/src/delegate/AxisRangeDelegate.cpp
    ${PROJECT_SOURCE_DIR}/src/delegate/ButtonPressedDelegate.cpp
 )

QT4_WRAP_UI( Mapper_UI_HPP
    ${PROJECT_SOURCE_DIR}/src/JoystickMapper.ui 
    ${PROJECT_SOURCE_DIR}/src/JoystickSelector.ui 
    ${PROJECT_SOURCE_DIR}/src/view/JoystickVisualizer.ui 
    ${PROJECT_SOURCE_DIR}/src/view/ActionConfigurationView.ui 
)

# 
# Project build
#
ADD_EXECUTABLE( MoGS_Joystick_Mapper ${Mapper_SRCS} ${Mapper_UI_HPP} ) 
TARGET_LINK_LIBRARIES( MoGS_Joystick_Mapper MoGS_Joystick ${QT_LIBRARIES} )

# 
# Project install
#
INSTALL( TARGETS MoGS_Joystick_Mapper RUNTIME DESTINATION bin )