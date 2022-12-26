#include "Normal.h"

#include "LoadUtil.h"

float DIFF = 0.0000001f;  // 判断两个向量是否相同的阈值

Normal::Normal(float nx, float ny, float nz) {
  this->nx = nx;
  this->ny = ny;
  this->nz = nz;
}

bool Normal::exist(Normal *normal, std::set<Normal *> sn) {
  for (Normal *nTemp: sn) {
    if (abs(nTemp->nx - normal->nx) < DIFF && abs(nTemp->ny - normal->ny) < DIFF
        && abs(nTemp->nz - normal->nz) < DIFF) {
      return true;
    }
  }
  return false;
}

float *Normal::getAverage(std::set<Normal *> sn) {
  float *result = new float[3]();
  for (Normal *tempHsn: sn) {
    result[0] += tempHsn->nx;
    result[1] += tempHsn->ny;
    result[2] += tempHsn->nz;
  }
  return LoadUtil::vectorNormal(result);
}
