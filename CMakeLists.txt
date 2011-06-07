#
#  Copyright
#

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(PROJECT_NAME bci-interface)
project(${PROJECT_NAME})

SET(PROJECT_VERSION 0.1)

SET(CMAKE_VERBOSE_MAKEFILE true)

IF(WIN32)
  SET(LIBDIR_KW "/LIBPATH:")
  SET(LIBINCL_KW "")
  SET(LIB_EXT ".lib")
ENDIF(WIN32)

IF(UNIX)
  SET(LIBDIR_KW "-L")
  SET(LIBINCL_KW "-l")
  SET(LIB_EXT "")
ENDIF(UNIX)

#
#  Specific CmakeModules
#
SET(CMAKE_MODULE_PATH ${${PROJECT_NAME}_SOURCE_DIR}/CMakeModules)

#
#  Look for dependencies
#

# Load Pkgconfig macros
INCLUDE(FindPkgConfig)
SET(PACKAGE_REQUIREMENTS "")

#
#  Dependecy to OMNIORB4
#
INCLUDE(${CMAKE_MODULE_PATH}/UseOmniORB4.cmake)

MESSAGE(STATUS "omniORB4_cflags: ${omniORB4_cflags}")
MESSAGE(STATUS "omniORB4_link_flags: ${omniORB4_link_FLAGS}")

SET(Boost_FOUND 0)
FIND_PACKAGE(Boost COMPONENTS thread)
IF(Boost_FOUND)
    MESSAGE(STATUS "Found Boost")
    MESSAGE(STATUS "Boost_INCLUDE_DIRS ${Boost_INCLUDE_DIRS}")
	INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
	IF(UNIX)
		LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
	ENDIF(UNIX)
ENDIF(Boost_FOUND)

#
#  Dependency on dg-middleware
#
pkg_check_modules(DGMIDDLEWARE dg-middleware)

MESSAGE(STATUS "PKG_CONFIG_EXEC: ${PKG_CONFIG_EXECUTABLE}")
EXEC_PROGRAM(${PKG_CONFIG_EXECUTABLE} ARGS dg-middleware --variable=datarootdir
  OUTPUT_VARIABLE DGMIDDLEWARE_DATADIR)

#
# Dependency on bci-middle ware
#
pkg_check_modules(BCIMIDDLEWARE bci-middleware)

EXEC_PROGRAM(${PKG_CONFIG_EXECUTABLE} ARGS bci-middleware --cflags
  OUTPUT_VARIABLE BCIMIDDLEWARE_CFLAGS)
MESSAGE(STATUS "BCIMIDDLEWARE_CFLAGS: ${BCIMIDDLEWARE_CFLAGS}")
EXEC_PROGRAM(${PKG_CONFIG_EXECUTABLE} ARGS bci-middleware --libs
  OUTPUT_VARIABLE BCIMIDDLEWARE_LIBS)
MESSAGE(STATUS "BCIMIDDLEWARE_LIBS: ${BCIMIDDLEWARE_LIBS}")

#
# Dependency on coshell-bci 
#
pkg_check_modules(COSHELLBCI coshell-bci)

IF(COSHELLBCI_FOUND)
ADD_DEFINITIONS(-DWITH_COSHELL)
EXEC_PROGRAM(${PKG_CONFIG_EXECUTABLE} ARGS coshell-bci --cflags
  OUTPUT_VARIABLE COSHELLBCI_CFLAGS)
MESSAGE(STATUS "COSHELLBCI_CFLAGS: ${COSHELLBCI_CFLAGS}")
EXEC_PROGRAM(${PKG_CONFIG_EXECUTABLE} ARGS coshell-bci --libs
  OUTPUT_VARIABLE COSHELLBCI_LIBS)
MESSAGE(STATUS "COSHELLBCI_LIBS: ${COSHELLBCI_LIBS}")
ENDIF(COSHELLBCI_FOUND)

IF(WIN32)
  ADD_DEFINITIONS("-D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS /wd4251 /wd4275 /wd4101 /wd4355")
ENDIF(WIN32)

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(include)
ADD_SUBDIRECTORY(test)
ADD_SUBDIRECTORY(application)

#
# pkg-config file
#

SET(install_pkg_prefix "\${prefix}")
SET(install_pkg_exec_prefix "\${exec_prefix}")
SET(install_pkg_libdir "\${libdir}")
SET(install_pkg_datarootdir "\${datarootdir}")
SET(install_pkg_include_dir "\${includedir}")

SET(libs-to-pc
        bci-middleware
)

FOREACH(lib ${libs-to-pc})
    SET(pc-libs "${pc-libs} ${LIBINCL_KW}${lib}${CMAKE_IMPORT_LIBRARY_SUFFIX}")
ENDFOREACH(lib)

IF(UNIX)
  SET (${PROJECT_NAME}_LDFLAGS "-Wl,-R${install_pkg_libdir} ${pc-libs} ${${PROJECT_NAME}_LDFLAGS}")
ENDIF(UNIX)

IF(WIN32)
  SET (${PROJECT_NAME}_LDFLAGS "${pc-libs}")
ENDIF(WIN32)

# Install the file in the appropriate place.
SET(install_libdir_pkgconfig ${CMAKE_INSTALL_PREFIX}/lib/pkgconfig)

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.pc.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc)

INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc
  DESTINATION ${install_libdir_pkgconfig}   
  PERMISSIONS OWNER_READ GROUP_READ WORLD_READ OWNER_WRITE)
