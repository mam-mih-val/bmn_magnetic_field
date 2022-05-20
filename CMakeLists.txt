cmake_minimum_required(VERSION 3.0)
project(bmn_magnetic_field)

find_package(ROOT REQUIRED COMPONENTS RIO Tree)

include_directories(
        ${CMAKE_SOURCE_DIR}
        ${Boost_INCLUDE_DIRS}
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
        LINKDEF
        src/FieldLinkDef.h
        OPTIONS
        -I ${CMAKE_SOURCE_DIR}
        )
add_library(Field SHARED ${SOURCES} G__Field.cxx )
target_link_libraries(Field PRIVATE ROOT::RIO)

add_executable(process src/main.cc)
target_link_libraries(process ROOT::EG ROOT::RIO Field)