# The following variables are set
#  CMAKE_THREAD_LIBS_INIT     - the thread library
#  CMAKE_USE_SPROC_INIT       - are we using sproc?
#  CMAKE_USE_WIN32_THREADS_INIT - using WIN32 threads?
#  CMAKE_USE_PTHREADS_INIT    - are we using pthreads
#  CMAKE_HP_PTHREADS_INIT     - are we using hp pthreads

IF( WIN32 )
	FIND_PACKAGE(Threads REQUIRED)
	
	
	#FIND_PATH(EXPAT_INCLUDE_DIR NAMES pthread.h /MinGW/include)
	#MARK_AS_ADVANCED(EXPAT_INCLUDE_DIR)
	#SET (EXPAT_INCLUDE_DIRS /MinGW/include)
	# Look for the library.
	
	
	#FIND_LIBRARY(CMAKE_THREAD_LIBS_INIT NAMES pthreadGCE2 PATHS /MinGW/lib)
	#SET(CMAKE_USE_PTHREADS_INIT 1)
	#MARK_AS_ADVANCED(CMAKE_USE_PTHREADS_INIT)
	
	
	#ADD_LIBRARY(pthreadGCE2 STATIC)
ELSE (WIN32)
	FIND_PACKAGE(Threads REQUIRED)
	#ADD_LIBRARY(pthread SHARED ${CMAKE_THREAD_LIBS_INIT})
ENDIF( WIN32 )