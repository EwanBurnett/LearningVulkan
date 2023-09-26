project("Renderer")

find_package(Vulkan REQUIRED)

include(FetchContent)

FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui
)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw
)


FetchContent_MakeAvailable(imgui glfw)

add_library("imgui-Vulkan" STATIC
    "${imgui_SOURCE_DIR}/imgui.h"
    "${imgui_SOURCE_DIR}/imgui.cpp"
    
    "${imgui_SOURCE_DIR}/imgui_demo.cpp"
    "${imgui_SOURCE_DIR}/imgui_widgets.cpp"
    "${imgui_SOURCE_DIR}/imgui_draw.cpp"

    "${imgui_SOURCE_DIR}/backends/imgui_impl_vulkan.cpp"
    "${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp"
)

target_include_directories("imgui-Vulkan" PUBLIC ${imgui_SOURCE_DIR})

add_executable("Renderer" main.cpp "src/Window.cpp" "src/Renderer.cpp" "src/GUI.cpp" "src/Engine.cpp")
target_link_libraries("Renderer" imgui-Vulkan ${Vulkan_LIBRARIES} glfw)
target_include_directories("Renderer" PUBLIC ${Vulkan_INCLUDE_DIRS})
