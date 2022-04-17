#ifndef DEEPERVULKAN_BELTDATA_H
#define DEEPERVULKAN_BELTDATA_H

class BeltData {
 public:
  static float *vdata;
  static int dataByteCount;
  static int vCount;
  static double toRadians(double d);
  static void genVertexData();
};

#endif
