#ifndef VULKANEXBASE_COLORRECT_H
#define VULKANEXBASE_COLORRECT_H

#include <cstdint>

class ColorRect {
 public:
  static float *vdata;
  static int dataByteCount;
  static int vCount;

  /// Sample4_12
  static float *vdataG;     // 青色正方形顶点数据指针
  static float *vdataY;     // 黄色正方形顶点数据指针
  static float UNIT_SIZEG;  // 青色正方形半边长
  static float UNIT_SIZEY;  // 黄色正方形半边长

  static void genVertexData();
};

#endif //VULKANEXBASE_COLORRECT_H
