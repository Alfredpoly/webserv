cmake_minimum_required(VERSION 3.23)
project(webserv)

set(CMAKE_CXX_STANDARD 98)

set (CMAKE_CXX_FLAGS "-Werror -Wextra -Wall")

add_definitions(-DDIRECTORY="/Users/maiadegraaf/Documents/webserv")

include_directories(inc)
include_directories(SYSTEM ./lib)

add_executable(webserv
        inc/CGIResponse.hpp
        inc/Client.hpp
        inc/Config.hpp
        inc/ConfigParser.hpp
        inc/Location.hpp
#        inc/ReadWrite.hpp
        inc/Request.hpp
        inc/Response.hpp
        inc/Server.hpp
        inc/ServerIO.hpp
        inc/Utils.h
        inc/webserv.h
        inc/WSException.hpp
        src/client/CGIResponse.cpp
        src/client/Client.cpp
        src/client/postHandler.cpp
        src/client/Request.cpp
        src/client/Response.cpp
        src/config/Config.cpp
        src/config/ConfigParser.cpp
        src/config/Location.cpp
        src/server/Server.cpp
        src/server/ServerIO.cpp
        src/server/ServerKq.cpp
        src/utils/errorMap.cpp
#        src/utils/ReadWrite.cpp
        src/utils/Utils.cpp
        src/utils/utils_Maia.cpp
        src/main.cpp)