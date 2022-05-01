#ifndef DEEPERVULKAN_FLATDATA_H
#define DEEPERVULKAN_FLATDATA_H

class FlatData {
 public:
  static float *vdata;
  static int dataByteCount;
  static int vCount;

  static void genData();
};

#endif // DEEPERVULKAN_FLATDATA_H
