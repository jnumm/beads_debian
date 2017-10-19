
# cmake helps to compile "beads"
# please install the cmake package depending on your linux distribution
# and type in the beads root directory :
# cmake .
# make
# make install
cmake_minimum_required(VERSION 2.6)


# The name of our project is "beads".  CMakeLists files in this project can
# refer to the root source directory of the project as ${beads_SOURCE_DIR} and
# to the root binary directory of the project as ${beads_BINARY_DIR}.
PROJECT(beads CXX C)


#message ("compiler : ${CMAKE_CXX_COMPILER} ")


IF ( CMAKE_BASE_NAME MATCHES "cl")
	message ("on est sous Windows")
	SET( WIN32 "true")
ENDIF(CMAKE_BASE_NAME MATCHES "cl")


SET( CMAKE_BUILD_TYPE "Release")
if (NOT CMAKE_INSTALL_PREFIX)
	SET (CMAKE_INSTALL_PREFIX /usr/local)
ENDIF(NOT CMAKE_INSTALL_PREFIX)

IF (CMAKE_INSTALL_PREFIX MATCHES "/usr/local")
	SET( CMAKE_BUILD_TYPE "Debug")
	#SET( CMAKE_BUILD_TYPE "Release")
ENDIF (CMAKE_INSTALL_PREFIX MATCHES "/usr/local")


IF ( CMAKE_BUILD_TYPE MATCHES "Release")
	MESSAGE("compiling as release version")
	ADD_DEFINITIONS("-DQT_NO_DEBUG_OUTPUT")
ELSE ( CMAKE_BUILD_TYPE MATCHES "Release" )
	MESSAGE("compiling as debug version")
ENDIF( CMAKE_BUILD_TYPE MATCHES "Release" )



#SET(WIN32 "true")
#SET(MINGW "true")

set(CMAKE_MODULE_PATH ${beads_SOURCE_DIR}/cmake_modules)
#FIND_PACKAGE ( FindPTHREADS REQUIRED )
#are we using pthreads ?
#IF ( CMAKE_USE_PTHREADS_INIT )
#	INCLUDE_DIRECTORIES ( ${EXPAT_INCLUDE_DIRS} )
#ELSE ( CMAKE_USE_PTHREADS_INIT )
#	MESSAGE( FATAL_ERROR "No pthread installed" )
#ENDIF( CMAKE_USE_PTHREADS_INIT )

#IF ( PTHREADS_FOUND )
	INCLUDE_DIRECTORIES ( ${PTHREADS_INCLUDE_DIR} )
#ELSE ( PTHREADS_FOUND )
#	MESSAGE( FATAL_ERROR "No pthread installed" )
#ENDIF( PTHREADS_FOUND )


cmake_minimum_required(VERSION 2.6)


SET ( EXTRA_CIMG_LIBRARY "")

SET ( CIMG_OS 0)
IF(WIN32)
	SET ( CIMG_OS 2)
	#-O2 -lgdi32
	#SET ( EXTRA_CIMG_LIBRARY "gdi32")
	#SET(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")
#	FIND_PACKAGE(FindMagick REQUIRED)
ELSE(WIN32)
	FIND_PACKAGE(Pthreads REQUIRED)
	SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
	SET ( CIMG_OS 1)
ENDIF(WIN32)

#
#CMAKE_CXX_FLAGS
SET(CMAKE_CXX_FLAGS_DEBUG "-g")
SET(CMAKE_CXX_FLAGS_MINSIZEREL "-Os -DNDEBUG")
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

SET (BEADS_VERSION "1.1.13")

SET(CPACK_CMAKE_GENERATOR "Unix Makefiles")
SET(CPACK_GENERATOR "STGZ;TGZ;TZ")
#SET(CPACK_INSTALL_CMAKE_PROJECTS "/tmp;beads;src;/src")
#SET(CPACK_NSIS_DISPLAY_NAME "CMake 2.5")
SET(CPACK_OUTPUT_CONFIG_FILE "./CPackConfig.cmake")
SET(CPACK_PACKAGE_DESCRIPTION_FILE ${beads_SOURCE_DIR}/COPYING)
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "beads is a detection software")
SET(CPACK_PACKAGE_EXECUTABLES "beads")
SET(CPACK_SOURCE_PACKAGE_FILE_NAME "beads-${BEADS_VERSION}")
SET(CPACK_PACKAGE_FILE_NAME "beads-${BEADS_VERSION}-Linux-i686")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "beads ${BEADS_VERSION}")
SET(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "beads ${BEADS_VERSION}")
SET(CPACK_PACKAGE_NAME "beads")
SET(CPACK_PACKAGE_VENDOR "CNRS")

