#ifndef DEEPERVULKAN_SKYDATA_H_
#define DEEPERVULKAN_SKYDATA_H_

class SkyData {
 public:
  static float *vdata;
  static int dataByteCount;
  static int vCount;

  /**
   * 生成星星顶点数据
   */
  static void genSkyData(int vCountIn);
};

#endif // DEEPERVULKAN_SKYDATA_H_
