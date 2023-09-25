cmake_minimum_required(VERSION 3.18)

set(CRDT_DEMOS_HOMEPAGE_URL "https://github.com/pseigo/crdt-demos")

project(CrdtDemos
  VERSION 0.0.1
  DESCRIPTION "CRDT demos."
  HOMEPAGE_URL ${CRDT_DEMOS_HOMEPAGE_URL}
  LANGUAGES CXX
)

# Public header files
#set(CRDTDEMOS_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/crdt_demos)

# Require out-of-source builds
file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
  message(FATAL_ERROR "You cannot build in a source directory (or any directory with a CMakeLists.txt file). Please make a build subdirectory. Feel free to remove CMakeCache.txt and CMakeFiles.")
endif()

# Only run if this is the main project (i.e., not added via add_subdirectory)
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  set(CMAKE_CXX_STANDARD 17)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
  set(CMAKE_CXX_EXTENSIONS OFF) # Ensure -std=c++xx instead of -std=g++xx
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # Generate `compile_commands.json`

  # Support folders in IDEs
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()

###
### Targets
###
add_subdirectory(src)
