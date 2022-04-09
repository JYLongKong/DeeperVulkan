#include "HelpFunction.h"

//内部使用的确定内存类型索引的方法
bool memoryTypeFromProperties(VkPhysicalDeviceMemoryProperties &memoryProperties, uint32_t typeBits,
                              VkFlags requirements_mask, uint32_t *typeIndex) {
  //循环确定内存类型索引
  for (uint32_t i = 0; i < 32; i++) {
    //若对应类型比特位为1
    if ((typeBits & 1) == 1) {
      //此类型与掩码匹配
      if ((memoryProperties.memoryTypes[i].propertyFlags & requirements_mask) ==
          requirements_mask) {
        *typeIndex = i;
        return true;
      }
    }
    typeBits >>= 1;
  }
  //没有找到所需的类型索引
  return false;
}