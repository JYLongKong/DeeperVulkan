#ifndef VULKANEXBASE_OBJECTDATA_H
#define VULKANEXBASE_OBJECTDATA_H

class ObjectData {
 public:
  static float *vdata;
  static int dataByteCount;
  static int vCount;
  static void genVertexData();
};

#endif
