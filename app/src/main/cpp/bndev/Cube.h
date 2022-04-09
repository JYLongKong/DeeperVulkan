#ifndef VULKANEXBASE_CUBE_H
#define VULKANEXBASE_CUBE_H

#include "ColorRect.h"
#include "DrawableObjectCommon.h"

class Cube {
 public:
  /// Sample4_12
  DrawableObjectCommon *colorRect;
  float unit_size;

  void drawSelf(VkCommandBuffer cmd,
                VkPipelineLayout &pipelineLayout,
                VkPipeline &pipeline,
                VkDescriptorSet *desSetPointer);

//  Cube(VkDevice &device, VkPhysicalDeviceMemoryProperties &memoryroperties);
  /// Sample4_12
  Cube(VkDevice &device, VkPhysicalDeviceMemoryProperties &memoryroperties, float *vdata, float uint_sizeIn);

  ~Cube();
};

#endif //VULKANEXBASE_CUBE_H
