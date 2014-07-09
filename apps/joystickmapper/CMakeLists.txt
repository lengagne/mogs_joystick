PROJECT( MoGS_Joystick_Mapper )

#
# Qt config
#
SET( CMAKE_INCLUDE_CURRENT_DIR ON )
SET( CMAKE_AUTOMOC ON )

FIND_PACKAGE( Qt4 4.8 COMPONENTS QTCORE QTGUI )
INCLUDE_DIRECTORIES( ${QT_INCLUDE_DIRS} )
INCLUDE( ${QT_USE_FILE} )
ADD_DEFINITIONS( ${QT_DEFINITIONS} )

#
# Translations
#
# SET( LANG_FILES
#   ${PROJECT_SOURCE_DIR}/resources/translations/joystickmapper_en.ts
#   ${PROJECT_SOURCE_DIR}/resources/translations/joystickmapper_fr.ts )
# 
# QT4_ADD_TRANSLATION( TRANSLATION_BIN ${LANG_FILES} )
# 
# # 
# # Project binary resources
# #
# CONFIGURE_FILE( ${PROJECT_SOURCE_DIR}/resources/joystickmapper.qrc.in ${PROJECT_BINARY_DIR}/joystickmapper.qrc IMMEDIATE @ONLY )
# QT4_ADD_RESOURCES( APP_RESOURCES ${PROJECT_BINARY_DIR}/joystickmapper.qrc )

# 
# Project source 
#
INCLUDE_DIRECTORIES( ${PROJECT_SOURCE_DIR}/src )

SET( Mapper_SRCS 
    ${PROJECT_SOURCE_DIR}/main.cpp
    ${PROJECT_SOURCE_DIR}/src/JoystickMapper.cpp 
    ${PROJECT_SOURCE_DIR}/src/ActionConfigurationView.cpp
    ${PROJECT_SOURCE_DIR}/src/JoystickSelector.cpp 
    ${PROJECT_SOURCE_DIR}/src/JoystickWrapper.cpp  
    ${PROJECT_SOURCE_DIR}/src/JoystickConfigurationModel.cpp
 )

SET( Mapper_UI 
    ${PROJECT_SOURCE_DIR}/src/JoystickMapper.ui 
    ${PROJECT_SOURCE_DIR}/src/ActionConfigurationView.ui 
    ${PROJECT_SOURCE_DIR}/src/JoystickSelector.ui )

QT4_WRAP_UI( Mapper_UI_HPP ${Mapper_UI} ${APP_RESOURCES} ${TRANSLATION_BIN} )

# 
# Project build
#
ADD_EXECUTABLE( MoGS_Joystick_Mapper ${Mapper_SRCS} ${Mapper_UI_HPP} ) 
TARGET_LINK_LIBRARIES( MoGS_Joystick_Mapper MoGS_Joystick ${QT_LIBRARIES} )

# 
# Project install
#