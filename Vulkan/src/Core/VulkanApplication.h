#pragma once

#define GLFW_INCLUDE_VULKAN
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <glfw/glfw3native.h>

#include <string>
#include <vector>
#include <optional>

#define ENABLE_VALIDATION_LAYERS true

namespace Vulkan {

	//////////////////////////////////////////////////////////////////////////////////
	// Window Properties
	//////////////////////////////////////////////////////////////////////////////////

	struct WindowProps
	{
		std::string WindowTitle;
		uint32_t Width, Height;

		WindowProps()
			: WindowTitle("Vulkan"), Width(1280), Height(720) {}
	};

	//////////////////////////////////////////////////////////////////////////////////
	// Queue Families
	//////////////////////////////////////////////////////////////////////////////////

	struct QueueFamilyIndicies
	{
		std::optional<uint32_t> GraphicsFamily;
		std::optional<uint32_t> PresentFamily;

		bool IsComplete()
		{
			return GraphicsFamily.has_value() && PresentFamily.has_value();
		}
	};

	//////////////////////////////////////////////////////////////////////////////////
	// Swapchain Primitives
	//////////////////////////////////////////////////////////////////////////////////

	struct SwapChainSupportDetails
	{
		VkSurfaceCapabilitiesKHR Capabilities;
		std::vector<VkSurfaceFormatKHR> Formats;
		std::vector<VkPresentModeKHR> PresentModes;
	};

	//////////////////////////////////////////////////////////////////////////////////
	// Vulkan Application Definition
	//////////////////////////////////////////////////////////////////////////////////

	class VulkanApplication
	{
	public:
		VulkanApplication(const WindowProps& props = WindowProps());
		~VulkanApplication();

		void Run();

	private:
		// Window
		void CreateApplicationWindow();

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

		// Extensions
		bool CheckDeviceExtensionSupport(VkPhysicalDevice device);

		// Swapchain
		SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
		
		void CreateSwapchain();

		// Image Views
		void CreateImageViews();

		// Graphics Pipeline
		VkShaderModule CreateShaderModule(const std::vector<char>& source);

		void CreateGraphicsPipeline();
		void CreateRenderPass();

	private:
		WindowProps m_Properties;
		GLFWwindow* m_Window;

		const std::vector<const char*> m_ValidationLayers = { "VK_LAYER_KHRONOS_validation" };
		const std::vector<const char*> m_DeviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

		// Vulkan Primitives
		VkInstance m_Instance;
		
		VkDebugUtilsMessengerEXT m_DebugMessenger;
		
		VkPhysicalDevice m_PhysicalDevice = VK_NULL_HANDLE;
		VkDevice m_Device;

		VkQueue m_GraphicsQueue;
		VkQueue m_PresentQueue;

		// Vulkan Context
		VkSurfaceKHR m_Surface;

		VkSwapchainKHR m_Swapchain;
		std::vector<VkImage> m_SwapchainImages;
		VkFormat m_SwapchainImageFormat;
		VkExtent2D m_SwapchainExtent;
		
		std::vector<VkImageView> m_SwapchainImageViews;

		// Vulkan Pipeline
		VkPipelineLayout m_PiplineLayout;
		VkPipeline m_GraphicsPipeline;
		VkRenderPass m_RenderPass;
	};

}