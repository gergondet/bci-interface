ADD_REQUIRED_DEPENDENCY("configparser")

ADD_DEFINITIONS(${${PROJECT_NAME}_CXXFLAGS})

SET(EXEC_NAME testssvep-stimuli)

SET(${EXEC_NAME}_src
    testssvep-stimuli.cpp
)

# TODO
LINK_DIRECTORIES(${VISIONSYSTEM_LIBRARY_DIRS})

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

IF(WIN32)
TARGET_LINK_LIBRARIES(${EXEC_NAME} "bci-interface.lib;sfml-graphics.lib;sfml-window.lib;sfml-system.lib;bci-middleware.lib;vision.lib;avcodec.lib;swscale.lib;avutil.lib;ws2_32.lib;OpenGL32.lib;glu32.lib")
ELSE(WIN32)
TARGET_LINK_LIBRARIES(${EXEC_NAME} bci-interface ${SFML_LIBRARIES})
ENDIF(WIN32)
TARGET_LINK_LIBRARIES(${EXEC_NAME} ${Boost_LIBRARIES})
PKG_CONFIG_USE_DEPENDENCY(${EXEC_NAME} configparser)

# -- EchoServer
SET(EXEC_NAME testssvep-interface)

SET(${EXEC_NAME}_src
    testssvep-interface.cpp
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

IF(WIN32)
TARGET_LINK_LIBRARIES(${EXEC_NAME} "bci-interface.lib;sfml-graphics.lib;sfml-window.lib;sfml-system.lib;bci-middleware.lib;vision.lib;avcodec.lib;swscale.lib;avutil.lib;ws2_32.lib;OpenGL32.lib;glu32.lib")
ELSE(WIN32)
TARGET_LINK_LIBRARIES(${EXEC_NAME} bci-interface ${SFML_LIBRARIES})
ENDIF(WIN32)
TARGET_LINK_LIBRARIES(${EXEC_NAME} ${Boost_LIBRARIES})

PKG_CONFIG_USE_DEPENDENCY(${EXEC_NAME} configparser)

# -- EchoServer
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

IF(WIN32)
TARGET_LINK_LIBRARIES(${EXEC_NAME} "bci-interface.lib;sfml-graphics.lib;sfml-window.lib;sfml-system.lib;bci-middleware.lib;vision.lib;avcodec.lib;swscale.lib;avutil.lib;ws2_32.lib;OpenGL32.lib;glu32.lib")
ELSE(WIN32)
TARGET_LINK_LIBRARIES(${EXEC_NAME} bci-interface ${SFML_LIBRARIES})
ENDIF(WIN32)
TARGET_LINK_LIBRARIES(${EXEC_NAME} ${Boost_LIBRARIES})
PKG_CONFIG_USE_DEPENDENCY(${EXEC_NAME} configparser)

# -- EchoServer

SET(EXEC_NAME EchoServer)

SET(${EXEC_NAME}_src
    EchoServer.cpp
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

IF(WIN32)
TARGET_LINK_LIBRARIES(${EXEC_NAME} "bci-interface.lib;sfml-graphics.lib;sfml-window.lib;sfml-system.lib;bci-middleware.lib;vision.lib;avcodec.lib;swscale.lib;avutil.lib;ws2_32.lib;OpenGL32.lib;glu32.lib")
ELSE(WIN32)
TARGET_LINK_LIBRARIES(${EXEC_NAME} bci-interface ${SFML_LIBRARIES})
ENDIF(WIN32)
TARGET_LINK_LIBRARIES(${EXEC_NAME} ${Boost_LIBRARIES})

PKG_CONFIG_USE_DEPENDENCY(${EXEC_NAME} configparser)


#SET(EXEC_NAME testp300-interface)
#
#SET(${EXEC_NAME}_src
#    testp300-interface.cpp
#)
#
#INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)
#LINK_DIRECTORIES(${${PROJECT_NAME}_BINARY_DIR}/src)
#
#
#ADD_EXECUTABLE(${EXEC_NAME} 
#  ${${EXEC_NAME}_src}
#  )
#
#SET_TARGET_PROPERTIES(${PROJECT_NAME_LIB}
#  PROPERTIES
#  COMPILE_FLAGS ${${PROJECT_NAME}_src_CXXFLAGS}
#  LINK_FLAGS "${${PROJECT_NAME}_src_LDFLAGS}"}
#)
#
#IF(UNIX)
#TARGET_LINK_LIBRARIES(${EXEC_NAME} "${PROJECT_NAME} -lsfml-graphics -lsfml-window -lsfml-system")
#ELSE(UNIX)
#TARGET_LINK_LIBRARIES(${EXEC_NAME} "bci-interface.lib;sfml-graphics.lib;sfml-window.lib;sfml-system.lib;bci-middleware.lib;ws2_32.lib")
#ENDIF(UNIX)
#TARGET_LINK_LIBRARIES(${EXEC_NAME} ${Boost_LIBRARIES})
