cmake_minimum_required(VERSION "3.21")

set(CMAKE_INSTALL_DATADIR "/usr/local/")
project ("itlib")

# add_subdirectory("libs")

find_package(PostgreSQL)

set(SRCS dates.cc gdbm_api.cc str2.cc system.cc)
set(LIBS gdbm)
if (${PostgreSQL_FOUND})
    set(SRCS ${SRC} pgdb.cc)
    set(LIBS ${LIBS} pqxx)
endif()
# file(GLOB SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.cc")
# file(GLOB HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.hxx")
# include_directories(${CMAKE_CURRENT_SOURCE_DIR})


add_library(itlib ${SRCS})
target_include_directories(itlib PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
set_target_properties(itlib PROPERTIES PUBLIC_HEADER "${HEADERS}")
target_link_libraries(itlib ${LIBS})

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