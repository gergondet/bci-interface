SET(LIB_NAME bci-interface)

SET(${LIB_NAME}_src
CommandReceiver/UDPReceiver.cpp
CommandReceiver/TCPClient.cpp
CommandReceiver/TCPReceiver.cpp
Utils/UDPServer.cpp
Utils/square-function.cpp
Utils/Win32.cpp
BCIInterface.cpp
Background/BufferBG.cpp
Background/StaticBG.cpp
DisplayObject/SSVEPStimulus.cpp
DisplayObject/P300Object.cpp
DisplayObject/SpriteObject.cpp
DisplayObject/TextObject.cpp
CommandInterpreter/HoldInterpreter.cpp
CommandInterpreter/SimpleInterpreter.cpp
CommandInterpreter/P300Interpreter.cpp
CommandOverrider.cpp
)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

ADD_LIBRARY(${LIB_NAME} 
  SHARED
  ${${LIB_NAME}_src}
  ${${LIB_NAME}_headers}
)

IF(WIN32)
add_library( "bci-interface-static" STATIC ${${LIB_NAME}_src} ${${LIB_NAME}_headers} )
set_target_properties( "bci-interface-static" PROPERTIES OUTPUT_NAME "bci-interface")
ENDIF(WIN32)

SET_TARGET_PROPERTIES(${LIB_NAME} 
  PROPERTIES 
  SOVERSION ${PROJECT_VERSION}
  INSTALL_RPATH lib)

ADD_DEFINITIONS(${${PROJECT_NAME}_CXXFLAGS})
TARGET_LINK_LIBRARIES(${LIB_NAME} ${BCIMIDDLEWARE_LIBS} glut GLU GL)
# -- Not needed in WIN32 because boost comes with autolink

IF(NOT WIN32)
TARGET_LINK_LIBRARIES(${LIB_NAME} ${Boost_LIBRARIES})
TARGET_LINK_LIBRARIES(${LIB_NAME} ${SFML_LIBRARIES})
ENDIF(NOT WIN32)

PKG_CONFIG_USE_DEPENDENCY(${LIB_NAME} bci-middleware)


# -- Manual link for WIN32
IF(WIN32)
TARGET_LINK_LIBRARIES(${LIB_NAME} "sfml-graphics.lib;sfml-window.lib;sfml-system.lib;bci-middleware.lib;OpenGL32.lib;glu32.lib")
ENDIF(WIN32)
  
INSTALL(TARGETS ${LIB_NAME}
    DESTINATION lib)
