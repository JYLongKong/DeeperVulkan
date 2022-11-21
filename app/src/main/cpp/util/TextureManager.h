#ifndef DEEPERVULKAN_TEXTUREMANAGER_H_
#define DEEPERVULKAN_TEXTUREMANAGER_H_

#include <vector>
#include <map>
#include <string>
#include <vulkan/vulkan.h>
#include "../vksysutil/vulkan_wrapper.h"
#include "TexDataObject.h"
#include "ThreeDTexDataObject.h"

#define SAMPLER_COUNT 1 // 采样器数量
//#define SAMPLER_COUNT 4 // Sample6_3-四种纹理拉伸方式的采样器
//#define SAMPLER_COUNT 2 // Sample6_4-两种纹理采样方式的采样器

class TextureManager {
 public:
  static std::vector<std::string> texNames;                               // 纹理文件名称列表
  static std::vector<VkSampler> samplerList;                              // 采样器列表
  static std::map<std::string, VkImage> textureImageList;                 // 纹理图像列表
  static std::map<std::string, VkDeviceMemory> textureMemoryList;         // 纹理图像内存列表
  static std::map<std::string, VkImageView> viewTextureList;              // 纹理图像视图列表
  static std::map<std::string, VkDescriptorImageInfo> texImageInfoList;   // 纹理图像描述信息列表
  static std::map<std::string, int> imageSampler;                         // Sample6_3-纹理文件名称对应的采样器索引
  static std::vector<std::string> texNamesSingle;                         // Sample6_6
  static std::vector<std::string> texNamesPair;                           // Sample6_6

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
      TexDataObject *ctdo
  );

  /**
   * Sample6_5
   * 加载2D纹理并生成MipMap
   */
  static void init_SPEC_Textures_ForMipMap(
      std::string texName,
      VkDevice &device,
      VkPhysicalDevice &gpu,
      VkPhysicalDeviceMemoryProperties &memoryroperties,
      VkCommandBuffer &cmdBuffer,
      VkQueue &queueGraphics,
      VkFormat format,
      TexDataObject *ctdo,
      int levels
  );

  /**
   * Sample6_9
   * 加载3D纹理
   */
  static void init_SPEC_3D_Textures(
      std::string texName,
      VkDevice &device,
      VkPhysicalDevice &gpu,
      VkPhysicalDeviceMemoryProperties &memoryProperties,
      VkCommandBuffer &cmdBuffer,
      VkQueue &queueGraphics,
      VkFormat format,
      ThreeDTexDataObject *ctdo
  );
};

#endif // DEEPERVULKAN_TEXTUREMANAGER_H_
