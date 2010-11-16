SET(EXEC_NAME ssvp-interface)

SET(${EXEC_NAME}_src
    ./ssvp-interface.cpp
    ./square-function.cpp
    ./FlickeringSquare.cpp
)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)

ADD_EXECUTABLE(${EXEC_NAME} 
  ${${EXEC_NAME}_src}
  ${${EXEC_NAME}_headers}
  )

IF(UNIX)
SET_TARGET_PROPERTIES(${EXEC_NAME}
  PROPERTIES
  LINK_FLAGS "-lsfml-graphics -lsfml-window -lsfml-system"
)
ENDIF(UNIX)

IF(WIN32)
SET_TARGET_PROPERTIES(${EXEC_NAME}
  PROPERTIES
  LINK_FLAGS "sfml-graphics.lib sfml-window.lib sfml-system.lib"
)
ENDIF(WIN32)

INSTALL(TARGETS ${EXEC_NAME}
    DESTINATION bin)
