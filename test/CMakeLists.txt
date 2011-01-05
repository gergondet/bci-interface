# 
#  Copyright 
# 

SET(EXEC_NAME testp300-interface)

SET(${EXEC_NAME}_src
    testp300-interface.cpp
)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)
LINK_DIRECTORIES(${${PROJECT_NAME}_BINARY_DIR}/src)


ADD_EXECUTABLE(${EXEC_NAME} 
  ${${EXEC_NAME}_src}
  )

SET_TARGET_PROPERTIES(${PROJECT_NAME_LIB}
  PROPERTIES
  COMPILE_FLAGS ${${PROJECT_NAME}_src_CXXFLAGS}
  LINK_FLAGS "${${PROJECT_NAME}_src_LDFLAGS} -pthread"}
)

IF(UNIX)
TARGET_LINK_LIBRARIES(${EXEC_NAME} "${PROJECT_NAME} -lsfml-graphics -lsfml-window -lsfml-system")
ELSE(UNIX)
TARGET_LINK_LIBRARIES(${EXEC_NAME} "p300-interface.lib sfml-graphics.lib sfml-window.lib sfml-system.lib")
ENDIF(UNIX)
TARGET_LINK_LIBRARIES(${EXEC_NAME} ${Boost_LIBRARIES})

SET(EXEC_NAME testssvp-interface)

SET(${EXEC_NAME}_src
    testssvp-interface.cpp
)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)
LINK_DIRECTORIES(${${PROJECT_NAME}_BINARY_DIR}/src)


ADD_EXECUTABLE(${EXEC_NAME} 
  ${${EXEC_NAME}_src}
  )

SET_TARGET_PROPERTIES(${PROJECT_NAME_LIB}
  PROPERTIES
  COMPILE_FLAGS ${${PROJECT_NAME}_src_CXXFLAGS}
  LINK_FLAGS "${${PROJECT_NAME}_src_LDFLAGS} -pthread"}
)

IF(UNIX)
TARGET_LINK_LIBRARIES(${EXEC_NAME} "${PROJECT_NAME} -lsfml-graphics -lsfml-window -lsfml-system")
ELSE(UNIX)
TARGET_LINK_LIBRARIES(${EXEC_NAME} "ssvp-interface.lib sfml-graphics.lib sfml-window.lib sfml-system.lib")
ENDIF(UNIX)
TARGET_LINK_LIBRARIES(${EXEC_NAME} ${Boost_LIBRARIES})

SET(EXEC_NAME testsquare-function)

SET(${EXEC_NAME}_src
    testsquare-function.cpp
)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)
LINK_DIRECTORIES(${${PROJECT_NAME}_BINARY_DIR}/src)


ADD_EXECUTABLE(${EXEC_NAME} 
  ${${EXEC_NAME}_src}
  )

SET_TARGET_PROPERTIES(${PROJECT_NAME_LIB}
  PROPERTIES
  COMPILE_FLAGS ${${PROJECT_NAME}_src_CXXFLAGS}
  LINK_FLAGS "${${PROJECT_NAME}_src_LDFLAGS}"}
)

IF(UNIX)
TARGET_LINK_LIBRARIES(${EXEC_NAME} "${PROJECT_NAME} -lsfml-graphics -lsfml-window -lsfml-system")
ELSE(UNIX)
TARGET_LINK_LIBRARIES(${EXEC_NAME} "ssvp-interface.lib sfml-graphics.lib sfml-window.lib sfml-system.lib")
ENDIF(UNIX)
TARGET_LINK_LIBRARIES(${EXEC_NAME} ${Boost_LIBRARIES})
