project("Vertex-Buffers")

find_package(Vulkan REQUIRED)

add_executable(${PROJECT_NAME} main.cpp Window.cpp)
target_link_libraries(${PROJECT_NAME} Common)
