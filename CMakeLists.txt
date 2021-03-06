cmake_minimum_required(VERSION "3.21")

set(CMAKE_INSTALL_DATADIR "/usr/local/")
project ("itlib")

# add_subdirectory("libs")

set(MAIN_PROJECT OFF)
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(MAIN_PROJECT ON)
endif()

find_package(PostgreSQL)

set(SRCS dates.cc gdbm_api.cc str2.cc system.cc)
set(HEADERS benchmark.hxx dates.hxx gdbm_api.hxx str2.hxx system.hxx)
set(LIBS gdbm)

if (${PostgreSQL_FOUND})
    set(SRCS ${SRCS} pgdb.cc)
    set(HEADERS ${HEADERS} ${PostgreSQL_LIBRARIES})
    set(LIBS ${LIBS} pqxx)
endif()
# file(GLOB SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.cc")
# file(GLOB HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.hxx")
# include_directories(${CMAKE_CURRENT_SOURCE_DIR})


add_library(itlib ${SRCS})
target_include_directories(itlib PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
set_target_properties(itlib PROPERTIES PUBLIC_HEADER "${HEADERS}")
target_link_libraries(itlib ${LIBS})

if (MAIN_PROJECT STREQUAL "ON")
install(TARGETS itlib PUBLIC_HEADER DESTINATION "/usr/local/include/")

install(TARGETS
      itlib
    EXPORT itlibConfig
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
export(TARGETS
        itlib
    NAMESPACE itlib::
    FILE "${CMAKE_CURRENT_BINARY_DIR}/itlibConfig.cmake"
)
install(EXPORT
        itlibConfig
    DESTINATION "${CMAKE_INSTALL_DATADIR}/itlib/cmake"
    NAMESPACE itlib::
)
endif()