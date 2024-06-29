#include "vulkan/vulkan_core.h"
#include <_types/_uint32_t.h>
#include <exception>
#include <sys/_types/_null.h>
#include <vector>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>

class HelloTriangleApplication {
    public:
        void run() {
            std::cout << "Starting Vulkan application!" << std::endl;
            initWindow();
            initVulkan();
            mainLoop();
            cleanup();
        }

    private:
        GLFWwindow* window;
        VkInstance instance;
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

        void initWindow() {
            glfwInit();
            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
            glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

            window = glfwCreateWindow(800, 600, "Vulkan", nullptr, nullptr);
        }

        void mainLoop() {
            while (!glfwWindowShouldClose(window)) {
                glfwPollEvents();
            }
        }

        void initVulkan() {
            createInstance();
            createPhysicalDevice();
        }

        void createInstance() {
            VkApplicationInfo appInfo{};
            appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
            appInfo.pApplicationName = "Hello Triangle";
            appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
            appInfo.pEngineName = "BEAST";
            appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
            appInfo.apiVersion = VK_API_VERSION_1_2;

            VkInstanceCreateInfo instanceCreateInfo{};
            instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
            instanceCreateInfo.pApplicationInfo = &appInfo;

            std::vector<const char*> requiredExtensions;

            uint32_t glfwExtensionCount = 0;
            const char** glfwExtensions;

            glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

            for (uint32_t i=0; i < glfwExtensionCount; i++) {
                requiredExtensions.emplace_back(glfwExtensions[i]);
            }

            requiredExtensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);

            instanceCreateInfo.enabledExtensionCount = requiredExtensions.size();
            instanceCreateInfo.ppEnabledExtensionNames = requiredExtensions.data();
            instanceCreateInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

            auto result = vkCreateInstance(&instanceCreateInfo, NULL, &instance);
            if (result != VK_SUCCESS) {
                throw std::runtime_error("failed to create instance!");
            }
        }

        void createPhysicalDevice() {
            uint32_t deviceCount = 0;

            vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
            std::cout << "Found " << deviceCount << " devices" << std::endl;

            std::vector<VkPhysicalDevice> devices(deviceCount);
            vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

            for (const auto& device: devices) {
                VkPhysicalDeviceProperties deviceProperties;
                vkGetPhysicalDeviceProperties(device, &deviceProperties);

                std::string deviceName(deviceProperties.deviceName);
                std::cout << "Device name: " << deviceName << std::endl;
            }

            std::cout << "Done!" << std::endl;
        }

        void cleanup() {
            vkDestroyInstance(instance, nullptr);
            glfwDestroyWindow(window);
            glfwTerminate();
        }

};

int main() {
    HelloTriangleApplication app;

    try {
        app.run();
    } catch(const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
