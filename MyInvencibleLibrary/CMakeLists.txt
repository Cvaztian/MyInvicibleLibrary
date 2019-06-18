cmake_minimum_required (VERSION 3.10)
if (UNIX)
  add_definitions(-Wno-sign-compare -Wno-enum-compare)
endif(UNIX)
project(MyInvencibleLibrary)
set (CMAKE_CXX_STANDARD 17)

option(DBMS "Dbms" OFF)
include_directories("${CMAKE_SOURCE_DIR}")
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/cmake-build-debug)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -lstdc++fs -pthread -g -Wall")

FIND_PACKAGE(Qt5Core)
FIND_PACKAGE(Qt5Gui)
FIND_PACKAGE(Qt5Widgets)
FIND_PACKAGE(Qt5PrintSupport REQUIRED)
SET(CMAKE_AUTOUIC ON)
SET(CMAKE_AUTOMOC ON)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)
SET(Gladiators_SOURCES main.cpp mainwindow.cpp)
SET(Gladiators_HEADERS mainwindow.h)
SET(Gladiators_FORMS mainwindow.ui)

add_executable(restserver
        mainP.cpp
          src/handler.cpp
          )


add_executable(restclient clientmain.cpp restclient.cpp restclient.h metadata.h metadata.cpp
        Interprete.h LinkedList.h Node.h)
add_executable(pruebasJson jsonmain.cpp metadata.cpp metadata.h)
add_executable(socketclient socketclient.cpp clientsocket.cpp clientsocket.h clientmanager.cpp
    clientmanager.h metadata.cpp metadata.h)
add_executable(socketserver socketserver.cpp serversocket.cpp serversocket.h )
add_executable(ServerLib libmain.cpp src/handler.cpp
    servermanager.cpp servermanager.h serversocket.h serversocket.cpp
    metadata.cpp metadata.h Interprete.h LinkedList.h Node.h)

add_executable(MetadataDB fiomain.cpp
    dbmanager.cpp dbmanager.h
    metadata.h metadata.cpp
    clientsocket.cpp clientsocket.h clientmanager.cpp
    clientmanager.h
        Interprete.h LinkedList.h Node.h)
    
add_executable(RAID mainP.cpp RAID.h
        clientmanager.h clientmanager.cpp
        dbmanager.cpp dbmanager.h
    metadata.h metadata.cpp
        clientsocket.h clientsocket.cpp
        metadata.h metadata.cpp
        )
add_executable(Interfaz Interfaz.pro mainwindow.cpp mainwindow.h mainwindow.ui restclient.cpp restclient.h
        metadata.h metadata.cpp Interprete.h LinkedList.h Node.h)
add_executable(Interprete InterpreteMain.cpp dbmanager.cpp dbmanager.h metadata.cpp
        metadata.h LinkedList.h Node.h restclient.cpp restclient.h)

add_executable(Huffman Huffman.h Huffmain.cpp)
add_subdirectory(json)

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
        target_link_libraries(restclient ${Casablanca_LIBRARIES} nlohmann_json::nlohmann_json)
        target_link_libraries(pruebasJson ${Casablanca_LIBRARIES} nlohmann_json::nlohmann_json)
        target_link_libraries(socketclient  nlohmann_json::nlohmann_json)
        target_link_libraries(socketserver  nlohmann_json::nlohmann_json)
        target_link_libraries(ServerLib nlohmann_json::nlohmann_json ${Casablanca_LIBRARIES})
    target_link_libraries(RAID nlohmann_json::nlohmann_json)
            target_link_libraries(MetadataDB nlohmann_json::nlohmann_json)
        target_link_libraries(Interfaz Qt5::Widgets Qt5::PrintSupport nlohmann_json::nlohmann_json ${Casablanca_LIBRARIES})
        target_link_libraries(Interprete nlohmann_json::nlohmann_json ${Casablanca_LIBRARIES})
endif(DBMS)






