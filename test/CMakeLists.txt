include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_CXX_STANDARD 98)

# find_package(Boost 1.70 COMPONENTS date_time)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(pru pru.cc)
target_link_libraries(pru itlibs)

add_executable(pru01 pru01.cc)
target_link_libraries(pru01 itlibs)

add_executable(test_str2 test_str2.cc)
target_link_libraries(test_str2 itlibs)
