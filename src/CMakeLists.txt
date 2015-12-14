SET(LIB_NAME bci-interface)

SET(${LIB_NAME}_src
CommandReceiver/UDPReceiver.cpp
CommandReceiver/TCPClientReceiver.cpp
CommandReceiver/TCPReceiver.cpp
Utils/FontManager.cpp
Utils/TCPClient.cpp
Utils/UDPSocket.cpp
#Utils/UDPServer.cpp
Utils/square-function.cpp
Utils/Win32.cpp
BCIInterface.cpp
private/BCIInterfaceImpl.cpp
Background/BufferBG.cpp
Background/StaticBG.cpp
DisplayObject/SSVEPStimulus.cpp
DisplayObject/P300Object.cpp
DisplayObject/SpriteObject.cpp
DisplayObject/TextObject.cpp
private/DisplayObject/TextObjectImpl.cpp
DisplayObject/BoxedTextObject.cpp
private/DisplayObject/BoxedTextObjectImpl.cpp
DisplayObject/FPSCounter.cpp
CommandInterpreter/HoldInterpreter.cpp
CommandInterpreter/SimpleInterpreter.cpp
CommandInterpreter/P300Interpreter.cpp
CommandOverrider.cpp
)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${CMAKE_INSTALL_PREFIX}/include)

IF(WIN32)
    ADD_LIBRARY(${LIB_NAME}
      SHARED
      ${${LIB_NAME}_src}
      ${${LIB_NAME}_headers}
    )

    TARGET_LINK_LIBRARIES(${LIB_NAME} "sfml-oculus.lib;sfml-graphics.lib;sfml-window.lib;sfml-system.lib;glu32.lib;OpenGL32.lib;")
ELSE()
    ADD_LIBRARY(${LIB_NAME}
      SHARED
      ${${LIB_NAME}_src}
      ${${LIB_NAME}_headers}
    )
    SET_TARGET_PROPERTIES(${LIB_NAME}
      PROPERTIES
      VERSION 1.0.0
      SOVERSION 1
    )
    TARGET_LINK_LIBRARIES(${LIB_NAME} glut GLU GL)
    TARGET_LINK_LIBRARIES(${LIB_NAME} ${Boost_LIBRARIES})
    TARGET_LINK_LIBRARIES(${LIB_NAME} ${SFML_LIBRARIES})
    TARGET_LINK_LIBRARIES(${LIB_NAME} udev Xinerama)
ENDIF()

ADD_DEFINITIONS(${${PROJECT_NAME}_CXXFLAGS})
LINK_DIRECTORIES(${CMAKE_INSTALL_PREFIX}/lib)

INSTALL(TARGETS ${LIB_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)
