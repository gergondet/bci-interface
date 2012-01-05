SET(LIB_NAME bci-interface)

SET(${LIB_NAME}_src
CommandReceiver/UDPReceiver.cpp
CommandReceiver/TCPReceiver.cpp
Utils/UDPServer.cpp
Utils/square-function.cpp
BCIInterface.cpp
Background/BufferBG.cpp
Background/VisionServerBG.cpp
DisplayObject/SSVEPStimulus.cpp
DisplayObject/P300Object.cpp
DisplayObject/SpriteObject.cpp
DisplayObject/TextObject.cpp
CommandInterpreter/CoshellInterpreter.cpp
CommandInterpreter/SimpleInterpreter.cpp
CommandInterpreter/P300Interpreter.cpp
CommandInterpreter/CoshellApplications/CupboardLookout.cpp
CommandInterpreter/CoshellApplications/ObjectsLookout.cpp
CommandInterpreter/CoshellApplications/AdaptiveSteering.cpp
)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include ${Boost_INCLUDE_DIRS})

ADD_LIBRARY(${LIB_NAME} 
  SHARED
  ${${LIB_NAME}_src}
  ${${LIB_NAME}_headers}
)


SET_TARGET_PROPERTIES(${LIB_NAME} 
  PROPERTIES 
  SOVERSION ${PROJECT_VERSION}
  INSTALL_RPATH lib)

ADD_DEFINITIONS(${${PROJECT_NAME}_CXXFLAGS})
TARGET_LINK_LIBRARIES(${LIB_NAME} ${${PROJECT_NAME}_LDFLAGS} ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${BCIMIDDLEWARE_LIBS})

INSTALL(TARGETS ${LIB_NAME}
    DESTINATION lib)
