cmake_minimum_required(VERSION 3.20)

project(rmluidx11)

file(GLOB_RECURSE PROJECT_SOURCE_FILES "src/*.h" "src/*.cpp")

macro(GroupSources curdir groupindex)
  file(GLOB children RELATIVE ${curdir} ${curdir}/*)

  foreach(child ${children})
    if(IS_DIRECTORY ${curdir}/${child})
      GroupSources(${curdir}/${child} ${groupindex}/${child})
    else()

      string(REPLACE "/" "\\" groupname ${groupindex})

      source_group(${groupname} FILES ${curdir}/${child})
    endif()
  endforeach()
endmacro()

GroupSources(${PROJECT_SOURCE_DIR}/src "Source Files")

include(pmm.cmake)
pmm(
  VCPKG
  REVISION master
  TRIPLET x64-windows
  REQUIRES rmlui directxtk
)

find_package(RmlUi CONFIG REQUIRED)
find_package(directxtk CONFIG REQUIRED)

add_executable(rmluidx11 ${PROJECT_SOURCE_FILES})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_definitions(-DUNICODE -D_UNICODE)

target_link_libraries(rmluidx11 PRIVATE d3d11.lib)
target_link_libraries(rmluidx11 PRIVATE d3dcompiler.lib)
target_link_libraries(rmluidx11 PRIVATE Microsoft::DirectXTK)
target_link_libraries(rmluidx11 PRIVATE RmlCore RmlDebugger)

include_directories("./" "./src")

set_property(TARGET rmluidx11 PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:rmluidx11>)

add_custom_command(TARGET rmluidx11 PRE_BUILD 
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets/ $<TARGET_FILE_DIR:rmluidx11> 
)
