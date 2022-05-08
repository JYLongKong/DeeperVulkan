#include "TexDataObject.h"

TexDataObject::TexDataObject(int width, int height, unsigned char *data, int dataByteCount) {
  this->width = width;
  this->height = height;
  this->data = data;
  this->dataByteCount = dataByteCount;
}

TexDataObject::~TexDataObject() {
  delete[] data;
}
