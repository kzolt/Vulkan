#include <iostream>
#include "Core/VulkanApplication.h"

int main()
{
	Vulkan::VulkanApplication* app = new Vulkan::VulkanApplication();

	app->Run();

	delete app;
}