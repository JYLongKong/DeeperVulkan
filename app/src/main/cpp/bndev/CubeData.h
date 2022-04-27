/// Sample4_4
#ifndef DEEPERVULKAN_CUBEDATA_H
#define DEEPERVULKAN_CUBEDATA_H

class CubeData {
 public:
  static float *vdata;
  static int dataByteCount;
  static int vCount;
  static void genBallData();

  /// Sample5_7-立方体面法向量
  static void genData();
};

#endif
