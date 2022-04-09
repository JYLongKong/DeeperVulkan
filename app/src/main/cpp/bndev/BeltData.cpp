#include <vector>
#include <cmath>
#include <string>

#include "BeltData.h"

float *BeltData::vdata;
int BeltData::dataByteCount;
int BeltData::vCount;

const double
    PI = 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170680;

double BeltData::toRadians(double d) {
  double e = d * PI / 180;
  return e;
}

void BeltData::genVertexData() {
  /// Sample4_8 ************************************************** start
//  int n = 6;
//  vCount = 2 * (n + 1);
//  dataByteCount = vCount * 6 * sizeof(float);
//  float angdegBegin = -90;
//  float angdegEnd = 90;
//  float angdegSpan = (angdegEnd - angdegBegin) / n;
//  vdata = new float[vCount * 6];
//  int count = 0;
//  for (float angdeg = angdegBegin; angdeg <= angdegEnd; angdeg += angdegSpan) {
//    double angrad = toRadians(angdeg);
//    vdata[count++] = (float) (-0.6f * 50 * sin(angrad));
//    vdata[count++] = (float) (0.6f * 50 * cos(angrad));
//    vdata[count++] = 0;
//    vdata[count++] = 1;
//    vdata[count++] = 1;
//    vdata[count++] = 1;
//    vdata[count++] = (float) (-50 * sin(angrad));
//    vdata[count++] = (float) (50 * cos(angrad));
//    vdata[count++] = 0;
//    vdata[count++] = 0;
//    vdata[count++] = 1;
//    vdata[count++] = 1;
//  }
  /// Sample4_8 **************************************************** end

  /// Sample4_9 ************************************************** start
  int n1 = 3;                                                             // 第一个条带切割份数
  int n2 = 5;                                                             // 第二个条带切割份数
  vCount = 2 * (n1 + n2 + 2) + 2;                                         // 计算总顶点数
  dataByteCount = vCount * 6 * sizeof(float);                             // 顶点数据所占总字节数
  vdata = new float[vCount * 6];                                          // 顶点数据数组
  float angdegBegin1 = 0;                                                 // 第一个条带起始度数
  float angdegEnd1 = 90;                                                  // 第一个条带结束度数
  float angdegSpan1 = (angdegEnd1 - angdegBegin1) / n1;                   // 第一个条带每份度数
  float angdegBegin2 = 180;                                               // 第二个条带起始度数
  float angdegEnd2 = 270;                                                 // 第二个条带结束度数
  float angdegSpan2 = (angdegEnd2 - angdegBegin2) / n2;                   // 第二个条带每份度数
  int count = 0;                                                          // 辅助索引
  for (float angdeg = angdegBegin1; angdeg <= angdegEnd1; angdeg += angdegSpan1) {
    double angrad = toRadians(angdeg);                                    // 当前弧度
    vdata[count++] = (float) (-0.6f * 80 * sin(angrad));                  // 外围大圆上的点X坐标
    vdata[count++] = (float) (0.6f * 80 * cos(angrad));                   // 外围大圆上的点Y坐标
    vdata[count++] = 0;                                                   // 外围大圆上的点Z坐标
    vdata[count++] = 1;
    vdata[count++] = 1;
    vdata[count++] = 1;                                                   // 外围大圆顶点颜色
    vdata[count++] = (float) (-80 * sin(angrad));                         // 内圈小圆上的点X坐标
    vdata[count++] = (float) (80 * cos(angrad));                          // 内圈小圆上的点Y坐标
    vdata[count++] = 0;                                                   // 内圈小圆上的点Z坐标
    vdata[count++] = 0;
    vdata[count++] = 1;
    vdata[count++] = 1;                                                   // 内圈小圆顶点颜色
  }
  // 重复第一批三角形的最后一个顶点数据
  float repeatX = vdata[count - 6];
  vdata[count++] = repeatX;
  float repeatY = vdata[count - 6];
  vdata[count++] = repeatY;
  vdata[count++] = 0;
  vdata[count++] = 1;
  vdata[count++] = 0;
  vdata[count++] = 0;
  for (float angdeg = angdegBegin2; angdeg <= angdegEnd2; angdeg += angdegSpan2) {
    double angrad = toRadians(angdeg);                                    // 当前弧度
    if (angdeg == angdegBegin2) {                                         // 重复第二批三角形的第一个顶点数据
      vdata[count++] = (float) (-0.6f * 80 * sin(angrad));                // 顶点X坐标
      vdata[count++] = (float) (0.6f * 80 * cos(angrad));                 // 顶点Y坐标
      vdata[count++] = 0;                                                 // 顶点Z坐标
      vdata[count++] = 0;
      vdata[count++] = 1;
      vdata[count++] = 0;
    }
    // 大圆上的点
    vdata[count++] = (float) (-0.6f * 80 * sin(angrad));
    vdata[count++] = (float) (0.6f * 80 * cos(angrad));
    vdata[count++] = 0;
    vdata[count++] = 1;
    vdata[count++] = 1;
    vdata[count++] = 1;
    // 小圆上的点
    vdata[count++] = (float) (-80 * sin(angrad));
    vdata[count++] = (float) (80 * cos(angrad));
    vdata[count++] = 0;
    vdata[count++] = 0;
    vdata[count++] = 1;
    vdata[count++] = 1;
  }
  /// Sample4_9 **************************************************** end
}
