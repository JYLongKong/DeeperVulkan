/// Sample4_2
#include "SixPointedStar.h"
#include <vector>
#include <math.h>
#include <string.h>

float *SixPointedStar::vdata;
int SixPointedStar::dataByteCount;
int SixPointedStar::vCount;

float toRadians(float degree) {
  return degree * 3.1415926535898 / 180;
}

void SixPointedStar::genStarData(float R, float r, float z) {
  std::vector<float> alVertix;                              // 存放顶点坐标的列表
  float tempAngle = 360 / 6;                                // 六角星的角间距
  float UNIT_SIZE = 1;                                      // 单位尺寸
  for (float angle = 0; angle < 360; angle += tempAngle) {  // 六角星的每个角计算一次
    alVertix.push_back(0);                                  // 第一个点的x坐标
    alVertix.push_back(0);                                  // 第一个点的y坐标
    alVertix.push_back(z);                                  // 第一个点的z坐标
    alVertix.push_back((float) (R * UNIT_SIZE * cos(toRadians(angle)))); // 第二个点的x坐标
    alVertix.push_back((float) (R * UNIT_SIZE * sin(toRadians(angle)))); // 第二个点的y坐标
    alVertix.push_back(z);                                  // 第二个点的z坐标
    alVertix.push_back((float) (r * UNIT_SIZE * cos(toRadians(angle + tempAngle / 2)))); // 第三个点的x坐标
    alVertix.push_back((float) (r * UNIT_SIZE * sin(toRadians(angle + tempAngle / 2)))); // 第三个点的y坐标
    alVertix.push_back(z);                                  // 第三个点的z坐标
    alVertix.push_back(0);                                  // 第四个点的x坐标
    alVertix.push_back(0);                                  // 第四个点的y坐标
    alVertix.push_back(z);                                  // 第四个点的z坐标
    alVertix.push_back((float) (r * UNIT_SIZE * cos(toRadians(angle + tempAngle / 2)))); // 第五个点的x坐标
    alVertix.push_back((float) (r * UNIT_SIZE * sin(toRadians(angle + tempAngle / 2)))); // 第五个点的y坐标
    alVertix.push_back(z);                                  // 第五个点的z坐标
    alVertix.push_back((float) (R * UNIT_SIZE * cos(toRadians(angle + tempAngle)))); // 第六个点的x坐标
    alVertix.push_back((float) (R * UNIT_SIZE * sin(toRadians(angle + tempAngle)))); // 第六个点的y坐标
    alVertix.push_back(z);                                  // 第六个点的z坐标
  }
  vCount = alVertix.size() / 3;                             // 计算顶点的数量
  dataByteCount = alVertix.size() * 2 * sizeof(float);      // 顶点数据总字节数
  vdata = new float[alVertix.size() * 2];                   // 创建顶点数据数组
  int index = 0;                                            // 辅助数组索引
  for (int i = 0; i < vCount; ++i) {                        // 向顶点数据数组中填充数据
    vdata[index++] = alVertix[i * 3];                       // 存入当前顶点x坐标
    vdata[index++] = alVertix[i * 3 + 1];                   // 存入当前顶点y坐标
    vdata[index++] = alVertix[i * 3 + 2];                   // 存入当前顶点z坐标
    if (i % 3 == 0) {                                       // 若为中心点
      vdata[index++] = 1;                                   // 中心点颜色R通道值
      vdata[index++] = 1;                                   // 中心点颜色G通道值
      vdata[index++] = 1;                                   // 中心点颜色B通道值
    } else {                                                // 若不为中心点
      vdata[index++] = 0.45f;                               // 非中心点颜色R通道值
      vdata[index++] = 0.75f;                               // 非中心点颜色G通道值
      vdata[index++] = 0.75f;                               // 非中心点颜色B通道值
    }
  }
}
