
 # this command finds Qt4 libraries and sets all required variables
  # note that it's Qt4, not QT4 or qt4
FIND_PACKAGE( Qt4 REQUIRED )

  
  # add some useful macros and variables
  # (QT_USE_FILE is a variable defined by FIND_PACKAGE( Qt4 ) that contains a path to CMake script)
INCLUDE( ${QT_USE_FILE} )

SET( BEADS_FOR_QT_CPP
	../ConfigFile/ConfigFile
	../parameters
	../encode/base64
	../images/imageIntensity.cpp
	../images/imageDirect.cpp
	../images/imageCode
	../images/imageCoule
	../images/imagePaths
	../images/imageConfluent
	../images/imageProb
	../images/parameterProb
	../parameterDetection
	../images/parameterConfluent
	../detection
	../images/imageDetection
	../images/imageNumber
	../images/parameterNumber
	../images/imageContours
	../spotPROTICdbDocument
	../spot
	../spotDocument
	../spot_document_gnumeric
	../spotSvgDocument
	../images/imageDeNovo
	../CImg
)
  
# with SET() command you can change variables or define new ones
  # here we define SAMPLE_SRCS variable that contains a list of all .cpp files
  # note that we don't need \ at the end of line
SET( QTBEADS_SRCS
	qtbeads.cpp
	main_window.cpp
	properties.cpp
	beads_results.cpp
	QCimg.cpp
	q_gel_image.cpp
	q_gel_image_scroll.cpp
#	qtbeads_error.h
)
  
  # another list, this time it includes all header files that should be treated with moc
SET( QTBEADS_MOC_HDRS
	main_window.h
	q_gel_image.h
)  
  # some .ui files
  #SET( SAMPLE_UIS
  #     ./src/ui/Dialog1.ui
  #     ./src/ui/Dialog2.ui
  #)
  
  # and finally an resource file
  #SET( SAMPLE_RCS
  #     ./src/rc/sample.qrc
  #)
  
  # enable warnings
ADD_DEFINITIONS( -Wall )
  
  # by default only QtCore and QtGui modules are enabled
  # other modules must be enabled like this:
 #SET( QT_USE_QT3SUPPORT TRUE )   
 # SET( QT_USE_QTXML TRUE )
    
  
#****************************
#I18N
# Translation files
SET(GLOB TRANS translations/*.ts)
# add translations ...
QT4_ADD_TRANSLATION(QM ${TRANS})



  
#set(CMAKE_MODULE_PATH ${abiftake_SOURCE_DIR}/Modules)
#set(Qwt5_DIR ${abiftake_SOURCE_DIR}/Modules)

#FIND_PACKAGE(Qwt5 REQUIRED) 
  
  
  
  # this command will generate rules that will run rcc on all files from SAMPLE_RCS
  # in result SAMPLE_RC_SRCS variable will contain paths to files produced by rcc
QT4_ADD_RESOURCES( QTBEADS_SRCS ${SAMPLE_RCS} )
  
  # this will run uic on .ui files:
  #QT4_WRAP_UI( SAMPLE_UI_HDRS ${SAMPLE_UIS} )
  
  # and finally this will run moc:
QT4_WRAP_CPP( QTBEADS_MOC_SRCS ${QTBEADS_MOC_HDRS} )
  
  # we need this to be able to include headers produced by uic in our code
  # (CMAKE_BINARY_DIR holds a path to the build directory, while INCLUDE_DIRECTORIES() works just like INCLUDEPATH from qmake)
  #INCLUDE_DIRECTORIES( ${CMAKE_BINARY_DIR} ${Qwt5_INCLUDE_DIR})
INCLUDE_DIRECTORIES( ${CMAKE_BINARY_DIR})
  
  # here we instruct CMake to build "sample" executable from all of the source files
ADD_EXECUTABLE( qtbeads ${BEADS_FOR_QT_CPP} ${QTBEADS_SRCS} ${QTBEADS_MOC_SRCS} ${SAMPLE_RC_SRCS} ${SAMPLE_UI_HDRS} ${QM} )


  # last thing we have to do is to tell CMake what libraries our executable needs,
  # luckily FIND_PACKAGE prepared QT_LIBRARIES variable for us:
  #TARGET_LINK_LIBRARIES( abiftake ${QT_LIBRARIES} ${Qwt5_Qt4_LIBRARY} ${EXTRA_CIMG_LIBRARY})

IF(WIN32)
	TARGET_LINK_LIBRARIES( qtbeads ${QT_LIBRARIES} ${EXTRA_CIMG_LIBRARY})
ELSE(WIN32)
	TARGET_LINK_LIBRARIES( qtbeads ${QT_LIBRARIES} m ${PTHREADS_LIBRARY} ${EXTRA_CIMG_LIBRARY})
ENDIF(WIN32)

  
