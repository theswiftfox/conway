cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

enable_language(CXX)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)					# don't let OPTION erase non-cache variables
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF CACHE INTERNAL "" FORCE)	# disable compiler extensions
set(CMAKE_POSITION_INDEPENDENT_CODE ON)					# enable PIC
set(CMAKE_DEBUG_POSTFIX "-d")

if("${CMAKE_BUILD_TYPE}" STREQUAL "")
  set(CMAKE_BUILD_TYPE Debug)			# ensure CMAKE_BUILD_TYPE is not empty
endif()

if(WIN32)
  set(SDL2_DIR "D:/dev/SDL2-2.0.22")
endif()
find_package(SDL2 REQUIRED)

# project parameters
set(project_name conway)
set(cxx_standard cxx_std_20)

project(${project_name} VERSION 1.0)

# sources
set(src
    src/main.cpp
    src/conway.cpp
)
set(hdr
    src/conway.h
)

add_executable(${PROJECT_NAME} ${src} ${hdr})
include_directories(${PROJECT_NAME} ${SDL2_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
