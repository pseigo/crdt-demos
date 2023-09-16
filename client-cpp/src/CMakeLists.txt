cmake_minimum_required(VERSION 3.18)

project(CrdtClientCpp
  VERSION 0.0.1
  HOMEPAGE_URL ${CRDT_DEMOS_HOMEPAGE_URL}
  LANGUAGES CXX
)

if(NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX d)
endif()

###
### Variables
###
set(SOURCE_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)
#set(HEADER_FILES
#        ${CODINGINTERVIEWS_INCLUDE_DIR}/foo.hpp)

###
### Define executable target
###
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

#target_compile_options(${PROJECT_NAME} PRIVATE -Werror -Wall -Wextra -pedantic)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
