#ifndef DEEPERVULKAN_SHADERCOMPILEUTIL_H
#define DEEPERVULKAN_SHADERCOMPILEUTIL_H

#include <shaderc/shaderc.hpp>
#include <vulkan/vulkan.h>
#include "../vksysutil/vulkan_wrapper.h"

//将GLSL着色器脚本编译为spirv的方法
bool GLSLtoSPV(const VkShaderStageFlagBits shader_type, const char *pshader,
               std::vector<unsigned int> &spirv);

#endif //DEEPERVULKAN_SHADERCOMPILEUTIL_H