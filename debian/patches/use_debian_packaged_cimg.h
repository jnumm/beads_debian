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
--- a/src/qtbeads/CMakeLists.txt
+++ b/src/qtbeads/CMakeLists.txt
@@ -33,7 +33,6 @@ SET( BEADS_FOR_QT_CPP
 	../spot_document_gnumeric
 	../spotSvgDocument
 	../images/imageDeNovo
-	../CImg
 )
   
 # with SET() command you can change variables or define new ones
--- a/src/images/imageCode.h
+++ b/src/images/imageCode.h
@@ -3,8 +3,8 @@
 
 #include "../config.h"
 
-#include "../CImg.h"
-//#include <CImg.h>
+//#include "../CImg.h"
+#include <CImg.h>
 
 //Images pour lesquelles les valeurs des pixels sont des codes et non des valeurs quantitatives
 //Sont des codes: l'image des directions, l'image des numeros
