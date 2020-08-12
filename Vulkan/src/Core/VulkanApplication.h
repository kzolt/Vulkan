#pragma once

#define GLFW_INCLUDE_VULKAN
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <glfw/glfw3native.h>

#include <string>
#include <vector>
#include <array>
#include <optional>

#include <glm/glm.hpp>

#define ENABLE_VALIDATION_LAYERS true
#define MAX_FRAMES_IN_FLIGHT 2

namespace Vulkan {

	//////////////////////////////////////////////////////////////////////////////////
// Data Structures
//////////////////////////////////////////////////////////////////////////////////

	struct Vertex
	{
		glm::vec2 Position;
		glm::vec3 Color;

		static VkVertexInputBindingDescription GetBindingDescription()
		{
			VkVertexInputBindingDescription bindingDescription{};
			bindingDescription.binding = 0;
			bindingDescription.stride = sizeof(Vertex);
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			return bindingDescription;
		}

		static std::array<VkVertexInputAttributeDescription, 2> GetAttributeDescriptions()
		{
			std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

			attributeDescriptions[0].binding = 0;
			attributeDescriptions[0].location = 0;
			attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
			attributeDescriptions[0].offset = offsetof(Vertex, Position);

			attributeDescriptions[1].binding = 0;
			attributeDescriptions[1].location = 1;
			attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[1].offset = offsetof(Vertex, Color);

			return attributeDescriptions;
		}
	};

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
		void RecreateSwapchain();
		void CleanupSwapchain();

		// Image Views
		void CreateImageViews();

		// Graphics Pipeline
		VkShaderModule CreateShaderModule(const std::vector<char>& source);

		void CreateGraphicsPipeline();
		void CreateRenderPass();

		// Framebuffers
		void CreateFrambuffer();

		// Command Buffer
		void CreateCommandPool();
		void CreateCommandBuffers();

		// Vertex Buffers
		void CreateVertexBuffer();
		uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

		// Rendering
		void CreateSyncObjects();

		void Present();

	public:
		bool framebufferResized = false;

	private:
		WindowProps m_Properties;
		GLFWwindow* m_Window;

		size_t m_CurrentFrame = 0;

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

		std::vector<VkFramebuffer> m_SwapchainFramebuffers;

		VkCommandPool m_CommandPool;
		std::vector<VkCommandBuffer> m_CommandBuffers;
		
		// Vulkan Rendering
		std::vector<VkSemaphore> m_ImageAvailableSemaphore;
		std::vector<VkSemaphore> m_RenderFinishedSemaphore;

		std::vector<VkFence> m_InFlightFences;
		std::vector<VkFence> m_ImagesInFlight;

		// Vulkan Vertex Buffer
		VkBuffer m_VertexBuffer;
		VkDeviceMemory m_VertexBufferMemory;

	private:
		const std::vector<Vertex> m_Verticies = {
			{ { 0.0f, -0.5f}, {1.0f, 0.0f, 0.0f} },
			{ { 0.5f,  0.5f}, {0.0f, 1.0f, 0.0f} },
			{ {-0.5f,  0.5f}, {0.0f, 0.0f, 1.0f} }
		};
	};

}