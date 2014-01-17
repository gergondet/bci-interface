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

SET_TARGET_PROPERTIES(${PROJECT_NAME_LIB}
  PROPERTIES
  COMPILE_FLAGS ${${PROJECT_NAME}_src_CXXFLAGS}
  LINK_FLAGS "${${PROJECT_NAME}_src_LDFLAGS}"}
)

IF(UNIX)
TARGET_LINK_LIBRARIES(${EXEC_NAME} ${PROJECT_NAME} ${SFML_LIBRARIES} pthread)
ELSE(UNIX)
TARGET_LINK_LIBRARIES(${EXEC_NAME} "bci-interface.lib;sfml-audio.lib;sfml-graphics.lib;sfml-window.lib;sfml-system.lib;OpenGL32.lib;glu32.lib")
ENDIF(UNIX)
TARGET_LINK_LIBRARIES(${EXEC_NAME} ${Boost_LIBRARIES})

# -- EchoServer
