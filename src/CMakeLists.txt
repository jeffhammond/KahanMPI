file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.c")
add_library(kahanmpi ${LIB_LINKAGE_TYPE} ${SOURCES})
