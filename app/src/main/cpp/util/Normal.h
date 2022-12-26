#ifndef DEEPERVULKAN_NORMAL_H_
#define DEEPERVULKAN_NORMAL_H_

#include <set>

/**
 * 代表法向量的类，此类的一个对象表示一个法向量
 */
class Normal {
 public:

  // 法向量在xyz方向的分量
  float nx;
  float ny;
  float nz;

  /**
   * 判断一个向量是否在指定集合中
   */
  static bool exist(Normal *normal, std::set<Normal *> sn);

  /**
   * 求平均向量
   */
  static float *getAverage(std::set<Normal *> sn);

  Normal(float nx, float ny, float nz);
  ~Normal();
};

#endif //DEEPERVULKAN_NORMAL_H_
