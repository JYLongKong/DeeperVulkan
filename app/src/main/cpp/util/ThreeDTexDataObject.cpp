#include "ThreeDTexDataObject.h"

ThreeDTexDataObject::ThreeDTexDataObject(int width, int height, int depth, unsigned char *data) {
  this->width = width;
  this->height = height;
  this->depth = depth;
  this->data = data;
  this->dataByteCount = width * height * depth * 4;
}

ThreeDTexDataObject::~ThreeDTexDataObject() {
  delete[] data;
}
