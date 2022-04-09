#ifndef TRIANGLESTRIP_CIRCLEDATA_H
#define TRIANGLESTRIP_CIRCLEDATA_H

class CircleData {
 public:
  static float *vdata;
  static int dataByteCount;
  static int vCount;
  static void genVertexData();
  static double toRadians(double d);

  /// Sample4_10
  static uint16_t *idata;     // 索引数据指针
  static int indexByteCount;  // 索引数据所占总字节数
  static uint32_t iCount;     // 索引数量
};

#endif