IF ( CMAKE_BUILD_TYPE MATCHES "Debug")
	SET (BEADS_VERSION "${BEADS_VERSION}d")
ENDIF( CMAKE_BUILD_TYPE MATCHES "Debug")
SET(CPACK_PACKAGE_VERSION ${BEADS_VERSION})
SET(CPACK_PACKAGE_VERSION_MAJOR "1")
SET(CPACK_PACKAGE_VERSION_MINOR "1")
SET(CPACK_PACKAGE_VERSION_PATCH "13")
SET(CPACK_RESOURCE_FILE_LICENSE ${beads_SOURCE_DIR}/COPYING)
SET(CPACK_RESOURCE_FILE_README ${beads_SOURCE_DIR}/README)
SET(CPACK_RESOURCE_FILE_WELCOME ${beads_SOURCE_DIR}/INSTALL)
SET(CPACK_SOURCE_GENERATOR "TGZ;TZ")
SET(CPACK_SOURCE_OUTPUT_CONFIG_FILE "./CPackSourceConfig.cmake")
SET(CPACK_SOURCE_STRIP_FILES "")
SET(CPACK_STRIP_FILES "bin/beads")
SET(CPACK_SYSTEM_NAME "Linux-i686")
SET(CPACK_TOPLEVEL_TAG "Linux-i686")

SET(CPACK_SOURCE_IGNORE_FILES 
	"moc_.*cxx"
	"/devel_archives/"
	"/figures/"
	"/Soumis/"
	"/samples/"
	"/win32/"
	"/tests/"
	"Makefile"
	"install_manifest.txt"
	"cmake_install.cmake"
	"cmake_install.cmake"
	"CMakeCache.txt"
	"CPackConfig.cmake"
	"CPackSourceConfig.cmake"
	"install_manifest.txt"
	"/CMakeFiles/"
	"/_CPack_Packages/"
	"/Debug/"
	"/Release/"
	"/\\\\.externalToolBuilders/"
	"/\\\\.svn/"
	"/\\\\.settings/"
	"Makefile"
	"\\\\.cdtbuild"
	"\\\\.cdtproject"
	"\\\\.project"
	"\\\\.cproject"
	"beads$"
	".*\\\\.tar\\\\.gz"
)
# cpack -G TGZ --config CPackSourceConfig.cmake
# dpkg-buildpackage -rfakeroot

#.dput.cf
#[olivier-langella]
#fqdn = ppa.launchpad.net
#method = ftp
#incoming = ~olivier-langella/ubuntu/
#login = olivier-langella
#allow_unsigned_uploads = 0
#
#  debuild -S -sa
# dput -f olivier-langella *changes

configure_file (${beads_SOURCE_DIR}/src/config.h.cmake ${beads_SOURCE_DIR}/src/config.h)


SET(CPACK_PACKAGE_EXECUTABLES "beads" "beads")


INCLUDE(CPack)


#INCLUDE(CPack)


SET(CPACK_STRIP_FILES "src/beads;src/qtbeads/qtbeads")


# Recurse into the "src" subdirectories.  This does not actually
# cause another cmake executable to run.  The same process will walk through
# the project's entire directory structure.
SUBDIRS (src)

INSTALL(PROGRAMS src/beads DESTINATION bin)
INSTALL(PROGRAMS src/qtbeads/qtbeads DESTINATION bin)
#INSTALL(DIRECTORY doc/man1 DESTINATION share/man/man1)
INSTALL(FILES doc/man1/beads.1 DESTINATION share/man/man1)
INSTALL(FILES share/applications/beads.desktop DESTINATION share/applications)
INSTALL(FILES share/beads/beads_blue.conf DESTINATION share/beads)
INSTALL(FILES share/beads/beads_silver.conf DESTINATION share/beads)
INSTALL(FILES share/beads/beads_silver_small.conf DESTINATION share/beads)
INSTALL(FILES share/beads/beads_icon.svg DESTINATION share/beads)

