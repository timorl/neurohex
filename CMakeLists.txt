cmake_minimum_required(VERSION 2.8)

include(FindPkgConfig)

project(Neurohex)
set(Neurohex_VERSION_MAJOR 0)
set(Neurohex_VERSION_MINOR 0)

ADD_DEFINITIONS("-std=c++11")
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -O0 -g -lpthread")
set(CMAKE_CXX_FLAGS_RELEASE"${CMAKE_CXX_FLAGS} -O2")

set(CMAKE_BUILD_TYPE "Debug")

set(CMAKE_DEBUG_POSTFIX "_d")

set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/src)
set(PROJECT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(PROJECT_DOC_DIR ${CMAKE_CURRENT_BINARY_DIR}/doc)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})

set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/dist")

file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR})
file(MAKE_DIRECTORY ${PROJECT_DOC_DIR})
file(MAKE_DIRECTORY ${CMAKE_INSTALL_PREFIX})
file(MAKE_DIRECTORY ${CMAKE_INSTALL_PREFIX}/bin)

set(HDRS
	${PROJECT_SOURCE_DIR}/ui/Observable.hpp
	${PROJECT_SOURCE_DIR}/neuro/Tile.hpp
	${PROJECT_SOURCE_DIR}/neuro/Board.hpp
	${PROJECT_SOURCE_DIR}/neuro/Army.hpp
	${PROJECT_SOURCE_DIR}/neuro/Hand.hpp
	${PROJECT_SOURCE_DIR}/neuro/Player.hpp
	${PROJECT_SOURCE_DIR}/neuro/Contestant.hpp
	${PROJECT_SOURCE_DIR}/neuro/Arbiter.hpp
	${PROJECT_SOURCE_DIR}/neuro/Game.hpp
	${PROJECT_SOURCE_DIR}/neuro/GameOptions.hpp
	)

set(SRCS
	${PROJECT_SOURCE_DIR}/Main.cpp
	${PROJECT_SOURCE_DIR}/neuro/Board.cpp
	${PROJECT_SOURCE_DIR}/neuro/Army.cpp
	${PROJECT_SOURCE_DIR}/neuro/Hand.cpp
	${PROJECT_SOURCE_DIR}/neuro/Player.cpp
	${PROJECT_SOURCE_DIR}/neuro/Arbiter.cpp
	${PROJECT_SOURCE_DIR}/neuro/Game.cpp
	)

pkg_search_module(SDL2 REQUIRED sdl2)
find_package(Boost 1.54 REQUIRED)
find_package(Threads)

include_directories(
	${SDL2_INCLUDE_DIRS}
	${Boost_INCLUDE_DIR}
	${PROJECT_SOURCE_DIR}
	)

set(LIBS ${LIBS} ${SDL2_LIBRARIES} ${Boost_LIBRARIES})

link_libraries(${LIBS} ${CMAKE_THREAD_LIBS_INIT})

add_executable(Neurohex ${HDRS} ${SRCS})

install(TARGETS Neurohex
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
	CONFIGURATIONS Relase
	)

# Use gtest for testing.

find_package(GTest REQUIRED)
if(GTEST_FOUND)
	enable_testing()
	include_directories(${GTEST_INCLUDE_DIR})
	
	set(NEURO_TEST_DIR ${PROJECT_SOURCE_DIR}/neuro/test)
	set(NEURO_TEST_SRCS
		${PROJECT_SOURCE_DIR}/neuro/Board.cpp
		${NEURO_TEST_DIR}/BoardTest.cpp
		${NEURO_TEST_DIR}/TileTest.cpp
		)
	add_executable(neuroTest ${NEURO_TEST_SRCS})
	target_link_libraries(neuroTest
		${GTEST_LIBRARY}	${GTEST_MAIN_LIBRARY})
	add_test(NAME neuroTest
		COMMAND neuroTest)
	
endif()
# Use doxygen for documentation.

find_package(Doxygen)
if(DOXYGEN_FOUND)
	configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
		${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
	add_custom_target(doc
		${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
		WORKING_DIRECTORY ${PROJECT_DOC_DIR}
		COMMENT "Generating API documentation with Doxygen" VERBATIM
		)
endif()
