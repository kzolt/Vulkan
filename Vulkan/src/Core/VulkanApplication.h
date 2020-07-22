#pragma once

#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>
#include <vector>
#include <optional>

#define ENABLE_VALIDATION_LAYERS true

namespace Vulkan {

	struct WindowProps
	{
		std::string WindowTitle;
		uint32_t Width, Height;

		WindowProps()
			: WindowTitle("Vulkan"), Width(1280), Height(720) {}
	};

	struct QueueFamilyIndicies
	{
		std::optional<uint32_t> GraphicsFamily;

		bool IsComplete()
		{
			return GraphicsFamily.has_value();
		}
	};

	class VulkanApplication
	{
	public:
		VulkanApplication(const WindowProps& props = WindowProps());
		~VulkanApplication();

		void Run();

	private:
		// Window
		void CreateWindow();

		// Validation Layers
		bool CheckValidationLayerSupport();
		std::vector<const char*> GetRequiredExtensions();
		
		// Debugger
		void SetupDebugMessenger();
		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

		// Physical Devices
		void PickPhysicalDevice();
		int RateDeviceSuitability(VkPhysicalDevice device);

		// Queue Families
		QueueFamilyIndicies FindQueueFamilies(VkPhysicalDevice device);

		// Logical Device
		void CreateLogicalDevice();

	private:
		WindowProps m_Properties;
		GLFWwindow* m_Window;

		const std::vector<const char*> m_ValidationLayers = { "VK_LAYER_KHRONOS_validation" };

		// Vulkan Primitives
		VkInstance m_Instance;
		
		VkDebugUtilsMessengerEXT m_DebugMessenger;
		
		VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
		VkDevice m_Device;

		VkQueue m_GraphicsQueue;

		// Vulkan Window
	};

}