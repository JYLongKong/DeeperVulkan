#ifndef DEEPERVULKAN_OBJOBJECT_H
#define DEEPERVULKAN_OBJOBJECT_H

#include <vulkan/vulkan.h>
#include "vulkan_wrapper.h"
#include <string>

class DrawableObjectCommon {
 public:
  /// Sample4_2-需推送的常量数据数组的首地址指针
  float *pushConstantData;

  VkDevice *devicePointer;                      // 指向逻辑设备的指针
  float *vdata;                                 // 顶点数据数组首地址指针
  int vCount;                                   // 顶点数量
  VkBuffer vertexDatabuf;                       // 顶点数据缓冲
  VkDeviceMemory vertexDataMem;                 // 顶点数据所需设备内存
  VkDescriptorBufferInfo vertexDataBufferInfo;  // 顶点数据缓冲描述信息

  /// Sample4_10
  uint16_t *idata;                              // 索引数据数组首地址指针
  int iCount;                                   // 索引数量
  VkBuffer indexDatabuf;                        // 索引数据缓冲
  VkDeviceMemory indexDataMem;                  // 索引数据所需设备内存
  VkDescriptorBufferInfo indexDataBufferInfo;   // 索引数据缓冲描述信息

  /// Sample4_15 ************************************************* start
  int indirectDrawCount;                        // 间接绘制信息数据组的数量
  int drawCmdbufbytes;                          // 间接绘制信息数据所占总字节数
  VkBuffer drawCmdbuf;                          // 间接绘制信息数据缓冲
  VkDeviceMemory drawCmdMem;                    // 间接绘制信息数据缓冲对应设备内存
  void initDrawCmdbuf(                          // 用于创建间接绘制信息数据缓冲的方法
      VkDevice &device, VkPhysicalDeviceMemoryProperties &memoryroperties);
  /// Sample4_15 *************************************************** end

  /// Sample6_5 ************************************************** start
  float *pushConstantDataVertex;
  float *pushConstantDataFrag;
  /// Sample6_5 **************************************************** end

  DrawableObjectCommon(
      float *vdataIn,
      int dataByteCount,
      int vCountIn,

      /// Sample4_10、Sample4_16
//      uint16_t *idataIn,
//      int indexByteCount,
//      int iCountIn,

      VkDevice &device,
      VkPhysicalDeviceMemoryProperties &memoryroperties
  );

  ~DrawableObjectCommon();

  /**
   * 绘制物体
   */
  void drawSelf(
      VkCommandBuffer &secondary_cmd,
      VkPipelineLayout &pipelineLayout,
      VkPipeline &pipeline,
      VkDescriptorSet *desSetPointer,

      /// Sample4_10
//      uint32_t sIndex,
//      uint32_t eIndex

      /// Sample4_16
//      int cmdDataOffset = 0

      /// Sample6_5
      float lodLevel
  );

  /// Sample4_10 ************************************************* start
 private:
  /**
   * 创建顶点数据缓冲的方法
   */
  void createVertexBuffer(int dataByteCount, VkDevice &device, VkPhysicalDeviceMemoryProperties &memoryroperties);

  /**
   * 创建索引数据缓冲的方法
   */
  void createIndexBuffer(int indexByteCount, VkDevice &device, VkPhysicalDeviceMemoryProperties &memoryroperties);
  /// Sample4_10 *************************************************** end
};

#endif
