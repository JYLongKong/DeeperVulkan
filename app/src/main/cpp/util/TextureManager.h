#ifndef DEEPERVULKAN_TEXTUREMANAGER_H_
#define DEEPERVULKAN_TEXTUREMANAGER_H_

#include <vector>
#include <map>
#include <string>
#include <vulkan/vulkan.h>
#include "../vksysutil/vulkan_wrapper.h"
#include "TexDataObject.h"

#define SAMPLER_COUNT 1 // 采样器数量

class TextureManager {
 public:
  static std::vector<std::string> texNames;                               // 纹理文件名称列表
  static std::vector<VkSampler> samplerList;                              // 采样器列表
  static std::map<std::string, VkImage> textureImageList;                 // 纹理图像列表
  static std::map<std::string, VkDeviceMemory> textureMemoryList;         // 纹理图像内存列表
  static std::map<std::string, VkImageView> viewTextureList;              // 纹理图像视图列表
  static std::map<std::string, VkDescriptorImageInfo> texImageInfoList;   // 纹理图像描述信息列表

  /**
   * 加载所有纹理
   */
  static void initTextures(
      VkDevice &device,
      VkPhysicalDevice &gpu,
      VkPhysicalDeviceMemoryProperties &memoryroperties,
      VkCommandBuffer &cmdBuffer,
      VkQueue &queueGraphics);

  /**
   * 销毁所有纹理
   */
  static void destroyTextures(VkDevice &device);

  /**
   * 获取指定名称纹理在描述集列表中的索引
   */
  static int getVkDescriptorSetIndex(std::string texName);

 private:

  /**
   * 初始化采样器
   */
  static void initSampler(VkDevice &device, VkPhysicalDevice &gpu);

  /**
   * 加载2D纹理
   */
  static void init_SPEC_2D_Textures(
      std::string texName,
      VkDevice &device,
      VkPhysicalDevice &gpu,
      VkPhysicalDeviceMemoryProperties &memoryroperties,
      VkCommandBuffer &cmdBuffer,
      VkQueue &queueGraphics,
      VkFormat format,
      TexDataObject *ctdo);
};

#endif // DEEPERVULKAN_TEXTUREMANAGER_H_
