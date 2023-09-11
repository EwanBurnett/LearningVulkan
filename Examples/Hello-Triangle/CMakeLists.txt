
project("Hello-Triangle")

find_package(Vulkan REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} Common)
