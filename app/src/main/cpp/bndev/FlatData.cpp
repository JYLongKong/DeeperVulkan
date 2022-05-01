#include "FlatData.h"
#include <vector>
#include <cmath>
#include <string>

float *FlatData::vdata;
int FlatData::dataByteCount;
int FlatData::vCount;

void FlatData::genData() {
  vCount = 6;
  dataByteCount = vCount * 6 * sizeof(float);
  vdata = new float[vCount * 6]{  // 顶点位置+法向量
      3, 2, 0, 0, 0, 1,
      -3, 2, 0, 0, 0, 1,
      -3, -2, 0, 0, 0, 1,
      3, -2, 0, 0, 0, 1,
      3, 2, 0, 0, 0, 1,
      -3, -2, 0, 0, 0, 1
  };
}
