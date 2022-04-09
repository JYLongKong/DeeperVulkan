#include "FPSUtil.h"
#include <sys/time.h>
#include <chrono>
#include <thread>
#include "../bndev/mylog.h"

int FPSUtil::FPSCount;
long long FPSUtil::startTime;
float FPSUtil::currFPS;
long long FPSUtil::beforeTime;

long long currentTimeMillis() {
  //声明结构体实例
  struct timeval tv;
  //获取时间数据
  gettimeofday(&tv, NULL);
  //计算出毫秒数
  long long time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
  return time;
}

void FPSUtil::init() {
  FPSCount = 0;
  startTime = currentTimeMillis();
}

void FPSUtil::calFPS() {
  FPSCount++;
  if (FPSCount == 100) {
    FPSCount = 0;
    long long endTime = currentTimeMillis();
    currFPS = 1000.0 / ((endTime - startTime) / 100.0);
    startTime = endTime;
    LOGE("FPS: %f", FPSUtil::currFPS);
  }
}

void FPSUtil::before() {
  beforeTime = currentTimeMillis();
}

void FPSUtil::after(int dstFPS) {
  //计算指定FPS对应的每帧毫秒数
  int dstSpan = (int) (1000 / dstFPS) + 1;
  //计算此帧耗时
  long long span = currentTimeMillis() - beforeTime;
  //如果此帧耗时小于指定FPS对应的每帧毫秒数则加入动态时间休眠
  if (span < dstSpan) {
    std::this_thread::sleep_for(std::chrono::milliseconds(dstSpan - span));
  }
}