cmake_minimum_required(VERSION 3.8)

project("BattleCity")

set(RESOURCE_DIR "${CMAKE_SOURCE_DIR}/../resources")

configure_file(
	"resources_path.h.in"
	"${CMAKE_SOURCE_DIR}/engine/src/resources_path.h"
	@ONLY
)

add_subdirectory("engine")

set(SOURCES "src/main.cpp" "${RESOURCE_DIR}/resources.json")

#add_executable(${PROJECT_NAME} WIN32 ${SOURCES})
add_executable(${PROJECT_NAME} ${SOURCES})

message(${CMAKE_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} "engine")

target_compile_features(${PROJECT_NAME} PUBLIC "cxx_std_17")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(
	DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY
		VS_STARTUP_PROJECT ${PROJECT_NAME}
)

set_target_properties(
	${PROJECT_NAME} PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/../bin"
)

add_custom_command(
	TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E remove_directory
			"${CMAKE_CURRENT_BINARY_DIR}/Debug"
			"${CMAKE_CURRENT_BINARY_DIR}/Release"
)
