# CMakeList.txt : CMake project for ConfigExporter, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.15)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("ConfigExporter")


file(GLOB_RECURSE SOURCES
		./src/*.h
		./src/*.hpp
		./src/*.cpp
	    ${CMAKE_SOURCE_DIR}/3rd-party/pugixml/*.hpp
		${CMAKE_SOURCE_DIR}/3rd-party/pugixml/*.cpp
)

# Print the list of source files
message("Source Files:")
foreach(source_file ${SOURCES})
    message("  ${source_file}")
endforeach()

# Add source to this project's executable.
add_library (ConfigExporter SHARED ${SOURCES})

add_compile_definitions(NOMINMAX)

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)


target_include_directories(ConfigExporter PUBLIC ${CMAKE_SOURCE_DIR}/safeCore_Cfg
 PUBLIC ${CMAKE_SOURCE_DIR}/safeCore_Cfg/local_inc
 PUBLIC ${CMAKE_SOURCE_DIR}/src
 PUBLIC ${CMAKE_SOURCE_DIR}/3rd-party
 PUBLIC ${CMAKE_SOURCE_DIR}/3rd-party/pugixml/
 PUBLIC ${CMAKE_SOURCE_DIR}/3rd-party/meta.hpp/headers
)

target_link_libraries(ConfigExporter PRIVATE nlohmann_json::nlohmann_json)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ConfigExporter PROPERTY CXX_STANDARD 20)
  add_compile_definitions(LIB_EXPORT)
endif()

# TODO: Add tests and install targets if needed.
