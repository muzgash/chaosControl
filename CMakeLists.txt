

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.0)

include( FindGSL.cmake )
# include( /usr/share/cmake-2.8/Modules/FindMPI.cmake )
# include( FindMPI )

# message(STATUS ${GSL_EXE_LINKER_FLAGS} )

INCLUDE_DIRECTORIES( include )
# LINK_DIRECTORIES( lib )

ADD_DEFINITIONS( -std=c++0x )

add_library(bif SHARED
include/bif.h
src/bif.cpp
)

ADD_EXECUTABLE( bifDiagram test/bD.cpp )

target_link_libraries(bifDiagram bif gsl gslcblas )

ADD_CUSTOM_TARGET( wipe 
 COMMAND rm -rf ../etc/*.dat ../etc/*.eps)
