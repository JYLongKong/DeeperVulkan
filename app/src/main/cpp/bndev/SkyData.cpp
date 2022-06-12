#include "SkyData.h"
#include <math.h>

#define random() (rand()%30000)/30000.0f  // 随机函数宏
#define PI 3.141592654  // PI的值
#define UNIT_SIZE 2000  // 单位尺寸

float *SkyData::vdata;
int SkyData::dataByteCount;
int SkyData::vCount;

void SkyData::genSkyData(int vCountIn) {
  vCount = vCountIn;
  dataByteCount = vCount * 6 * sizeof(float);
  vdata = new float[vCount * 6];
  for (int i = 0; i < vCount; ++i) { // 遍历每颗星星(即每个顶点)
    double angleTempJD = PI * 2 * random(); // 随机产生星星的经度
    double angleTempWD = PI * (random() - 0.5f); // 随机产生星星的纬度
    vdata[i * 6] = float(UNIT_SIZE * cos(angleTempWD) * sin(angleTempJD)); // 顶点位置X分量
    vdata[i * 6 + 1] = float(UNIT_SIZE * sin(angleTempWD)); // 顶点位置Y分量
    vdata[i * 6 + 2] = float(UNIT_SIZE * cos(angleTempWD) * cos(angleTempJD)); // 顶点位置Z分量
    vdata[i * 6 + 3] = 1.0; // 颜色值R分量
    vdata[i * 6 + 4] = 1.0; // 颜色值G分量
    vdata[i * 6 + 5] = 1.0; // 颜色值B分量
  }
}
