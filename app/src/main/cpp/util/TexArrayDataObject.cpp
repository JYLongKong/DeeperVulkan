#include "TexArrayDataObject.h"

TexArrayDataObject::TexArrayDataObject(int width, int height, int length, unsigned char *data) {
  this->width = width;
  this->height = height;
  this->length = length;
  this->data = data;
  this->dataByteCount = width * height * length * 4;
}

TexArrayDataObject::~TexArrayDataObject() {
  delete[] data;
}
