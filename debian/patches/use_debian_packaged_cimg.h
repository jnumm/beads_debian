Author: Andreas Tille <tille@debian.org>
Last-Update: Thu, 19 Oct 2017 14:58:04 +0200
Description: Use Debian packaged CImg.h from cimg-dev package since
 the code copy shipped together with upstream is outdated and does not
 build with recent gcc

--- a/src/CMakeLists.txt
+++ b/src/CMakeLists.txt
@@ -35,7 +35,6 @@ SET( BEADS_CPP
 	spot_document_gnumeric
 	spotSvgDocument
 	images/imageDeNovo
-	CImg
 )
 IF(WIN32)
 ELSE(WIN32)
@@ -63,7 +62,7 @@ IF(WIN32)
 	target_link_libraries (beads ${EXTRA_CIMG_LIBRARY} ${QT_LIBRARIES} )
 ELSE(WIN32)
 	 # Link the executable to the Hello library.
-	target_link_libraries (beads ${X11_LIBRARIES} m ${PTHREADS_LIBRARY} ${EXTRA_CIMG_LIBRARY} ${QT_LIBRARIES} Qt5::Core Qt5::Widgets )
+	target_link_libraries (beads ${X11_LIBRARIES} m ${PTHREADS_LIBRARY} ${QT_LIBRARIES} Qt5::Core Qt5::Widgets )
 ENDIF(WIN32)
 
 SUBDIRS (qtbeads)
