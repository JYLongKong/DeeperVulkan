#include <vector>
#include <cmath>
#include <string>

#include "ColorRect.h"

float *ColorRect::vdata;
int ColorRect::dataByteCount;
int ColorRect::vCount;

/// Sample4_12
//float *ColorRect::vdataG;
//float *ColorRect::vdataY;
//float ColorRect::UNIT_SIZEG = 500;
//float ColorRect::UNIT_SIZEY = 499.5;

/// Sample4_13
float *ColorRect::vdataG;
float *ColorRect::vdataY;
float ColorRect::UNIT_SIZEG = 600;
float ColorRect::UNIT_SIZEY = 600;

void ColorRect::genVertexData() {
  vCount = 6;
  dataByteCount = vCount * 6 * sizeof(float);
  vdata = new float[vCount * 6]{
      0, 0, 0, 1, 1, 1,
      30, 30, 0, 0, 0, 1,
      -30, 30, 0, 0, 0, 1,
      -30, -30, 0, 0, 0, 1,
      30, -30, 0, 0, 0, 1,
      30, 30, 0, 0, 0, 1
  };

  /// Sample4_12 ************************************************* start
  vdataG = new float[vCount * 6]{
      0, 0, 0, 0, 1, 1,
      UNIT_SIZEG, UNIT_SIZEG, 0, 0, 1, 1,
      -UNIT_SIZEG, UNIT_SIZEG, 0, 0, 1, 1,
      -UNIT_SIZEG, -UNIT_SIZEG, 0, 0, 1, 1,
      UNIT_SIZEG, -UNIT_SIZEG, 0, 0, 1, 1,
      UNIT_SIZEG, UNIT_SIZEG, 0, 0, 1, 1
  };
  vdataY = new float[vCount * 6]{
      0, 0, 0, 1, 1, 0,
      UNIT_SIZEY, UNIT_SIZEY, 0, 1, 1, 0,
      -UNIT_SIZEY, UNIT_SIZEY, 0, 1, 1, 0,
      -UNIT_SIZEY, -UNIT_SIZEY, 0, 1, 1, 0,
      UNIT_SIZEY, -UNIT_SIZEY, 0, 1, 1, 0,
      UNIT_SIZEY, UNIT_SIZEY, 0, 1, 1, 0
  };
  /// Sample4_12 *************************************************** end
}
