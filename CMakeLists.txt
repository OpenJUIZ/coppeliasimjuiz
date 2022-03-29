message("---------------------- CoppeliaSimJUIZ project --------------------------")

cmake_minimum_required(VERSION 3.10)

project(CoppeliaSimJUIZ LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message("----------- For CoppeliaSim Section ------------")
set(remote_api_srcs 
    CoppeliaSim/common/shared_memory.c
    CoppeliaSim/remoteApi/extApi.h 
    CoppeliaSim/remoteApi/extApi.c 
    CoppeliaSim/remoteApi/extApiInternal.h 
    CoppeliaSim/remoteApi/extApiPlatform.c 
    CoppeliaSim/remoteApi/extApiPlatform.h)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/CoppeliaSim/include)
add_library(remoteApi SHARED ${remote_api_srcs})
target_compile_definitions(remoteApi PUBLIC NON_MATLAB_PARSING)
target_compile_definitions(remoteApi PUBLIC MAX_EXT_API_CONNECTIONS=255)
set_target_properties(remoteApi
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/CoppeliaSim)

set(CONTAINERS CoppeliaSim)
set(CONTAINER_OPERATIONS CoppeliaSim_start CoppeliaSim_finish CoppeliaSim_startSimulation CoppeliaSim_stopSimulation CoppeliaSim_getObjectPosition CoppeliaSim_getObjectOrientation CoppeliaSim_getObjectQuaternion CoppeliaSim_setJointTargetVelocity CoppeliaSim_getJointPosition CoppeliaSim_getStringSignal)
set(OPERATIONS)

find_package(juiz)

set(JUIZ_LIBS ${juiz_LIBRARIES})
set(EXT_LIBS remoteApi)

include(juiz_component.cmake)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${juiz_INCLUDE_DIR})
link_directories(${juiz_LIB_DIR})


message("----------------- JUIZ Setting ------------------")
message("- juiz_ROOT_DIR     : " ${juiz_ROOT_DIR})
message("- juiz_INCLUDE_DIR  : " ${juiz_INCLUDE_DIR})
message("- juiz_LIB_DIR      : " ${juiz_LIB_DIR})
message("- juiz_LIBRARIES    : " ${juiz_LIBRARIES})
message("- CMakeLists.txt    : " ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt)
message("- JUIZ_LIBS         : " ${JUIZ_LIBS})
message("- EXT_LIBS          : " ${EXT_LIBS})
message("----------------- ------------ ------------------")

# Output basic information of this project.

# ----- IMPLEMENTATION AREA ------

# Add projects to target

message("----------------- Registering Section ------------------")
add_containers(CONTAINERS)
add_container_operations(CONTAINER_OPERATIONS)
add_operations(OPERATIONS)

message("----------------- ------------------- ------------------")
# Define modules for install
set(COPPELIASIMJUIZ_MODULES ${OPERATIONS} ${CONTAINERS} ${ECS} PARENT_SCOPE)

# Installing files for targets
INSTALL(TARGETS ${CONTAINERS} ${CONTAINER_OPERATIONS} ${OPERATIONS} ${EXT_LIBS} DESTINATION ${juiz_ROOT_DIR}/share/juiz/components/CoppeliaSimJUIZ)
INSTALL(FILES CoppeliaSimJUIZ.jshelf DESTINATION ${juiz_ROOT_DIR}/share/juiz/components/CoppeliaSimJUIZ)

message("-----------------------------------------------------------------------")
