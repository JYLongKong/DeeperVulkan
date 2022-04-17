#include "BallData.h"
#include <vector>
#include <cmath>
#include <string>

float *BallData::vdata;
int BallData::dataByteCount;
int BallData::vCount;

float BallData::toRadians(float degree) {
  return degree * 3.1415926535898 / 180;
}

/**
 * 生成球面上各个小三角形顶点数据的方法
 */
void BallData::genBallData(float angleSpan) {
  const float r = 1.0f;                                                   // 球的半径
  std::vector<float> alVertix;                                            // 存放顶点坐标值的vector
  for (int vAngle = -90; vAngle < 90; vAngle = vAngle + angleSpan) {      // 垂直方向切分
    for (int hAngle = 0; hAngle <= 360; hAngle = hAngle + angleSpan) {    // 水平方向切分
      float x0 = (float) (r * cos(toRadians(vAngle)) * cos(toRadians(hAngle)));
      float y0 = (float) (r * cos(toRadians(vAngle)) * sin(toRadians(hAngle)));
      float z0 = (float) (r * sin(toRadians(vAngle)));
      float x1 = (float) (r * cos(toRadians(vAngle)) * cos(toRadians(hAngle + angleSpan)));
      float y1 = (float) (r * cos(toRadians(vAngle)) * sin(toRadians(hAngle + angleSpan)));
      float z1 = (float) (r * sin(toRadians(vAngle)));
      float x2 = (float) (r * cos(toRadians(vAngle + angleSpan)) * cos(toRadians(hAngle + angleSpan)));
      float y2 = (float) (r * cos(toRadians(vAngle + angleSpan)) * sin(toRadians(hAngle + angleSpan)));
      float z2 = (float) (r * sin(toRadians(vAngle + angleSpan)));
      float x3 = (float) (r * cos(toRadians(vAngle + angleSpan)) * cos(toRadians(hAngle)));
      float y3 = (float) (r * cos(toRadians(vAngle + angleSpan)) * sin(toRadians(hAngle)));
      float z3 = (float) (r * sin(toRadians(vAngle + angleSpan)));
      alVertix.push_back(x1);
      alVertix.push_back(y1);
      alVertix.push_back(z1);
      alVertix.push_back(x3);
      alVertix.push_back(y3);
      alVertix.push_back(z3);
      alVertix.push_back(x0);
      alVertix.push_back(y0);
      alVertix.push_back(z0);
      alVertix.push_back(x1);
      alVertix.push_back(y1);
      alVertix.push_back(z1);
      alVertix.push_back(x2);
      alVertix.push_back(y2);
      alVertix.push_back(z2);
      alVertix.push_back(x3);
      alVertix.push_back(y3);
      alVertix.push_back(z3);
    }
  }
  vCount = alVertix.size() / 3;
  dataByteCount = alVertix.size() * sizeof(float);
  vdata = new float[alVertix.size()];
  int index = 0;
  for (int i = 0; i < vCount; ++i) {
    vdata[index++] = alVertix[i * 3];
    vdata[index++] = alVertix[i * 3 + 1];
    vdata[index++] = alVertix[i * 3 + 2];
  }
}
