project("Vertex-Buffers")

find_package(Vulkan REQUIRED)

add_executable(${PROJECT_NAME} main.cpp Window.cpp Renderer.cpp)
target_link_libraries(${PROJECT_NAME} Common)

set(VKDemo_DEBUG_DEFINES DEBUG)

target_compile_definitions(${PROJECT_NAME} PUBLIC "$<$<CONFIG:Debug>:${VKDemo_DEBUG_DEFINES}>")
