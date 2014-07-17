FIND_PACKAGE( Qt4 4.8 )

IF( ${QT4_FOUND} )
    ADD_SUBDIRECTORY( joystickmapper ) 
ENDIF( )

ADD_SUBDIRECTORY(config_joystick)