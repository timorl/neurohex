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

set(COMMON_HDRS
	${PROJECT_SOURCE_DIR}/ui/Observable.hpp
	${PROJECT_SOURCE_DIR}/neuro/Tile.hpp
	${PROJECT_SOURCE_DIR}/neuro/Board.hpp
	${PROJECT_SOURCE_DIR}/neuro/Army.hpp
	${PROJECT_SOURCE_DIR}/neuro/Hand.hpp
	${PROJECT_SOURCE_DIR}/neuro/Player.hpp
	${PROJECT_SOURCE_DIR}/neuro/Game.hpp
	${PROJECT_SOURCE_DIR}/utility/Environment.hpp
	${PROJECT_SOURCE_DIR}/utility/DFStyleReader.hpp
	${PROJECT_SOURCE_DIR}/utility/DFStyleCreator.hpp
	${PROJECT_SOURCE_DIR}/utility/DFDirectoryReader.hpp
	)

set(COMMON_SRCS
	${PROJECT_SOURCE_DIR}/neuro/Tile.cpp
	${PROJECT_SOURCE_DIR}/neuro/Board.cpp
	${PROJECT_SOURCE_DIR}/neuro/Army.cpp
	${PROJECT_SOURCE_DIR}/neuro/Hand.cpp
	${PROJECT_SOURCE_DIR}/neuro/Player.cpp
	${PROJECT_SOURCE_DIR}/neuro/Game.cpp
	${PROJECT_SOURCE_DIR}/utility/Environment.cpp
	${PROJECT_SOURCE_DIR}/utility/DFStyleReader.cpp
	${PROJECT_SOURCE_DIR}/utility/DFStyleCreator.cpp
	${PROJECT_SOURCE_DIR}/utility/DFDirectoryReader.cpp
	)

set(NEUROSERVER_HDRS
	${COMMON_HDRS}
	${PROJECT_SOURCE_DIR}/network/Connection.hpp
	${PROJECT_SOURCE_DIR}/network/Acceptor.hpp
	${PROJECT_SOURCE_DIR}/neuroServer/Contestant.hpp
	${PROJECT_SOURCE_DIR}/neuroServer/NetworkContestant.hpp
	${PROJECT_SOURCE_DIR}/neuroServer/Arbiter.hpp
	${PROJECT_SOURCE_DIR}/neuroServer/Game.hpp
	${PROJECT_SOURCE_DIR}/neuroServer/GameOptions.hpp
	${PROJECT_SOURCE_DIR}/neuroServer/BoardLoader.hpp
	${PROJECT_SOURCE_DIR}/neuroServer/ArmyLoader.hpp
	${PROJECT_SOURCE_DIR}/neuroServer/Configurer.hpp
	${PROJECT_SOURCE_DIR}/neuroServer/Server.hpp
	${PROJECT_SOURCE_DIR}/neuroServer/Application.hpp
	${PROJECT_SOURCE_DIR}/ui/text/CommandShell.hpp
	${PROJECT_SOURCE_DIR}/ui/text/gameServer/Application.hpp
	${PROJECT_SOURCE_DIR}/ui/text/gameServer/Server.hpp
	${PROJECT_SOURCE_DIR}/ui/text/gameServer/ArmyLoader.hpp
	${PROJECT_SOURCE_DIR}/ui/text/gameServer/BoardLoader.hpp
	)

set(NEUROSERVER_SRCS
	${COMMON_SRCS}
	${PROJECT_SOURCE_DIR}/network/Connection.cpp
	${PROJECT_SOURCE_DIR}/network/Acceptor.cpp
	${PROJECT_SOURCE_DIR}/neuroServer/NetworkContestant.cpp
	${PROJECT_SOURCE_DIR}/neuroServer/Arbiter.cpp
	${PROJECT_SOURCE_DIR}/neuroServer/Game.cpp
	${PROJECT_SOURCE_DIR}/neuroServer/BoardLoader.cpp
	${PROJECT_SOURCE_DIR}/neuroServer/ArmyLoader.cpp
	${PROJECT_SOURCE_DIR}/neuroServer/Configurer.cpp
	${PROJECT_SOURCE_DIR}/neuroServer/Server.cpp
	${PROJECT_SOURCE_DIR}/neuroServer/Application.cpp
	${PROJECT_SOURCE_DIR}/ui/text/CommandShell.cpp
	${PROJECT_SOURCE_DIR}/ui/text/gameServer/Application.cpp
	${PROJECT_SOURCE_DIR}/ui/text/gameServer/Server.cpp
	${PROJECT_SOURCE_DIR}/ui/text/gameServer/ArmyLoader.cpp
	${PROJECT_SOURCE_DIR}/ui/text/gameServer/BoardLoader.cpp
	)

