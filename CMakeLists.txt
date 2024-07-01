cmake_minimum_required(VERSION 3.10)
project(wulkan VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(wulkan src/main.cpp)

# Vulkan
include_directories(/Users/kane/VulkanSDK/1.3.280.1/macOS/include)
target_include_directories(wulkan PRIVATE include)
link_directories(/Users/kane/VulkanSDK/1.3.280.1/macOS/lib)
target_link_libraries(wulkan PRIVATE ${VULKAN_LIBRARY})

# GLFW
include_directories(extern/glfw/include)
add_subdirectory(extern/glfw)
target_link_libraries(wulkan PRIVATE glfw)

# glm
add_subdirectory(extern/glm)
