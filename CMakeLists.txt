cmake_minimum_required(VERSION 3.0)
project(bmn_magnetic_field)
find_package(ROOT REQUIRED COMPONENTS RIO Core)

set(CMAKE_CXX_STANDARD 17)
include(${ROOT_USE_FILE})

include_directories(
        ${CMAKE_SOURCE_DIR}
        ${ROOT_INCLUDE_DIRS}
)
set(SOURCES
        src/data_parser.cc
        src/field_plane.cc
        src/field_point.cc
        )
set(HEADERS
        src/data_parser.h
        src/field_plane.h
        src/field_point.h
        )

set(PCM_FILE_NAME libField)
ROOT_GENERATE_DICTIONARY(G__Field ${HEADERS}
        LINKDEF src/FieldLinkDef.h)
add_library(Field SHARED ${SOURCES} G__Field.cxx)
target_link_libraries(Field PUBLIC ROOT::RIO ROOT::Core)

add_executable(process src/main.cc)
target_link_libraries(process ROOT::Core ROOT::RIO Field)