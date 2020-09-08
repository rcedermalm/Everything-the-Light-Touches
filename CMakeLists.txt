cmake_minimum_required(VERSION 3.9)
project(Everything_the_Light_Touches)

set(CMAKE_CXX_STANDARD 11)

## Set name of folders that will be in use
set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(PROJECT_EXTERNAL_DIR ${PROJECT_SOURCE_DIR}/external)
set(PROJECT_SOURCES_DIR ${PROJECT_SOURCE_DIR}/src)

##########################################
#######     Set Libraries     ############
##########################################

### GLM ###
set(EXTERNAL_INCLUDE_DIRS ${EXTERNAL_INCLUDE_DIRS} ${PROJECT_EXTERNAL_DIR}/glm)
set(EXTERNAL_INCLUDE_DIRS ${EXTERNAL_INCLUDE_DIRS} ${PROJECT_EXTERNAL_DIR}/glm/gtx)

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    set(ALL_LIBRARIES ${ALL_LIBRARIES} OpenMP::OpenMP_CXX)
endif()

####################################################
###### Include directories, add executables ########
######          and link libraries          ########
####################################################

# Lump all external- and project includes into one variable
set(ALL_INCLUDES ${EXTERNAL_INCLUDE_DIRS} ${PROJECT_INCLUDE_DIR} ${CMAKE_MODULE_PATH})

# Set the include directories so that #include statements work
include_directories(${ALL_INCLUDES})
message( "All include dirs: ${ALL_INCLUDES}")

# Get all source files by traversing the source directory recursively
file(GLOB_RECURSE PROJECT_CPP_FILES ${PROJECT_SOURCES_DIR}/*.cpp)

# Adds executable files
set(SOURCE_FILES ${PROJECT_CPP_FILES} main.cpp)
add_executable(Everything_the_Light_Touches ${SOURCE_FILES})

# Links libraries
target_link_libraries(Everything_the_Light_Touches ${ALL_LIBRARIES})
message("All include libraries: ${ALL_LIBRARIES}")