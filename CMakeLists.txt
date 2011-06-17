

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.0)

include( FindGSL.cmake )
# include( /usr/share/cmake-2.8/Modules/FindMPI.cmake )
include( FindMPI )

# message(STATUS ${GSL_EXE_LINKER_FLAGS} )

INCLUDE_DIRECTORIES( include )
# LINK_DIRECTORIES( lib )

ADD_DEFINITIONS( -std=c++0x )

add_library(rplot SHARED
include/rPlot.h
src/rPlot.cpp
)

ADD_EXECUTABLE( rPlot test/plot.cpp )

target_link_libraries(rPlot ${MPI_LIBRARIES} rplot gsl gslcblas )

ADD_CUSTOM_TARGET( wipe 
 COMMAND rm -rf ../etc/*.dat ../etc/*.eps)
