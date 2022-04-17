#ifndef DEEPERVULKAN_BALLDATA_H
#define DEEPERVULKAN_BALLDATA_H

class BallData {
 public:
  static float *vdata;
  static int dataByteCount;
  static int vCount;
  static void genBallData(float angleSpan);
 private:
  static float toRadians(float degree);
};

#endif // DEEPERVULKAN_BALLDATA_H
