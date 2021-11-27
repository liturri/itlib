cmake_minimum_required(VERSION "3.21")


set(CMAKE_INSTALL_DATADIR "/usr/local/")
project ("itlib")

# add_subdirectory("libs")

file(GLOB SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.cc")
file(GLOB HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.hxx")
include_directories(${CMAKE_CURRENT_SOURCE_DIR})


add_library(itlib SHARED ${SRCS})
set_target_properties(itlib PROPERTIES PUBLIC_HEADER "${HEADERS}")
target_link_libraries(itlib pqxx gdbm)

install(TARGETS itlib PUBLIC_HEADER DESTINATION "/usr/local/include/")
add_subdirectory(test)


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