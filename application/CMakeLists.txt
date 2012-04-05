ADD_DEFINITIONS(${${PROJECT_NAME}_CXXFLAGS})

# -------------------------
#  StaticSteering App
# -------------------------
SET(EXEC_NAME StaticSteering)

SET(${EXEC_NAME}_src
    StaticSteering.cpp
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
TARGET_LINK_LIBRARIES(${EXEC_NAME} "bci-interface.lib sfml-graphics.lib sfml-window.lib sfml-system.lib")
ENDIF(UNIX)
TARGET_LINK_LIBRARIES(${EXEC_NAME} ${Boost_LIBRARIES})

# -------------------------
#  AdaptiveSteering App
# -------------------------

SET(EXEC_NAME AdaptiveSteering)

SET(${EXEC_NAME}_src
    AdaptiveSteering.cpp
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
TARGET_LINK_LIBRARIES(${EXEC_NAME} "bci-interface.lib sfml-graphics.lib sfml-window.lib sfml-system.lib")
ENDIF(UNIX)
TARGET_LINK_LIBRARIES(${EXEC_NAME} ${Boost_LIBRARIES})


# -------------------------
#  HUDSteering App
# -------------------------

SET(EXEC_NAME HUDSteering)

SET(${EXEC_NAME}_src
    HUDSteering.cpp
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
TARGET_LINK_LIBRARIES(${EXEC_NAME} "bci-interface.lib sfml-graphics.lib sfml-window.lib sfml-system.lib")
ENDIF(UNIX)
TARGET_LINK_LIBRARIES(${EXEC_NAME} ${Boost_LIBRARIES})

# -------------------------
#  DynamicTraining App
# -------------------------

SET(EXEC_NAME DynamicTraining)

SET(${EXEC_NAME}_src
    DynamicTraining.cpp
    SSVEPTrainingManager.cpp
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
TARGET_LINK_LIBRARIES(${EXEC_NAME} "bci-interface.lib sfml-graphics.lib sfml-window.lib sfml-system.lib")
ENDIF(UNIX)
TARGET_LINK_LIBRARIES(${EXEC_NAME} ${Boost_LIBRARIES})




# -------------------------
#  whack-a-mole App
# -------------------------

SET(EXEC_NAME whack-a-mole)

SET(${EXEC_NAME}_src
    whack-a-mole/whack-a-mole.cpp
    whack-a-mole/WhackInterpreter.cpp
)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/application/whack-a-mole)
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
TARGET_LINK_LIBRARIES(${EXEC_NAME} "${PROJECT_NAME} -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system")
ELSE(UNIX)
TARGET_LINK_LIBRARIES(${EXEC_NAME} "bci-interface.lib sfml-audio.lib sfml-graphics.lib sfml-window.lib sfml-system.lib")
ENDIF(UNIX)
TARGET_LINK_LIBRARIES(${EXEC_NAME} ${Boost_LIBRARIES})
