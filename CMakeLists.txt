

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.0)

include( FindGSL.cmake )

# message(STATUS ${GSL_EXE_LINKER_FLAGS} )

INCLUDE_DIRECTORIES( include )
# LINK_DIRECTORIES( lib )

ADD_DEFINITIONS( -std=c++0x )

add_library(rossler SHARED
include/rossler.h
src/rossler.cpp
)

ADD_EXECUTABLE( pyragas test/find.cpp )

target_link_libraries(pyragas rossler gsl gslcblas )

ADD_CUSTOM_TARGET( wipe 
 COMMAND rm -rf ../etc/*.dat ../etc/*.eps)
