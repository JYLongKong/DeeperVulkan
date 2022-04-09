#ifndef VULKANEXBASE_FPSUTIL_H
#define VULKANEXBASE_FPSUTIL_H

class FPSUtil {
 public:
  //用于计算FPS的辅助变量
  static int FPSCount;
  static long long startTime;
  static float currFPS;

  //用于控制帧速率的辅助变量
  static long long beforeTime;

  //计算FPS相关方法
  static void init();

  static void calFPS();

  //控制帧速率相关方法
  static void before();

  static void after(int dstFPS);
};

#endif //VULKANEXBASE_FPSUTIL_H