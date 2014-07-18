ADD_CUSTOM_TARGET( test_mogs_joystick
    COMMAND make test_pad
    COMMAND make test_MoGS_pad
)

ADD_DEPENDENCIES( test test_mogs_joystick )

ADD_EXECUTABLE( test_pad EXCLUDE_FROM_ALL test_pad.cpp )
TARGET_LINK_LIBRARIES( test_pad MoGS_Joystick )

ADD_EXECUTABLE( test_MoGS_pad EXCLUDE_FROM_ALL  test_MoGS_pad.cpp )
TARGET_LINK_LIBRARIES( test_MoGS_pad MoGS_Joystick )
