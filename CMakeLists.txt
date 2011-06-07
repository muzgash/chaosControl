

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.0)

include( FindGSL.cmake )

# message(STATUS ${GSL_EXE_LINKER_FLAGS} )

INCLUDE_DIRECTORIES( include )
# LINK_DIRECTORIES( lib )

ADD_DEFINITIONS( -std=c++0x )

add_library(chua SHARED
include/chua.h
src/chua.cpp
)

ADD_EXECUTABLE( trajectory test/find.cpp )

target_link_libraries(trajectory chua gsl gslcblas )

ADD_CUSTOM_TARGET( wipe 
 COMMAND rm -rf ../etc/*.dat ../etc/*.eps)
