
 # this command finds Qt4 libraries and sets all required variables
  # note that it's Qt4, not QT4 or qt4
FIND_PACKAGE( Qt4 REQUIRED )

  
  # add some useful macros and variables
  # (QT_USE_FILE is a variable defined by FIND_PACKAGE( Qt4 ) that contains a path to CMake script)
INCLUDE( ${QT_USE_FILE} )

SET( BEADS_CPP
	beads.cpp
	ConfigFile/ConfigFile
	parameters
	encode/base64
	images/imageIntensity.cpp
	images/imageDirect.cpp
	images/imageCode
	images/imageCoule
	images/imagePaths
	images/imageConfluent
	images/imageProb
	images/parameterProb
	parameterDetection
	images/parameterConfluent
	detection
	images/imageDetection
	images/imageNumber
	images/parameterNumber
	images/imageContours
	spotPROTICdbDocument
	spot
	spotDocument
	spot_document_gnumeric
	spotSvgDocument
	images/imageDeNovo
	CImg
)
IF(WIN32)
ELSE(WIN32)
	FIND_PACKAGE(X11 REQUIRED)
	
	INCLUDE_DIRECTORIES(X11_INCLUDE_DIR)
	# X11_FOUND is true if X11 is available.
	#    * X11_INCLUDE_DIR contains the include directories to use X11.
	#    * X11_LIBRARIES points to the libraries to link against to use X11.
	# Make sure the compiler can find include files from our Hello library.
	include_directories (${X11_INCLUDE_DIR})
	
	ADD_DEFINITIONS(${PTHREADS_DEFINITIONS})
	# -m -pthread FindThreads
	
	# Make sure the linker can find the Hello library once it is built.
	link_directories (${X11_INCLUDE_DIR})
ENDIF(WIN32)


add_executable (beads ${BEADS_CPP})

IF(WIN32)
	 # Link the executable to the Hello library.
	target_link_libraries (beads ${EXTRA_CIMG_LIBRARY} ${QT_LIBRARIES} )
ELSE(WIN32)
	 # Link the executable to the Hello library.
	target_link_libraries (beads ${X11_LIBRARIES} m ${PTHREADS_LIBRARY} ${EXTRA_CIMG_LIBRARY} ${QT_LIBRARIES} )
ENDIF(WIN32)

SUBDIRS (qtbeads)