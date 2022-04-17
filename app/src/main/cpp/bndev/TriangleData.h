#ifndef DEEPERVULKAN_TRIANGLE_DATA_H
#define DEEPERVULKAN_TRIANGLE_DATA_H

class TriangleData {
 public:
  static float *vdata;          // 数据数组首地址指针
  static int dataByteCount;     // 数据所占总字节数量
  static int vCount;            // 顶点数量
  static void genVertexData();  // 生成数据的方法
};

#endif
