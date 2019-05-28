cmake_minimum_required (VERSION 3.5)
if (UNIX)
  add_definitions(-Wno-sign-compare -Wno-enum-compare)
endif(UNIX)
project(MyInvencibleLibrary)
set (CMAKE_CXX_STANDARD 11)
option(DBMS "Dbms" OFF)
include_directories("${CMAKE_SOURCE_DIR}")
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/Build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")


add_executable(restserver
  main.cpp
          src/handler.cpp
          )

add_executable(restclient clientmain.cpp restclient.cpp restclient.h)

set(Casablanca_LIBRARIES "-lboost_system -lcrypto -lssl -lcpprest")
if(DBMS)
	set(NEW_DB_SRC_DIR ${CMAKE_SOURCE_DIR}/dbms/src)
	add_library(dbms SHARED
		${NEW_DB_SRC_DIR}/Dbms.cpp
		)
	set(Dbms_dep_lib "-lpqxx -lpq")
	target_link_libraries(restserver dbms ${Casablanca_LIBRARIES} ${Dbms_dep_lib})
else(DBMS)
        target_link_libraries(restserver ${Casablanca_LIBRARIES})
        target_link_libraries(restclient ${Casablanca_LIBRARIES})
endif(DBMS)





