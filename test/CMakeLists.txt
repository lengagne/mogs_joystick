ADD_EXECUTABLE( test_pad test_pad.cpp )
TARGET_LINK_LIBRARIES( test_pad MoGS_Joystick )

ADD_EXECUTABLE( test_MoGS_pad test_MoGS_pad.cpp )
TARGET_LINK_LIBRARIES(test_MoGS_pad MoGS_Joystick)
