INCLUDE_DIRECTORIES(${CMAKE_INSTALL_PREFIX}/include)

ADD_DEFINITIONS(${${PROJECT_NAME}_CXXFLAGS})

SET(EXEC_NAME testssvep-stimuli)

SET(${EXEC_NAME}_src
    testssvep-stimuli.cpp
)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)
LINK_DIRECTORIES(${${PROJECT_NAME}_BINARY_DIR}/src)
LINK_DIRECTORIES(${CMAKE_INSTALL_PREFIX}/lib)

ADD_EXECUTABLE(${EXEC_NAME}
  ${${EXEC_NAME}_src}
  )

IF(UNIX)
TARGET_LINK_LIBRARIES(${EXEC_NAME} ${PROJECT_NAME} ${SFML_LIBRARIES} pthread)
ELSE(UNIX)
TARGET_LINK_LIBRARIES(${EXEC_NAME} "bci-interface.lib;sfml-oculus.lib;sfml-graphics.lib;sfml-window.lib;sfml-system.lib;glu32.lib;${OCULUS_LIB}")
ENDIF(UNIX)
TARGET_LINK_LIBRARIES(${EXEC_NAME} ${Boost_LIBRARIES})

# -- testssvep-interface 
SET(EXEC_NAME testssvep-interface)

SET(${EXEC_NAME}_src
    testssvep-interface.cpp
)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)
LINK_DIRECTORIES(${${PROJECT_NAME}_BINARY_DIR}/src)


ADD_EXECUTABLE(${EXEC_NAME}
  ${${EXEC_NAME}_src}
  )

IF(UNIX)
TARGET_LINK_LIBRARIES(${EXEC_NAME} ${PROJECT_NAME} ${SFML_LIBRARIES} pthread)
ELSE(UNIX)
TARGET_LINK_LIBRARIES(${EXEC_NAME} "bci-interface.lib;sfml-oculus.lib;sfml-graphics.lib;sfml-window.lib;sfml-system.lib;glu32.lib;${OCULUS_LIB}")
ENDIF(UNIX)
TARGET_LINK_LIBRARIES(${EXEC_NAME} ${Boost_LIBRARIES})

#testoculus-interface
SET(EXEC_NAME testoculus-interface)

SET(${EXEC_NAME}_src
    testoculus-interface.cpp
)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)
LINK_DIRECTORIES(${${PROJECT_NAME}_BINARY_DIR}/src)


ADD_EXECUTABLE(${EXEC_NAME}
  ${${EXEC_NAME}_src}
  )

IF(UNIX)
TARGET_LINK_LIBRARIES(${EXEC_NAME} ${PROJECT_NAME} ${SFML_LIBRARIES} pthread)
ELSE(UNIX)
TARGET_LINK_LIBRARIES(${EXEC_NAME} "bci-interface.lib;sfml-oculus.lib;sfml-graphics.lib;sfml-window.lib;sfml-system.lib;glu32.lib;${OCULUS_LIB}")
ENDIF(UNIX)
TARGET_LINK_LIBRARIES(${EXEC_NAME} ${Boost_LIBRARIES})

# -- testsquare-function 
SET(EXEC_NAME testsquare-function)

SET(${EXEC_NAME}_src
    testsquare-function.cpp
)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)
LINK_DIRECTORIES(${${PROJECT_NAME}_BINARY_DIR}/src)


ADD_EXECUTABLE(${EXEC_NAME}
  ${${EXEC_NAME}_src}
  )

IF(UNIX)
TARGET_LINK_LIBRARIES(${EXEC_NAME} ${PROJECT_NAME} ${SFML_LIBRARIES} pthread)
ELSE(UNIX)
TARGET_LINK_LIBRARIES(${EXEC_NAME} "bci-interface.lib;sfml-oculus.lib;sfml-graphics.lib;sfml-window.lib;sfml-system.lib;glu32.lib;${OCULUS_LIB}")
ENDIF(UNIX)
TARGET_LINK_LIBRARIES(${EXEC_NAME} ${Boost_LIBRARIES})

# -- testfontmanager 
SET(EXEC_NAME testfontmanager)

SET(${EXEC_NAME}_src
    testfontmanager.cpp
)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)
LINK_DIRECTORIES(${${PROJECT_NAME}_BINARY_DIR}/src)


ADD_EXECUTABLE(${EXEC_NAME}
  ${${EXEC_NAME}_src}
  )

IF(WIN32)
TARGET_LINK_LIBRARIES(${EXEC_NAME} "bci-interface.lib;sfml-graphics.lib;sfml-window.lib;sfml-system.lib;ws2_32.lib;OpenGL32.lib;glu32.lib")
ELSE(WIN32)
TARGET_LINK_LIBRARIES(${EXEC_NAME} bci-interface ${SFML_LIBRARIES} pthread)
ENDIF(WIN32)
TARGET_LINK_LIBRARIES(${EXEC_NAME} ${Boost_LIBRARIES})


# -- EchoServer
SET(EXEC_NAME EchoServer)

SET(${EXEC_NAME}_src
    EchoServer.cpp
)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)

ADD_EXECUTABLE(${EXEC_NAME}
  ${${EXEC_NAME}_src}
  )

IF(UNIX)
TARGET_LINK_LIBRARIES(${EXEC_NAME} ${PROJECT_NAME} ${SFML_LIBRARIES} pthread)
ELSE(UNIX)
TARGET_LINK_LIBRARIES(${EXEC_NAME} ${PROJECT_NAME} "sfml-oculus.lib;sfml-graphics.lib;sfml-window.lib;sfml-system.lib;glu32.lib;${OCULUS_LIB}")
ENDIF(UNIX)
TARGET_LINK_LIBRARIES(${EXEC_NAME} ${Boost_LIBRARIES})


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
#TARGET_LINK_LIBRARIES(${EXEC_NAME} "bci-interface.lib;sfml-graphics.lib;sfml-window.lib;sfml-system.lib;ws2_32.lib")
#ENDIF(UNIX)
#TARGET_LINK_LIBRARIES(${EXEC_NAME} ${Boost_LIBRARIES})
