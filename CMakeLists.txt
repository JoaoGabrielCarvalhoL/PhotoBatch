cmake_minimum_required(VERSION 3.12)

project(PhotoBatch)

add_executable(PhotoBatch 
"PhotoBatch.cpp" 
"ArgumentParser.h" 
"ArgumentParser.cpp" 
"Utils.h" 
"Utils.cpp"  
"Mode.cpp"
"Mode.h"
 "RenameMode.h" 
 "RenameMode.cpp" 
 "ConvertMode.h" 
 "ConvertMode.cpp" 
 "ResizeMode.h" 
 "ResizeMode.cpp" 
 "ScaleMode.h" 
 "ScaleMode.cpp")

set_target_properties(PhotoBatch PROPERTIES CXX_STANDARD 17)

set(FETCHCONTENT_QUIET OFF CACHE BOOL "Disabilita o modo quiet to FetchContent" FORCE)
include(FetchContent)

add_subdirectory(External)

target_link_libraries(PhotoBatch PRIVATE stb)