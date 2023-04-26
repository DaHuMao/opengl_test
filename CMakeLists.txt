cmake_minimum_required(VERSION 3.4.1)
set(CMAKE_C_COMPILER /usr/bin/clang)
set(CMAKE_CXX_COMPILER /usr/bin/clang++)
project(main)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -std=c++14 -DOBOE_NO_INCLUDE_AAUDIO -fvisibility=hidden")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O2")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
#message(FATAL_ERROR ${CMAKE_CURRENT_LIST_DIR})

set(CUR_DIR ${CMAKE_CURRENT_LIST_DIR})
set(GLEW_DIR  /usr/local/Cellar/glew/2.2.0_1)
set(GLFW_DIR  /usr/local/Cellar/glfw/3.3.8)
#add_definitions(-DWEBRTC_POSIX)
set(include_dir
  ${GLEW_DIR}/include
  ${GLFW_DIR}/include
)

set(libs_dir
    ${GLEW_DIR}/lib
    ${GLFW_DIR}/lib
)

set(app_native_sources
    ${CUR_DIR}/main1.cc
    ${CUR_DIR}/vertex_buffer.h
    ${CUR_DIR}/vertex_buffer.cc
    ${CUR_DIR}/index_buffer.h
    ${CUR_DIR}/index_buffer.cc
    ${CUR_DIR}/vertex_array.h
    ${CUR_DIR}/vertex_array.cc
    ${CUR_DIR}/vertex_buffer_layout.h
    ${CUR_DIR}/vertex_buffer_layout.cc
    ${CUR_DIR}/shader.h
    ${CUR_DIR}/shader.cc
    ${CUR_DIR}/render.h
    ${CUR_DIR}/render.cc
    ${CUR_DIR}/texture_load.h
    ${CUR_DIR}/texture_load.cc
    ${CUR_DIR}/camera.h
    ${CUR_DIR}/camera.cc
    ${CUR_DIR}/third_party/stb_image/stb_image.h
    ${CUR_DIR}/third_party/stb_image/stb_image.cc
)
message(INFO ${libs_dir})
##add_library(native_lib SHARED ${app_native_sources})

### INCLUDE OBOE LIBRARY ###

# Add the Oboe library as a subproject. Since Oboe is an out-of-tree source library we must also
# specify a binary directory
#add_subdirectory(${OBOE_DIR})

# Specify the path to the Oboe header files and the source.
include_directories(
  ${include_dir}
)

link_directories(
  ${libs_dir}
)
### END OBOE INCLUDE SECTION ###
add_executable(test_opengl ${app_native_sources})

# link to oboe
target_link_libraries(test_opengl libGLEW.a libGLFW.a)

if (APPLE)
    target_link_libraries(test_opengl "-framework OpenGL")
    target_link_libraries(test_opengl "-framework GLUT")
endif()


# bump 4 to resync CMake
