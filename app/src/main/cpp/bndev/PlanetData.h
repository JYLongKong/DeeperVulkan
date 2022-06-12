#ifndef DEEPERVULKAN_PLANETDATA_H
#define DEEPERVULKAN_PLANETDATA_H

class PlanetData {
 public:
  static float *vdata;
  static int dataByteCount;
  static int vCount;
  static void genPlanetData(float angleSpan);
};

#endif // DEEPERVULKAN_PLANETDATA_H
