/// Sample4_4
#include "CubeData.h"
#include <vector>
#include <cmath>
#include <string>

float *CubeData::vdata;
int CubeData::dataByteCount;
int CubeData::vCount;

void CubeData::genBallData() {
  float UNIT_SIZE = 1;
  float vertices[] = {
      // 前面
      0, 0, UNIT_SIZE,
      UNIT_SIZE, UNIT_SIZE, UNIT_SIZE,
      -UNIT_SIZE, UNIT_SIZE, UNIT_SIZE,
      0, 0, UNIT_SIZE,
      -UNIT_SIZE, UNIT_SIZE, UNIT_SIZE,
      -UNIT_SIZE, -UNIT_SIZE, UNIT_SIZE,
      0, 0, UNIT_SIZE,
      -UNIT_SIZE, -UNIT_SIZE, UNIT_SIZE,
      UNIT_SIZE, -UNIT_SIZE, UNIT_SIZE,
      0, 0, UNIT_SIZE,
      UNIT_SIZE, -UNIT_SIZE, UNIT_SIZE,
      UNIT_SIZE, UNIT_SIZE, UNIT_SIZE,
      // 后面
      0, 0, -UNIT_SIZE,
      UNIT_SIZE, UNIT_SIZE, -UNIT_SIZE,
      UNIT_SIZE, -UNIT_SIZE, -UNIT_SIZE,
      0, 0, -UNIT_SIZE,
      UNIT_SIZE, -UNIT_SIZE, -UNIT_SIZE,
      -UNIT_SIZE, -UNIT_SIZE, -UNIT_SIZE,
      0, 0, -UNIT_SIZE,
      -UNIT_SIZE, -UNIT_SIZE, -UNIT_SIZE,
      -UNIT_SIZE, UNIT_SIZE, -UNIT_SIZE,
      0, 0, -UNIT_SIZE,
      -UNIT_SIZE, UNIT_SIZE, -UNIT_SIZE,
      UNIT_SIZE, UNIT_SIZE, -UNIT_SIZE,
      // 左面
      -UNIT_SIZE, 0, 0,
      -UNIT_SIZE, UNIT_SIZE, UNIT_SIZE,
      -UNIT_SIZE, UNIT_SIZE, -UNIT_SIZE,
      -UNIT_SIZE, 0, 0,
      -UNIT_SIZE, UNIT_SIZE, -UNIT_SIZE,
      -UNIT_SIZE, -UNIT_SIZE, -UNIT_SIZE,
      -UNIT_SIZE, 0, 0,
      -UNIT_SIZE, -UNIT_SIZE, -UNIT_SIZE,
      -UNIT_SIZE, -UNIT_SIZE, UNIT_SIZE,
      -UNIT_SIZE, 0, 0,
      -UNIT_SIZE, -UNIT_SIZE, UNIT_SIZE,
      -UNIT_SIZE, UNIT_SIZE, UNIT_SIZE,
      // 右面
      UNIT_SIZE, 0, 0,
      UNIT_SIZE, UNIT_SIZE, UNIT_SIZE,
      UNIT_SIZE, -UNIT_SIZE, UNIT_SIZE,
      UNIT_SIZE, 0, 0,
      UNIT_SIZE, -UNIT_SIZE, UNIT_SIZE,
      UNIT_SIZE, -UNIT_SIZE, -UNIT_SIZE,
      UNIT_SIZE, 0, 0,
      UNIT_SIZE, -UNIT_SIZE, -UNIT_SIZE,
      UNIT_SIZE, UNIT_SIZE, -UNIT_SIZE,
      UNIT_SIZE, 0, 0,
      UNIT_SIZE, UNIT_SIZE, -UNIT_SIZE,
      UNIT_SIZE, UNIT_SIZE, UNIT_SIZE,
      // 上面
      0, UNIT_SIZE, 0,
      UNIT_SIZE, UNIT_SIZE, UNIT_SIZE,
      UNIT_SIZE, UNIT_SIZE, -UNIT_SIZE,
      0, UNIT_SIZE, 0,
      UNIT_SIZE, UNIT_SIZE, -UNIT_SIZE,
      -UNIT_SIZE, UNIT_SIZE, -UNIT_SIZE,
      0, UNIT_SIZE, 0,
      -UNIT_SIZE, UNIT_SIZE, -UNIT_SIZE,
      -UNIT_SIZE, UNIT_SIZE, UNIT_SIZE,
      0, UNIT_SIZE, 0,
      -UNIT_SIZE, UNIT_SIZE, UNIT_SIZE,
      UNIT_SIZE, UNIT_SIZE, UNIT_SIZE,
      // 下面
      0, -UNIT_SIZE, 0,
      UNIT_SIZE, -UNIT_SIZE, UNIT_SIZE,
      -UNIT_SIZE, -UNIT_SIZE, UNIT_SIZE,
      0, -UNIT_SIZE, 0,
      -UNIT_SIZE, -UNIT_SIZE, UNIT_SIZE,
      -UNIT_SIZE, -UNIT_SIZE, -UNIT_SIZE,
      0, -UNIT_SIZE, 0,
      -UNIT_SIZE, -UNIT_SIZE, -UNIT_SIZE,
      UNIT_SIZE, -UNIT_SIZE, -UNIT_SIZE,
      0, -UNIT_SIZE, 0,
      UNIT_SIZE, -UNIT_SIZE, -UNIT_SIZE,
      UNIT_SIZE, -UNIT_SIZE, UNIT_SIZE,
  };

  float colors[] = {
      // 前面
      1, 1, 1,
      1, 0, 0,
      1, 0, 0,
      1, 1, 1,
      1, 0, 0,
      1, 0, 0,
      1, 1, 1,
      1, 0, 0,
      1, 0, 0,
      1, 1, 1,
      1, 0, 0,
      1, 0, 0,
      // 后面
      1, 1, 1,
      0, 0, 1,
      0, 0, 1,
      1, 1, 1,
      0, 0, 1,
      0, 0, 1,
      1, 1, 1,
      0, 0, 1,
      0, 0, 1,
      1, 1, 1,
      0, 0, 1,
      0, 0, 1,
      // 左面
      1, 1, 1,
      1, 0, 1,
      1, 0, 1,
      1, 1, 1,
      1, 0, 1,
      1, 0, 1,
      1, 1, 1,
      1, 0, 1,
      1, 0, 1,
      1, 1, 1,
      1, 0, 1,
      1, 0, 1,
      // 右面
      1, 1, 1,
      1, 1, 0,
      1, 1, 0,
      1, 1, 1,
      1, 1, 0,
      1, 1, 0,
      1, 1, 1,
      1, 1, 0,
      1, 1, 0,
      1, 1, 1,
      1, 1, 0,
      1, 1, 0,
      // 上面
      1, 1, 1,
      0, 1, 0,
      0, 1, 0,
      1, 1, 1,
      0, 1, 0,
      0, 1, 0,
      1, 1, 1,
      0, 1, 0,
      0, 1, 0,
      1, 1, 1,
      0, 1, 0,
      0, 1, 0,
      // 下面
      1, 1, 1,
      0, 1, 1,
      0, 1, 1,
      1, 1, 1,
      0, 1, 1,
      0, 1, 1,
      1, 1, 1,
      0, 1, 1,
      0, 1, 1,
      1, 1, 1,
      0, 1, 1,
      0, 1, 1,
  };

  vCount = (sizeof(vertices) / sizeof(vertices[0])) / 3;
  // 顶点的数量为坐标值数量的1/3，因为一个顶点有3个坐标
  dataByteCount = (sizeof(vertices) / sizeof(vertices[0])) * 2 * sizeof(float);
  vdata = new float[(sizeof(vertices) / sizeof(vertices[0])) * 2];
  int index = 0;
  for (int i = 0; i < vCount; i++) {
    vdata[index++] = vertices[i * 3 + 0];
    vdata[index++] = vertices[i * 3 + 1];
    vdata[index++] = vertices[i * 3 + 2];

    vdata[index++] = colors[i * 3 + 0];
    vdata[index++] = colors[i * 3 + 1];
    vdata[index++] = colors[i * 3 + 2];
  }
}
