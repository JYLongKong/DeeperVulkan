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

/// Sample5_7-立方体面法向量
void CubeData::genData() {
  const float rect = 1.0f;                                                // 立方体的边长
  std::vector<float> alVertix = {                                         // 存放顶点坐标的vector
      // 前面
      rect, rect, rect,                                                   // 1
      -rect, rect, rect,                                                  // 2
      -rect, -rect, rect,                                                 // 3

      rect, rect, rect,                                                   // 1
      -rect, -rect, rect,                                                 // 3
      rect, -rect, rect,                                                  // 2
      // 后面
      rect, rect, -rect,                                                  // 1
      -rect, rect, -rect,                                                 // 2
      -rect, -rect, -rect,                                                // 3

      rect, rect, -rect,                                                  // 1
      -rect, -rect, -rect,                                                // 3
      rect, -rect, -rect,                                                 // 2
      // 左面
      -rect, -rect, -rect,                                                // 3
      -rect, rect, -rect,                                                 // 2
      -rect, rect, rect,                                                  // 1

      -rect, -rect, rect,                                                 // 3
      -rect, -rect, -rect,                                                // 2
      -rect, rect, rect,                                                  // 1
      // 右面
      rect, -rect, -rect,                                                 // 3
      rect, rect, -rect,                                                  // 2
      rect, rect, rect,                                                   // 1

      rect, -rect, rect,                                                  // 3
      rect, -rect, -rect,                                                 // 2
      rect, rect, rect,                                                   // 1
      // 上面
      -rect, rect, -rect,                                                 // 3
      rect, rect, -rect,                                                  // 2
      rect, rect, rect,                                                   // 1

      -rect, rect, rect,                                                  // 3
      -rect, rect, -rect,                                                 // 2
      rect, rect, rect,                                                   // 1
      // 下面
      -rect, -rect, -rect,                                                // 3
      rect, -rect, -rect,                                                 // 2
      rect, -rect, rect,                                                  // 1

      -rect, -rect, rect,                                                 // 3
      -rect, -rect, -rect,                                                // 2
      rect, -rect, rect                                                   // 1
  };
  std::vector<float> alNormal = {                                         // 存放法向量的vector
      // 前面
      0, 0, 1,
      0, 0, 1,
      0, 0, 1,
      0, 0, 1,
      0, 0, 1,
      0, 0, 1,
      // 后面
      0, 0, -1,
      0, 0, -1,
      0, 0, -1,
      0, 0, -1,
      0, 0, -1,
      0, 0, -1,
      // 左面
      -1, 0, 0,
      -1, 0, 0,
      -1, 0, 0,
      -1, 0, 0,
      -1, 0, 0,
      -1, 0, 0,
      // 右面
      1, 0, 0,
      1, 0, 0,
      1, 0, 0,
      1, 0, 0,
      1, 0, 0,
      1, 0, 0,
      // 上面
      1, 0, 0,
      1, 0, 0,
      1, 0, 0,
      1, 0, 0,
      1, 0, 0,
      1, 0, 0,
      // 下面
      0, -1, 0,
      0, -1, 0,
      0, -1, 0,
      0, -1, 0,
      0, -1, 0,
      0, -1, 0
  };
  vCount = alVertix.size() / 3;                                           // 顶点的数量为坐标值数量的1/3，因为一个顶点有3个坐标
  dataByteCount = alVertix.size() * 2 * sizeof(float);                    // 顶点和法向量数据的总字节数
  vdata = new float[alVertix.size() * 2];                                 // 存放顶点坐标和法向量数据的数组
  int index = 0;
  for (int i = 0; i < vCount; ++i) {
    vdata[index++] = alVertix[i * 3];                                     // 保存顶点坐标
    vdata[index++] = alVertix[i * 3 + 1];
    vdata[index++] = alVertix[i * 3 + 2];
    vdata[index++] = alNormal[i * 3];                                     // 保存顶点法向量
    vdata[index++] = alNormal[i * 3 + 1];
    vdata[index++] = alNormal[i * 3 + 2];
  }
}
