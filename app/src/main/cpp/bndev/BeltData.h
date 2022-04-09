#ifndef VULKANEXBASE_BELTDATA_H
#define VULKANEXBASE_BELTDATA_H

class BeltData {
 public:
  static float *vdata;
  static int dataByteCount;
  static int vCount;
  static double toRadians(double d);
  static void genVertexData();
};

#endif
