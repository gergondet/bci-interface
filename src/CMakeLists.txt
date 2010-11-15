SET(EXEC_NAME ssvp-interface)

SET(${EXEC_NAME}_src
    ./ssvp-interface.cpp
    ./square-function.cpp
)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)

ADD_EXECUTABLE(${EXEC_NAME} 
  ${${EXEC_NAME}_src}
  ${${EXEC_NAME}_headers}
  )

INSTALL(TARGETS ${EXEC_NAME}
    DESTINATION bin)
