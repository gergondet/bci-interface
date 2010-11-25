SET(EXEC_NAME ssvp-interface)

SET(${EXEC_NAME}_src
    ./square-function.cpp
    ./SSVPInterface.cpp
    ./FlickeringSquare.cpp
    ./BackgroundSprite.cpp
)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)

ADD_LIBRARY(${EXEC_NAME} 
  SHARED
  ${${EXEC_NAME}_src}
  ${${EXEC_NAME}_headers}
)


SET_TARGET_PROPERTIES(${LIB_NAME} 
  PROPERTIES 
  SOVERSION ${PROJECT_VERSION}
  INSTALL_RPATH lib)

IF(UNIX)
SET_TARGET_PROPERTIES(${EXEC_NAME}
  PROPERTIES
  LINK_FLAGS "-L/usr/local/lib -lsfml-graphics -lsfml-window -lsfml-system"
)
ENDIF(UNIX)

IF(WIN32)
SET_TARGET_PROPERTIES(${EXEC_NAME}
  PROPERTIES
  LINK_FLAGS "sfml-graphics.lib sfml-window.lib sfml-system.lib"
)
ENDIF(WIN32)

TARGET_LINK_LIBRARIES(${EXEC_NAME} ${Boost_LIBRARIES})

INSTALL(TARGETS ${EXEC_NAME}
    DESTINATION lib)
