ADD_EXECUTABLE( MoGS_Config_Joystick Config_pad.cpp )
TARGET_LINK_LIBRARIES( MoGS_Config_Joystick MoGS_Joystick )

# install the binary files
INSTALL( TARGETS MoGS_Config_Joystick RUNTIME DESTINATION bin COMPONENT main )