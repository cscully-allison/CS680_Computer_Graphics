# - Try to find Assimp
# Once done this will define
#
#  GLUT_FOUND - system has GLUT
#  GLUT_INCLUDE_DIR - the GLUT include directory
#  GLUT_LIBRARIES - Link these to use GLUT
#

SET(GLUT "glut")

FIND_PATH(GLUT_INCLUDE_DIR NAMES GLUT/ai_assert.h
  PATHS
  /usr/include
  /usr/local/include
  /opt/local/include
  /usr/share/doc/
  /usr/lib/i386-linux-gnu/
  NO_DEFAULT_PATH
)
 

FIND_LIBRARY(LIBGLUT
  NAMES 
  ${GLUT}
  PATHS
  /usr/lib
  /usr/include/
  /usr/local/lib
  /opt/local/lib
  /usr/share/doc/
  /usr/lib/i386-linux-gnu/
  NO_DEFAULT_PATH
)

SET (GLUT_LIBRARIES
  ${LIBGLUT} 
)

IF(GLUT_INCLUDE_DIR AND GLUT_LIBRARIES)
   SET(GLUT_FOUND TRUE)
ENDIF(GLUT_INCLUDE_DIR AND GLUT_LIBRARIES)

IF(GLUT_FOUND)
  MARK_AS_ADVANCED(GLUT_INCLUDE_DIR GLUT_LIBRARIES )
ENDIF(GLUT_FOUND)
