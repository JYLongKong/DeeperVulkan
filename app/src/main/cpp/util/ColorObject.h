#ifndef DEEPERVULKAN_COLOROBJECT_H
#define DEEPERVULKAN_COLOROBJECT_H

#include <vulkan/vulkan.h>
#include <string>

class ColorObject {

 public:
  VkDevice *devicePointer;
  float *vdata;
  int vCount;
  float *pushConstantData;
  float pointSize;
  VkBuffer vertexDatabuf;
  VkDeviceMemory vertexDataMem;
  VkDescriptorBufferInfo vertexDataBufferInfo;

  ColorObject(float *vdataIn,
              int dataByteCount,
              int vCountIn,
              VkDevice &device,
              VkPhysicalDeviceMemoryProperties &memoryroperties,
              float pointSizeIn);

  ~ColorObject();

  void drawSelf(VkCommandBuffer &cmd, VkPipelineLayout &pipelineLayout, VkPipeline &pipeline);
};

#endif 
