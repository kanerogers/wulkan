cmake_minimum_required(VERSION 3.10)
project(MyProject VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(wulkan src/main.cpp)

include_directories(/Users/kane/VulkanSDK/1.3.280.1/macOS/include /opt/homebrew/include)

target_include_directories(wulkan PRIVATE include)
link_directories(/Users/kane/VulkanSDK/1.3.280.1/macOS/lib /opt/homebrew/lib)

# GLFW
find_library(GLFW_LIBRARY NAMES glfw glfw3 PATHS /opt/homebrew/lib)
if(NOT GLFW_LIBRARY)
    message(FATAL_ERROR "GLFW not found")
endif()

# Vulkan
find_library(VULKAN_LIBRARY NAMES vulkan PATHS /Users/kane/VulkanSDK/1.3.280.1/macOS/lib)
if(NOT VULKAN_LIBRARY)
    message(FATAL_ERROR "Vulkan not found")
endif()

# Linking libraries
target_link_libraries(wulkan PRIVATE ${GLFW_LIBRARY} ${VULKAN_LIBRARY})
