#ifndef DEEPERVULKAN_LOADUTIL_H_
#define DEEPERVULKAN_LOADUTIL_H_

#include <string>
#include "DrawableObjectCommon.h"

class LoadUtil {
 public:

  /**
   * 读取obj文件内容生成绘制用物体对象的方法
   */
  static DrawableObjectCommon *loadFromFile(const std::string &fname,
                                            VkDevice &device,
                                            VkPhysicalDeviceMemoryProperties &memoryProperties);
};

#endif //DEEPERVULKAN_LOADUTIL_H_
