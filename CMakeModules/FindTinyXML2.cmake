#-------------------------------------------
#  TinyXML2_FOUND
#  TinyXML2_INCLUDE_DIR
#  TinyXML2_LIBRARIES
#------------------------------------------

FIND_PATH(TinyXML2_INCLUDE_DIR tinyxml2.h
	HINTS $ENV{TinyXML2_DIR}
	PATH_SUFFIXES include
	PATHS /usr/local
	      /usr
)

FIND_LIBRARY(TinyXML2_LIBRARIES
	tinyxml2
	HINTS $ENV{TinyXML2_DIR}
	PATH_SUFFIXES lib64 lib
	PATHS /usr/local
	      /usr
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(TinyXML2 DEFAULT_MSG TinyXML2_LIBRARIES)

MARK_AS_ADVANCED(TinyXML2_LIBRARIES TinyXML2_INCLUDE_DIR)
