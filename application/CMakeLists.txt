ADD_DEFINITIONS(${${PROJECT_NAME}_CXXFLAGS})

find_package(SFML COMPONENTS audio graphics window system)


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
LINK_DIRECTORIES(${CMAKE_INSTALL_PREFIX}/lib)


ADD_EXECUTABLE(${EXEC_NAME}
  ${${EXEC_NAME}_src}
  )

IF(UNIX)
TARGET_LINK_LIBRARIES(${EXEC_NAME} ${PROJECT_NAME} ${SFML_LIBRARIES} pthread)
ELSE(UNIX)
TARGET_LINK_LIBRARIES(${EXEC_NAME} "bci-interface.lib;sfml-oculus.lib;sfml-audio-s.lib;sfml-graphics-s.lib;sfml-window-s.lib;sfml-system-s.lib;glu32.lib;${OCULUS_LIB}")
ENDIF(UNIX)
TARGET_LINK_LIBRARIES(${EXEC_NAME} ${Boost_LIBRARIES})

# -------------------------
#  ssvep-training-oculus App
# -------------------------

SET(EXEC_NAME ssvep-training-oculus)

SET(${EXEC_NAME}_src
    ssvep-training/ssvep-training-oculus.cpp
)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/application/ssvep-training-oculus)
LINK_DIRECTORIES(${${PROJECT_NAME}_BINARY_DIR}/src)
LINK_DIRECTORIES(${CMAKE_INSTALL_PREFIX}/lib)


ADD_EXECUTABLE(${EXEC_NAME}
  ${${EXEC_NAME}_src}
  )

IF(UNIX)
TARGET_LINK_LIBRARIES(${EXEC_NAME} ${PROJECT_NAME} ${SFML_LIBRARIES} pthread)
ELSE(UNIX)
TARGET_LINK_LIBRARIES(${EXEC_NAME} "bci-interface.lib;sfml-oculus.lib;sfml-graphics-s.lib;sfml-window-s.lib;sfml-system-s.lib;glu32.lib;${OCULUS_LIB}")
ENDIF(UNIX)
TARGET_LINK_LIBRARIES(${EXEC_NAME} ${Boost_LIBRARIES})

# -------------------------
#  ssvep-training App
# -------------------------

SET(EXEC_NAME ssvep-training)

SET(${EXEC_NAME}_src
    ssvep-training/ssvep-training.cpp
)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/application/ssvep-training)
LINK_DIRECTORIES(${${PROJECT_NAME}_BINARY_DIR}/src)
LINK_DIRECTORIES(${CMAKE_INSTALL_PREFIX}/lib)


ADD_EXECUTABLE(${EXEC_NAME}
  ${${EXEC_NAME}_src}
  )

IF(UNIX)
TARGET_LINK_LIBRARIES(${EXEC_NAME} ${PROJECT_NAME} ${SFML_LIBRARIES} pthread)
ELSE(UNIX)
TARGET_LINK_LIBRARIES(${EXEC_NAME} "bci-interface.lib;sfml-oculus.lib;sfml-graphics-s.lib;sfml-window-s.lib;sfml-system-s.lib;glu32.lib;${OCULUS_LIB}")
ENDIF(UNIX)
TARGET_LINK_LIBRARIES(${EXEC_NAME} ${Boost_LIBRARIES})
