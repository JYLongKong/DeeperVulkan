#ifndef VULKANEXBASE_HELPFUNCTION_H
#define VULKANEXBASE_HELPFUNCTION_H

#include <vulkan/vulkan.h>
#include "../vksysutil/vulkan_wrapper.h"

bool memoryTypeFromProperties(VkPhysicalDeviceMemoryProperties &memoryProperties, uint32_t typeBits,
                              VkFlags requirements_mask, uint32_t *typeIndex);
#endif