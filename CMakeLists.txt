cmake_minimum_required(VERSION 2.8)

project(ColPack)

set(CMAKE_BUILD_TYPE Debug)

file(GLOB_RECURSE COLPACK_SOURCES "src/*.cpp")
file(GLOB_RECURSE COLPACK_HEADERS "src/*.h")

set (COLPACK_INCLUDE_DIRS "")
foreach (_headerFile ${COLPACK_HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list (APPEND COLPACK_INCLUDE_DIRS ${_dir})
endforeach()
list(REMOVE_DUPLICATES COLPACK_INCLUDE_DIRS)

include_directories(${COLPACK_INCLUDE_DIRS})
add_library (ColPack SHARED ${COLPACK_SOURCES})

SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-fPIC")

install(FILES ${COLPACK_HEADERS}
        DESTINATION ${CMAKE_SOURCE_DIR}/ColPack/include/ColPack
       )

install(TARGETS ColPack
        DESTINATION ${CMAKE_SOURCE_DIR}/ColPack/lib64
       )

#file(GLOB_RECURSE COLPACK_EXAMPLE_SOURCES "examples/*.cpp")
#foreach (_exampleFile ${COLPACK_EXAMPLE_SOURCES})
#    get_filename_component(_exampleFileName ${_exampleFile} NAME_WE)
#    add_executable(${_exampleFileName} ${_exampleFile})
#    target_link_libraries(${_exampleFileName} ColPack)
#endforeach()



