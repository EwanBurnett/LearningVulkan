cmake_minimum_required(VERSION 3.16)

include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw
)

FetchContent_MakeAvailable(glfw)

project("Common")

find_package(Vulkan REQUIRED)

add_library(${PROJECT_NAME} src/Helpers.cpp)

target_link_libraries(${PROJECT_NAME} ${Vulkan_LIBRARIES} glfw)

target_include_directories(${PROJECT_NAME} PUBLIC ${Vulkan_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/include)

add_subdirectory("Examples")

add_custom_target(CompileShader
    TARGET "Common" POST_BUILD
    COMMENT "Compiling Shaders"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMAND ./CompileShaders.sh
)
