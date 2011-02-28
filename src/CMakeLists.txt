SET(EXEC_NAME bci-interface)

SET(${EXEC_NAME}_src
    ./square-function.cpp
    ./SSVEPInterface.cpp
    ./SSVEPInterfaceConfig.cpp
    ./FlickeringSquare.cpp
    ./BackgroundSprite.cpp
    ./P300Object.cpp
    ./P300ObjectVector.cpp
    ./P300Interface.cpp
    ./MoovingCursor.cpp
    ./RandomPoint.cpp
    ./HybridInterface.cpp
)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)

IF(COSHELLBCI_FOUND)
    SET(IDL_STUBS_DIRECTORY ${PROJECT_BINARY_DIR}/stubs)
    FILE(MAKE_DIRECTORY ${IDL_STUBS_DIRECTORY})
    
    SET(IDL_INCLUDE_DIR "${OPENHRP_HOME}/Common/corba" )
    
    # IDL File rules
    INCLUDE_DIRECTORIES(${IDL_STUBS_DIRECTORY})
    
    IF(DGMIDDLEWARE_FOUND)
    
      MESSAGE(STATUS "DGMIDDLEWARE_DATADIR: ${DGMIDDLEWARE_DATADIR}")
      IDLFILERULE(${DGMIDDLEWARE_DATADIR}/server-command-corba.idl 
          ${IDL_STUBS_DIRECTORY}/server-command-corbaSK.cc 
          ${IDL_STUBS_DIRECTORY}/server-command-corba.h  
          ${IDL_STUBS_DIRECTORY}
          ${IDL_INCLUDE_DIR} )
    
      SET( ${EXEC_NAME}_src ${${EXEC_NAME}_src} ${IDL_STUBS_DIRECTORY}/server-command-corbaSK.cc)
      SET( ${EXEC_NAME}_headers ${${EXEC_NAME}_headers} ${IDL_STUBS_DIRECTORY}/server-command-corba.h)
    
    ENDIF(DGMIDDLEWARE_FOUND)
ENDIF(COSHELLBCI_FOUND)

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

# Add bci-middleware cflags and link flags
ADD_DEFINITIONS(${BCIMIDDLEWARE_CFLAGS})
TARGET_LINK_LIBRARIES(${EXEC_NAME} ${BCIMIDDLEWARE_LDFLAGS})

# Add coshell-bci cflags and link flags
ADD_DEFINITIONS(${COSHELLBCI_CFLAGS})
TARGET_LINK_LIBRARIES(${EXEC_NAME} ${COSHELLBCI_LIBS})

INSTALL(TARGETS ${EXEC_NAME}
    DESTINATION lib)
