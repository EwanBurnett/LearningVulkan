cmake_minimum_required(VERSION 3.16)

include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw
)

FetchContent_MakeAvailable(glfw)

project("VKDemo")

find_package(Vulkan REQUIRED)

add_executable(${PROJECT_NAME} main.cpp Helpers.cpp)

target_link_libraries(${PROJECT_NAME} ${Vulkan_LIBRARIES} glfw)

target_include_directories(${PROJECT_NAME} PRIVATE ${Vulkan_INCLUDE_DIRS})
