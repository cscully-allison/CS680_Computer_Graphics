# - Try to find libxml
# Once done this will define
#
#  LIBXML_FOUND - system has libxml
#  LIBXML_INCLUDE_DIR - the libxml include directory
#  LIBXML_LIBRARIES - Link these to use libxml
#

message("<FindLIBXML.cmake>")

SET(LibXML++ "libxml++")

FIND_PATH(LIBXML_INCLUDE_DIR 
  NAMES libxml++/libxml++.h
  PATHS
  /usr/include/
  /usr/local/include/
  /opt/local/include/
  /usr/share/doc/
  /usr/lib/i386-linux-gnu/
  NO_DEFAULT_PATH

)
 

FIND_LIBRARY(LIBXML
  NAMES ${LIBXML}
  PATHS
  /usr/lib
  /usr/include/
  /usr/local/lib
  /opt/local/lib
  /usr/share/doc/
  /usr/lib/i386-linux-gnu/
  NO_DEFAULT_PATH
)

SET (LIBXML_LIBRARIES
  ${LIBLIBXML} 
)

IF(LIBXML_INCLUDE_DIR AND LIBXML_LIBRARIES)
   SET(LIBXML_FOUND TRUE)
   message("libxml found")
ENDIF(LIBXML_INCLUDE_DIR AND LIBXML_LIBRARIES)
   message("libxml not found")
IF(LIBXML_FOUND)
  MARK_AS_ADVANCED(LIBXML_INCLUDE_DIR LIBXML_LIBRARIES )
  message("libxml found")
ENDIF(LIBXML_FOUND)

message("</FindLIBXML.cmake>")
