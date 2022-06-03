cmake_minimum_required(VERSION 3.20)

project(cmake-project-template)

set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3")

# set(CMAKE_FIND_DEBUG_MODE TRUE)
find_package(Boost REQUIRED)

add_executable(BoostPolygon1 example1.cpp)
target_link_libraries(BoostPolygon1 ${Boost_LIBRARIES})
target_include_directories(BoostPolygon1 PUBLIC ${Boost_INCLUDE_DIRS})

add_executable(BoostPolygon2 example2.cpp)
target_link_libraries(BoostPolygon2 ${Boost_LIBRARIES})
target_include_directories(BoostPolygon2 PUBLIC ${Boost_INCLUDE_DIRS})

add_executable(BoostPolygonMain main.cpp)
target_link_libraries(BoostPolygonMain ${Boost_LIBRARIES})
target_include_directories(BoostPolygonMain PUBLIC ${Boost_INCLUDE_DIRS})