
FILE(GLOB L1 "[0-9a-zA-Z_]*.cc")
FILE(GLOB L2 "*/[0-9a-zA-Z_]*.cc")

list(APPEND EXTRA_LIBS  )

include_directories("..") # scheme source dir
include_directories(".")
link_directories(${CMAKE_LIBRARY_PATH})

#if ( ${USEHDF5} )
#    FIND_PACKAGE(HDF5 REQUIRED COMPONENTS C CXX HL)
#    message("bcov"  ${HDF5_LIBRARIES} )
#    message("bcov2"  ${HDF5_INCLUDE_DIRS} )
#    INCLUDE_DIRECTORIES(${HDF5_INCLUDE_DIRS})
#    link_directories( ${HDF5_LIBRARY_DIRS} )
#endif()

add_library(riflib SHARED ${L1} ${L2} ${L3} ${L4} ${L5} )
target_link_libraries ( riflib ${ALL_ROSETTA_LIBS} )

install ( TARGETS riflib LIBRARY DESTINATION lib )
