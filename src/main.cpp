#include "vulkan/vulkan_core.h"
#include "vulkan/vulkan_beta.h"
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
        VkDevice device = VK_NULL_HANDLE;
        VkSurfaceKHR surface;

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
            createDevice();
            createSurface();
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

#ifdef __APPLE__
            requiredExtensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
            instanceCreateInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif

            instanceCreateInfo.enabledExtensionCount = requiredExtensions.size();
            instanceCreateInfo.ppEnabledExtensionNames = requiredExtensions.data();

            auto result = vkCreateInstance(&instanceCreateInfo, NULL, &instance);
            if (result != VK_SUCCESS) {
                throw std::runtime_error("failed to create instance!");
            }
        }

        void createPhysicalDevice() {
            uint32_t deviceCount = 0;

            vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

            std::vector<VkPhysicalDevice> devices(deviceCount);
            vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

            std::string deviceName;

            for (const auto &device: devices) {
                VkPhysicalDeviceProperties deviceProperties;
                vkGetPhysicalDeviceProperties(device, &deviceProperties);

                deviceName = deviceProperties.deviceName;

                if (deviceName.starts_with("Apple")) {
                    physicalDevice = device;
                    break;
                }

                if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
                    physicalDevice = device;
                    break;
                }
            }

            if (physicalDevice == VK_NULL_HANDLE) {
                throw std::runtime_error("Unable to find valid device");
            }

            std::cout << "Using device: " << deviceName << std::endl;
        }

        void createDevice() {
            const float priority = 1.0;

            VkDeviceQueueCreateInfo createQueueInfo{};
            createQueueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            createQueueInfo.queueCount = 1;
            createQueueInfo.queueFamilyIndex = 0;
            createQueueInfo.pQueuePriorities = &priority;

            VkPhysicalDeviceFeatures deviceFeatures{};

            std::vector<const char*> deviceExtensions = {
                VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME
            };

            VkDeviceCreateInfo createDeviceInfo{};
            createDeviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
            createDeviceInfo.pQueueCreateInfos = &createQueueInfo;
            createDeviceInfo.queueCreateInfoCount = 1;
            createDeviceInfo.pEnabledFeatures = &deviceFeatures;
            createDeviceInfo.ppEnabledExtensionNames = deviceExtensions.data();
            createDeviceInfo.enabledExtensionCount = deviceExtensions.size();

            auto result = vkCreateDevice(physicalDevice, &createDeviceInfo, nullptr, &device);

            if (result != VK_SUCCESS) {
                throw std::runtime_error("Unable to create device");
            }
        }

        void createSurface() {
            auto result = glfwCreateWindowSurface(instance, window, nullptr, &surface);
            if (result != VK_SUCCESS) {
                throw std::runtime_error("Unable to create surface");
            }


        }

        void cleanup() {
            vkDestroySurfaceKHR(instance, surface, nullptr);
            vkDestroyDevice(device, nullptr);
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
