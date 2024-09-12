# CMakeList.txt : CMake project for DPLMPServer, include source and define
# project specific logic here.
#

# Add source to this project's executable.
# add_executable (DPLMPServer "DPLMPServer.cpp" "DPLMPServer.h")
include_directories("server/")
file(GLOB SOURCES "server/*.cpp")
add_executable(DPLMPServer  ${SOURCES})

include_directories("../thirdparty/RakNet/Source")

file(GLOB RAKNET_SOURCES "../thirdparty/RakNet/Source/*.cpp")

target_sources(DPLMPServer PRIVATE ${RAKNET_SOURCES})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET DPLMPServer PROPERTY CXX_STANDARD 14)
endif()

IF (WIN32 AND NOT UNIX)
	target_link_libraries(DPLMPServer ws2_32.lib)
ELSE(WIN32 AND NOT UNIX)
	target_link_libraries(DPLMPServer pthread)
ENDIF(WIN32 AND NOT UNIX)