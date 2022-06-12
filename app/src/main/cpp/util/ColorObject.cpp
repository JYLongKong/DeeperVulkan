#include "ColorObject.h"
#include <assert.h>
#include "HelpFunction.h"
#include "MatrixState3D.h"
#include <string.h>

ColorObject::ColorObject(float *vdataIn,
                         int dataByteCount,
                         int vCountIn,
                         VkDevice &device,
                         VkPhysicalDeviceMemoryProperties &memoryroperties,
                         float pointSizeIn) {
  this->devicePointer = &device;
  this->vdata = vdataIn;
  this->vCount = vCountIn;
  this->pushConstantData = new float[17];
  this->pointSize = pointSizeIn;
  VkBufferCreateInfo buf_info = {};
  buf_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  buf_info.pNext = NULL;
  buf_info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
  buf_info.size = dataByteCount;
  buf_info.queueFamilyIndexCount = 0;
  buf_info.pQueueFamilyIndices = NULL;
  buf_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  buf_info.flags = 0;
  VkResult result = vk::vkCreateBuffer(device, &buf_info, NULL, &vertexDatabuf);
  assert(result == VK_SUCCESS);
  VkMemoryRequirements mem_reqs;
  vk::vkGetBufferMemoryRequirements(device, vertexDatabuf, &mem_reqs);
  assert(dataByteCount <= mem_reqs.size);
  VkMemoryAllocateInfo alloc_info = {};
  alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  alloc_info.pNext = NULL;
  alloc_info.memoryTypeIndex = 0;
  alloc_info.allocationSize = mem_reqs.size;
  VkFlags requirements_mask = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
  bool flag = memoryTypeFromProperties(memoryroperties,
                                       mem_reqs.memoryTypeBits,
                                       requirements_mask,
                                       &alloc_info.memoryTypeIndex);
  if (flag) {
    printf("确定内存类型成功 类型索引为%d", alloc_info.memoryTypeIndex);
  } else {
    printf("确定内存类型失败!");
  }
  result = vk::vkAllocateMemory(device, &alloc_info, NULL, &vertexDataMem);
  assert(result == VK_SUCCESS);
  uint8_t *pData;
  result = vk::vkMapMemory(device, vertexDataMem, 0, mem_reqs.size, 0, (void **) &pData);
  assert(result == VK_SUCCESS);
  memcpy(pData, vdata, dataByteCount);
  vk::vkUnmapMemory(device, vertexDataMem);
  result = vk::vkBindBufferMemory(device, vertexDatabuf, vertexDataMem, 0);
  assert(result == VK_SUCCESS);
  vertexDataBufferInfo.buffer = vertexDatabuf;
  vertexDataBufferInfo.offset = 0;
  vertexDataBufferInfo.range = mem_reqs.size;
}

ColorObject::~ColorObject() {
  delete[] vdata;
  delete[] pushConstantData;
  vk::vkDestroyBuffer(*devicePointer, vertexDatabuf, NULL);
  vk::vkFreeMemory(*devicePointer, vertexDataMem, NULL);
}

void ColorObject::drawSelf(VkCommandBuffer &cmd, VkPipelineLayout &pipelineLayout, VkPipeline &pipeline) {
  vk::vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
  const VkDeviceSize offsetsVertex[1] = {0};
  vk::vkCmdBindVertexBuffers
      (
          cmd,
          0,
          1,
          &(vertexDatabuf),
          offsetsVertex
      );
  float *mvp = MatrixState3D::getFinalMatrix();
  memcpy(pushConstantData, mvp, sizeof(float) * 16);
  float *pontSizeP = new float[1]{pointSize};
  memcpy(pushConstantData + 16, pontSizeP, sizeof(float));
  delete[] pontSizeP;
  vk::vkCmdPushConstants(cmd, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(float) * 17, pushConstantData);
  vk::vkCmdDraw(cmd, vCount, 1, 0, 0);
}
