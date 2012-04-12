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
Background/VisionServerBG.cpp
DisplayObject/SSVEPStimulus.cpp
DisplayObject/P300Object.cpp
DisplayObject/SpriteObject.cpp
DisplayObject/TextObject.cpp
CommandInterpreter/HoldInterpreter.cpp
CommandInterpreter/SimpleInterpreter.cpp
CommandInterpreter/P300Interpreter.cpp
)

IF(COSHELLBCI_FOUND)
SET(${LIB_NAME}_src
${${LIB_NAME}_src}
CommandInterpreter/CoshellInterpreter.cpp
CommandInterpreter/CoshellApplications/CupboardLookout.cpp
CommandInterpreter/CoshellApplications/ObjectsLookout.cpp
CommandInterpreter/CoshellApplications/AdaptiveSteering.cpp
CommandInterpreter/CoshellApplications/StaticSteering.cpp
)
ENDIF(COSHELLBCI_FOUND)

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
TARGET_LINK_LIBRARIES(${LIB_NAME} ${BCIMIDDLEWARE_LIBS})
# -- Not needed in WIN32 because boost comes with autolink
IF(NOT WIN32)
TARGET_LINK_LIBRARIES(${LIB_NAME} ${Boost_LIBRARIES})
ENDIF(NOT WIN32)

# -- Manual link for WIN32
IF(WIN32)
TARGET_LINK_LIBRARIES(${LIB_NAME} "sfml-graphics.lib;sfml-window.lib;sfml-system.lib;bci-middleware.lib;vision.lib;avcodec.lib;swscale.lib;avutil.lib")
ENDIF(WIN32)

IF(COSHELLBCI_FOUND)
TARGET_LINK_LIBRARIES(${LIB_NAME}  XmlRpc xmlrpc-client visionsystem vision configparser)
ENDIF(COSHELLBCI_FOUND)

INSTALL(TARGETS ${LIB_NAME}
    DESTINATION lib)