set(NEUROCLIENT_HDRS
	${COMMON_HDRS}
	${PROJECT_SOURCE_DIR}/network/Connection.hpp
	${PROJECT_SOURCE_DIR}/neuroClient/Application.hpp
	${PROJECT_SOURCE_DIR}/ui/text/client/Application.hpp
	${PROJECT_SOURCE_DIR}/ui/text/CommandShell.hpp
    ${PROJECT_SOURCE_DIR}/ui/text/client/ServerJoiner.hpp
    ${PROJECT_SOURCE_DIR}/ui/text/client/GameConfigurer.hpp
	)

set(NEUROCLIENT_SRCS
	${COMMON_SRCS}
	${PROJECT_SOURCE_DIR}/network/Connection.cpp
	${PROJECT_SOURCE_DIR}/neuroClient/Application.cpp
	${PROJECT_SOURCE_DIR}/ui/text/client/Application.cpp
	${PROJECT_SOURCE_DIR}/ui/text/CommandShell.cpp
    ${PROJECT_SOURCE_DIR}/ui/text/client/ServerJoiner.cpp
    ${PROJECT_SOURCE_DIR}/ui/text/client/GameConfigurer.cpp
	)

pkg_search_module(SDL2 REQUIRED sdl2)
find_package(Boost 1.54 COMPONENTS system filesystem REQUIRED)
find_package(Threads)

include_directories(
	${SDL2_INCLUDE_DIRS}
	${Boost_INCLUDE_DIR}
	${PROJECT_SOURCE_DIR}
	)

set(LIBS ${LIBS} ${SDL2_LIBRARIES} ${Boost_LIBRARIES})

link_libraries(${LIBS} ${CMAKE_THREAD_LIBS_INIT})

add_executable(NeurohexServer ${NEUROSERVER_HDRS} ${NEUROSERVER_SRCS} ${PROJECT_SOURCE_DIR}/NeuroServer.cpp)

install(TARGETS NeurohexServer
	RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
	CONFIGURATIONS Relase
	)

add_executable(NeurohexClient ${NEUROCLIENT_HDRS} ${NEUROCLIENT_SRCS} ${PROJECT_SOURCE_DIR}/NeuroClient.cpp)

install(TARGETS NeurohexClient
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
		${COMMON_SRCS}
		${NEURO_TEST_DIR}/BoardTest.cpp
		${NEURO_TEST_DIR}/TileTest.cpp
		)
	add_executable(neuroTest ${NEURO_TEST_SRCS})
	target_link_libraries(neuroTest
		${GTEST_LIBRARY}	${GTEST_MAIN_LIBRARY})
	add_test(NAME neuroTest
		COMMAND neuroTest)

	set(NEUROSERVER_TEST_DIR ${PROJECT_SOURCE_DIR}/neuroServer/test)
	set(NEUROSERVER_TEST_SRCS
		${NEUROSERVER_SRCS}
		${NEUROSERVER_TEST_DIR}/ArmyLoaderTest.cpp
		${NEUROSERVER_TEST_DIR}/BoardLoaderTest.cpp
		)
	add_executable(neuroServerTest ${NEUROSERVER_TEST_SRCS})
	target_link_libraries(neuroServerTest
		${GTEST_LIBRARY}	${GTEST_MAIN_LIBRARY})
	add_test(NAME neuroServerTest
		COMMAND neuroServerTest)

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
